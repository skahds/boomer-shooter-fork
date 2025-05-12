#include "include.h"
#include "mem.h"

char* ReadFile(const char* path)
{
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    LogError("could not open file '%s'", path);
    return NULL;
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char* buf = CreateArray(char, file_size + 1);
  if (buf == NULL) {
    LogError("error: file '%s' too large to read", path);
    return NULL;
  }

  size_t bytes_read = fread(buf, sizeof(char), file_size, file);
  if (bytes_read < file_size) {
    Destroy(buf);
    LogError("error: could not read file '%s'", path);
    return NULL;
  }

  buf[bytes_read] = '\0';

  fclose(file);

  LogDebug("loaded file '%s'", path);
  return buf;
}
