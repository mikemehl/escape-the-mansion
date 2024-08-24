local tiny = require("lib.tiny")
local bump = require("lib.bump")

local M = {
  world = bump.newWorld(4),
  SystemBumpUpdate = tiny.processingSystem({ filter = tiny.requireAll("collisionBox", "position", "velocity") })
}

assert(M.world)

function M.SystemBumpUpdate:process(entity, _)
  local new_x, new_y = M.world:move(entity, entity.position.x + entity.velocity.x, entity.position.y + entity.velocity.y)
  entity.position.x = new_x
  entity.position.y = new_y
end

function M.position(obj)
  obj.position = {
    x = 0,
    y = 0
  }
  return obj
end

function M.velocity(obj)
  obj.velocity = {
    x = 0,
    y = 0
  }
  return obj
end

function M.collisionBox(obj, params)
  assert(obj.position)
  obj.collisionBox = params or {
    x_off = 0,
    y_off = 0,
    width = 0,
    height = 0,
  }
  return M.world:add(obj, obj.position.x + obj.collisionBox.x_off, obj.position.y + obj.collisionBox.y_off,
    obj.collisionBox.width, obj.collisionBox.height)
end

return M
