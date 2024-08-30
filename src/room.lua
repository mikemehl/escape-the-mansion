---@class Room
---@field raw Tiled
---@field tilesheetImage love.Image
---@field characterIdleImage love.Image
---@field characterWalkImage love.Image
---@field quads love.Quad[]
---@field spriteBatch love.SpriteBatch?
---@field walls Rectangle[]
local M = {
    raw = require('assets.test-room'),
    tilesheetImage = love.graphics.newImage('assets/haunted_house/Tileset/HauntedHouseTileset.png'),
    characterIdleImage = love.graphics.newImage('assets/characters/HumanTownsfolkIdle.png'),
    characterWalkImage = love.graphics.newImage('assets/characters/HumanTownsfolkWalk.png'),
}

---@param data Tiled
---@return love.Quad[]
local function getTileQuads(data)
    local tileQuads = {}
    for _, tileset in pairs(data.tilesets) do
        local tileindex = tileset.firstgid
        for y = 0, tileset.imageheight - 1, data.tileheight do
            for x = 0, tileset.imagewidth - 1, data.tilewidth do
                assert(tileQuads[tileindex] == nil)
                tileQuads[tileindex] = love.graphics.newQuad(
                    x,
                    y,
                    tileset.tilewidth,
                    tileset.tileheight,
                    tileset.imagewidth,
                    tileset.imageheight
                )
                tileindex = tileindex + 1
            end
        end
    end
    return tileQuads
end

---@param data Tiled
---@param image love.Image
---@param quads love.Quad[]
---@return love.SpriteBatch
local function setupSpriteBatch(data, image, quads)
    local spriteBatch = love.graphics.newSpriteBatch(image, 5000)
    for _, layer in pairs(data.layers) do
        if layer.type == 'tilelayer' then
            for idx, gid in ipairs(layer.data) do
                if gid > 0 then
                    local x = (idx - 1) % data.width
                    local y, _ = math.modf((idx - 1) / data.width)
                    local unflipped_gid = bit.band(gid, 0x0FFFFFFF)
                    spriteBatch:add(quads[unflipped_gid], x * data.tilewidth, y * data.tileheight)
                end
            end
        end
    end
    return spriteBatch
end

---@param data Tiled
---@return Rectangle[]
local function loadWalls(data)
    local walls = {}
    for _, layer in pairs(data.layers) do
        if layer.name == 'WallObjs' then
            for _, obj in pairs(layer.objects) do
                local wall = {
                    x = obj.x,
                    y = obj.y,
                    width = obj.width,
                    height = obj.height,
                }
                table.insert(walls, wall)
            end
        end
    end
    return walls
end

local function loadDoors(data)
    local doors = {}
    for _, layer in pairs(data.layers) do
        if layer.name == 'Doors' then
            for _, obj in pairs(layer.objects) do
                local id = obj.id
                doors[id] = {
                    id = obj.id,
                    x = obj.x,
                    y = obj.y,
                    dest = obj.properties['to'].id,
                }
            end
        end
    end
    return doors
end

function M:init(world)
    self.quads = getTileQuads(self.raw)
    self.spriteBatch = setupSpriteBatch(self.raw, self.tilesheetImage, self.quads)
    self.walls = loadWalls(self.raw)
    self.doors = loadDoors(self.raw)
    for _, wall in ipairs(self.walls) do
        world
            :newEntity()
            :give('position', wall.x, wall.y)
            :give('collisionBox', wall.x, wall.y, wall.width, wall.height)
    end
    for _, door in pairs(self.doors) do
        print('DOOR')
        world
            :newEntity()
            :give('position', door.x, door.y)
            :give('collisionBox', door.x - 2, door.y - 2, 4, 4)
    end
end

function M:draw() love.graphics.draw(self.spriteBatch) end

return M
