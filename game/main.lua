package.path = "./?/init.lua;" .. package.path
if core.os == "windows" then
  package.path = package.path .. "game/?.lua;game/?/init.lua"
elseif core.os == "linux" then
  package.path = package.path .. ";game/?.lua;game/?/init.lua"
end
local gcore = require("core")

local tex = core.LoadTexture("res/textures/puzzle_cube.png")
tex:SetFilter(core.tex_filter.NEAREST, core.tex_filter.NEAREST)

local fmt = core.CreateVertexFormat()
fmt:AddAttrib("float", 3) -- position
fmt:AddAttrib("float", 3) -- normal
fmt:AddAttrib("float", 2) -- uv
fmt:AddAttrib("float", 4) -- color

local shader = core.LoadShader("res/vdefault.glsl", "res/fdefault.glsl")

local cube = core.CreateMesh(fmt)
local vp = {
  {0.5, 0.5, -0.5},
  {0.5, -0.5, -0.5},
  {0.5, 0.5, 0.5},
  {0.5, -0.5, 0.5},
  {-0.5, 0.5, -0.5},
  {-0.5, -0.5, -0.5},
  {-0.5, 0.5, 0.5},
  {-0.5, -0.5, 0.5},
}

-- 1 2
-- |/|
-- 3 4

-- 1 2 3  1 3 4
cube:SetVertices({
  {vp[7][1], vp[7][2], vp[7][3],  0, 1, 0,  1, 1,  1, 0.5, 0, 1},
  {vp[5][1], vp[5][2], vp[5][3],  0, 1, 0,  1, 0,  1, 0.5, 0, 1},
  {vp[1][1], vp[1][2], vp[1][3],  0, 1, 0,  0, 0,  1, 0.5, 0, 1}, -- orange OK
  {vp[3][1], vp[3][2], vp[3][3],  0, 1, 0,  0, 1,  1, 0.5, 0, 1},
  {vp[7][1], vp[7][2], vp[7][3],  0, 1, 0,  1, 1,  1, 0.5, 0, 1},
  {vp[1][1], vp[1][2], vp[1][3],  0, 1, 0,  0, 0,  1, 0.5, 0, 1},

  -- 4 3 7 8
  {vp[7][1], vp[7][2], vp[7][3],  0, 0, 1,  0, 1,  0, 0, 1, 1}, -- red
  {vp[3][1], vp[3][2], vp[3][3],  0, 0, 1,  1, 1,  0, 0, 1, 1}, -- 1
  {vp[4][1], vp[4][2], vp[4][3],  0, 0, 1,  1, 0,  0, 0, 1, 1}, -- 2
  {vp[8][1], vp[8][2], vp[8][3],  0, 0, 1,  0, 0,  0, 0, 1, 1}, -- 3
  {vp[7][1], vp[7][2], vp[7][3],  0, 0, 1,  0, 1,  0, 0, 1, 1}, -- 4
  {vp[4][1], vp[4][2], vp[4][3],  0, 0, 1,  1, 0,  0, 0, 1, 1}, -- 5

  -- 8 7 5 6
  {vp[5][1], vp[5][2], vp[5][3],  -1, 0, 0,  0, 1,  1, 1, 0, 1}, -- green
  {vp[7][1], vp[7][2], vp[7][3],  -1, 0, 0,  1, 1,  1, 1, 0, 1},
  {vp[8][1], vp[8][2], vp[8][3],  -1, 0, 0,  1, 0,  1, 1, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  -1, 0, 0,  0, 0,  1, 1, 0, 1},
  {vp[5][1], vp[5][2], vp[5][3],  -1, 0, 0,  0, 1,  1, 1, 0, 1},
  {vp[8][1], vp[8][2], vp[8][3],  -1, 0, 0,  1, 0,  1, 1, 0, 1},

  -- 6 2 4 8
  {vp[4][1], vp[4][2], vp[4][3],  0, -1, 0,  0, 1,  1, 0, 0, 1},
  {vp[2][1], vp[2][2], vp[2][3],  0, -1, 0,  1, 1,  1, 0, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, -1, 0,  1, 0,  1, 0, 0, 1}, -- blue OK
  {vp[8][1], vp[8][2], vp[8][3],  0, -1, 0,  0, 0,  1, 0, 0, 1},
  {vp[4][1], vp[4][2], vp[4][3],  0, -1, 0,  0, 1,  1, 0, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, -1, 0,  1, 0,  1, 0, 0, 1},

  -- 2 1 3 4
  {vp[3][1], vp[3][2], vp[3][3],  1, 0, 0,  0, 1,  1, 1, 1, 1}, -- white
  {vp[1][1], vp[1][2], vp[1][3],  1, 0, 0,  1, 1,  1, 1, 1, 1},
  {vp[2][1], vp[2][2], vp[2][3],  1, 0, 0,  1, 0,  1, 1, 1, 1},
  {vp[4][1], vp[4][2], vp[4][3],  1, 0, 0,  0, 0,  1, 1, 1, 1},
  {vp[3][1], vp[3][2], vp[3][3],  1, 0, 0,  0, 1,  1, 1, 1, 1},
  {vp[2][1], vp[2][2], vp[2][3],  1, 0, 0,  1, 0,  1, 1, 1, 1},

  -- 6 5 1 2
  {vp[1][1], vp[1][2], vp[1][3],  0, 0, -1,  0, 1,  0, 1, 0, 1},
  {vp[5][1], vp[5][2], vp[5][3],  0, 0, -1,  1, 1,  0, 1, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, 0, -1,  1, 0,  0, 1, 0, 1}, -- yellow OK
  {vp[2][1], vp[2][2], vp[2][3],  0, 0, -1,  0, 0,  0, 1, 0, 1},
  {vp[1][1], vp[1][2], vp[1][3],  0, 0, -1,  0, 1,  0, 1, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, 0, -1,  1, 0,  0, 1, 0, 1},
})
cube:Finalize(true)

local r = 0
local rx = gcore.CreateLerpedNumber()
local ry = gcore.CreateLerpedNumber()

function step()
  if core.IsKeyDown(core.key.ESCAPE) then
    core.CloseEngine()
  end

  if core.IsKeyDown(core.key.SPACE) then
    core.LogInfo(core.GetFps(), core.GetTps())
  end

  r = r + (1/30)
  rx:set(r * 0.25)
  ry:set(r)
end

function draw()
  shader:Bind()

  local cubet = {
    x = 0,
    y = 0,
    z = -3,
    -- rx = 0,
    rx = rx:get(),--math.rad(-24),
    ry = ry:get(),--math.rad(45),
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
  p:Perspective(45, a, 1, 100)

  shader:SendMat4("m", m)
  shader:SendMat4("v", v)
  shader:SendMat4("p", p)

  tex:Bind(0)
  shader:SendI("tex0", 0)

  cube:Draw()
end
