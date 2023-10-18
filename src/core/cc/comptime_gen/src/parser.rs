use crate::scanner::Scanner;
use crate::token::{Token, TokenKind};

use std::rc::Rc;
use std::slice::Iter;

struct Cursor<'a> {
    iter: Iter<'a, Rc<Token<'a>>>,
    previous: Option<&'a Rc<Token<'a>>>,
    current: Option<&'a Rc<Token<'a>>>,
}

impl<'a> Cursor<'a> {
    fn new(scanner: &'a Scanner<'a>) -> Self {
        Self {
            iter: scanner.tokens.iter(),
            previous: None,
            current: None,
        }
    }

    pub fn init(&mut self) {
        self.current = self.iter.next();
    }

    fn next(&mut self) {
        self.previous = self.current;
        self.current = self.iter.next();
    }
}

pub struct Parser<'a> {
    scanner: &'a Scanner<'a>,
    cursor: Cursor<'a>,
}

impl<'a> Parser<'a> {
    pub fn new(scanner: &'a Scanner<'a>) -> Self {
        Self {
            cursor: Cursor::new(&scanner),
            scanner,
        }
    }

    fn parse_struct(&mut self) {}

    pub fn run(&mut self) {
        self.cursor.init();

        loop {
            match &self.cursor.current {
                Some(token) => match &token.kind {
                    TokenKind::Eof => break,
                    _ => todo!("parse token"),
                },
                _ => break,
            }
        }
    }
}
