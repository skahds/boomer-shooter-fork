local LerpedNumber = {}
LerpedNumber.__index = LerpedNumber

local function CreateLerpedNumber(init)
  local n = setmetatable({}, LerpedNumber)
  n.val = init or 0
  return n
end

function LerpedNumber:get()
  return core.Interpolate(self.prev, self.val)
end

function LerpedNumber:getFinal()
  return self.val
end

function LerpedNumber:set(v)
  self.prev = self.val
  self.val = v
end

return CreateLerpedNumber
