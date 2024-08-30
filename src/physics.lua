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

Component('collisionBox', function(c, x, y, w, h, react)
    bumpWorld:add(c, x, y, w, h)
    if react then c.react = react end
end)

M.ApplyVelocity = System({ pool = { 'position', 'velocity' } })
function M.ApplyVelocity:update(dt)
    for _, e in ipairs(self.pool) do
        e.position.x = e.position.x + e.velocity.x
        e.position.y = e.position.y + e.velocity.y
        if e.collisionBox then
            local newx, newy, collisions, len =
                bumpWorld:move(e.collisionBox, e.position.x, e.position.y)
            for _, c in pairs(collisions) do
                if c.react then c.react(e) end
            end
            e.position.x = newx
            e.position.y = newy
        end
    end
end

M.DebugPositions = System({ pool = { 'position' } })
function M.DebugPositions:draw()
    if not DEBUG_PHYSICS then return end
    local r, g, b, a = love.graphics.getColor()
    love.graphics.setColor(0, 0, 1, 1)
    for _, e in ipairs(self.pool) do
        love.graphics.line(e.position.x - 4, e.position.y, e.position.x + 4, e.position.y)
        love.graphics.line(e.position.x, e.position.y - 4, e.position.x, e.position.y + 4)
    end
    love.graphics.setColor(r, g, b, a)
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

function M.init(world) world:addSystem(M.ApplyVelocity):addSystem(M.DebugPositions) end

return M
