local concord = require("concord")
local Component = concord.component

Component("rectangle", function(c, w, h)
  c.w = w or 0
  c.h = h or 0
end)

Component("sprite", function(c, image, bounds_x, bounds_y, bounds_w, bounds_h)
  assert(image and bounds_x and bounds_y and bounds_w and bounds_h)
  c.image = image
  c.bounds_x = bounds_x
  c.bounds_y = bounds_y
  c.bounds_w = bounds_w
  c.bounds_h = bounds_h
end)
