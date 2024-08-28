---@class Render
local M = {}

require('src.physics')
local anim8 = require('lib.anim8')

Component('rectangleSprite', function(c, params)
    c.rect = params.rectangle or { x = 0, y = 0, width = 100, height = 60 }
    c.color = params.color or { r = 0, g = 255, b = 0, a = 1 }
end)

M.animatedSprite = Component(
    'animatedSprite',
    ---@param c table
    ---@param width number
    ---@param height number
    ---@param image love.Image
    ---@param frames_x string
    ---@param frames_y string
    ---@param durations number | [number]
    function(c, width, height, image, frames_x, frames_y, durations)
        c.image = image
        c.grid = anim8.newGrid(width, height, image:getWidth(), image:getHeight())
        c.anim = anim8.newAnimation(c.grid(frames_x, frames_y), durations)
    end
)

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

M.AnimatedSpritesSystem = System({ pool = { 'animatedSprite' } })
function M.AnimatedSpritesSystem:update(dt)
    for _, e in ipairs(self.pool) do
        e.animatedSprite.anim:update(dt)
    end
end

function M.AnimatedSpritesSystem:draw()
    for _, e in ipairs(self.pool) do
        if e.position then
            e.animatedSprite.anim:draw(e.animatedSprite.image, e.position.x, e.position.y)
        end
    end
end

function M.init(world) world:addSystem(M.DrawRectangleSprite):addSystem(M.AnimatedSpritesSystem) end

return M
