local cflags = {
  ["debug"] = {"-g"},
  ["releasesym"] = {"-O2", "-g"},
  ["release"] = {"-O2"},
}

ctx.cflags = cflags[ctx.config]

local function GenVar(name, val)
  return name .. " = " .. val .. "\n"
end

local function CollectCFlags(target)
  local flags = ""

  for _, flag in ipairs(ctx.cflags) do
    flags = flags .. flag .. " "
  end

  for _, flag in ipairs(target.opt.flags or {}) do
    if type(flag) == "string" then
      flags = flags .. flag .. " "
    else
      LMakeError("invalid type in flags for target '" .. target.name .. "'")
      os.exit(1)
    end
  end

  for _, include in ipairs(target.opt.include or {}) do
    if type(include) == "string" then
      flags = flags .. "-I" .. include .. " "
    else
      LMakeError("invalid type in include for target '" .. target.name .. "'")
      os.exit(1)
    end
  end

  for _, include in ipairs(target.opt.define or {}) do
    if type(include) == "string" then
      flags = flags .. "-D" .. include .. " "
    else
      LMakeError("invalid type in include for target '" .. target.name .. "'")
      os.exit(1)
    end
  end

  if target.type == "staticlib" then
    flags = flags .. "-c "
  elseif target.type == "sharedlib" then
    flags = flags .. "-fpic "
  end

  return flags
end

local function CollectLibs(target)
  local libs = ""
  for _, lib in ipairs(target.opt.libs or {}) do
    if ctx.targets[lib] then
      libs = libs .. ctx.targets[lib].output .. " "
    else
      libs = libs .. "-l" .. lib .. " "
    end
  end
  return libs
end

local function CollectTable(t)
  local str = ""
  for _, v in ipairs(t) do
    str = str .. v .. " "
  end
  return str
end

local function GenTargetCompilation(target)
  local src = target.output .. ": $(OBJ) $(LIBS)\n"
  src = src .. "\t@$(MKDIR) $(@D)\n"

  if target.type == "exe" then

    src = src .. "\t@echo \"cc $@\"\n"
    src = src
      .. "\t@$(CC) -o " .. target.output
      .. " $(OBJ) $(LIBS) $(CFLAGS) $(LDFLAGS)"
    src = src .. "\n"

  elseif target.type == "staticlib" then

    src = src .. "\t@echo \"ar $@\"\n"
    src = src
      .. "\t@$(AR) "
      .. target.output
      .. " $(OBJ) $(LIBS)"
    src = src .. "\n"

  elseif target.type == "sharedlib" then

    src = src .. "\t@echo \"cc $@\"\n"
    src = src
      .. "\t@$(CC) -shared -o " .. target.output
      .. " $(OBJ) $(LIBS) $(CFLAGS) $(LDFLAGS)"
    src = src .. "\n"

  elseif target.type == "command" then

    for _, command in ipairs(target.opt.commands) do
      if type(command) ~= "string" then
        LMakeError("Command for target '" .. target.name .. "' is not a string")
        return src
      end
      src = src .. "\t" .. command .. "\n"
    end

    src = src .. "\n"

  end

  return src
end

local function GenClean(target)
  local clean = "clean:\n"
  clean = clean .. "\t$(RM) " .. target.output .. " $(OBJ)\n"

  if target.type == "command" then
    for _, command in ipairs(target.opt.clean_commands) do
      if type(command) ~= "string" then
        LMakeError(
          "clean command for target '" .. target.name .. "' is not a string"
        )
        return clean
      end
      clean = clean .. "\t" .. command .. "\n"
    end

    clean = clean .. "\n"
  end

  return clean
end

local function Pad(n)
  n = n or 1
  return ("\n"):rep(n)
end

local function GetTargetMakefilePath(target)
  return ctx.target_dir .. target.name .. ".make"
end

local function GenerateTarget(target)
  print("generating target '" .. target.name .. "'")

  local src = ""
  src = src .. GenVar("CC", "gcc")
  src = src .. GenVar("AR", "ar rcs")
  src = src .. GenVar("RM", "rm -f")
  src = src .. GenVar("MKDIR", "mkdir -p")
  src = src .. GenVar("CD", "cd")
  src = src .. Pad()
  src = src .. GenVar("CFLAGS", CollectCFlags(target))
  src = src .. GenVar("LDFLAGS", CollectTable(target.opt.ldflags or {}))
  src = src .. GenVar("LIBS", CollectLibs(target))
  src = src .. GenVar("OBJ", CollectTable(target.objs))

  src = src .. Pad(2)

  src = src .. ".PHONY: default clean\n"
  src = src .. Pad()

  src = src .. "default: " .. target.output .. "\n"
  src = src .. Pad()

  src = src .. GenTargetCompilation(target)
  src = src .. Pad()

  if target.type ~= "command" then
    src = src .. ctx.target_dir .. "%.o: %.c\n"
      .. "\t@$(MKDIR) $(@D)\n"
      .. "\t@echo \"cc $< -> $@\"\n"
      .. "\t@$(CC) -c $< -o $@ $(CFLAGS) -MMD\n"
    src = src .. Pad()
  end

  src = src .. GenClean(target)

  src = src .. "-include " .. CollectTable(target.d_files or {})

  local makefile = io.open(GetTargetMakefilePath(target), "w")
  if not makefile then
    LMakeError(
      "could not write makefile for '" .. target.name .. "' "
      .. "at '" .. GetTargetMakefilePath(target) .. "'"
    )
    return
  end
  makefile:write(src)
end

local function GenerateGMake()
  -- generate gmake vars
  for _, target in pairs(ctx.targets) do
    GenerateTarget(target)
  end

  print("finalizing")

  local src = ""

  src = src .. ".PHONY: default all clean\n"
  src = src .. Pad()

  src = src .. "default: all\n"
  src = src .. Pad()

  src = src .. "all: "

  for _, target in pairs(ctx.targets) do
    src = src .. target.output .. " "
  end
  src = src .. "\n"
  src = src .. Pad()

  for _, target in pairs(ctx.targets) do
    local deps = CollectTable(target.deps)
    local files = CollectTable(target.opt.src or {})
    src = src .. target.output .. ": " .. deps .. " " .. files
    src = src .. "\n"
    src = src .. "\t@echo \"making " .. target.name .. "\"\n"
    src = src
      .. "\t@$(MAKE) --no-print-directory -f "
      .. GetTargetMakefilePath(target) .. "\n"
    src = src .. Pad()
  end

  src = src .. "clean:\n"

  for _, target in pairs(ctx.targets) do
    src = src
      .. "\t@$(MAKE) --no-print-directory -f "
      .. GetTargetMakefilePath(target) .. " clean\n"
  end

  local makefile = io.open(ctx.target_dir .. "Makefile", "w")
  if not makefile then
    LMakeError("could not write makefile")
    return
  end
  makefile:write(src)
end


return GenerateGMake
