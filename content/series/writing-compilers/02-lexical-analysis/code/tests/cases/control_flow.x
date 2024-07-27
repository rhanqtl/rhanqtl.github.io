use std::io::println
use std::sys::exit
use std::cvt

fn main(args: List[String]) {
    if args.length() != 2 {
        println("usage: {} integer", args[0])
        exit(1)
    }

    let n: Int = cvt::parse(args[1])?
    while n > 0 {
        println(n)
        n -= 1
    }
}
