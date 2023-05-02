#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct Location<'a> {
    pub start_line: usize,
    pub end_line: usize,
    pub start_column: usize,
    pub end_column: usize,
    pub filename: &'a str,
}

impl<'a> Location<'a> {
    pub fn new(
        start_line: usize,
        end_line: usize,
        start_column: usize,
        end_column: usize,
        filename: &'a str,
    ) -> Self {
        Self {
            start_line,
            end_line,
            start_column,
            end_column,
            filename,
        }
    }

    #[inline]
    pub fn start(&mut self, line: usize, column: usize) {
        self.start_line = line;
        self.start_column = column;
    }

    #[inline]
    pub fn end(&mut self, line: usize, column: usize) {
        self.end_line = line;
        self.end_column = column;
    }
}
