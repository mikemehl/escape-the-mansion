---@class Render
local M = {}

local Position = require("src.physics").Position

---@class RectangleSprite
---@field rect Rectangle
---@field color Color
---@type fun(RectangleSprite): RectangleSprite
local RectangleSprite = ECS.Component(function(params)
  return {
    rect = params.rectangle or { x = 0, y = 0, width = 0, height = 0 },
    color = params.color or { red = 0, green = 0, blue = 0, alpha = 1 }
  }
end)

M.RectangleSprite = RectangleSprite

M.DrawRectangleSprite = ECS.System("render", ECS.Query.All({ Position, RectangleSprite }), function(self, _)
  for _, e in self:Result():Iterator() do
    local pos = e:Get(Position) ---@type Position
    local rect = e:Get(RectangleSprite) ---@type RectangleSprite
    local r, g, b, a = love.graphics.getColor()
    love.graphics.setColor(rect.color.red, rect.color.green, rect.color.blue, rect.color.alpha)
    love.graphics.rectangle("fill", rect.rect.x, rect.rect.y, rect.rect.width, rect.rect.height)
    love.graphics.setColor(r, g, b, a)
  end
end)


return M
