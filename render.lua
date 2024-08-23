local ecs = require("tiny")

local M = {}

function M.rectangle(obj)
  obj.rectangle = {
    w = 25,
    h = 25,
    color = {
      r = 0,
      g = 255,
      b = 0,
      a = 1
    }
  }
  return obj
end

M.SystemDrawRectangle = ecs.processingSystem()
M.SystemDrawRectangle.filter = ecs.requireAll("position", "rectangle")

function M.SystemDrawRectangle:process(e, _)
  local r, g, b, a = love.graphics.getColor()
  love.graphics.setColor(e.rectangle.color.r, e.rectangle.color.g, e.rectangle.color.b, e.rectangle.color.a)
  love.graphics.rectangle("fill", e.position.x, e.position.y, e.rectangle.w, e.rectangle.h)
  love.graphics.setColor(r, g, b, a)
end

return M
