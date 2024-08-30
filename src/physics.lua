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

Component(
    'collisionBox',
    ---@param c table
    ---@param x number
    ---@param y number
    ---@param w number
    ---@param h number
    ---@param filter fun(item: table, other: table): string
    ---@param flags {isDoor: boolean, dest: {x: number, y: number}?}
    function(c, x, y, w, h, filter, flags)
        bumpWorld:add(c, x, y, w, h)
        c.filter = filter or function() return 'slide' end
        c.flags = flags or { isDoor = false }
    end
)

Component(
    'collisionResolve',
    ---@param c table
    ---@param x number The new x position taking into account collisions
    ---@param y number The new y position taking into account collisions
    ---@param colliders [table]
    function(c, x, y, colliders)
        c.x = x
        c.y = y
        c.colliders = colliders
    end
)

M.ApplyVelocity = System({ pool = { 'position', 'velocity' } })
function M.ApplyVelocity:update(dt)
    for _, e in ipairs(self.pool) do
        local goalx = e.position.x + e.velocity.x
        local goaly = e.position.y + e.velocity.y
        e.position.x = goalx
        e.position.y = goaly
    end
end

M.DetectCollisions = System({ pool = { 'position', 'collisionBox' } })
function M.DetectCollisions:update(dt)
    for _, e in ipairs(self.pool) do
        if e.collisionBox.flags.isWall then goto continue end
        local newx, newy, collisions, len =
            bumpWorld:check(e.collisionBox, e.position.x, e.position.y, e.collisionBox.filter)
        e:give('collisionResolve', newx, newy, collisions)
        ::continue::
    end
end

M.ResolveCollisions = System({
    pool = { 'position', 'collisionBox', 'collisionResolve' },
})
function M.ResolveCollisions:update(dt)
    for _, e in ipairs(self.pool) do
        e.position.x = e.collisionResolve.x
        e.position.y = e.collisionResolve.y

        bumpWorld:update(
            e.collisionBox,
            e.position.x,
            e.position.y,
            e.collisionBox.w,
            e.collisionBox.h
        )
        e:remove('collisionResolve')
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

function M:getBumpWorld() return bumpWorld end

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

function M.init(world)
    world
        :addSystem(M.ApplyVelocity)
        :addSystem(M.DetectCollisions)
        :addSystem(M.ResolveCollisions)
        :addSystem(M.DebugPositions)
end

return M
