local anim8 = require('lib.anim8')
local baton = require('lib.baton.baton')
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
local concord = require('lib.concord')

--[[ Components ]]
local componentList = {
    ['position'] = function(c, x, y)
        c.x = x or 0
        c.y = y or 0
    end,
    ['drawable'] = function(c) c.drawable = true end,
    ['controllable'] = function(c) c.controllable = true end,
    ['anim'] = function(c, image, grid, anim)
        c.image = image or error()
        c.grid = grid or error()
        c.anim = anim or error()
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
function ControlSys:update(_)
    for _, e in ipairs(self.pool) do
        if Input:pressed('left') and e.drawable and e.anim then e.anim.anim:flipH() end
    end
end

--[[ Core ]]

local World = concord.world()
World:addSystems(DrawAnimSys, ControlSys)

local Player = {}

--- Load the character animation.
---@return love.Image, table, table
local function loadCharacterIdleAnim()
    local image = love.graphics.newImage('assets/characters/HumanTownsfolkIdle.png')
    local grid = anim8.newGrid(32, 32, image:getWidth(), image:getHeight())
    local anim = anim8.newAnimation(grid('1-4', 1), 0.3)
    return image, grid, anim
end

--- Love init routine
function love.load()
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
                :give('anim', loadCharacterIdleAnim())
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
    local xc, yc = WINDOW_W / 2, WINDOW_H / 2
    xc, yc = xc - Player.position.x, yc - Player.position.y
    Room:draw(-xc / 4, -yc / 4, 4, 4)
end

--- Love update routine
---@diagnostic disable-next-line: duplicate-set-field
function love.update(dt)
    Input:update()
    if Input:pressed('quit') then love.event.quit(0) end
    Room:update(dt)
end
