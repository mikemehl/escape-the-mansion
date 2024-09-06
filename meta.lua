---@meta

--[[
    Auxillary Definitions
--]]

---@alias Rectangle {x: number, y: number, width: number, height: number}
---@alias Vector2 {x: number, y: number}
---@alias Color {red: number, green: number, blue: number, alpoha: number}
---@alias AnimDef {image: love.Image, grid: table, anim: table}

--[[ Concord Meta]]

---@class Component
local Component = {}

---@class Entity
local Entity = {}

---@class World
local World = {}

---@param populate function
---@return Component
function Component:new(populate) end

---@return boolean
function Component:hasName() end

---@return string
function Component:getName() end

---@param worlds World[]
function Entity:new(worlds) end

---@param c Component
---@param ... any
function Entity:give(c, ...) end

---@param c Component
---@param ... any
function Entity:ensure(c, ...) end

---@param c Component
function Entity:remove(c) end

---@return World
function World:new() end

--[[ STI Meta]]

--- @class TileLayer
---@field name string The name of the layer
---@field x number Position on the X axis (in pixels)
---@field y number Position on the Y axis (in pixels)
---@field width number Width of layer (in tiles)
---@field height number Height of layer (in tiles)
---@field visible boolean Toggle if layer is visible or hidden
---@field opacity any Opacity of layer
---@field properties table Custom properties
---@field data number[][] A tileWo dimensional table filled with individual tiles indexed by [y][x] (in tiles)
---@field update fun(number)
---@field draw fun()
---@see Map.layers
---@see Tile

--- @class ObjectLayer
--- @field name string The name of the layer
--- @field x number Position on the X axis (in pixels)
--- @field y number Position on the Y axis (in pixels)
--- @field visible boolean Toggle if layer is visible or hidden
--- @field opacity any Opacity of layer
--- @field properties table Custom properties
--- @field objects table[] List of objects indexed by draw order
--- @field update fun(number) Update function
--- @field draw fun() Draw function
--- @see Map.layers
--- @see Object

--- @class ImageLayer
--- @field name string The name of the layer
--- @field x number Position on the X axis (in pixels)
--- @field y number Position on the Y axis (in pixels)
--- @field visible boolean Toggle if layer is visible or hidden
--- @field opacity any Opacity of layer
--- @field properties table Custom properties
--- @field image love.Image Image to be drawn
--- @field update fun(number) Update function
--- @field draw fun() Draw function
--- @see Map.layers

--- Custom Layers are used to place userdata such as sprites within the draw order of the map.
--- @class CustomLayer
--- @field name string The name of the layer
--- @field x number Position on the X axis (in pixels)
--- @field y number Position on the Y axis (in pixels)
--- @field visible boolean Toggle if layer is visible or hidden
--- @field opacity any Opacity of layer
--- @field properties table Custom properties
--- @field update fun(self, number) Update function
--- @field draw fun(self) Draw function
--- @see Map.layers
--- @usage
--	-- Create a Custom Layer
--	local spriteLayer = map:addCustomLayer("Sprite Layer", 3)
--
--	-- Add data to Custom Layer
--	spriteLayer.sprites = {
--		player = {
--			image = lg.newImage("assets/sprites/player.png"),
--			x = 64,
--			y = 64,
--			r = 0,
--		}
--	}
--
--	-- Update callback for Custom Layer
--	function spriteLayer:update(dt)
--		for _, sprite in pairs(self.sprites) do
--			sprite.r = sprite.r + math.rad(90 * dt)
--		end
--	end
--
--	-- Draw callback for Custom Layer
--	function spriteLayer:draw()
--		for _, sprite in pairs(self.sprites) do
--			local x = math.floor(sprite.x)
--			local y = math.floor(sprite.y)
--			local r = sprite.r
--			lg.draw(sprite.image, x, y, r)
--		end
--	end

--- @class Tile
---@field id number Local ID within Tileset
---@field gid number Global ID
---@field tileset number Tileset ID
---@field quad love.Quad Quad object
---@field properties table Custom properties
---@field terrain any Terrain data
---@field animation any Animation data
---@field frame any Current animation frame
---@field time number Time spent on current animation frame
---@field width number Width of tile
---@field height number Height of tile
---@field sx number Scale value on the X axis
---@field sy number Scale value on the Y axis
---@field r number Rotation of tile (in radians)
---@field offset number Offset drawing position
---@field offset.x number Offset value on the X axis
---@field offset.y number Offset value on the Y axis
---@see Map.tiles

