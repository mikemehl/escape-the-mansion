local sti = require('lib.sti')

WINDOW_W = love.graphics.getWidth()
WINDOW_H = love.graphics.getHeight()

---@type Map
Room = sti('assets/test-room.lua')

local function checkQuit()
    if love.keyboard.isDown('q') then love.event.quit(0) end
end

function love.load()
    assert(Room)
    Room:removeLayer(5)
    Room:removeLayer('Doors')
    local points = Room:convertToCustomLayer('Points')
end

---@diagnostic disable-next-line: duplicate-set-field
function love.draw()
    love.graphics.setBackgroundColor(0, 0, 0, 0)
    Room:draw()
end

function love.update(_) checkQuit() end
