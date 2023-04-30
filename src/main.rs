mod help;
mod location;
mod scanner;
mod token;

use std::env;
use std::process::exit;

use scanner::{Scanner, Source};

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        eprintln!("error: please speficy the filename");
        exit(1);
    } else {
        for filename in &args[1..] {
            let filename = filename.as_str();
            let source = Source::new(filename, Source::read(filename));
            let mut scanner = Scanner::new(&source);

            scanner.run();
        }
    }
}
