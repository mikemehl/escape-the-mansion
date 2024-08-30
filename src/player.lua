---@class Player
local M = {}

local PLAYER_SPEED = 100

local anim8 = require('lib.anim8')
local input = require('src.input')
local render = require('src.render')
local room = require('src.room')

local physics = require('src.physics')

local idleSpriteGrid =
    anim8.newGrid(32, 32, room.characterIdleImage:getWidth(), room.characterIdleImage:getHeight())
local walkingSpriteGrid =
    anim8.newGrid(32, 32, room.characterWalkImage:getWidth(), room.characterWalkImage:getHeight())

local sprites = {
    idle = {
        anim = anim8.newAnimation(idleSpriteGrid('1-8', 1), 0.1),
        image = room.characterIdleImage,
    },
    walkingrf = {
        anim = anim8.newAnimation(walkingSpriteGrid('1-4', 1), 0.1),
        image = room.characterWalkImage,
    },
    walkinglf = {
        anim = anim8.newAnimation(walkingSpriteGrid('1-4', 2), 0.1),
        image = room.characterWalkImage,
    },
    walkingrb = {
        anim = anim8.newAnimation(walkingSpriteGrid('1-4', 3), 0.1),
        image = room.characterWalkImage,
    },
    walkinglb = {
        anim = anim8.newAnimation(walkingSpriteGrid('1-4', 4), 0.1),
        image = room.characterWalkImage,
    },
}

local sprite_lut = {
    { 0, 0, 'idle' },
    { 0, 1, 'walkingrf' },
    { 0, -1, 'walkingrb' },
    { 1, 0, 'walkingrf' },
    { 1, 1, 'walkingrf' },
    { 1, -1, 'walkingrb' },
    { -1, 0, 'walkinglf' },
    { -1, 1, 'walkinglf' },
    { -1, -1, 'walkinglb' },
}

local curr_sprite = nil

local function setSprite(entity, sprite)
    if sprite == curr_sprite then
        return
    else
        curr_sprite = sprite
    end
    if entity:has('animatedSprite') then entity:remove('animatedSprite') end
    local sprite_spec = sprites[sprite]
    assert(sprite_spec)
    entity = entity:give('animatedSprite', sprite_spec.anim, sprite_spec.image)
    return entity
end

Component('player')

Component('doorCooldown', function(c, t) c.t = t or 10 end)

M.UpdatePlayer = System({ pool = { 'player', 'doorCooldown' } })

function M.UpdatePlayer:update(dt)
    for _, e in ipairs(self.pool) do
        e.velocity.x, e.velocity.y = 0, 0
        if input.up then
            e.velocity.y = -PLAYER_SPEED * dt
        elseif input.down then
            e.velocity.y = PLAYER_SPEED * dt
        end

        if input.left then
            e.velocity.x = -PLAYER_SPEED * dt
        elseif input.right then
            e.velocity.x = PLAYER_SPEED * dt
        end
        if e.doorCooldown.t > 0 then e.doorCooldown.t = e.doorCooldown.t - 1 end
        if e.doorCooldown.t < 0 then e.doorCooldown.t = 0 end
    end
end

M.UpdatePlayerSprite = System({ pool = { 'player' } })
function M.UpdatePlayerSprite:update(_)
    for _, e in ipairs(self.pool) do
        local newSprite = 'idle'
        local xkey = 0
        local ykey = 0
        if e.velocity.x > 0 then
            xkey = 1
        elseif e.velocity.x < 0 then
            xkey = -1
        end
        if e.velocity.y > 0 then
            ykey = 1
        elseif e.velocity.y < 0 then
            ykey = -1
        end
        for _, row in ipairs(sprite_lut) do
            if xkey == row[1] and ykey == row[2] then
                newSprite = row[3]
                break
            end
        end
        e = setSprite(e, newSprite)
    end
end

local function playerCollisionQuery(item, other)
    if other.flags.isDoor then return 'cross' end
    if other.flags.isWall then return 'touch' end
    return 'slide'
end

local function playerDoorQuery(item) return item.flags.isDoor end

M.CheckPlayerOnDoor = System({ pool = { 'player', 'position', 'collisionBox', 'doorCooldown' } })
function M.CheckPlayerOnDoor:update(_)
    local bumpWorld = physics:getBumpWorld()
    for _, player in ipairs(self.pool) do
        local doors, len =
            bumpWorld:queryRect(player.position.x, player.position.y, 8, 8, playerDoorQuery)
        if len >= 1 and input.action and player.doorCooldown.t <= 0 then
            local destX, destY = doors[1].flags.dest.x, doors[1].flags.dest.y
            bumpWorld:update(player.collisionBox, destX, destY, 8, 8)
            player.position.x, player.position.y = destX, destY
            player.doorCooldown.t = 10
        end
    end
end

function M.init(world)
    local startPoint = room:getPlayerStartPoint()
    local player = Entity(world)
        :give('position', startPoint.x, startPoint.y)
        :give('velocity', 0, 0)
        :give('player')
        :give('collisionBox', startPoint.x + 8, startPoint.y + 8, 8, 8, playerCollisionQuery)
        :give('doorCooldown', 0)
        :give('spotlightTarget')
    player = setSprite(player, 'idle')
    world:addSystem(M.UpdatePlayer):addSystem(M.UpdatePlayerSprite):addSystem(M.CheckPlayerOnDoor)
end

return M
