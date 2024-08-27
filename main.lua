if os.getenv('LOCAL_LUA_DEBUGGER_VSCODE') == '1' then
    package.loaded['lldebugger'] = assert(
        loadfile('/usr/lib/node_modules/local-lua-debugger-vscode/debugger/lldebugger.lua')
    )()
    require('lldebugger').start()
end

require('src.globals')
local input = require('src.input')
local player = require('src.player')
local render = require('src.render')
local room = require('src.room')

local world = World()

function love.load()
    love.graphics.setDefaultFilter('nearest', 'nearest')
    room:init()
    player.init(world)
    render.init(world)
end

function love.update(dt)
    input:update()
    world:emit('update', dt)
end

function love.draw()
    room:draw()
    world:emit('draw')
    love.graphics.print('HELLO WORLD', 50, 50)
end
