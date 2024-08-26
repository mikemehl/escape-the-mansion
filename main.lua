if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
  package.loaded["lldebugger"] = assert(loadfile(
    "/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua"))()
  require("lldebugger").start()
end

require("lib.ECS")
local input = require("src.input")
local room = require("src.room")

function love.load()
  love.graphics.setDefaultFilter("nearest", "nearest")
  room:init()
end

function love.update(dt)
  input:update()
end

function love.draw()
  room:draw()
  love.graphics.print("HELLO WORLD", 50, 50)
end
