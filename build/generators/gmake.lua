local cflags = {
  ["debug"] = {"-g"},
  ["releasesym"] = {"-O2", "-g"},
  ["release"] = {"-O2"},
}

ctx.cflags = cflags[ctx.config]

local function GMakeVar(name, val)
  return name .. " = " .. val .. "\n"
end

local function GMakeCollectCFlags(target)
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
    flags = flags .. "-fPIC "
  end

  return flags
end

local function CollectTable(t)
  local str = ""
  for _, v in ipairs(t) do
    str = str .. v .. " "
  end
  return str
end

local function GMakeCompile(target)
  local src = target.output .. ": $(OBJ) $(LIBS)\n"
  src = src .. "\t@$(MKDIR) $(@D)\n"

  if target.type == "exe" then

    src = src .. "\t@echo \"cc $@\"\n"
    src = src
      .. "\t@$(CC) $(OBJ)"
      .. " -o " .. target.output
      .. " $(LIBS)"
      .. " $(CFLAGS)"
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
      .. "\t@$(CC) $(OBJ)"
      .. " -shared -o " .. target.output
      .. " $(LIBS)"
      .. " $(CFLAGS)"
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

local function GMakeClean(target)
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

local function GMakePad(n)
  n = n or 1
  return ("\n"):rep(n)
end

local function GMakeTargetMakefile(target)
  return ctx.target_dir .. target.name .. ".make"
end

local function GenerateTarget(target)
  print("generating target '" .. target.name .. "'")

  local src = ""
  src = src .. GMakeVar("CC", "gcc")
  src = src .. GMakeVar("AR", "ar rcs")
  src = src .. GMakeVar("RM", "rm -f")
  src = src .. GMakeVar("MKDIR", "mkdir -p")
  src = src .. GMakeVar("CD", "cd")
  src = src .. GMakePad()
  src = src .. GMakeVar("CFLAGS", GMakeCollectCFlags(target))
  src = src .. GMakeVar("LIBS", CollectTable(target.libs or {}))
  src = src .. GMakeVar("OBJ", CollectTable(target.objs))

  src = src .. GMakePad(2)

  src = src .. ".PHONY: default clean\n"
  src = src .. GMakePad()

  src = src .. "default: " .. target.output .. "\n"
  src = src .. GMakePad()

  src = src .. GMakeCompile(target)
  src = src .. GMakePad()

  if target.type ~= "command" then
    src = src .. ctx.target_dir .. "%.o: %.c\n"
      .. "\t@$(MKDIR) $(@D)\n"
      .. "\t@echo \"cc $< -> $@\"\n"
      .. "\t@$(CC) -c $< -o $@ $(CFLAGS) -MMD\n"
    src = src .. GMakePad()
  end

  src = src .. GMakeClean(target)

  src = src .. "-include " .. CollectTable(target.d_files or {})

  local makefile = io.open(GMakeTargetMakefile(target), "w")
  if not makefile then
    LMakeError(
      "could not write makefile for '" .. target.name .. "' "
      .. "at '" .. GMakeTargetMakefile(target) .. "'"
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
  src = src .. GMakePad()

  src = src .. "default: all\n"
  src = src .. GMakePad()

  src = src .. "all: "

  for _, target in pairs(ctx.targets) do
    src = src .. target.output .. " "
  end
  src = src .. "\n"
  src = src .. GMakePad()

  for _, target in pairs(ctx.targets) do
    local deps = CollectTable(target.deps)
    local files = CollectTable(target.src or {})
    src = src .. target.output .. ": " .. deps .. " " .. files
    src = src .. "\n"
    src = src .. "\t@echo \"making " .. target.name .. "\"\n"
    src = src
      .. "\t@$(MAKE) --no-print-directory -f "
      .. GMakeTargetMakefile(target) .. "\n"
    src = src .. GMakePad()
  end

  src = src .. "clean:\n"

  for _, target in pairs(ctx.targets) do
    src = src
      .. "\t@$(MAKE) --no-print-directory -f "
      .. GMakeTargetMakefile(target) .. " clean\n"
  end

  local makefile = io.open(ctx.target_dir .. "Makefile", "w")
  if not makefile then
    LMakeError("could not write makefile")
    return
  end
  makefile:write(src)
end


return GenerateGMake
