--[[
    Globals.....yes, I'm gonna use them.
    I know, it's terrible. But, some of these are just going to clog everything up unless they are
    available everywhere.
    So, I hate to do it, but here we are. It's global time.
--]]

if not _G.Component then
  _G.Component = require("lib.concord").component
end

if not _G.Components then
  _G.Components = require("lib.concord").components
end

if not _G.System then
  _G.System = require("lib.concord").system
end

if not _G.Entity then
  _G.Entity = require("lib.concord").entity
end

if not _G.World then
  _G.World = require("lib.concord").world
end

if not _G.Utils then
  _G.Utils = require("lib.concord").utils
end
