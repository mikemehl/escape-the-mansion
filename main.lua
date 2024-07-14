local sti = require("sti")

local map = sti("assets/test-room.lua")
assert(map)


function love.load()
end

function love.update(dt)
  map:update(dt)
end

function love.draw()
  map:draw()
end
