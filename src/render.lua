---@class Render
local M = {}

require('src.physics')
local anim8 = require('lib.anim8')

Component('rectangleSprite', function(c, params)
    c.rect = params.rectangle or { x = 0, y = 0, width = 100, height = 60 }
    c.color = params.color or { r = 0, g = 255, b = 0, a = 1 }
end)

local shader = love.graphics.newShader([[
  vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords ) {
    vec4 pixel = Texel(texture, texture_coords);
    return pixel * color;
  }
]])

M.animatedSprite = Component(
    'animatedSprite',
    ---@param c table
    ---@param anim table
    ---@param xoff number
    ---@param yoff number
    function(c, anim, image, xoff, yoff)
        c.anim = anim
        c.image = image
        c.xoff = xoff or 12
        c.yoff = yoff or 12
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
            e.animatedSprite.anim:draw(
                e.animatedSprite.image,
                e.position.x - e.animatedSprite.xoff,
                e.position.y - e.animatedSprite.yoff
            )
        end
    end
end

function M.init(world)
    love.graphics.setShader(shader)
    world:addSystem(M.DrawRectangleSprite):addSystem(M.AnimatedSpritesSystem)
end

return M
