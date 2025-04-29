local tex = core.LoadTexture("textures/puzzle_cube.png")
tex:SetFilter(core.tex_filter.NEAREST_MIPMAP, core.tex_filter.NEAREST)
tex:GenerateMipmaps()

local fmt = core.CreateVertexFormat()
fmt:AddAttrib("float", 3) -- position
fmt:AddAttrib("float", 2) -- uv
fmt:AddAttrib("float", 4) -- color

local shader = core.LoadShader("vdefault.glsl", "fdefault.glsl")

local cube = core.CreateMesh(fmt)
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
    -- bse.LogInfo(bse.GetFps(), bse.GetTps())
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
  local pos = core.Mat4Identity()
  pos:Translate(0, 0, -3)

  local drx = core.Interpolate(prx, rx)
  local dry = core.Interpolate(pry, ry)

  local rot = core.Mat4Identity()
  rot:Rotate(drx, dry, 0)

  local m = pos:Multiply(rot)

  local v = core.Mat4Identity()

  local ww, wh = core.GetWindowSize()
  local a = ww / wh

  local p = core.Mat4Identity()
  p:Perspective(45, a, 0.01, 100)

  shader:SendMat4("m", m)
  shader:SendMat4("v", v)
  shader:SendMat4("p", p)

  tex:Bind(0)
  shader:SendI("tex0", 0)

  cube:Draw()
end
