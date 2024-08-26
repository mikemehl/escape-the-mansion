local ecs = require("lib.tiny")
local anim8 = require("lib.anim8")
local gamera = require("lib.gamera")

local M = {
  SystemDrawRectangle = ecs.processingSystem({ filter = ecs.requireAll("position", "rectangle") }),
  SystemDrawAnimatedSprite = ecs.processingSystem({ filter = ecs.requireAll("position", "animatedSprite") }),
  camera = gamera.new(0, 0, 640, 480),
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

function M.animatedSprite(obj, image_path, gridSize, tileRows, tileColumn)
  local image = love.graphics.newImage(image_path)
  assert(image)
  local grid = anim8.newGrid(gridSize, gridSize, image:getWidth(), image:getHeight())
  assert(grid)
  local animation = anim8.newAnimation(grid(tileRows, tileColumn), 1)
  assert(animation)
  obj.animatedSprite = {
    image = image,
    grid = grid,
    animation = animation,
    enabled = true,
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

  e.animatedSprite.animation:update(dt)
  e.animatedSprite.animation:draw(e.animatedSprite.image, e.position.x, e.position.y)
end

function M:init()
  self.camera:setScale(4.0)
end

return M