--- @class TileInstance
---@field batch love.SpriteBatch Spritebatch the Tile Instance belongs to
---@field id number ID within the spritebatch
---@field gid number Global ID
---@field x number Position on the X axis (in pixels)
---@field y number Position on the Y axis (in pixels)
---@see Map.tileInstances
---@see Tile

---@class Sti
local STI = {}

--- @class Object
---@field id number Global ID
---@field name string Name of object (non-unique)
---@field shape love.Shape Shape of object
---@field x number Position of object on X axis (in pixels)
---@field y number Position of object on Y axis (in pixels)
---@field width number Width of object (in pixels)
---@field height number Heigh tof object (in pixels)
---@field rotation number Rotation of object (in radians)
---@field visible boolean Toggle if object is visible or hidden
---@field properties table Custom properties
---@field ellipse any[] List of verticies of specific shape
---@field rectangle any[] List of verticies of specific shape
---@field polygon any[] List of verticies of specific shape
---@field polyline any[] List of verticies of specific shape
---@see Map.objects
---@return Map
function STI.new(map, plugins, ox, oy) end

---@alias Layer  TileLayer|ObjectLayer|ImageLayer|CustomLayer
---@class Tileset
---@class Tile
---@class Chunk

---@class Map
---@field layers Layer[] A list of individual layers indexed both by draw order and name
---@field tiles table<number, Tile> A list of individual tiles indexed by Global ID
---@field tileInstances table<number, TileInstance> A list of tile instances indexed by Global ID
---@field freeBatchSprites any A list of no-longer-used batch sprites, indexed by batch
---@field objects table<number, Object> A list of individual objects indexed by Global ID
local Map = {}

--- Instance a new map.
---@param path string Path to the map file
---@param plugins [string] A list of plugins to load
---@param ox number Offset of map on the X axis (in pixels)
---@param oy number Offset of map on the Y axis (in pixels)
---@return Map
function STI.__call(path, map, plugins, ox, oy) end

--- Flush image cache.
function STI:flush() end

--- Instance a new map
---@param path string Path to the map file
---@param plugins [string] A list of plugins to load
---@param ox number Offset of map on the X axis (in pixels)
---@param oy number Offset of map on the Y axis (in pixels)
function Map:init(path, plugins, ox, oy) end

--- Layers from the group are added to the list
---@param layers [Layer] List of layers
---@param layer Layer Layer data
function Map:groupAppendToList(layers, layer) end

--- Load plugins
---@param plugins [string] A list of plugins to load
function Map:loadPlugins(plugins) end

--- Create Tiles based on a single tileset image
---@param index number Index of the Tileset
---@param tileset Tileset Tileset data
---@param gid number First Global ID in Tileset
---@return number Next Tileset's first Global ID
function Map:setTiles(index, tileset, gid) end

--- Create Tiles based on a texture atlas
---@param index number Index of the Tileset
---@param tileset Tileset Tileset data
---@param coords [ number, number ] Tile XY location in the atlas
---@param gid number First Global ID in Tileset
---@return number Next Tileset's first Global ID
function Map:setAtlasTiles(index, tileset, coords, gid) end

--- Create Layers
---@param layer Layer Layer data
---@param path string? (Optional) Path to an Image Layer's image
function Map:setLayer(layer, path) end

--- Add Tiles to Tile Layer
---@param layer TileLayer The Tile Layer
function Map:setTileData(layer) end

--- Add Objects to Layer
---@param layer ObjectLayer The Object Layer
function Map:setObjectData(layer) end

--- Correct position and orientation of Objects in an Object Layer
---@param layer ObjectLayer The Object Layer
function Map:setObjectCoordinates(layer) end

--- Convert tile location to tile instance location
---@param layer TileLayer Tile layer
---@param tile Tile Tile
---@param x number Tile location on X axis (in tiles)
---@param y number Tile location on Y axis (in tiles)
---@return number, number The x, y Tile instance location on X axis (in pixels)
function Map:getLayerTilePosition(layer, tile, x, y) end

