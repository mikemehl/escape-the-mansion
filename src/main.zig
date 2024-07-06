// raylib-zig (c) Nikolas Wipper 2023

const rl = @import("raylib");
const title = @import("title.zig");
const screen = @import("screen.zig");
const room = @import("room.zig");

pub fn main() anyerror!void {
    // Initialization
    //--------------------------------------------------------------------------------------
    rl.initWindow(screen.width, screen.height, "escape the mansion");
    defer rl.closeWindow(); // Close window and OpenGL context

    rl.setTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    title.loop();

    try room.Room.new(50, 50, &[_]*room.Door{}, null);

    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        defer rl.endDrawing();
        rl.clearBackground(rl.Color.black);
        room.Room.draw();

        // if (rl.getKeyPressed() == rl.KeyboardKey.key_null) {
        //     break;
        // }
    }
}
