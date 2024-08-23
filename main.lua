if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
  package.loaded["lldebugger"] = assert(loadfile(
    "/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua"))()
  require("lldebugger").start()
end


local sti = require("sti")
local bump = require("bump")
local ecs = require("tiny")
local physics = require("physics")
local render = require("render")

---@type table
local map = nil
local bump_world = nil
local world = nil

function love.load()
  ---@type table
  bump_world = bump.newWorld(4)
  map = sti("assets/test-room.lua")
  assert(bump_world)
  assert(map)


  local player_start = {
    x = map.layers["Points"].objects[1].x,
    y = map.layers["Points"].objects[1].y,
  }

  local rect = {}
  rect = physics.position(rect)
  rect = render.rectangle(rect)
  rect.position.x = player_start.x
  rect.position.y = player_start.y

  world = ecs.world(render.SystemDrawRectangle, rect)
end

function love.update(dt)
  map:update(dt)
  if love.keyboard.isDown("q") then
    love.event.quit(0)
  end
end

function love.draw()
  love.graphics.clear()
  map:draw()
  world:update(1)
  love.graphics.print("HELLO WORLD", 50, 50)
  local bump_len = bump_world:countItems()
  love.graphics.print("BUMP ITEMS: " .. tostring(bump_len), 100, 100)
end
