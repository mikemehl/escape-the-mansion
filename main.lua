local sti = require("sti")
assert(Map)

local map = sti("assets/test-room.lua")
local character = require("assets.character.lua")
assert(map)
assert(character)


function love.load()
  local player_layer = map:convertToCustomLayer(3)
  assert(player_layer)
end

function love.update(dt)
  map:update(dt)
end

function love.draw()
  map:draw(0, 0, 4, 4)
end
