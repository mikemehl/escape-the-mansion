---@module 'input'
local M = {
  up = false,
  down = false,
  right = false,
  left = false,
}

function M:update()
  self.up = love.keyboard.isDown('w')
  self.left = love.keyboard.isDown('a')
  self.down = love.keyboard.isDown('s')
  self.right = love.keyboard.isDown('d')

  if love.keyboard.isDown('q') then
    love.event.quit(0)
  end
end

---@type Input
return M
