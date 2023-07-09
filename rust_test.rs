// 引入 std 库中的 println! 宏
use std::io::{self, Write};

fn print_num(a: i32, b: u32) {
    println!("Var is {}, Constant is {}", a, b);
}

// main 函数是程序的入口点
fn main() {
    // 输出 "Hello, world!
    println!("Hello, world!");

    // 定义一个变量（可变）
    let mut var = 5;

    //'let' 关键字用于绑定变量的
    // 可以用类型标注来明确变量类型
    let var_str: &str = "I am a string.";

    // 定义一个常量（不可变）
    const CONSTANT: u32 = 100;

    // 调用一个简单的函数
    print_num(var, CONSTANT);

    // 匿名函数（闭包）示例
    let add_one = |x: i32| x + 1;
    let result = add_one(var);

    println!("Add-one result: {}", result);

    // 结构体示例
    struct Point {
        x: f64,
        y: f64,
    }

    // 定义结构体实例
    let point = Point { x: 1.0, y: 1.0 };

    println!("Point is at ({}, {})", point.x, point.y);

    // 枚举示例
    enum Color {
        Red,
        Green,
        Blue,
    }

    let color = Color::Red;
    match color {
        Color::Red => println!("Red"),
        Color::Green => println!("Green"),
        Color::Blue => println!("Blue"),
    }
}
