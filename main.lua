if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
  package.loaded["lldebugger"] = assert(loadfile(
    "/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua"))()
  require("lldebugger").start()
end

require("src.globals")
local input = require("src.input")
local render = require("src.render")
local room = require("src.room")
local player = require("src.player")

local world = World({
  render.DrawRectangleSprite,
}, nil, false)

function love.load()
  love.graphics.setDefaultFilter("nearest", "nearest")
  room:init()
  player.init(world)
end

function love.update(dt)
  input:update()
  world:Update("process", dt)
end

function love.draw()
  room:draw()
  world:Update("render", os.clock())
  love.graphics.print("HELLO WORLD", 50, 50)
end
