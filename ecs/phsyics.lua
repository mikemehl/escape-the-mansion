local concord = require("concord")
local Component = concord.component

Component("position", function(c, x, y)
  c.x = x or 0
  c.y = y or 0
end)

Component("velocity", function(c, x, y)
  c.x = x or 0
  c.y = y or 0
end)

Component("collidable", function(c, x_off, y_off)
  c.x_off = x_off or 0
  c.y_off = y_off or 0
end)
