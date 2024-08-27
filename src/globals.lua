--[[
    Globals.....yes, I'm gonna use them.
    I know, it's terrible. But, some of these are just going to clog everything up unless they are
    available everywhere.
    So, I hate to do it, but here we are. It's global time.
--]]

if not _G.Component then
  _G.Component = require("concord").component
end

if not _G.Components then
  _G.Components = require("concord").components
end

if not _G.System then
  _G.System = require("concord").system
end

if not _G.Entity then
  _G.Entity = require("concord").entity
end

if not _G.World then
  _G.World = require("concord").world
end

if not _G.Utils then
  _G.Utils = require("concord").utils
end
