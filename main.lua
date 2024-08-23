if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
  package.loaded["lldebugger"] = assert(loadfile(
    "/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua"))()
  require("lldebugger").start()
end


local concord = require("concord")
local sti = require("sti")
local bump = require("bump")
local dbg = require("debugger")

---@type table
local map = nil

function love.load()
  map = sti("assets/test-room.lua")
  assert(map)

  local player = {
    x = map.layers["Points"].objects[1].x,
    y = map.layers["Points"].objects[1].y,
  }

  map:removeLayer(6)
  map:removeLayer(5)
  map:removeLayer(4)


  local world = concord.world()
  local ecs_layer = map:addCustomLayer("ecs", 8)

  ecs_layer.update = function(_, _)
    world:emit("update")
  end

  ecs_layer.draw = function(_, _)
    world:emit("draw")
  end
end

function love.update(dt)
  map:update(dt)
  if love.keyboard.isDown("q") then
    love.event.quit(0)
  end
end

function love.draw()
  map:draw()
  love.graphics.print("HELLO WORLD", 50, 50)
end
