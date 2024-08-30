---@class Input
---@field up boolean
---@field down boolean
---@field right boolean
---@field left boolean
local M = {
    up = false,
    down = false,
    right = false,
    left = false,
    action = false,
}

function M:update()
    self.up = love.keyboard.isDown('w')
    self.left = love.keyboard.isDown('a')
    self.down = love.keyboard.isDown('s')
    self.right = love.keyboard.isDown('d')
    self.action = love.keyboard.isDown('j')

    if love.keyboard.isDown('q') then love.event.quit(0) end
end

return M
