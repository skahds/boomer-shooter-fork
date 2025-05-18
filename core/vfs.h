#ifndef __engine_vfs__
#define __engine_vfs__

#include "include.h"

#define fseeko fseek
#define ftello ftell
#define fseeko64 fseek
#define ftello64 ftell
#include "miniz.h"

enum VfsError
{
  VFS_OK = 0,
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
  enum VfsType type;
  mz_zip_archive zip;
  char* path;
};

enum VfsError VfsInit(struct Vfs* vfs, const char* path);
void VfsDestroy(struct Vfs* vfs);
// read the entirity of a file
char* VfsReadFile(struct Vfs* vfs, const char* path, size_t* size);

#endif
