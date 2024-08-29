---@class Physics
local M = {}

local DEBUG_PHYSICS = os.getenv('DEBUG_PHYSICS')
local bump = require('lib.bump')
local bumpWorld = bump.newWorld(8)

Component('position', function(c, x, y)
    c.x = x or 25
    c.y = y or 25
end)

Component('velocity', function(c, x, y)
    c.x = x or 0
    c.y = y or 0
end)

Component('collisionBox', function(c, x, y, w, h) bumpWorld:add(c, x, y, w, h) end)

M.ApplyVelocity = System({ pool = { 'position', 'velocity' } })

function M.ApplyVelocity:update(dt)
    for _, e in ipairs(self.pool) do
        e.position.x = e.position.x + e.velocity.x
        e.position.y = e.position.y + e.velocity.y
        if e.collisionBox then
            e.position.x, e.position.y, _, _ =
                bumpWorld:move(e.collisionBox, e.position.x, e.position.y)
        end
    end
end

function M:draw()
    if DEBUG_PHYSICS then
        local items, _len = bumpWorld:getItems()
        local r, g, b, a = love.graphics.getColor()
        love.graphics.setColor(1, 0, 0, 1)
        for _, item in ipairs(items) do
            love.graphics.rectangle('line', bumpWorld:getRect(item))
        end
        love.graphics.setColor(r, g, b, a)
    end
end

function M.init(world) world:addSystem(M.ApplyVelocity) end

return M
