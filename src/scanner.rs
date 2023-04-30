use crate::keyword;
use crate::location::Location;
use crate::token::{Keyword, LiteralConstant, Token, TokenKind};

use std::fs;

pub struct Cursor {
    pub position: usize,
    pub current: char,
}

impl Cursor {
    pub fn new(content: &String) -> Self {
        Self {
            position: 0,
            current: content.chars().nth(0).unwrap(),
        }
    }

    pub fn next(&mut self, current: char) {
        self.position += 1;
        self.current = current;
    }

    pub fn previous(&mut self, previous: char) {
        self.position -= 1;
        self.current = previous;
    }
}

pub struct Source<'a> {
    filename: &'a str,
    content: String,
}

impl<'a> Source<'a> {
    pub fn new(filename: &'a str, content: String) -> Self {
        Self { filename, content }
    }

    pub fn read(filename: &'a str) -> String {
        fs::read_to_string(filename).expect("error: cannot read file")
    }
}

pub struct Scanner<'a> {
    pub tokens: Vec<Token<'a>>,
    cursor: Cursor,
    source: &'a Source<'a>,
    location: Location<'a>,
    line: usize,
    column: usize,
}

impl<'a> Scanner<'a> {
    pub fn new(source: &'a Source<'a>) -> Self {
        Self {
            tokens: Vec::new(),
            cursor: Cursor::new(&source.content),
            source,
            location: Location::new(1, 1, 1, 1, source.filename),
            line: 1,
            column: 1,
        }
    }

    fn next(&mut self) {
        if self.cursor.position + 1 < self.source.content.len() {
            if self.cursor.current == '\n' {
                self.line += 1;
                self.column = 1;
            } else {
                self.column += 1;
            }

            self.cursor.next(
                self.source
                    .content
                    .chars()
                    .nth(self.cursor.position + 1)
                    .unwrap(),
            );
        }
    }

    fn previous(&mut self) {
        if self.cursor.position - 1 > 0 {
            self.column -= 1;

            self.cursor.previous(
                self.source
                    .content
                    .chars()
                    .nth(self.cursor.position - 1)
                    .unwrap(),
            );
        }
    }

    fn jump(&mut self, n: usize) {
        for _ in 0..n {
            self.next();
        }
    }

    #[inline]
    fn peek(&self, n: usize) -> Option<char> {
        self.source.content.chars().nth(self.cursor.position + n)
    }

    fn skip_space(&mut self) {
        while self.cursor.current.is_ascii_whitespace()
            && self.cursor.position < self.source.content.len()
        {
            self.next();
        }
    }

    fn next_char_by_token(&mut self, token: &Token) {
        match &token.kind {
            TokenKind::PlusPlus
            | TokenKind::MinusMinus
            | TokenKind::LShiftEq
            | TokenKind::RShiftEq
            | TokenKind::LShiftLShift
            | TokenKind::RShiftRShift
            | TokenKind::EqEq
            | TokenKind::BangEq
            | TokenKind::AmpersandAmpersand
            | TokenKind::BarBar
            | TokenKind::PlusEq
            | TokenKind::MinusEq
            | TokenKind::StarEq
            | TokenKind::SlashEq
            | TokenKind::PercentageEq
            | TokenKind::AmpersandEq
            | TokenKind::HatEq
            | TokenKind::WaveEq
            | TokenKind::BarEq => self.next(),
            TokenKind::LShiftLShiftEq | TokenKind::RShiftRShiftEq => self.jump(2),
            _ => (),
        }
    }

    fn convert_id_to_token(id: String) -> TokenKind {
        match (&id).as_str() {
            "auto" => keyword!(Auto),
            "break" => keyword!(Break),
            "case" => keyword!(Case),
            "char" => keyword!(Char),
            "const" => keyword!(Const),
            "continue" => keyword!(Continue),
            "default" => keyword!(Default),
            "do" => keyword!(Do),
            "double" => keyword!(Double),
            "else" => keyword!(Else),
            "enum" => keyword!(Enum),
            "extern" => keyword!(Extern),
            "float" => keyword!(Float),
            "for" => keyword!(For),
            "goto" => keyword!(Goto),
            "if" => keyword!(If),
            "int" => keyword!(Int),
            "long" => keyword!(Long),
            "register" => keyword!(Register),
            "return" => keyword!(Return),
            "short" => keyword!(Short),
            "signed" => keyword!(Signed),
            "sizeof" => keyword!(Sizeof),
            "static" => keyword!(Static),
            "struct" => keyword!(Struct),
            "switch" => keyword!(Switch),
            "typedef" => keyword!(Typedef),
            "union" => keyword!(Union),
            "unsigned" => keyword!(Unsigned),
            "void" => keyword!(Void),
            "volatile" => keyword!(Volatile),
            "while" => keyword!(While),
            _ => TokenKind::Identifier(id),
        }
    }

    fn scan_id(&mut self) -> Token<'a> {
        let mut id = String::new();

        while self.cursor.current.is_alphanumeric() {
            id.push(self.cursor.current);
            self.next();
        }

        self.previous();

