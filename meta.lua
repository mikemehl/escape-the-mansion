---@meta

--[[
    Auxillary Definitions
--]]

---@alias Rectangle {x: number, y: number, width: number, height: number}
---@alias Vector2 {x: number, y: number}
---@alias Color {red: number, green: number, blue: number, alpoha: number}


--[[
    Tiled Export Definitions
--]]
---@class WangTile
---@field wangid number[]
---@field tileid number

---@class WangSet
---@field name string
---@field class string
---@field tile number
---@field wangsettype string
---@field properties table
---@field colors {color: [number, number, number, number], name: string, class: string, probability: number, tile: number, properties: table}
---@field wangtiles WangTile[]

---@class TiledObject
---@field id number
---@field name string
---@field type string
---@field shape string
---@field x number
---@field y number
---@field width number
---@field height number
---@field rotation number
---@field visible boolean
---@field properties table

---@class ObjectGroup
---@field type string
---@field draworder string
---@field id number
---@field name string
---@field class string
---@field visible boolean
---@field opacity number
---@field offsetx number
---@field offsety number
---@field parallaxx number
---@field parallaxy number
---@field properites table
---@field objects TiledObject[]

---@class Tile
---@field id number
---@field objectGruop ObjectGroup

---@class Tileset
---@field name string
---@field firstgid number
---@field class string
---@field tilewidth number
---@field tileheight number
---@field spacing number
---@field margin number
---@field columns number
---@field image string
---@field imagewidth number
---@field imageheight number
---@field objectalignment string
---@field tilerendersize string
---@field fillmode string
---@field tileoffset  {x: number, y: number}
---@field grid  {orientation: string, width: number, height: number}
---@field properties table
---@field wangsets WangSet[]
---@field tilecount 4332
---@field tiles Tile[]

---@class Layer
---@field type string
---@field x number
---@field y number
---@field width number
---@field height number
---@field id number
---@field name string
---@field calss string
---@field visible boolean
---@field opacity number
---@field offsetx number
---@field offsety number
---@field parallaxx number
---@field parallaxy number
---@field properties table
---@field encoding string
---@field data number[]?
---@field objects TiledObject[]?

---@class Tiled
---@field version string
---@field luaversion string
---@field tiledversion string
---@field class string
---@field orientation string
---@field renderorder string
---@field width number
---@field height number
---@field tilewidth number
---@field tileheight number
---@field nextlayerid number
---@field nextobjectid number
---@field properties table
---@field tilesets Tileset[]
---@field layers Layer[]

--[[
    ECS Definitions
--]]
