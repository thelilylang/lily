use crate::keyword;
use crate::location::Location;
use crate::token::{Comment, Keyword, LiteralConstant, Token, TokenKind};

use std::fs;
use std::rc::Rc;

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
    pub tokens: Vec<Rc<Token<'a>>>,
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

    #[inline]
    fn is_digit(&self) -> bool {
        self.cursor.current >= '0' && self.cursor.current <= '9'
    }

    #[inline]
    fn is_ident(&self) -> bool {
        self.is_digit()
            || (self.cursor.current >= 'a' && self.cursor.current <= 'z')
            || (self.cursor.current >= 'A' && self.cursor.current <= 'Z')
            || self.cursor.current == '_'
            || self.cursor.current == '$'
    }

    #[inline]
    fn is_hex(&self) -> bool {
        self.is_digit()
            || (self.cursor.current >= 'a' && self.cursor.current <= 'f')
            || (self.cursor.current >= 'A' && self.cursor.current <= 'F')
            || self.cursor.current == '_'
    }

    #[inline]
    fn is_oct(&self) -> bool {
        (self.cursor.current >= '0' && self.cursor.current <= '7') || self.cursor.current == '_'
    }

    #[inline]
    fn is_bin(&self) -> bool {
        (self.cursor.current >= '0' && self.cursor.current <= '1') || self.cursor.current == '_'
    }

    #[inline]
    fn is_num(&self) -> bool {
        self.is_digit()
            || (self.cursor.current == '.' && self.peek(1) != Some('.'))
            || self.cursor.current == 'e'
            || self.cursor.current == 'E'
            || self.cursor.current == '_'
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
            _ => TokenKind::Identifier(Rc::new(id)),
        }
    }

    fn scan_id(&mut self) -> String {
        let mut id = String::new();

        while self.is_ident() {
            id.push(self.cursor.current);
            self.next();
        }

        self.previous();

        id
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
            TokenKind::LiteralConstant(LiteralConstant::String(Rc::new(string))),
            self.location.clone(),
        )
    }

    fn scan_comment_line(&mut self) -> Token<'a> {
        let mut s = String::new();

        self.jump(2);

        while self.cursor.current.is_whitespace() && self.cursor.current != '\n' {
            self.next();
        }

        while self.cursor.current != '\n' {
            s.push(self.cursor.current);
            self.next();
        }

        Token::new(TokenKind::Comment(Comment::Line(s)), self.location.clone())
    }

    fn scan_hex(&mut self) -> Token<'a> {
        let mut s = String::new();

        if self.cursor.current == '0' {
            while self.cursor.current == '0' {
                self.next();
            }

            if !self.is_hex() {
                s.push('0');
            }
        }

        while self.is_hex() {
            if self.cursor.current != '_' {
                s.push(self.cursor.current);
            }

            self.next();
        }

        if s.is_empty() {
            unreachable!("add a digit 0 to 9 or a letter a (A) to f (F)");
        }

        self.previous();

        Token::new(
            TokenKind::LiteralConstant(LiteralConstant::Hex(s)),
            self.location.clone(),
        )
    }

    fn scan_oct(&mut self) -> Token<'a> {
        let mut s = String::new();

        if self.cursor.current == '0' {
            while self.cursor.current == '0' {
                self.next();
            }

            if !self.is_oct() {
                s.push('0');
            }
        }

        while self.is_oct() {
            if self.cursor.current != '_' {
                s.push(self.cursor.current);
            }

            self.next();
        }

        if s.is_empty() {
            unreachable!("add a digit 0 to 7");
        }

        self.previous();

        Token::new(
            TokenKind::LiteralConstant(LiteralConstant::Octal(s)),
            self.location.clone(),
        )
    }

    fn scan_bin(&mut self) -> Token<'a> {
        let mut s = String::new();

        if self.cursor.current == '0' {
            while self.cursor.current == '0' {
                self.next();
            }

            if !self.is_bin() {
                s.push('0');
            }
        }

        while self.is_bin() {
            if self.cursor.current != '_' {
                s.push(self.cursor.current);
            }

            self.next();
        }

        if s.is_empty() {
            unreachable!("add a digit 0 to 1");
        }

        self.previous();

        Token::new(
            TokenKind::LiteralConstant(LiteralConstant::Bin(s)),
            self.location.clone(),
        )
    }

    fn scan_num(&mut self) -> Token<'a> {
        let mut s = String::new();
        let mut is_float = false;
        let mut is_scientific = false;

        while self.is_num() {
            if self.cursor.current == '.' && !is_float {
                is_float = true;
            } else if self.cursor.current == '.' && is_float {
                unreachable!("in a float literal it is forbiden to add more than one `.`")
            }

            if (self.cursor.current == 'e' || self.cursor.current == 'E') && !is_scientific {
                is_scientific = true;
            } else if (self.cursor.current == 'e' || self.cursor.current == 'E') && is_scientific {
                unreachable!("in a float literal it is forbiden to add more than one `e` or `E`")
            }

            if self.cursor.current != '_' {
                s.push(self.cursor.current);
            }

            self.next();
        }

        self.previous();

        if is_float || is_scientific {
            return Token::new(
                TokenKind::LiteralConstant(LiteralConstant::Float(s)),
                self.location.clone(),
            );
        }

        Token::new(
            TokenKind::LiteralConstant(LiteralConstant::Int(s)),
            self.location.clone(),
        )
    }

    fn get_num(&mut self) -> Token<'a> {
        match (self.cursor.current, self.peek(1)) {
            ('0', Some('x')) => self.scan_hex(),
            ('0', Some('o')) => self.scan_oct(),
            ('0', Some('b')) => self.scan_bin(),
            _ => self.scan_num(),
        }
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
            ('/', Some('/'), _) => self.scan_comment_line(),
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
            ('a'..='z', _, _) | ('A'..='Z', _, _) | ('_', _, _) => Token::new(
                Scanner::convert_id_to_token(self.scan_id()),
                self.location.clone(),
            ),
            ('0', _, _) => self.get_num(),
            ('1'..='9', _, _) => self.scan_num(),
            ('\"', _, _) => self.scan_string(),
            ('\'', _, _) => {
                self.next();
                let c = self.cursor.current;
                self.next();

                if self.cursor.current != '\'' {
                    unreachable!("expected `'`");
                }

                Token::new(
                    TokenKind::LiteralConstant(LiteralConstant::Character(Rc::new(c))),
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
                self.tokens.push(Rc::new(token));

                if self.cursor.position >= self.source.content.len() - 1 {
                    break;
                }
            }
        }

        let location_eof = self.location.clone();

        self.location
            .start(location_eof.end_line, location_eof.end_column);
        self.tokens
            .push(Rc::new(Token::new(TokenKind::Eof, location_eof)));
    }
}