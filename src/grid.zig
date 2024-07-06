pub const GridSize = 32;

pub fn align_to(val: i32) i32 {
    return val + @mod(val, GridSize);
}
