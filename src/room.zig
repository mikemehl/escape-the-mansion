const std = @import("std");
const rl = @import("raylib");
const screen = @import("screen.zig");
const grid = @import("grid.zig");

const MAX_W = screen.width;
const MAX_H = screen.height;

var room_allocator = std.heap.ArenaAllocator.init(std.heap.page_allocator);
var door_allocator = std.heap.ArenaAllocator.init(std.heap.page_allocator);

pub const Door = struct {
    const Self = @This();
    x: i32,
    y: i32,
    to: ?*Room,
    pub fn new(x: i32, y: i32, to: ?*Room) *Self {
        return door_allocator.alloc(Self{ .x = x, .y = y, .to = to });
    }
};

pub const Room = struct {
    const Self = @This();
    var list = std.ArrayList(Room).init(room_allocator.allocator());
    var curr: ?*Room = null;
    id: u32,
    width: i32,
    height: i32,
    doors: []*Door,

    pub fn new(width: i32, height: i32, doors: []*Door, id: ?u32) !void {
        const id_set = id_set: {
            if (id == null) {
                break :id_set std.crypto.random.int(u32);
            }
            break :id_set id.?;
        };
        const room = Room{ .width = grid.align_to(width), .height = grid.align_to(height), .doors = doors, .id = id_set };
        try Self.list.append(room);
        if (Self.list.items.len == 1) {
            Self.curr = &Self.list.items[0];
        }
    }

    pub fn draw() void {
        if (Self.curr != null) {
            Self.curr.?.draw_room();
        }
    }

    pub fn cleanup() void {
        door_allocator.deinit();
        room_allocator.deinit();
    }

    fn draw_room(self: *const Self) void {
        const x_offset = screen.center_x - @divTrunc(self.*.width, 2);
        const y_offset = screen.center_y - @divTrunc(self.*.height, 2);
        rl.drawRectangle(x_offset, y_offset, self.*.width, self.*.height, rl.Color.light_gray);
    }
};
