local sti = require("sti")

local map = sti("assets/test-room.lua")
assert(map)


function love.load()
  local layer = map:addCustomLayer("Sprites", 4)
  local player
  for _, obj in pairs(map.objects) do
    if obj.name == "PlayerStart" then
      player = obj
      break
    end
  end

  layer.player = {
    x = player.x,
    y = player.y
  }

  layer.draw = function(self)
    love.graphics.rectangle("fill", self.player.x, self.player.y, 8, 8)
  end
end

function love.update(dt)
  map:update(dt)
end

function love.draw()
  map:draw(0, 0, 4, 4)
end
