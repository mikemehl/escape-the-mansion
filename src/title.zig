const rl = @import("raylib");

fn draw() void {
    const frame_ctr = struct {
        var cnt: i32 = 0;
    };
    frame_ctr.cnt += 1;
    frame_ctr.cnt = @mod(frame_ctr.cnt, 30);
    rl.clearBackground(rl.Color.black);
    rl.drawText("Escape The Mansion", 190, 200, 20, rl.Color.red);
    const message_color = color_blk: {
        if (frame_ctr.cnt <= 15) {
            break :color_blk rl.Color.red;
        } else {
            break :color_blk rl.Color.blue;
        }
    };
    rl.drawText("Press any key to start", 190, 225, 10, message_color);
}

fn update() bool {
    return rl.getKeyPressed() != rl.KeyboardKey.key_null;
}

pub fn loop() void {
    while (!rl.windowShouldClose()) { // Detect window close button or ESC key
        if (update()) {
            break;
        }

        rl.beginDrawing();
        defer rl.endDrawing();
        draw();
    }
}
