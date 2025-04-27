local tex = bse.LoadTexture("textures/puzzle_cube.png")

local format = bse.CreateMeshFormat()
format:AddAttr("float", 3) -- position
format:AddAttr("float", 2) -- uv
format:AddAttr("float", 4) -- color

local shader = bse.LoadShader("vdefault.glsl", "fdefault.glsl")

local cube = bse.CreateMesh(format)
cube:SetVertices({
  {-0.5, -0.5, -0.5,  0.0, 0.0,  1, 0.5, 0, 1}, -- orange
  { 0.5, -0.5, -0.5,  1.0, 0.0,  1, 0.5, 0, 1},
  { 0.5,  0.5, -0.5,  1.0, 1.0,  1, 0.5, 0, 1},
  { 0.5,  0.5, -0.5,  1.0, 1.0,  1, 0.5, 0, 1},
  {-0.5,  0.5, -0.5,  0.0, 1.0,  1, 0.5, 0, 1},
  {-0.5, -0.5, -0.5,  0.0, 0.0,  1, 0.5, 0, 1},

  {-0.5, -0.5,  0.5,  0.0, 0.0,  1, 0, 0, 1}, -- red
  { 0.5, -0.5,  0.5,  1.0, 0.0,  1, 0, 0, 1},
  { 0.5,  0.5,  0.5,  1.0, 1.0,  1, 0, 0, 1},
  { 0.5,  0.5,  0.5,  1.0, 1.0,  1, 0, 0, 1},
  {-0.5,  0.5,  0.5,  0.0, 1.0,  1, 0, 0, 1},
  {-0.5, -0.5,  0.5,  0.0, 0.0,  1, 0, 0, 1},

  {-0.5,  0.5,  0.5,  1.0, 0.0,  0, 1, 0, 1}, -- green
  {-0.5,  0.5, -0.5,  1.0, 1.0,  0, 1, 0, 1},
  {-0.5, -0.5, -0.5,  0.0, 1.0,  0, 1, 0, 1},
  {-0.5, -0.5, -0.5,  0.0, 1.0,  0, 1, 0, 1},
  {-0.5, -0.5,  0.5,  0.0, 0.0,  0, 1, 0, 1},
  {-0.5,  0.5,  0.5,  1.0, 0.0,  0, 1, 0, 1},

  { 0.5,  0.5,  0.5,  1.0, 0.0,  0, 0, 1, 1}, -- blue
  { 0.5,  0.5, -0.5,  1.0, 1.0,  0, 0, 1, 1},
  { 0.5, -0.5, -0.5,  0.0, 1.0,  0, 0, 1, 1},
  { 0.5, -0.5, -0.5,  0.0, 1.0,  0, 0, 1, 1},
  { 0.5, -0.5,  0.5,  0.0, 0.0,  0, 0, 1, 1},
  { 0.5,  0.5,  0.5,  1.0, 0.0,  0, 0, 1, 1},

  {-0.5, -0.5, -0.5,  0.0, 1.0,  1, 1, 0, 1}, -- yellow
  { 0.5, -0.5, -0.5,  1.0, 1.0,  1, 1, 0, 1},
  { 0.5, -0.5,  0.5,  1.0, 0.0,  1, 1, 0, 1},
  { 0.5, -0.5,  0.5,  1.0, 0.0,  1, 1, 0, 1},
  {-0.5, -0.5,  0.5,  0.0, 0.0,  1, 1, 0, 1},
  {-0.5, -0.5, -0.5,  0.0, 1.0,  1, 1, 0, 1},

  {-0.5,  0.5, -0.5,  0.0, 1.0,  1, 1, 1, 1}, -- white
  { 0.5,  0.5, -0.5,  1.0, 1.0,  1, 1, 1, 1},
  { 0.5,  0.5,  0.5,  1.0, 0.0,  1, 1, 1, 1},
  { 0.5,  0.5,  0.5,  1.0, 0.0,  1, 1, 1, 1},
  {-0.5,  0.5,  0.5,  0.0, 0.0,  1, 1, 1, 1},
  {-0.5,  0.5, -0.5,  0.0, 1.0,  1, 1, 1, 1}
})
cube:Finalize(true)

local t = 0

local r = 0
local prx = 0
local pry = 0
local rx = 0
local ry = 0

function step()
  t = t + (1/30)
  if t > 1 then
    t = t - 1
    bse.LogInfo(bse.GetFps(), bse.GetTps())
  end

  r = r + (1/30)
  prx = rx
  pry = ry
  rx = r * 0.25
  ry = r
end

function draw()
  shader:Bind()

  -- local transform = {
  --   x = 0,
  --   y = 0,
  --   z = -2,
  --   rx = bse.GetTotalTime() * 0.25,
  --   ry = bse.GetTotalTime(),
  --   rz = 0,
  -- }
  -- local m = bse.Mat4FromTransform(transform)
  local pos = bse.Mat4Identity()
  pos:Translate(0, 0, -3)

  local drx = bse.Interpolate(prx, rx)
  local dry = bse.Interpolate(pry, ry)

  local rot = bse.Mat4Identity()
  rot:Rotate(drx, dry, 0)

  local m = pos:Multiply(rot)

  local v = bse.Mat4Identity()

  local ww, wh = bse.GetWindowSize()
  local a = ww / wh

  local p = bse.Mat4Identity()
  p:Perspective(45, a, 0.01, 100)

  shader:SendMat4("m", m)
  shader:SendMat4("v", v)
  shader:SendMat4("p", p)

  tex:Bind(0)
  shader:SendI("tex0", 0)

  cube:Draw()
end
