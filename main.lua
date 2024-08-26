if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
  package.loaded["lldebugger"] = assert(loadfile(
    "/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua"))()
  require("lldebugger").start()
end


local tiled = require("src.tiled")
local ecs = require("lib.tiny")
local physics = require("src.physics")
local render = require("src.render")
local input = require("src.input")
local player = require("src.player")

---@type table
local world = nil

function love.load()
  love.graphics.setDefaultFilter("nearest", "nearest")
  tiled:init()
  render:init()
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
  render.camera:draw(function()
    tiled:draw()
    world:update(1)
  end)
  love.graphics.print("HELLO WORLD", 50, 50)
end
