mod ast;
mod generator;
mod help;
mod location;
mod parser;
mod scanner;
mod token;

use std::cell::RefCell;
use std::env;
use std::process::exit;
use std::rc::Rc;

use generator::Generator;
use parser::Parser;
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
            let scanner = Rc::new(RefCell::new(Scanner::new(&source)));
            let mut parser = Parser::new(Rc::clone(&scanner));

            scanner.borrow_mut().run();
            parser.run();

            let mut generator = Generator::new(&source, Rc::clone(&scanner), &parser);

            generator.run();
        }
    }
}
