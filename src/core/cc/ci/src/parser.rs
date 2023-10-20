use crate::ast::*;
use crate::keyword;
use crate::scanner::Scanner;
use crate::token::{Keyword, Token, TokenKind};

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
    pub nodes: Vec<Decl<'a>>,
    cursor: Cursor<'a>,
}

impl<'a> Parser<'a> {
    pub fn new(scanner: &'a Scanner<'a>) -> Self {
        Self {
            nodes: Vec::new(),
            cursor: Cursor::new(&scanner),
        }
    }

    #[inline]
    fn previous(&self) -> &'a Rc<Token<'a>> {
        self.cursor.previous.unwrap()
    }

    #[inline]
    fn current(&self) -> &'a Rc<Token<'a>> {
        self.cursor.current.unwrap()
    }

    fn expect(&mut self, kind: TokenKind) {
        if &self.current().kind == &kind {
            return self.cursor.next();
        }

        unreachable!("expected {:?}", kind);
    }

    fn parse_data_type(&mut self) -> DataType<'a> {
        todo!("parse data type");
    }

    fn parse_struct(&mut self) -> Struct<'a> {
        // 1. Get the name of the struct
        let name = match &self.current().kind {
            TokenKind::Identifier(name) => {
                self.cursor.next();
                name.clone()
            }
            _ => unreachable!("expected name of the struct"),
        };
        let mut generics = Vec::new();
        let mut fields = Vec::new();

        // 2. Parse generics.
        match &self.current().kind {
            TokenKind::LShift => {
                self.cursor.next();

                let mut current = self.current();

                loop {
                    match &current.kind {
                        TokenKind::RShift => {
                            self.cursor.next();
                            break;
                        }
                        TokenKind::Identifier(generic_param_name) => {
                            generics.push(generic_param_name.clone());
                            self.cursor.next();

                            match &self.current().kind {
                                TokenKind::Comma => self.cursor.next(),
                                TokenKind::RShift => {
                                    self.cursor.next();
                                    break;
                                }
                                _ => unreachable!("expected `,`"),
                            }
                        }
                        TokenKind::Eof => unreachable!("expected `>`"),
                        _ => unreachable!("unexpected character"),
                    }

                    current = self.current();
                }
            }
            _ => (),
        }

        self.expect(TokenKind::LBrace);

        // Parse fields.
        loop {
            let data_type = self.parse_data_type();
            let name = match &self.current().kind {
                TokenKind::Identifier(field_name) => {
                    self.cursor.next();
                    field_name.clone()
                }
                _ => unreachable!("expected field name"),
            };

            fields.push(StructField::new(name, data_type));

            match &self.current().kind {
                TokenKind::RBrace => {
                    self.cursor.next();
                    break;
                }
                TokenKind::Comma => self.cursor.next(),
                _ => unreachable!("unexpected character"),
            }
        }

        self.expect(TokenKind::Semicolon);

        Struct::new(name, fields, Vec::new(), Vec::new(), Vec::new())
    }

    pub fn run(&mut self) {
        self.cursor.init();

        loop {
            let current = self.current();
            let mut location_decl = current.location.clone();

            self.cursor.next();

            match &current.kind {
                TokenKind::Eof => break,
                keyword!(Struct) => {
                    let r#struct = self.parse_struct();
                    let previous = self.previous();

                    location_decl.end(previous.location.end_line, previous.location.end_column);

                    self.nodes
                        .push(Decl::new(DeclKind::Struct(r#struct), location_decl))
                }
                _ => todo!("parse token"),
            }
        }
    }
}
