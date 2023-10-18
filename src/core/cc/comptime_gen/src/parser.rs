use crate::scanner::Scanner;

use std::cell::RefCell;
use std::rc::Rc;

pub struct Parser<'a> {
    pub scanner: Rc<RefCell<Scanner<'a>>>,
}

impl<'a> Parser<'a> {
    pub fn new(scanner: Rc<RefCell<Scanner<'a>>>) -> Self {
        Self { scanner }
    }

    pub fn run(&mut self) {}
}
