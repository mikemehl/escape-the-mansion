local ecs = require("lib.tiny")

local M = {
  SystemDrawRectangle = ecs.processingSystem({ filter = ecs.requireAll("position", "rectangle") }),
  SystemDrawAnimatedSprite = ecs.processingSystem({ filter = ecs.requireAll("position", "animatedSprite") })
}

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

function M.animatedSprite(obj, params)
  obj.animatedSprite = params or {
    frames = {},
    durations = {},
    curr_frame = 1,
    curr_duration = 0,
    enabled = false,
  }
  return obj
end

function M.SystemDrawRectangle:process(e, _)
  local r, g, b, a = love.graphics.getColor()
  love.graphics.setColor(e.rectangle.color.r, e.rectangle.color.g, e.rectangle.color.b, e.rectangle.color.a)
  love.graphics.rectangle("fill", e.position.x, e.position.y, e.rectangle.w, e.rectangle.h)
  love.graphics.setColor(r, g, b, a)
end

function M.SystemDrawAnimatedSprite:process(e, dt)
  if not e.animatedSprite.enabled then
    return
  end

  e.animatedSprite.curr_duration = e.animatedSprite.curr_duration + dt
  if e.animatedSprite.curr_duration > e.animatedSprite.durations[e.curr_frame] then
    e.animatedSprite.curr_frame = e.animatedSprite.curr_frame + 1
    if e.animatedSprite.curr_frame > #e.animatedSprite.frames then
      e.animatedSprite.curr_frame = 1
    end
  end

  love.graphics.draw(e.animatedSprite.frames[e.enimatedSprite.curr_frame], e.position.x, e.position.y)
end

return M
