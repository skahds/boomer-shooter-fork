CC = gcc

INCLUDE = \
	-Icore -Ilib/glad/include -Ilib/miniz/include \
	-Ilib/stb/include -Ilib/luajit -Ilib 
CFLAGS = -std=c99 -Wextra $(INCLUDE) -Dbse_allow_opengl
LDFLAGS =

PROJECT_NAME = DEMONCHIME

EXE = $(PROJECT_NAME)
# HAD files store game content
# this one contains every core resource and script
OBJ = \
	main.o core/include.o core/log.o core/engine.o core/image.o core/prng.o \
	core/timer.o core/vfs.o core/math/mat4.o core/math/transform.o \
	core/wrap/wrap.o core/wrap/wrap_enums.o core/wrap/wrap_engine.o \
	core/wrap/wrap_log.o core/wrap/wrap_mat4.o core/wrap/wrap_mesh.o \
	core/wrap/wrap_vertex_format.o core/wrap/wrap_shader.o \
	core/wrap/wrap_texture.o core/wrap/wrap_prng.o core/gfx/buffer_object.o \
	core/gfx/framebuffer.o core/gfx/gfx.o core/gfx/gfx_types.o core/gfx/mesh.o \
	core/gfx/shader.o core/gfx/texture.o core/gfx/vertex_array.o \
	core/gfx/opengl/gl_buffer_object.o core/gfx/opengl/gl_framebuffer.o \
	core/gfx/opengl/gl_gfx.o core/gfx/opengl/gl_shader.o \
	core/gfx/opengl/gl_texture.o core/gfx/opengl/gl_type_conv.o \
	core/gfx/opengl/gl_vertex_array.o lib/glad/src/glad.o lib/stb/stb.o

CORE_HAD = CORE.HAD
CORE_HAD_DIR = core/had
CORE_HAD_FILES = \
	core/init.lua core/LerpedNumber.lua \
	res/fdefault.glsl res/vdefault.glsl res/ffbdraw.glsl res/vfbdraw.glsl

CLEAN_FILES = $(OBJ) $(OBJ:%.o=%.d) $(EXE) $(CORE_HAD)

RM = rm -f

ifeq (,$(findstring Windows,$(OS)))
	HOST_SYS = $(shell uname -s)
else
	HOST_SYS = Windows
	CLEAN_FILES := $(subst /,\,$(CLEAN_FILES))
	RM = del
endif

ifeq ($(config),release)
	CFLAGS += -O2 -Dbse_release
else
	config = debug
	CFLAGS += -O2 -g -Dbse_debug
	ifeq ($(HOST_SYS),Linux)
		CFLAGS += -fsanitize=address
	endif
endif

ifeq (Linux,$(HOST_SYS))
	CFLAGS += -Dbse_linux
	LDFLAGS += -lluajit -lglfw -lm -L.
endif
ifeq (Windows,$(HOST_SYS))
	CFLAGS += -Dbse_windows
	LDFLAGS += -llua51 -lglfw3 -L.
endif

.PHONY: all clean compile_flags

all: $(EXE) $(CORE_HAD)

$(EXE): $(OBJ)
	@echo "cc $@"
	@$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDFLAGS)

%.o: %.c
	@echo "cc $< -> $@"
	@$(CC) -o $@ -c $< $(CFLAGS) -MMD

$(CORE_HAD): $(CORE_HAD_FILES:%=$(CORE_HAD_DIR)/%)
	@echo "had $@"
ifeq (Linux,$(HOST_SYS))
	@$(RM) CORE.HAD
	@(cd $(CORE_HAD_DIR) && zip -r - $(CORE_HAD_FILES)) > $(CORE_HAD)
endif
# TODO: handle windows

clean:
	$(RM) $(CLEAN_FILES)

compile_flags:
	@echo "" > compile_flags.txt
	@$(foreach flag,$(CFLAGS),echo $(flag) >> compile_flags.txt;)

-include $(OBJ:%.o=%.d)
