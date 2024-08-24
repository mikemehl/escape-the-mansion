local tiny = require("lib.tiny")

local M = {}

M.SystemGatherInput = tiny.system()
M.SystemGatherInput.left = false
M.SystemGatherInput.right = false
M.SystemGatherInput.up = false
M.SystemGatherInput.down = false


function M.SystemGatherInput:update(_)
  self.left = love.keyboard.isDown("a")
  self.right = love.keyboard.isDown("d")
  self.up = love.keyboard.isDown("w")
  self.down = love.keyboard.isDown("s")
  if love.keyboard.isDown("q") then
    love.event.quit(0)
  end
end

function M:get()
  return {
    left = self.SystemGatherInput.left,
    right = self.SystemGatherInput.right,
    up = self.SystemGatherInput.up,
    down = self.SystemGatherInput.down,
  }
end

return M
