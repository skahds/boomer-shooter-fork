// based on https://github.com/rxi/juno/blob/master/src/fs.c

#include "vfs.h"

#include <stdio.h>
#include <sys/stat.h>

#include "mem.h"

#define fseeko fseek
#define ftello ftell
#define _fseeki64 fseek
#define _ftelli64 ftell
#include "miniz/miniz.c"

// Design goals:
// - can initialize any number of virtual filesystems
// - using a filesystem initialized from a zip is the same as one made from 
//   directories, with the exception of writing being disabled.
// - any given file paths should work fine

// to concat any file path to another:
// - make sure the rhs path does not have a cwd prefix (./)

static bool IsCharPathSep(char c)
{
  return c == '/' || c == '\\';
}

// removes ./ from the beginning of paths
static const char* RemoveCwdPrefix(const char* path)
{
  if (path[0] == '.' && IsCharPathSep(path[1])) return path + 2;
  return path;
}

// checks if a path is a dir
static bool IsPathDir(const char* path)
{
  struct stat f;
  int res = stat(path, &f);
  return res == 0 && S_ISDIR(f.st_mode);
}

static bool IsFilenameValid(const char *filename) {
  return !(
    *filename == '/' ||
    strstr(filename, "..") ||
    strstr(filename, ":\\"));
}

enum VfsError VfsInit(struct Vfs* vfs, const char* path)
{
  size_t len = strlen(path);
  char* owned_path = CreateArray(char, len + 1);
  if (!owned_path) return VFS_OUT_OF_MEM;
  strcpy(owned_path, path);
  if (IsCharPathSep(owned_path[len - 1])) owned_path[len - 1] = '\0';

  bool is_dir = IsPathDir(owned_path);
  memset(&vfs->zip, 0, sizeof(mz_zip_archive));
  if (!is_dir && !mz_zip_reader_init_file(&vfs->zip, owned_path, 0))
    return VFS_COULD_NOT_MOUNT;

  vfs->path = owned_path;
  vfs->type = is_dir ? VFS_DIR : VFS_ZIP;

  LogInfo("mounted vfs at '%s'", path);
  
  return VFS_OK;
}

bool VfsDoesFileExist(struct Vfs* vfs, const char* path)
{
  if (!IsFilenameValid(path)) return false;
  path = RemoveCwdPrefix(path);

  if (vfs->type == VFS_DIR) {
    struct stat stats;
    char* full_path = Concat(vfs->path, "/", path, NULL);
    if (!full_path) return false;
    int res = stat(full_path, &stats);
    Destroy(full_path);
    return res == 0;
  } else if (vfs->type == VFS_ZIP) {
    int index = mz_zip_reader_locate_file(&vfs->zip, path, NULL, 0);
    return index != -1;
  }
  return false;
}

char* VfsReadFile(struct Vfs* vfs, const char* path, size_t* size)
{
  if (!IsFilenameValid(path)) {
    LogWarning("could not load '%s', because the path is invalid", path);
    return NULL;
  }
  const char* lpath = RemoveCwdPrefix(path);

  if (vfs->type == VFS_DIR) {
    char* full_path = Concat(vfs->path, "/", lpath, NULL);
    FILE* file = fopen(full_path, "rb");
    if (file == NULL) {
      LogWarning("could not open file '%s'", full_path);
      Destroy(full_path);
      return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* dat = CreateArray(char, file_size);
    if (dat == NULL) {
      LogWarning("ran out of memory to read '%s'", full_path);
      Destroy(full_path);
      return NULL;
    }

    size_t bytes_read = fread(dat, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
      LogWarning("could not read file '%s'", full_path);
      Destroy(dat);
      Destroy(full_path);
      return NULL;
    }
    fclose(file);

    if (size) *size = file_size;

    LogDebug("loaded file '%s'", full_path);
    Destroy(full_path);

    return dat;
  } else if (vfs->type == VFS_ZIP) {
    char* dat = mz_zip_reader_extract_file_to_heap(
      &vfs->zip, lpath, size, 0);
    if (!dat) {
      LogWarning("could not load file '%s'", path);
      return NULL;
    }
    LogDebug("loaded file '%s'");
    return dat;
  }
  return NULL;
}

char* VfsReadTxtFile(struct Vfs* vfs, const char* path, size_t* size)
{
  if (!IsFilenameValid(path)) {
    LogWarning("could not load '%s', because the path is invalid", path);
    return NULL;
  }
  const char* lpath = RemoveCwdPrefix(path);

  if (vfs->type == VFS_DIR) {
    char* full_path = Concat(vfs->path, "/", lpath, NULL);
    FILE* file = fopen(full_path, "rb");
    if (file == NULL) {
      LogWarning("could not open file '%s'", full_path);
      Destroy(full_path);
      return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* dat = CreateArray(char, file_size + 1);
    if (dat == NULL) {
      LogWarning("ran out of memory to read '%s'", full_path);
      Destroy(full_path);
      return NULL;
    }

    size_t bytes_read = fread(dat, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
      LogWarning("could not read file '%s'", full_path);
      Destroy(dat);
      Destroy(full_path);
      return NULL;
    }
    dat[bytes_read] = '\0';
    fclose(file);

    if (size) *size = file_size;

    LogDebug("loaded file '%s'", full_path);
    Destroy(full_path);

    return dat;
  } else if (vfs->type == VFS_ZIP) {
    size_t zdat_size;
    char* zdat = mz_zip_reader_extract_file_to_heap(
      &vfs->zip, lpath, &zdat_size, 0);
    if (!zdat) {
      LogWarning("could not load file '%s'", path);
      return NULL;
    }

    // miniz doesn't null terminate their stuff :(

    char* dat = CreateArray(char, zdat_size + 1);
    if (!dat) {
      LogWarning("could not load file '%s'", path);
      free(zdat);
      return NULL;
    }
    memcpy(dat, zdat, zdat_size);
    dat[zdat_size] = '\0';

    if (size) *size = zdat_size;

    free(zdat);

    LogDebug("loaded file '%s'", path);
    return dat;
  }
  return NULL;
}

void VfsDestroy(struct Vfs* vfs)
{
  if (vfs->type == VFS_ZIP) mz_zip_reader_end(&vfs->zip);
  Destroy(vfs->path);
}
