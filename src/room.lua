---@module 'room'
---@class Room
---@field raw Tiled
---@field image love.Image
---@field quads love.Quad[]
---@field spriteBatch love.SpriteBatch
local M = {
  ---@type Tiled
  raw = require('assets.test-room')
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
        tileQuads[tileindex] = love.graphics.newQuad(x, y, tileset.tilewidth,
          tileset.tileheight, tileset.imagewidth,
          tileset.imageheight)
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
    if layer.type == "tilelayer" then
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
local function loadWalls(data)
end

function M:init()
  self.image = love.graphics.newImage("assets/haunted_house/Tileset/HauntedHouseTileset.png")
  self.quads = getTileQuads(self.raw)
  self.spriteBatch = setupSpriteBatch(self.raw, self.image, self.quads)
end

function M:draw()
  love.graphics.draw(self.spriteBatch)
end

---@type Room
return M
