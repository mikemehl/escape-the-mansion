---@class Player
local M = {}

local PLAYER_SPEED = 100

local input = require('src.input')
local render = require('src.render')

local sprites = {
    idle = {
        gx = 32,
        gy = 32,
        image = love.graphics.newImage('assets/characters/HumanTownsfolkIdle.png'),
        xframes = '1-4',
        yframes = 1,
        duration = 0.1,
    },
    walkingrf = {
        gx = 32,
        gy = 32,
        image = love.graphics.newImage('assets/characters/HumanTownsfolkWalk.png'),
        xframes = '1-4',
        yframes = 1,
        duration = 0.1,
    },
    walkinglf = {
        gx = 32,
        gy = 32,
        image = love.graphics.newImage('assets/characters/HumanTownsfolkWalk.png'),
        xframes = '1-4',
        yframes = 2,
        duration = 0.1,
    },
    walkingrb = {
        gx = 32,
        gy = 32,
        image = love.graphics.newImage('assets/characters/HumanTownsfolkWalk.png'),
        xframes = '1-4',
        yframes = 3,
        duration = 0.1,
    },
    walkinglb = {
        gx = 32,
        gy = 32,
        image = love.graphics.newImage('assets/characters/HumanTownsfolkWalk.png'),
        xframes = '1-4',
        yframes = 4,
        duration = 0.1,
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
    entity = entity:give(
        'animatedSprite',
        sprite_spec.gx,
        sprite_spec.gy,
        sprite_spec.image,
        sprite_spec.xframes,
        sprite_spec.yframes,
        sprite_spec.duration
    )
    return entity
end

Component('player')

M.UpdatePlayer = System({ pool = { 'player' } })

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

function M.init(world)
    local player = Entity(world):give('position', 50, 50):give('velocity', 0, 0):give('player')
    player = setSprite(player, 'idle')
    world:addSystem(M.UpdatePlayer)
    world:addSystem(M.UpdatePlayerSprite)
end

return M
