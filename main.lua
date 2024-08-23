local concord = require("concord")
local sti = require("sti")

local map = nil

function love.load()
  map = sti("assets/test-room.lua")
  assert(map)
end

function love.update(dt)
  assert(map)
  map:update(dt)
  if love.keyboard.isDown("q") then
    love.event.quit(0)
  end
end

function love.draw()
  assert(map)
  love.graphics.print("HELLO WORLD", 50, 50)
  map:draw()
end
