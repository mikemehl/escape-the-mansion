---@class Player
local M = {}

local input = require('src.input')

Component('player', function(c) c.player = true end)

M.UpdatePlayer = System({ pool = { 'player' } })

function M.UpdatePlayer:update(dt)
    for _, e in ipairs(self.pool) do
        if input.up then
            e.position.y = e.position.y - 100 * dt
        elseif input.down then
            e.position.y = e.position.y + 100 * dt
        end
        if input.left then
            e.position.x = e.position.x - 100 * dt
        elseif input.right then
            e.position.x = e.position.x + 100 * dt
        end
    end
end

function M.init(world)
    Entity(world):give('position', 50, 50):give('rectangleSprite', {}):give('player')
    world:addSystem(M.UpdatePlayer)
end

return M
