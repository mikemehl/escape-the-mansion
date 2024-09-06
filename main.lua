local anim8 = require('lib.anim8')
local sti = require('lib.sti')

WINDOW_W = love.graphics.getWidth()
WINDOW_H = love.graphics.getHeight()

---@type Map
Room = sti('assets/test-room.lua')

--- Check if the quit key `q` was pressed
local function checkQuit()
    if love.keyboard.isDown('q') then love.event.quit(0) end
end

--[[ ECS ]]
local concord = require('lib.concord')

--[[ Components ]]
local componentList = {
    ['position'] = function(c, x, y)
        c.x = x or 0
        c.y = y or 0
    end,
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

--[[ Core ]]

local World = concord.world()

--- Load the character animation.
---@return love.Image, table, table
local function loadCharacterIdleAnim()
    local image = love.graphics.newImage('assets/characters/HumanTownsfolkIdle.png')
    local grid = anim8.newGrid(32, 32, image:getWidth(), image:getHeight())
    local anim = anim8.newAnimation(grid('1-4', 1), 0.1)
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
            concord
                .entity(World)
                :give('position', obj.x, obj.y)
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
    Room:draw()
end

--- Love update routine
function love.update(dt)
    checkQuit()
    Room:update(dt)
end
