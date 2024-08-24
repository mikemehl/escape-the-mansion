local sti = require("lib.sti")

local M = {
  map = sti("assets/test-room.lua")
}

assert(M.map)

function M:init()
  self.player_start = {
    x = self.map.layers["Points"].objects[1].x,
    y = self.map.layers["Points"].objects[1].y,
  }
end

function M:update(dt)
  self.map:update(dt)
end

function M:draw()
  self.map:draw()
end

return M
