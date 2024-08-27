---@class Player
local M = {}

local Position = require("src.physics").Position
local RectangleSprite = require("src.render").RectangleSprite

function M.init(world)
  world:Entity(Position({
      x = 50, y = 50
    }),
    RectangleSprite({
      rect = {
        x = 0, y = 0, width = 50, height = 50
      },
      color = { red = 0, green = 255, blue = 0, alpha = 0
      }
    }))
end

return M
