---@class Physics
local M = {}


---@class Position
---@type fun(params: Vector2?): Position
M.Position = ECS.Component(function(params)
  return {
    x = params.x or 0,
    y = params.y or 0,
  }
end)

---@class Velocity
---@type fun(params: Vector2?): Velocity
M.Velocity = ECS.Component(function(params)
  return {
    x = params.x or 0,
    y = params.y or 0,
  }
end)

---@class CollisionBox
---@type fun(params: Rectangle): CollisionBox
M.CollisionBox = ECS.Component(function(params)
  return {
    x = params.x or 0,
    y = params.y or 0,
    width = params.width or 0,
    height = params.height or 0,
  }
end)


return M
