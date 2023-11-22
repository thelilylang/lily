use crate::ast::*;
use crate::keyword;
use crate::scanner::Scanner;
use crate::token::LiteralConstant;
use crate::token::{Keyword, Token, TokenKind};

use std::iter::Peekable;
use std::rc::Rc;
use std::slice::Iter;

enum StackedBinary<'a> {
    Expr(Expr<'a>),
    Binary(BinaryKind),
}

impl<'a> StackedBinary<'a> {
    pub fn unwrap_expr(self) -> Expr<'a> {
        match self {
            Self::Expr(expr) => expr,
            _ => unreachable!("expected expr"),
        }
    }

    pub fn unwrap_binary(self) -> BinaryKind {
        match self {
            Self::Binary(binary) => binary,
            _ => unreachable!("expected binary"),
        }
    }
}

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
            keyword!(Char) => DataType::new(DataTypeKind::Char, current.location.clone()),
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
                let long_data_type_location = self.current().location.clone();
                let current = self.cursor.peek();
                let mut long_data_type = match &current {
                    Some(token_ptr) => match &****token_ptr {
                        Token {
                            kind: keyword!(Double),
                            ..
                        } => {
                            self.cursor.next();
                            DataType::new(DataTypeKind::LongDouble, long_data_type_location)
                        }
                        Token {
                            kind: keyword!(Long),
                            ..
                        } => {
                            self.cursor.next();

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

                            DataType::new(DataTypeKind::LongLongInt, long_data_type_location)
                        }
                        Token {
                            kind: keyword!(Int),
                            ..
                        } => {
                            self.cursor.next();

                            DataType::new(DataTypeKind::LongInt, long_data_type_location)
                        }
                        _ => DataType::new(DataTypeKind::LongInt, long_data_type_location),
                    },
                    None => DataType::new(DataTypeKind::LongInt, long_data_type_location),
                };
                let current = self.current();

                long_data_type
                    .location
                    .end(current.location.end_line, current.location.end_column);

                long_data_type
            }
            keyword!(Short) => {
                let short_data_type_location = self.current().location.clone();
                let current = self.cursor.peek();
                let mut short_data_type = match &current {
                    Some(token_ptr) => match &****token_ptr {
                        Token {
                            kind: keyword!(Int),
                            ..
                        } => {
                            self.cursor.next();

                            DataType::new(DataTypeKind::ShortInt, short_data_type_location)
                        }
                        _ => DataType::new(DataTypeKind::ShortInt, short_data_type_location),
                    },
                    None => DataType::new(DataTypeKind::ShortInt, short_data_type_location),
                };
                let current = self.current();

                short_data_type
                    .location
                    .end(current.location.end_line, current.location.end_column);

                short_data_type
            }
            keyword!(Signed) => {
                let signed_data_type_location = self.current().location.clone();
                let current = self.cursor.peek();
                let mut signed_data_type = match &current {
                    Some(token_ptr) => match &****token_ptr {
                        Token {
                            kind: keyword!(Char),
                            ..
                        } => {
                            self.cursor.next();
                            DataType::new(DataTypeKind::Char, signed_data_type_location)
                        }
                        Token {
                            kind: keyword!(Short),
                            ..
                        } => {
                            self.cursor.next();

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

                            DataType::new(DataTypeKind::ShortInt, signed_data_type_location)
                        }
                        Token {
                            kind: keyword!(Long),
                            ..
                        } => {
                            self.cursor.next();

                            let mut data_type_kind = None;

                            match &self.cursor.peek() {
                                Some(token_ptr) => match &****token_ptr {
                                    Token {
                                        kind: keyword!(Int),
                                        ..
                                    } => self.cursor.next(),
                                    Token {
                                        kind: keyword!(Long),
                                        ..
                                    } => {
                                        self.cursor.next();

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

                                        data_type_kind = Some(DataTypeKind::LongLongInt);
                                    }
                                    _ => (),
                                },
                                None => (),
                            }

                            DataType::new(
                                data_type_kind.unwrap_or(DataTypeKind::LongInt),
                                signed_data_type_location,
                            )
                        }
                        Token {
                            kind: keyword!(Int),
                            ..
                        } => {
                            self.cursor.next();

                            DataType::new(DataTypeKind::Int, signed_data_type_location)
                        }
                        _ => DataType::new(DataTypeKind::Int, signed_data_type_location),
                    },
                    None => DataType::new(DataTypeKind::Int, signed_data_type_location),
                };
                let current = self.current();

                signed_data_type
                    .location
                    .end(current.location.end_line, current.location.end_column);

                signed_data_type
            }
            keyword!(Struct) => todo!("struct"),
            keyword!(Union) => todo!("union"),
            keyword!(Unsigned) => {
                let unsigned_data_type_location = self.current().location.clone();
                let current = self.cursor.peek();
                let mut unsigned_data_type = match &current {
                    Some(token_ptr) => match &****token_ptr {
                        Token {
                            kind: keyword!(Char),
                            ..
                        } => {
                            self.cursor.next();
                            DataType::new(DataTypeKind::UnsignedChar, unsigned_data_type_location)
                        }
                        Token {
                            kind: keyword!(Short),
                            ..
                        } => {
                            self.cursor.next();

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

                            DataType::new(
                                DataTypeKind::UnsignedShortInt,
                                unsigned_data_type_location,
                            )
                        }
                        Token {
                            kind: keyword!(Long),
                            ..
                        } => {
                            self.cursor.next();

                            let mut data_type_kind = None;

                            match &self.cursor.peek() {
                                Some(token_ptr) => match &****token_ptr {
                                    Token {
                                        kind: keyword!(Int),
                                        ..
                                    } => self.cursor.next(),
                                    Token {
                                        kind: keyword!(Long),
                                        ..
                                    } => {
                                        self.cursor.next();

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

                                        data_type_kind = Some(DataTypeKind::UnsignedLongLongInt);
                                    }
                                    _ => (),
                                },
                                None => (),
                            }

                            DataType::new(
                                data_type_kind.unwrap_or(DataTypeKind::UnsignedLongInt),
                                unsigned_data_type_location,
                            )
                        }
                        Token {
                            kind: keyword!(Int),
                            ..
                        } => {
                            self.cursor.next();

                            DataType::new(DataTypeKind::UnsignedInt, unsigned_data_type_location)
                        }
                        _ => DataType::new(DataTypeKind::UnsignedInt, unsigned_data_type_location),
                    },
                    None => DataType::new(DataTypeKind::UnsignedInt, unsigned_data_type_location),
                };
                let current = self.current();

                unsigned_data_type
                    .location
                    .end(current.location.end_line, current.location.end_column);

                unsigned_data_type
            }
            keyword!(Void) => DataType::new(DataTypeKind::Void, current.location.clone()),
            TokenKind::Identifier(name) => {
                DataType::new(DataTypeKind::Custom(name.clone()), current.location.clone())
            }
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

    fn parse_primary_expr(&mut self) -> Expr<'a> {
        let mut location = self.current().location.clone();

        self.cursor.next();

        match &self.previous().kind {
            TokenKind::LiteralConstant(literal) => match literal {
                LiteralConstant::Int(i) => {
                    let mut i_parsed = None;

                    match i.parse::<i64>() {
                        Ok(i) => i_parsed = Some(i),
                        Err(_) => unreachable!("expected integer"),
                    }

                    Expr::new(ExprKind::Literal(Literal::Int(i_parsed.unwrap())), location)
                }
                LiteralConstant::Float(f) => {
                    let mut f_parsed = None;

                    match f.parse::<f64>() {
                        Ok(f) => f_parsed = Some(f),
                        Err(_) => unreachable!("expected float"),
                    }

                    Expr::new(
                        ExprKind::Literal(Literal::Float(f_parsed.unwrap())),
                        location,
                    )
                }
                LiteralConstant::Octal(o) => {
                    let mut o_parsed = None;

                    match i64::from_str_radix(o, 8) {
                        Ok(o) => o_parsed = Some(o),
                        Err(_) => unreachable!("expected octal"),
                    }

                    Expr::new(ExprKind::Literal(Literal::Int(o_parsed.unwrap())), location)
                }
                LiteralConstant::Hex(h) => {
                    let mut h_parsed = None;

                    match i64::from_str_radix(h, 16) {
                        Ok(h) => h_parsed = Some(h),
                        Err(_) => unreachable!("expected hex"),
                    }

                    Expr::new(ExprKind::Literal(Literal::Int(h_parsed.unwrap())), location)
                }
                LiteralConstant::Bin(b) => {
                    let mut b_parsed = None;

                    match i64::from_str_radix(b, 2) {
                        Ok(b) => b_parsed = Some(b),
                        Err(_) => unreachable!("expected bin"),
                    }

                    Expr::new(ExprKind::Literal(Literal::Int(b_parsed.unwrap())), location)
                }
                LiteralConstant::Character(c) => {
                    Expr::new(ExprKind::Literal(Literal::Character(*c.clone())), location)
                }
                LiteralConstant::String(s) => {
                    Expr::new(ExprKind::Literal(Literal::String(s.clone())), location)
                }
            },
            TokenKind::Star
            | TokenKind::Minus
            | TokenKind::Bang
            | TokenKind::Ampersand
            | TokenKind::Wave => {
                let unary_op = UnaryKind::from_token(&self.previous().kind);
                let right = self.parse_primary_expr();

                location.end(
                    self.previous().location.end_line,
                    self.previous().location.end_column,
                );

                Expr::new(ExprKind::Unary(Unary::new(unary_op, right)), location)
            }
            TokenKind::Keyword(Keyword::Sizeof) => {
                let mut location_sizeof = location.clone();

                self.cursor.next();

                match &self.current().kind {
                    TokenKind::LParen => {
                        self.cursor.next();

                        let data_type = self.parse_data_type();

                        self.expect(TokenKind::RParen);

                        location_sizeof.end(
                            self.previous().location.end_line,
                            self.previous().location.end_column,
                        );

                        Expr::new(ExprKind::Sizeof(data_type), location_sizeof)
                    }
                    _ => unreachable!("expected `(`"),
                }
            }
            TokenKind::PlusPlus | TokenKind::MinusMinus => {
                let op = &self.previous().kind;
                let expr = self.parse_primary_expr();

                location.end(
                    self.previous().location.end_line,
                    self.previous().location.end_column,
                );

                match op {
                    TokenKind::PlusPlus => Expr::new(ExprKind::PrefixInc(Box::new(expr)), location),
                    TokenKind::MinusMinus => {
                        Expr::new(ExprKind::PrefixDec(Box::new(expr)), location)
                    }
                    _ => unreachable!("unexpected token"),
                }
            }
            _ => unreachable!("unexpected token"),
        }
    }

    fn parse_binary(&mut self, left: Expr<'a>) -> Expr<'a> {
        let mut stack = Vec::new();
        let mut last_precedence = BinaryKind::MAX_PRECEDENCE + 1;
        let mut current = &self.current().kind;

        stack.push(StackedBinary::Expr(left));

        loop {
            if BinaryKind::is_binary(current) {
                let op = BinaryKind::from_token(current);
                let precedence = op.to_precedence();

                self.cursor.next();

                current = &self.current().kind;

                let right = self.parse_primary_expr();

                while precedence <= last_precedence && stack.len() > 1 {
                    let top_right = stack.pop().unwrap().unwrap_expr();
                    let top_op = stack.pop().unwrap().unwrap_binary();

                    last_precedence = top_op.to_precedence();

                    if last_precedence < precedence {
                        stack.push(StackedBinary::Binary(top_op));
                        stack.push(StackedBinary::Expr(top_right));

                        break;
                    }

                    let top_left = stack.pop().unwrap().unwrap_expr();
                    let mut expr_location = top_left.location.clone();

                    expr_location.end(right.location.end_line, right.location.end_column);

                    stack.push(StackedBinary::Expr(Expr::new(
                        ExprKind::Binary(Binary::new(top_left, top_op, top_right)),
                        expr_location,
                    )));
                }

                stack.push(StackedBinary::Binary(op));
                stack.push(StackedBinary::Expr(right));

                last_precedence = precedence;
            } else {
                break;
            }
        }

        while stack.len() > 1 {
            let rhs = stack.pop().unwrap().unwrap_expr();
            let op = stack.pop().unwrap().unwrap_binary();
            let lhs = stack.pop().unwrap().unwrap_expr();
            let mut location = lhs.location.clone();

            location.end(rhs.location.end_line, rhs.location.end_column);

            stack.push(StackedBinary::Expr(Expr::new(
                ExprKind::Binary(Binary::new(lhs, op, rhs)),
                location,
            )));
        }

        stack.pop().unwrap().unwrap_expr()
    }

    fn parse_expr(&mut self) -> Expr<'a> {
        let mut left = self.parse_primary_expr();

        if BinaryKind::is_binary(&self.current().kind) {
            left = self.parse_binary(left);
        }

        match &self.current().kind {
            TokenKind::Semicolon => {
                self.cursor.next();
                left
            }
            _ => unreachable!("unexpected token"),
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
