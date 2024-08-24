local tiled = require("src.tiled")
local tiny = require("lib.tiny")
local physics = require("src.physics")
local render = require("src.render")
local input = require("src.input")


local M = {
  SystemPlayerUpdate = tiny.processingSystem({
    filter = tiny.requireAll("position", "player", "velocity")
  })
}

function M.SystemPlayerUpdate:process(entity, _)
  local curr_input = input:get()
  if curr_input.up then
    entity.velocity.y = -1
  elseif curr_input.down then
    entity.velocity.y = 1
  else
    entity.velocity.y = 0
  end

  if curr_input.right then
    entity.velocity.x = 1
  elseif curr_input.left then
    entity.velocity.x = -1
  else
    entity.velocity.x = 0
  end
end

function M.player(obj)
  local player_obj = obj or {}
  player_obj = physics.position(player_obj)
  player_obj = physics.velocity(player_obj)
  player_obj = physics.collisionBox(player_obj, { x_off = 0, y_off = 0, width = 20, height = 20 })
  player_obj = render.rectangle(player_obj)
  player_obj.position.x = tiled.player_start.x
  player_obj.position.y = tiled.player_start.y
  player_obj.player = true
  return player_obj
end

function M:init()
  return M.player({})
end

return M
