---@class Physics
local M = {}

Component('position', function(c, x, y)
    c.x = x or 25
    c.y = y or 25
end)

Component('velocity', function(c, x, y)
    c.x = x or 0
    c.y = y or 0
end)

Component('collisionBox', function(c, x, y, w, h)
    c.x = x or 0
    c.y = y or 0
    c.w = w or 0
    c.h = h or 0
end)

return M
