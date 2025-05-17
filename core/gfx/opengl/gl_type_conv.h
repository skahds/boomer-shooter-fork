#ifndef __engine_gfx_opengl_gl_type_conv__
#define __engine_gfx_opengl_gl_type_conv__

#include "include.h"
#include "gfx/gfx_types.h"

uint32_t BufferObjectTypeToOpenGl(enum BufferObjectType type);
uint32_t DrawModeToOpenGl(enum DrawMode mode);
uint32_t GfxDataTypeToOpenGl(enum GfxDataType type);
uint32_t IndexModeToOpenGl(enum IndexMode mode);
uint32_t ImageFormatToOpenGl(enum ImageFormat format);
uint32_t TextureFilterToOpenGl(enum TextureFilter filter);
uint32_t TextureWrapToOpenGl(enum TextureWrap wrap);

#endif
