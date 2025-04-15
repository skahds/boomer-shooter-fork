#include "context.h"

#include "luawrap.h"
#include "prng.h"

char* readFile(const char* path)
{
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    logError("could not open file '%s'", path);
    return NULL;
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char* buf = createArr(char, file_size + 1);
  if (buf == NULL) {
    logError("error: file '%s' too large to read", path);
    return NULL;
  }

  size_t bytes_read = fread(buf, sizeof(char), file_size, file);
  if (bytes_read < file_size) {
    destroy(buf);
    logError("error: could not read file '%s'", path);
    return NULL;
  }

  buf[bytes_read] = '\0';

  fclose(file);
  return buf;
}

int main()
{
  Context* ctx = ctxCreate("Boomer Shooter", (Vec2i){320, 240});

  lua_State* L = initLua();

  wrapLog(L);
  wrapContext(L, ctx);
  wrapInput(L);
  wrapMesh(L);
  wrapMaterial(L);
  wrapTexture(L);
  wrapPrng(L);

  protectedDoFile(L, "game/main.lua");

  while (ctxShouldDoUpdate(ctx)) {
    ctxUpdate(ctx);
    ctxDraw(ctx);
  }

  ctxDestroy(ctx);
  lua_close(L);
  return 0;
}
