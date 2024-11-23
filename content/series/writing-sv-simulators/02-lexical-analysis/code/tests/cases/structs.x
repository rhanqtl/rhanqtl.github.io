use std.math

struct Point {
    x: f64
    y: f64
}

impl Point {
    fn new(x: f64, y: f64) -> Point {
        Point(x, y = y)
    }

    fn distance(&self, other: &Point) -> Float {
        math.sqrt(self.x + self.y)
    }
}

trait ToString {
    fn to_string(&self) -> String
}
