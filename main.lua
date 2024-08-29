if os.getenv('LOCAL_LUA_DEBUGGER_VSCODE') == '1' then
    package.loaded['lldebugger'] = assert(
        loadfile('/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua')
    )()
    require('lldebugger').start()
end

love.graphics.setDefaultFilter('nearest', 'nearest')

require('src.globals')
local room = require('src.room')

local input = require('src.input')
local physics = require('src.physics')
local player = require('src.player')
local render = require('src.render')

local camera = require('src.camera')
local world = World()

function love.load()
    room:init(world)
    physics.init(world)
    player.init(world)
    render.init(world)
    camera.init(world)
end

function love.update(dt)
    input:update()
    world:emit('update', dt)
end

function love.draw()
    camera:draw(function(l, t, w, h)
        room:draw()
        world:emit('draw')
    end)
    love.graphics.print('HELLO WORLD', 50, 50)
end
