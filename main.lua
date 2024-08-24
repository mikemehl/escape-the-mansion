if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
  package.loaded["lldebugger"] = assert(loadfile(
    "/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua"))()
  require("lldebugger").start()
end


local tiled = require("tiled")
local ecs = require("tiny")
local physics = require("physics")
local render = require("render")
local input = require("input")
local player = require("player")

---@type table
local world = nil

function love.load()
  tiled:init()
  world = ecs.world(
    render.SystemDrawRectangle,
    render.SystemDrawAnimatedSprite,
    input.SystemGatherInput,
    physics.SystemBumpUpdate,
    player.SystemPlayerUpdate,
    player:init())
  ecs.setSystemIndex(world, input.SystemGatherInput, 1)
end

function love.update(dt)
  tiled:update(dt)
end

function love.draw()
  love.graphics.clear()
  tiled:draw()
  world:update(1)
  love.graphics.print("HELLO WORLD", 50, 50)
end
