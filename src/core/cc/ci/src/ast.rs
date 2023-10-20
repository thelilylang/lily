use crate::location::Location;

use std::rc::Rc;

#[derive(Debug, Clone, PartialEq)]
pub enum DataTypeKind<'a> {
    UnsignedChar,
    Char,
    SignedChar,
    Int,
    UnsignedInt,
    SignedInt,
    ShortInt,
    UnsignedShortInt,
    SignedShortInt,
    LongInt,
    LongLongInt,
    SignedLongInt,
    UnsignedLongInt,
    UnsignedLongLongInt,
    Float,
    Double,
    LongDouble,
    Void,
    Const(Box<DataType<'a>>),
    Pointer(Box<DataType<'a>>),
    Struct(Rc<String>),
    Enum(Rc<String>),
    Union(Rc<String>),
    Function(Box<DataType<'a>>, Vec<DataType<'a>>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct DataType<'a> {
    kind: DataTypeKind<'a>,
    location: Location<'a>,
}

impl<'a> DataType<'a> {
    pub fn new(kind: DataTypeKind<'a>, location: Location<'a>) -> Self {
        Self { kind, location }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Unary<'a> {
    Dereference(Box<Expr<'a>>),
    Neg(Box<Expr<'a>>),
    Not(Box<Expr<'a>>),
    Ref(Box<Expr<'a>>),
    BitNot(Box<Expr<'a>>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum BinaryKind {
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Percentage,
    Less,
    Greater,
    LessEq,
    GreaterEq,
    BitLShift,
    BitRShift,
    Eq,
    NotEq,
    BitXor,
    BitOr,
    BitAnd,
    And,
    Or,
    Assign,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
    ModAssign,
    BitLShiftAssign,
    BitRShiftAssign,
    BitAndAssign,
    BitXorAssign,
    BitOrAssign,
    BitNotAssign,
}

impl BinaryKind {
    pub fn to_precedence(&self) -> u8 {
        match &self {
            Self::Mul | Self::Div | Self::Mod => 100,
            Self::Add | Self::Sub => 95,
            Self::BitLShift | Self::BitRShift => 90,
            Self::Less | Self::Greater | Self::LessEq | Self::GreaterEq => 85,
            Self::Eq | Self::NotEq => 80,
            Self::BitAnd => 75,
            Self::BitXor => 70,
            Self::BitOr => 65,
            Self::And => 60,
            Self::Or => 55,
            _ => 50,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Binary<'a> {
    left: Box<Expr<'a>>,
    kind: BinaryKind,
    right: Box<Expr<'a>>,
}

impl<'a> Binary<'a> {
    pub fn new(left: Expr<'a>, kind: BinaryKind, right: Expr<'a>) -> Self {
        Self {
            left: Box::new(left),
            kind,
            right: Box::new(right),
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Literal {
    Int(i64),
    Float(f64),
    Character(char),
    String(Rc<String>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum ExprKind<'a> {
    Binary(Binary<'a>),
    Sizeof(DataType<'a>),
    Unary(Unary<'a>),
    Literal(Literal),
    PostfixInc(Box<Expr<'a>>),
    PostfixDec(Box<Expr<'a>>),
    PrefixInc(Box<Expr<'a>>),
    PrefixDec(Box<Expr<'a>>),
    DataType(DataType<'a>),
    Cast {
        dest_dt: Box<Expr<'a>>,
        expr: Box<Expr<'a>>,
    },
    Ternary {
        cond: Box<Expr<'a>>,
        case: Box<Expr<'a>>,
        else_case: Box<Expr<'a>>,
    },
    ArrayAccess {
        access: Box<Expr<'a>>,
        index: Box<Expr<'a>>,
    },
    DotAccess(Vec<Rc<String>>),
    ArrowAccess(Vec<Rc<String>>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct Expr<'a> {
    kind: ExprKind<'a>,
    location: Location<'a>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct IfBranch<'a> {
    cond: Expr<'a>,
    body: Vec<FunctionBodyItem<'a>>,
}

impl<'a> IfBranch<'a> {
    pub fn new(cond: Expr<'a>, body: Vec<FunctionBodyItem<'a>>) -> Self {
        Self { cond, body }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct If<'a> {
    r#if: IfBranch<'a>,
    else_if: Vec<IfBranch<'a>>,
    r#else: Vec<FunctionBodyItem<'a>>,
}

impl<'a> If<'a> {
    pub fn new(
        r#if: IfBranch<'a>,
        else_if: Vec<IfBranch<'a>>,
        r#else: Vec<FunctionBodyItem<'a>>,
    ) -> Self {
        Self {
            r#if,
            else_if,
            r#else,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct SwitchCase<'a> {
    expr: Expr<'a>,
    body: Vec<FunctionBodyItem<'a>>,
}

impl<'a> SwitchCase<'a> {
    pub fn new(expr: Expr<'a>, body: Vec<FunctionBodyItem<'a>>) -> Self {
        Self { expr, body }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Switch<'a> {
    expr: Expr<'a>,
    cases: SwitchCase<'a>,
    default: Option<Vec<FunctionBodyItem<'a>>>,
}

impl<'a> Switch<'a> {
    pub fn new(
        expr: Expr<'a>,
        cases: SwitchCase<'a>,
        default: Option<Vec<FunctionBodyItem<'a>>>,
    ) -> Self {
        Self {
            expr,
            cases,
            default,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct While<'a> {
    cond: Expr<'a>,
    body: Vec<FunctionBodyItem<'a>>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct For<'a> {
    var: Vec<Variable<'a>>,
    cond: Option<Expr<'a>>,
    inc: Option<Expr<'a>>,
    body: Vec<FunctionBodyItem<'a>>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Variable<'a> {
    visibility: Visibility,
    location: Location<'a>,
    name: Rc<String>,
    data_type: DataType<'a>,
    expr: Expr<'a>,
}

impl<'a> Variable<'a> {
    pub fn new(
        visibility: Visibility,
        location: Location<'a>,
        name: Rc<String>,
        data_type: DataType<'a>,
        expr: Expr<'a>,
    ) -> Self {
        Self {
            visibility,
            location,
            name,
            data_type,
            expr,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Constant<'a> {
    visibility: Visibility,
    name: Rc<String>,
    data_type: DataType<'a>,
    expr: Expr<'a>,
}

impl<'a> Constant<'a> {
    pub fn new(
        visibility: Visibility,
        name: Rc<String>,
        data_type: DataType<'a>,
        expr: Expr<'a>,
    ) -> Self {
        Self {
            visibility,
            name,
            data_type,
            expr,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct DoWhile<'a> {
    body: Vec<FunctionBodyItem<'a>>,
    cond: Expr<'a>,
}

impl<'a> DoWhile<'a> {
    pub fn new(body: Vec<FunctionBodyItem<'a>>, cond: Expr<'a>) -> Self {
        Self { body, cond }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt<'a> {
    If(If<'a>),
    Switch(Switch<'a>),
    While(While<'a>),
    For(For<'a>),
    Variable(Variable<'a>),
    Constant(Constant<'a>),
    Return(Option<Expr<'a>>),
    Break,
    Continue,
    DoWhile(DoWhile<'a>),
    Goto(Rc<String>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Visibility {
    Extern,
    Static,
    Normal,
}

#[derive(Debug, Clone, PartialEq)]
pub enum FunctionBodyItemKind<'a> {
    Expr(Expr<'a>),
    Stmt(Stmt<'a>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct FunctionBodyItem<'a> {
    kind: FunctionBodyItemKind<'a>,
    location: Location<'a>,
}

impl<'a> FunctionBodyItem<'a> {
    pub fn new(kind: FunctionBodyItemKind<'a>, location: Location<'a>) -> Self {
        Self { kind, location }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Typedef<'a> {
    Struct(Rc<String>, Struct<'a>),
    Enum(Rc<String>, Enum<'a>),
    Alias,
}

#[derive(Debug, Clone, PartialEq)]
pub struct StructField<'a> {
    name: Rc<String>,
    data_type: DataType<'a>,
}

impl<'a> StructField<'a> {
    pub fn new(name: Rc<String>, data_type: DataType<'a>) -> Self {
        Self { name, data_type }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Struct<'a> {
    name: Rc<String>,
    fields: Vec<StructField<'a>>,
    unions: Vec<Union<'a>>,
    structs: Vec<Struct<'a>>,
    enums: Vec<Enum<'a>>,
}

impl<'a> Struct<'a> {
    pub fn new(
        name: Rc<String>,
        fields: Vec<StructField<'a>>,
        unions: Vec<Union<'a>>,
        structs: Vec<Struct<'a>>,
        enums: Vec<Enum<'a>>,
    ) -> Self {
        Self {
            name,
            fields,
            unions,
            structs,
            enums,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct EnumVariant<'a> {
    name: Rc<String>,
    value: Option<Expr<'a>>,
}

impl<'a> EnumVariant<'a> {
    pub fn new(name: Rc<String>, value: Option<Expr<'a>>) -> Self {
        Self { name, value }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Enum<'a> {
    name: Rc<String>,
    variants: Vec<EnumVariant<'a>>,
}

impl<'a> Enum<'a> {
    pub fn new(name: Rc<String>, variants: Vec<EnumVariant<'a>>) -> Self {
        Self { name, variants }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Union<'a> {
    name: Rc<String>,
    fields: Vec<StructField<'a>>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct FunctionParam<'a> {
    name: Rc<String>,
    data_type: DataType<'a>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Function<'a> {
    visibility: Visibility,
    name: Rc<String>,
    params: Vec<FunctionParam<'a>>,
    return_data_type: DataType<'a>,
    body: Vec<FunctionBodyItem<'a>>,
}

#[derive(Debug, Clone, PartialEq)]
// TODO: Complete preprocessor type
pub enum Preprocessor {
    If,
    Undef,
    Define,
    Include,
    Error,
    Warning,
    Ifdef,
    Ifndef,
    Pragma,
}

#[derive(Debug, Clone, PartialEq)]
pub enum DeclKind<'a> {
    Constant(Constant<'a>),
    Variable(Variable<'a>),
    Typedef(Typedef<'a>),
    Struct(Struct<'a>),
    Enum(Enum<'a>),
    Function(Function<'a>),
    Preprocessor(Preprocessor),
    Union(Union<'a>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct Decl<'a> {
    kind: DeclKind<'a>,
    location: Location<'a>,
}

impl<'a> Decl<'a> {
    pub fn new(kind: DeclKind<'a>, location: Location<'a>) -> Self {
        Self { kind, location }
    }
}
