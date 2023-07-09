fn main() {
    // 变量绑定
    let x = 5;
    let y: i32 = 10;

    // 控制流语句
    if x < y {
        println!("x is less than y");
    } else if x > y {
        println!("x is greater than y");
    } else {
        println!("x is equal to y");
    }

    // 循环语句
    for i in 0..5 {
        println!("{}", i);
    }

    // 函数定义
    fn add_numbers(x: i32, y: i32) -> i32 {
        x + y
    }

    // 结构体定义和实例化
    struct Point {
        x: i32,
        y: i32,
    }
    let p = Point { x: 1, y: 2 };

    // 枚举类型定义和匹配
    enum Direction {
        Up,
        Down,
        Left,
        Right,
    }
    let d = Direction::Up;
    match d {
        Direction::Up => println!("Going up!"),
        Direction::Down => println!("Going down!"),
        Direction::Left => println!("Going left!"),
        Direction::Right => println!("Going right!"),
    }

    // 泛型函数定义
    fn print_vec<T>(vec: Vec<T>) {
        for v in vec {
            println!("{}", v);
        }
    }
    let vec = vec![1, 2, 3];
    print_vec(vec);
}