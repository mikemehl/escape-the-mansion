---@class Render
local M = {}

require('src.physics')

Component('rectangleSprite', function(c, params)
    c.rect = params.rectangle or { x = 0, y = 0, width = 100, height = 60 }
    c.color = params.color or { r = 0, g = 255, b = 0, a = 1 }
end)

M.DrawRectangleSprite = System({ pool = { 'rectangleSprite', 'position' } })
function M.DrawRectangleSprite:draw()
    for _, e in ipairs(self.pool) do
        local r, g, b, a = love.graphics.getColor()
        love.graphics.setColor(
            e.rectangleSprite.color.r,
            e.rectangleSprite.color.g,
            e.rectangleSprite.color.b,
            e.rectangleSprite.color.a
        )
        love.graphics.rectangle(
            'fill',
            e.position.x,
            e.position.y,
            e.rectangleSprite.rect.width,
            e.rectangleSprite.rect.height
        )
        love.graphics.setColor(r, g, b, a)
    end
end

return M
