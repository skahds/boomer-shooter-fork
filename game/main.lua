local tex = core.LoadTexture("res/textures/puzzle_cube.png")
tex:SetFilter(core.tex_filter.NEAREST, core.tex_filter.NEAREST)
-- tex:GenerateMipmaps()

local fmt = core.CreateVertexFormat()
fmt:AddAttrib("float", 3) -- position
fmt:AddAttrib("float", 2) -- uv
fmt:AddAttrib("float", 4) -- color

local shader = core.LoadShader("res/vdefault.glsl", "res/fdefault.glsl")

local cube = core.CreateMesh(fmt)
cube:SetVertices({
  {-0.5, -0.5, -0.5,  0, 0,  1, 0.5, 0, 1}, -- orange OK
  { 0.5, -0.5, -0.5,  1, 0,  1, 0.5, 0, 1},
  { 0.5,  0.5, -0.5,  1, 1,  1, 0.5, 0, 1},
  { 0.5,  0.5, -0.5,  1, 1,  1, 0.5, 0, 1},
  {-0.5,  0.5, -0.5,  0, 1,  1, 0.5, 0, 1},
  {-0.5, -0.5, -0.5,  0, 0,  1, 0.5, 0, 1},

  { 0.5,  0.5,  0.5,  1, 1,  1, 0, 0, 1}, -- 2
  { 0.5, -0.5,  0.5,  1, 0,  1, 0, 0, 1}, -- 1
  {-0.5, -0.5,  0.5,  0, 0,  1, 0, 0, 1}, -- red
  {-0.5, -0.5,  0.5,  0, 0,  1, 0, 0, 1}, -- 5
  {-0.5,  0.5,  0.5,  0, 1,  1, 0, 0, 1}, -- 4
  { 0.5,  0.5,  0.5,  1, 1,  1, 0, 0, 1}, -- 3

  {-0.5, -0.5, -0.5,  0, 1,  0, 1, 0, 1},
  {-0.5,  0.5, -0.5,  1, 1,  0, 1, 0, 1},
  {-0.5,  0.5,  0.5,  1, 0,  0, 1, 0, 1}, -- green
  {-0.5,  0.5,  0.5,  1, 0,  0, 1, 0, 1},
  {-0.5, -0.5,  0.5,  0, 0,  0, 1, 0, 1},
  {-0.5, -0.5, -0.5,  0, 1,  0, 1, 0, 1},

  { 0.5,  0.5,  0.5,  1, 0,  0, 0, 1, 1}, -- blue OK
  { 0.5,  0.5, -0.5,  1, 1,  0, 0, 1, 1},
  { 0.5, -0.5, -0.5,  0, 1,  0, 0, 1, 1},
  { 0.5, -0.5, -0.5,  0, 1,  0, 0, 1, 1},
  { 0.5, -0.5,  0.5,  0, 0,  0, 0, 1, 1},
  { 0.5,  0.5,  0.5,  1, 0,  0, 0, 1, 1},

  { 0.5, -0.5,  0.5,  1, 0,  1, 1, 1, 1},
  { 0.5, -0.5, -0.5,  1, 1,  1, 1, 1, 1},
  {-0.5, -0.5, -0.5,  0, 1,  1, 1, 1, 1}, -- yellow
  {-0.5, -0.5, -0.5,  0, 1,  1, 1, 1, 1},
  {-0.5, -0.5,  0.5,  0, 0,  1, 1, 1, 1},
  { 0.5, -0.5,  0.5,  1, 0,  1, 1, 1, 1},

  {-0.5,  0.5, -0.5,  0, 1,  1, 1, 0, 1}, -- white OK
  { 0.5,  0.5, -0.5,  1, 1,  1, 1, 0, 1},
  { 0.5,  0.5,  0.5,  1, 0,  1, 1, 0, 1},
  { 0.5,  0.5,  0.5,  1, 0,  1, 1, 0, 1},
  {-0.5,  0.5,  0.5,  0, 0,  1, 1, 0, 1},
  {-0.5,  0.5, -0.5,  0, 1,  1, 1, 0, 1}
})
cube:Finalize(true)

local LerpedNumber_mt = {}

function LerpedNumber_mt:__index(k)
  if k ~= "val" then
    error("cannot get any value other than 'val' on a lerped number")
  end

  return core.Interpolate(rawget(self, "prev"), rawget(self, "val"))
end

function LerpedNumber_mt:__newindex(k, v)
  if k ~= "val" then
    error("cannot set any value other than 'val' on a lerped number")
  end

  rawset(self, "prev", rawget(self, "val"))
  rawset(self, "val", v)
end

local function LerpedNumber(init)
  local n = setmetatable({}, LerpedNumber_mt)
  n.val = init or 0
  return n
end

local r = 0
local rx = LerpedNumber()
local ry = LerpedNumber()

function step()
  if core.IsKeyDown(core.key.ESCAPE) then
    core.CloseEngine()
  end

  if core.IsKeyDown(core.key.SPACE) then
    core.LogInfo(core.GetFps(), core.GetTps())
  end

  r = r + (1/30)
  rx.val = r * 0.25
  ry.val = r
end

function draw()
  shader:Bind()

  local cubet = {
    x = 0,
    y = 0,
    z = -3,
    rx = rx.val,
    ry = ry.val,
    rz = 0,
    sx = 1,
    sz = 1,
    sy = 1,
  }

  local m = core.Mat4FromTransform(cubet)

  local v = core.Mat4Identity()

  local sw, sh = core.GetScreenSize()
  local a = sw / sh

  local p = core.Mat4Identity()
  p:Perspective(45, a, 0.01, 100)

  shader:SendMat4("m", m)
  shader:SendMat4("v", v)
  shader:SendMat4("p", p)

  tex:Bind(0)
  shader:SendI("tex0", 0)

  cube:Draw()
end
