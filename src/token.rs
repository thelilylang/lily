use crate::location::Location;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Keyword {
    Auto,
    Break,
    Case,
    Char,
    Const,
    Continue,
    Default,
    Do,
    Double,
    Else,
    Enum,
    Extern,
    Float,
    For,
    Goto,
    If,
    Int,
    Long,
    Register,
    Return,
    Short,
    Signed,
    Sizeof,
    Static,
    Struct,
    Switch,
    Typedef,
    Union,
    Unsigned,
    Void,
    Volatile,
    While,
}

impl ToString for Keyword {
    fn to_string(&self) -> String {
        match self {
            Self::Auto => "auto",
            Self::Break => "break",
            Self::Case => "case",
            Self::Char => "char",
            Self::Const => "const",
            Self::Continue => "continue",
            Self::Default => "default",
            Self::Do => "do",
            Self::Double => "double",
            Self::Else => "else",
            Self::Enum => "enum",
            Self::Extern => "extern",
            Self::Float => "float",
            Self::For => "for",
            Self::Goto => "goto",
            Self::If => "if",
            Self::Int => "int",
            Self::Long => "long",
            Self::Register => "register",
            Self::Return => "return",
            Self::Short => "short",
            Self::Signed => "signed",
            Self::Sizeof => "sizeof",
            Self::Static => "static",
            Self::Struct => "struct",
            Self::Switch => "switch",
            Self::Typedef => "typedef",
            Self::Union => "union",
            Self::Unsigned => "unsigned",
            Self::Void => "void",
            Self::Volatile => "volatile",
            Self::While => "while",
        }
        .to_string()
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum LiteralConstant {
    Int(String),
    Float(String),
    Octal(String),
    Hex(String),
    Bin(String),
    Character(char),
    String(String),
}

impl ToString for LiteralConstant {
    fn to_string(&self) -> String {
        match &self {
            Self::Int(s) | Self::Float(s) | Self::Octal(s) | Self::Hex(s) | Self::Bin(s) | Self::String(s) => {
                s.clone()
            }
            Self::Character(c) => format!("{}", c),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Comment {
    Line(String),
    Block,
    Doc,
}

impl ToString for Comment {
    fn to_string(&self) -> String {
        match &self {
            Self::Line(comment) => format!("// {}", comment),
            Self::Block => "/**/".to_string(),
            Self::Doc => "/** */".to_string(),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TokenKind {
    Keyword(Keyword),
    LiteralConstant(LiteralConstant),
    LHook,
    RHook,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Comma,
    Semicolon,
    Hashtag,
    Dot,
    Arrow,
    PlusPlus,
    MinusMinus,
    Plus,
    Minus,
    Bang,
    Ampersand,
    Star,
    Slash,
    Percentage,
    LShift,
    RShift,
    LShiftEq,
    RShiftEq,
    LShiftLShift,
    RShiftRShift,
    EqEq,
    BangEq,
    Hat,
    Bar,
    AmpersandAmpersand,
    BarBar,
    Interrogation,
    Colon,
    Eq,
    PlusEq,
    MinusEq,
    StarEq,
    SlashEq,
    PercentageEq,
    LShiftLShiftEq,
    RShiftRShiftEq,
    AmpersandEq,
    HatEq,
    BarEq,
    Wave,
    WaveEq,
    Identifier(String),
    Comment(Comment),
}

impl ToString for TokenKind {
    fn to_string(&self) -> String {
        match &self {
            Self::Keyword(k) => k.to_string(),
            Self::LiteralConstant(l) => l.to_string(),
            Self::LHook => "[".to_string(),
            Self::RHook => "]".to_string(),
            Self::LParen => "(".to_string(),
            Self::RParen => ")".to_string(),
            Self::LBrace => "{".to_string(),
            Self::RBrace => "}".to_string(),
            Self::Comma => ",".to_string(),
            Self::Semicolon => ";".to_string(),
            Self::Hashtag => "#".to_string(),
            Self::Dot => ".".to_string(),
            Self::Arrow => "->".to_string(),
            Self::PlusPlus => "++".to_string(),
            Self::MinusMinus => "--".to_string(),
            Self::Plus => "+".to_string(),
            Self::Minus => "-".to_string(),
            Self::Bang => "!".to_string(),
            Self::Ampersand => "&".to_string(),
            Self::Star => "*".to_string(),
            Self::Slash => "/".to_string(),
            Self::Percentage => "%".to_string(),
            Self::LShift => "<".to_string(),
            Self::RShift => ">".to_string(),
            Self::LShiftEq => "<=".to_string(),
            Self::RShiftEq => ">=".to_string(),
            Self::LShiftLShift => "<<".to_string(),
            Self::RShiftRShift => ">>".to_string(),
            Self::EqEq => "==".to_string(),
            Self::BangEq => "!=".to_string(),
            Self::Hat => "^".to_string(),
            Self::Bar => "|".to_string(),
            Self::AmpersandAmpersand => "&&".to_string(),
            Self::BarBar => "||".to_string(),
            Self::Interrogation => "?".to_string(),
            Self::Colon => ":".to_string(),
            Self::Eq => "=".to_string(),
            Self::PlusEq => "+=".to_string(),
            Self::MinusEq => "-=".to_string(),
            Self::StarEq => "*=".to_string(),
            Self::SlashEq => "/=".to_string(),
            Self::PercentageEq => "%=".to_string(),
            Self::LShiftLShiftEq => "<<=".to_string(),
            Self::RShiftRShiftEq => ">>=".to_string(),
            Self::AmpersandEq => "&=".to_string(),
            Self::HatEq => "^=".to_string(),
            Self::BarEq => "|=".to_string(),
            Self::Wave => "~".to_string(),
            Self::WaveEq => "~=".to_string(),
            Self::Identifier(s) => s.clone(),
            Self::Comment(comment) => comment.to_string(),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Token<'a> {
    pub kind: TokenKind,
    pub location: Location<'a>,
}

impl<'a> Token<'a> {
    pub fn new(kind: TokenKind, location: Location<'a>) -> Self {
        Self { kind, location }
    }

    #[inline]
    pub fn start_token(&mut self, line: usize, column: usize) {
        self.location.start(line, column)
    }

    #[inline]
    pub fn end_token(&mut self, line: usize, column: usize) {
        self.location.end(line, column);
    }
}

#[macro_export]
macro_rules! keyword {
    ($k: ident) => {
        TokenKind::Keyword(Keyword::$k)
    };
}
