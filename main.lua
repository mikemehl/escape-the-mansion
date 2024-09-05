local sti = require('lib.sti')

C = {
    WALL_OBJ_LAYER = 8,
}

Room = sti('assets/test-room.lua')

function love.init() assert(Room) end

function love.draw() Room:draw() end

function love.update(_) end
