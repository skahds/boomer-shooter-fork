function LMakeError(msg)
  io.stderr:write(msg .. "\n")
end

ctx = {}

ctx.config = "debug"
ctx.gen_type = "gmake"

-- collect args
do
  local i = 1
  while arg[i] ~= nil do
    local v = arg[i]

    if v == "-type" then
      ctx.gen_type = arg[i + 1]
      if not ctx.gen_type then
        LMakeError("invalid parameters, -type expects an arg")
        return
      end
    end

    if v == "-config" then
      ctx.config = arg[i + 1]
      if not ctx.config then
        LMakeError("invalid parameters, -config expects an arg")
        return
      end
    end

    if v == "-help" then
      ctx.help = true
    end

    i = i + 1
  end
end

if ctx.config ~= "debug"
and ctx.config ~= "release"
and ctx.config ~= "releasesym" then
  LMakeError(
    "invalid config type. expected 'debug', 'release', or 'releasesym'"
  )
  return
end

print("config: " .. ctx.config)

ctx.target_dir = "bin/" .. ctx.config .. "/"
ctx.targets = {}

local generators = {
  gmake = require("build.generators.gmake")
}

local function GetFileExt(file)
  return file:match("%.([^/\\]*)$")
end

-- c files should compile to object files, which are what should be used as
-- dependencies, so this function just takes a file and replaces the extension
-- with a .o one if it's a c file
local function ReplaceExt(src_file, ext)
  return ctx.target_dir .. src_file:gsub("%.([^/\\]*)$", ext)
end

local function ResolveTargetOutput(target)
  if target.type == "staticlib" then
    return ctx.target_dir .. "lib" .. target.name .. ".a"
  elseif target.type == "sharedlib" then
    return ctx.target_dir .. "lib" .. target.name .. ".so"
  elseif target.type == "exe" then
    return ctx.target_dir .. target.name
  elseif target.type == "command" then
    return target.opt.output_binary
  else
    LMakeError(
      "invalid target type '"
      .. target.type .. "' for target '" .. target.name .. "'"
    )
    return ctx.target_dir .. target.name
  end
end

function SetTargetDir(dir)
  ctx.target_dir = dir .. "/" .. ctx.config .. "/"
end

function AddTarget(target_name, type, opt)
  local target = {
    name = target_name,
    type = type,
    opt = opt,
  }
  target.output = ResolveTargetOutput(target)
  ctx.targets[target_name] = target
end

function Include(directory)
  local path = directory
  if path:sub(#path, #path) ~= "/" then
    path = path .. "/"
  end

  local chunk, err = loadfile(path .. "build.lua")
  if not chunk then
    LMakeError("error loading '" .. path .. "': " .. err)
    return
  end

  chunk()
end

--- Figures out what targets depend on what other targets
local function ResolveDependencies()
  for _, target in pairs(ctx.targets) do
    local objs = {}
    local deps = {}
    local d_files = {}

    local libs = target.opt.libs or {}
    for _, lib_name in ipairs(libs) do
      if ctx.targets[lib_name] then -- this is not a system library
        table.insert(deps, ctx.targets[lib_name].output)
      end
    end

    local src = target.opt.src or {}
    for _, file in ipairs(src) do
      if GetFileExt(file) == "c" then
        table.insert(objs, ReplaceExt(file, ".o"))
        table.insert(d_files, ReplaceExt(file, ".d"))
      end
    end

    target.objs = objs
    target.deps = deps
    target.d_files = d_files
  end
end

do
  Include(".")
  ResolveDependencies()
  generators[ctx.gen_type]()
end
