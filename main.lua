---@module 'anim8'
local anim8 = require('lib.anim8')

---@module 'baton.baton'
local baton = require('lib.baton.baton')

---@module 'sti.init'
local sti = require('lib.sti')

WINDOW_W = love.graphics.getWidth()
WINDOW_H = love.graphics.getHeight()

local Input = baton.new({
    controls = {
        left = { 'key:left', 'key:a', 'axis:leftx-', 'button:dpleft' },
        right = { 'key:right', 'key:d', 'axis:leftx+', 'button:dpright' },
        up = { 'key:up', 'key:w', 'axis:lefty-', 'button:dpup' },
        down = { 'key:down', 'key:s', 'axis:lefty+', 'button:dpdown' },
        action = { 'key:j', 'button:a' },
        quit = { 'key:q' },
    },
    pairs = {
        move = { 'left', 'right', 'up', 'down' },
    },
    joystick = love.joystick.getJoysticks()[1],
})

---@type Map
Room = sti('assets/test-room.lua')

--[[ ECS ]]
---@module 'concord.init'
local concord = require('lib.concord')

--[[ Components ]]
local componentList = {
    ['position'] = function(c, x, y)
        c.x = x or 0
        c.y = y or 0
    end,
    ['drawable'] = function(c) c.drawable = true end,
    ['controllable'] = function(c) c.controllable = true end,
    ['anim'] = function(c, anim)
        c.image = anim.image
        c.grid = anim.grid
        c.anim = anim.anim
    end,
}

for k, v in pairs(componentList) do
    concord.component(k, v)
end

--[[ Systems ]]
local DrawAnimSys = concord.system({ pool = { 'position', 'anim' } })
function DrawAnimSys:update(dt)
    for _, e in ipairs(self.pool) do
        e.anim.anim:update(dt)
    end
end
function DrawAnimSys:draw()
    for _, e in ipairs(self.pool) do
        e.anim.anim:draw(e.anim.image, e.position.x, e.position.y)
    end
end

local ControlSys = concord.system({ pool = { 'controllable' } })
function ControlSys:update(dt)
    for _, e in ipairs(self.pool) do
        if e.drawable and e.anim then
            if Input:pressed('left') and not e.anim.flipped then
                e.anim.flipped = true
                e.anim.anim:flipH()
            elseif Input:pressed('right') and e.anim.flipped then
                e.anim.flipped = false
                e.anim.anim:flipH()
            end
        end
        if e.position then
            if Input:down('left') then e.position.x = e.position.x - 100 * dt end
            if Input:down('right') then e.position.x = e.position.x + 100 * dt end
        end
    end
end

--[[ Core ]]

local World = concord.world()
World:addSystems(DrawAnimSys, ControlSys)

local Player = {}

---@type table<string, AnimDef>
local PlayerAnims = {}

--- Load the character animation.
local function loadCharacterIdleAnim()
    local image = love.graphics.newImage('assets/characters/HumanTownsfolkIdle.png')
    local grid = anim8.newGrid(32, 32, image:getWidth(), image:getHeight())
    local anim = anim8.newAnimation(grid('1-4', 1), 0.3)
    PlayerAnims.idle = {
        image = image,
        anim = anim,
        grid = grid,
    }
end

--- Love init routine
function love.load()
    love.graphics.setDefaultFilter('nearest', 'nearest')
    loadCharacterIdleAnim()
    assert(World)
    assert(Room)
    Room:removeLayer('WallObjs')
    Room:removeLayer('Doors')
    for _, obj in pairs(Room.objects) do
        if obj.name == 'PlayerStart' then
            Player = concord
                .entity(World)
                :give('position', obj.x, obj.y)
                :give('drawable')
                :give('controllable')
                :give('anim', PlayerAnims.idle)
            break
        end
    end
    Room:removeLayer('Points')
    local ecsLayer = Room:addCustomLayer('ecs', #Room.layers + 1)
    ecsLayer.update = function(_, dt) World:emit('update', dt) end
    ecsLayer.draw = function(_) World:emit('draw') end
end

--- Love draw routine
---@diagnostic disable-next-line: duplicate-set-field
function love.draw()
    love.graphics.setBackgroundColor(0, 0, 0, 0)
    local xc, yc = love.graphics.transformPoint(Player.position.x, Player.position.y)
    xc, yc = WINDOW_W / 8 - xc, yc + WINDOW_H / 8 - yc
    Room:draw(xc, -yc, 4, 4)
end

--- Love update routine
---@diagnostic disable-next-line: duplicate-set-field
function love.update(dt)
    Input:update()
    if Input:pressed('quit') then love.event.quit(0) end
    Room:update(dt)
end
