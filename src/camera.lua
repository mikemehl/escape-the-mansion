local M = {
    camera = require('lib.gamera').new(0, 0, 640, 480),
}

M.SystemUpdateCamera = System({ pool = { 'player', 'position' } })

function M.SystemUpdateCamera:update()
    for _, e in ipairs(self.pool) do
        M.camera:setPosition(e.position.x, e.position.y)
    end
end

function M:draw(fn) self.camera:draw(fn) end

function M.init(world)
    M.camera:setScale(4.0)
    world:addSystem(M.SystemUpdateCamera)
end

return M
