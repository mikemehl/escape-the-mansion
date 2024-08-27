---@class Player
local M = {}

function M.init(world)
    Entity(world):give('position', 50, 50):give('rectangleSprite', {})
    return world
end

return M