        Token::new(TokenKind::Identifier(id), self.location.clone())
    }

    fn scan_string(&mut self) -> Token<'a> {
        let mut string = String::new();

        self.next();

        while self.cursor.current != '\"' {
            if self.cursor.position > self.source.content.len() - 2 {
                unreachable!("unmatched string literal");
            }

            string.push(self.cursor.current);
            self.next();
        }

        Token::new(
            TokenKind::LiteralConstant(LiteralConstant::String(string)),
            self.location.clone(),
        )
    }

    fn get_token(&mut self) -> Token<'a> {
        self.location.start(self.line, self.column);

        match (self.cursor.current, self.peek(1), self.peek(2)) {
            ('[', _, _) => Token::new(TokenKind::LHook, self.location.clone()),
            (']', _, _) => Token::new(TokenKind::RHook, self.location.clone()),
            ('(', _, _) => Token::new(TokenKind::LParen, self.location.clone()),
            (')', _, _) => Token::new(TokenKind::RParen, self.location.clone()),
            ('{', _, _) => Token::new(TokenKind::LBrace, self.location.clone()),
            ('}', _, _) => Token::new(TokenKind::RBrace, self.location.clone()),
            (',', _, _) => Token::new(TokenKind::Comma, self.location.clone()),
            (';', _, _) => Token::new(TokenKind::Semicolon, self.location.clone()),
            ('#', _, _) => Token::new(TokenKind::Hashtag, self.location.clone()),
            ('.', _, _) => Token::new(TokenKind::Dot, self.location.clone()),
            ('+', Some('+'), _) => Token::new(TokenKind::PlusPlus, self.location.clone()),
            ('+', Some('='), _) => Token::new(TokenKind::PlusEq, self.location.clone()),
            ('+', _, _) => Token::new(TokenKind::Plus, self.location.clone()),
            ('-', Some('>'), _) => Token::new(TokenKind::Arrow, self.location.clone()),
            ('-', Some('-'), _) => Token::new(TokenKind::PlusPlus, self.location.clone()),
            ('-', Some('='), _) => Token::new(TokenKind::MinusEq, self.location.clone()),
            ('-', _, _) => Token::new(TokenKind::Minus, self.location.clone()),
            ('!', Some('='), _) => Token::new(TokenKind::BangEq, self.location.clone()),
            ('!', _, _) => Token::new(TokenKind::Bang, self.location.clone()),
            ('&', Some('='), _) => Token::new(TokenKind::AmpersandEq, self.location.clone()),
            ('&', Some('&'), _) => Token::new(TokenKind::AmpersandAmpersand, self.location.clone()),
            ('&', _, _) => Token::new(TokenKind::Ampersand, self.location.clone()),
            ('*', Some('='), _) => Token::new(TokenKind::StarEq, self.location.clone()),
            ('*', _, _) => Token::new(TokenKind::Star, self.location.clone()),
            ('/', Some('='), _) => Token::new(TokenKind::SlashEq, self.location.clone()),
            ('/', _, _) => Token::new(TokenKind::Slash, self.location.clone()),
            ('%', Some('='), _) => Token::new(TokenKind::PercentageEq, self.location.clone()),
            ('%', _, _) => Token::new(TokenKind::Percentage, self.location.clone()),
            ('<', Some('<'), Some('=')) => {
                Token::new(TokenKind::LShiftLShiftEq, self.location.clone())
            }
            ('<', Some('='), _) => Token::new(TokenKind::LShiftEq, self.location.clone()),
            ('<', Some('<'), _) => Token::new(TokenKind::LShiftLShift, self.location.clone()),
            ('<', _, _) => Token::new(TokenKind::LShift, self.location.clone()),
            ('>', Some('>'), Some('=')) => {
                Token::new(TokenKind::RShiftRShiftEq, self.location.clone())
            }
            ('>', Some('='), _) => Token::new(TokenKind::RShiftEq, self.location.clone()),
            ('>', Some('>'), _) => Token::new(TokenKind::RShiftRShift, self.location.clone()),
            ('>', _, _) => Token::new(TokenKind::RShift, self.location.clone()),
            ('=', Some('='), _) => Token::new(TokenKind::EqEq, self.location.clone()),
            ('=', _, _) => Token::new(TokenKind::Eq, self.location.clone()),
            ('^', Some('='), _) => Token::new(TokenKind::HatEq, self.location.clone()),
            ('^', _, _) => Token::new(TokenKind::Hat, self.location.clone()),
            ('|', Some('='), _) => Token::new(TokenKind::BarEq, self.location.clone()),
            ('|', Some('|'), _) => Token::new(TokenKind::BarBar, self.location.clone()),
            ('|', _, _) => Token::new(TokenKind::Bar, self.location.clone()),
            ('?', _, _) => Token::new(TokenKind::Interrogation, self.location.clone()),
            (':', _, _) => Token::new(TokenKind::Colon, self.location.clone()),
            ('~', Some('='), _) => Token::new(TokenKind::WaveEq, self.location.clone()),
            ('~', _, _) => Token::new(TokenKind::Wave, self.location.clone()),
            ('a'..='z', _, _) | ('A'..='Z', _, _) | ('_', _, _) => self.scan_id(),
            ('0', _, _) => todo!("hex, oct, bin, float, int"),
            ('1' ..= '9', _, _) => todo!("int, float"),
            ('\"', _, _) => self.scan_string(),
            ('\'', _, _) => {
                self.next();
                let c = self.cursor.current;
                self.next();

                if self.cursor.current != '\'' {
                    unreachable!("expected `'`");
                }

                Token::new(
                    TokenKind::LiteralConstant(LiteralConstant::Character(c)),
                    self.location.clone(),
                )
            }
            _ => unreachable!("unknown character: `{}`", self.cursor.current),
        }
    }

    pub fn run(&mut self) {
        if self.source.content.len() > 1 {
            while self.cursor.position < self.source.content.len() - 1 {
                self.skip_space();

                if self.cursor.position >= self.source.content.len() - 1 {
                    break;
                }

                let mut token = self.get_token();

                self.next_char_by_token(&token);
                self.location.end(self.line, self.column);
                token.end_token(self.location.end_line, self.location.end_column);

                self.next();
                self.tokens.push(token);

                if self.cursor.position >= self.source.content.len() - 1 {
                    break;
                }
            }
        }
    }
}
