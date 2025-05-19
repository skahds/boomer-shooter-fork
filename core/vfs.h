#ifndef __engine_vfs__
#define __engine_vfs__

#include "include.h"

#include "miniz.h"

enum VfsError
{
  VFS_OK = 0,
  VFS_COULD_NOT_MOUNT,
  VFS_FILE_NOT_FOUND,
  VFS_OUT_OF_MEM,
  VFS_MALFORMED_PATH,
  VFS_CANNOT_OPEN,
  VFS_CANNOT_WRITE,
  VFS_CANNOT_READ,
};

enum VfsType
{
  VFS_DIR,
  VFS_ZIP,
};

struct Vfs
{
  struct Vfs* next;
  enum VfsType type;
  mz_zip_archive zip;
  char* path;
};

enum VfsError VfsInit(struct Vfs* vfs, const char* path);
void VfsDestroy(struct Vfs* vfs);
bool VfsDoesFileExist(struct Vfs* vfs, const char* path);
// read the entirity of a file
char* VfsReadFile(struct Vfs* vfs, const char* path, size_t* size);
// same as `VfsReadFile`, except it adds a null terminator
char* VfsReadTxtFile(struct Vfs* vfs, const char* path, size_t* size);

#endif
