use crate::ast::*;
use crate::keyword;
use crate::scanner::Scanner;
use crate::token::{Keyword, Token, TokenKind};

use std::iter::Peekable;
use std::rc::Rc;
use std::slice::Iter;

struct Cursor<'a> {
    iter: Peekable<Iter<'a, Rc<Token<'a>>>>,
    previous: Option<&'a Rc<Token<'a>>>,
    current: Option<&'a Rc<Token<'a>>>,
}

impl<'a> Cursor<'a> {
    fn new(scanner: &'a Scanner<'a>) -> Self {
        Self {
            iter: scanner.tokens.iter().peekable(),
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

    fn peek(&mut self) -> Option<&&'a Rc<Token<'a>>> {
        self.iter.peek()
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
        let current = self.current();
        let res = match &current.kind {
            keyword!(Char) => todo!("char"),
            keyword!(Const) => {
                self.cursor.next();

                let mut location_const = current.location.clone();
                let const_data_type = self.parse_data_type();
                let previous = self.previous();

                location_const.end(previous.location.end_line, previous.location.end_line);

                return DataType::new(
                    DataTypeKind::Const(Box::new(const_data_type)),
                    location_const,
                );
            }
            keyword!(Double) => DataType::new(DataTypeKind::Double, current.location.clone()),
            keyword!(Enum) => todo!("enum"),
            keyword!(Float) => DataType::new(DataTypeKind::Float, current.location.clone()),
            keyword!(Int) => DataType::new(DataTypeKind::Int, current.location.clone()),
            keyword!(Long) => {
                self.cursor.next();

                let current = self.current();
                let mut long_data_type = match &current.kind {
                    keyword!(Double) => {
                        DataType::new(DataTypeKind::LongDouble, current.location.clone())
                    }
                    keyword!(Long) => {
                        match &self.cursor.peek() {
                            Some(token_ptr) => match &****token_ptr {
                                Token {
                                    kind: keyword!(Int),
                                    ..
                                } => self.cursor.next(),
                                _ => (),
                            },
                            None => (),
                        }

                        DataType::new(DataTypeKind::LongLongInt, current.location.clone())
                    }
                    keyword!(Int) | _ => {
                        DataType::new(DataTypeKind::LongInt, current.location.clone())
                    }
                };
                let current = self.current();

                long_data_type
                    .location
                    .end(current.location.end_line, current.location.end_column);

                long_data_type
            }
            keyword!(Short) => todo!("short"),
            keyword!(Signed) => todo!("signed"),
            keyword!(Struct) => todo!("struct"),
            keyword!(Union) => todo!("union"),
            keyword!(Unsigned) => todo!("unsigned"),
            keyword!(Void) => DataType::new(DataTypeKind::Void, current.location.clone()),
            TokenKind::Identifier(_) => todo!("custom data type"),
            k => unreachable!("unexpected token: {:?}", k),
        };

        self.cursor.next();

        let current = self.current();

        match &current.kind {
            TokenKind::LParen => todo!("function"),
            TokenKind::Star => {
                let mut location_pointer = res.location.clone();

                location_pointer.end(current.location.end_line, current.location.end_column);
                self.cursor.next();

                DataType::new(DataTypeKind::Pointer(Box::new(res)), location_pointer)
            }
            _ => res,
        }
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

            self.expect(TokenKind::Semicolon);

            match &self.current().kind {
                TokenKind::RBrace => {
                    self.cursor.next();
                    break;
                }
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
