// based on https://github.com/rxi/juno/blob/master/src/fs.c

#include "vfs.h"

#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "mem.h"

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

static char* Concat(const char* lhs, ...)
{
  const char* str;

  va_list args;
  va_start(args, lhs);

  size_t len = strlen(lhs);

  while ((str = va_arg(args, const char*))) {
    len += strlen(str);
  }

  va_end(args);

  char* cat = CreateArray(char, len + 1);

  va_start(args, lhs);

  strcpy(cat, lhs);
  while ((str = va_arg(args, const char*))) {
    strcat(cat, str);
  }
  
  va_end(args);

  return cat;
}

// removes ./ from the beginning of paths
static const char* RemoveCwdPrefix(const char* path)
{
  if (path[0] == '.' && path[1] == '/') return path + 2;
  return path;
}

// checks if a path is a dir
static bool IsPathDir(const char* path)
{
  struct stat f;
  int res = stat(path, &f);
  return res == 0 && S_ISDIR(f.st_mode);
}

enum VfsError VfsInit(struct Vfs* vfs, const char* path)
{
  bool is_dir = IsPathDir(path);
  mz_zip_archive zip;
  if (!is_dir && !mz_zip_reader_init_file(&zip, path, 0))
    return VFS_MALFORMED_PATH;

  size_t len = strlen(path);
  vfs->path = CreateArray(char, len + 1);
  strcpy(vfs->path, path);

  if (vfs->path[len - 1] == '/') vfs->path[len - 1] = '\0';

  if (is_dir) {
    vfs->type = VFS_DIR;
  } else {
    vfs->type = VFS_ZIP;
    vfs->zip = zip;
  }
  
  return VFS_OK;
}

char* VfsReadFile(struct Vfs* vfs, const char* path, size_t* size)
{
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
  } else {
    char* dat = mz_zip_reader_extract_file_to_heap(&vfs->zip, lpath, size, 0);

    LogDebug("loaded file '%s'");
    if (dat) return dat;
  }
  return NULL;
}

void VfsDestroy(struct Vfs* vfs)
{
  if (vfs->type == VFS_ZIP) {
    mz_zip_reader_end(&vfs->zip);
  }
  Destroy(vfs->path);
}