--- Place new tile instance
---@param layer TileLayer Tile layer
---@param chunk Chunk Layer chunk
---@param tile Tile Tile
---@param x number Tile location on X axis (in tiles)
---@param y number Tile location on Y axis (in tiles)
function Map:addNewLayerTile(layer, chunk, tile, x, y) end

function Map:set_batches(layer, chunk) end

--- Batch Tiles in Tile Layer for improved draw speed
---@param layer TileLayer The Tile Layer
function Map:setSpriteBatches(layer) end

--- Batch Tiles in Object Layer for improved draw speed
---@param layer ObjectLayer The Object Layer
function Map:setObjectSpriteBatches(layer) end

--- Create a Custom Layer to place userdata in (such as player sprites)
---@param name string Name of Custom Layer
---@param index number Draw order within Layer stack
---@return CustomLayer Custom Layer
function Map:addCustomLayer(name, index) end

--- Convert another Layer into a Custom Layer
---@param index number|string Index or name of Layer to convert
---@return CustomLayer Custom Layer
function Map:convertToCustomLayer(index) end

--- Remove a Layer from the Layer stack
---@param index number|string Index or name of Layer to remove
function Map:removeLayer(index) end

--- Animate Tiles and update every Layer
---@param dt number Delta Time
function Map:update(dt) end

--- Draw every Layer
---@param tx number? Translate on X
---@param ty number? Translate on Y
---@param sx number? Scale on X
---@param sy number? Scale on Y
function Map:draw(tx, ty, sx, sy) end

--- Draw an individual Layer
---@param layer Layer The Layer to draw
function Map.drawLayer(_, layer) end

--- Default draw function for Tile Layers
---@param layer TileLayer The Tile Layer to draw
function Map:drawTileLayer(layer) end

--- Default draw function for Object Layers
---@param layer ObjectLayer The Object Layer to draw
function Map:drawObjectLayer(layer) end

--- Default draw function for Image Layers
---@param layer ImageLayer The Image Layer to draw
function Map:drawImageLayer(layer) end

--- Resize the drawable area of the Map
---@param w number The new width of the drawable area (in pixels)
---@param h number The new Height of the drawable area (in pixels)
function Map:resize(w, h) end

--- Create flipped or rotated Tiles based on bitop flags
---@param gid number The flagged Global ID
---@return Tile Flipped Tile
function Map:setFlippedGID(gid) end

--- Get custom properties from Layer
---@param layer Layer The Layer
---@return table List of properties
function Map:getLayerProperties(layer) end

--- Get custom properties from Tile
---@param layer TileLayer The Layer that the Tile belongs to
---@param x number The X axis location of the Tile (in tiles)
---@param y number The Y axis location of the Tile (in tiles)
---@return table List of properties
function Map:getTileProperties(layer, x, y) end

--- Get custom properties from Object
---@param layer ObjectLayer The Layer that the Object belongs to
---@param object string|number The index or name of the Object
---@return table List of properties
function Map:getObjectProperties(layer, object) end

--- Change a tile in a layer to another tile
---@param layer TileLayer The Layer that the Tile belongs to
---@param x number The X axis location of the Tile (in tiles)
---@param y number The Y axis location of the Tile (in tiles)
---@param gid number The gid of the new tile
function Map:setLayerTile(layer, x, y, gid) end

--- Swap a tile in a spritebatch
---@param instance Tile The current Instance object we want to replace
---@param tile Tile The Tile object we want to use
function Map:swapTile(instance, tile) end

--- Convert tile location to pixel location
---@param x number The X axis location of the point (in tiles)
---@param y number The Y axis location of the point (in tiles)
---@return number, number The X,Y axis location of the point (in pixels)
function Map:convertTileToPixel(x, y) end

--- Convert pixel location to tile location
---@param x number The X axis location of the point (in pixels)
---@param y number The Y axis location of the point (in pixels)
---@return number, number The X,Y axis location of the point (in tiles)
function Map:convertPixelToTile(x, y) end

return setmetatable({}, STI)
