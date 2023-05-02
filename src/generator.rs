use crate::scanner::{Scanner, Source};

use std::process::exit;
use std::{fs, io, path::Path};

pub struct Generator<'a> {
    source: &'a Source<'a>,
    scanner: &'a Scanner<'a>,
}

impl<'a> Generator<'a> {
    pub fn new(source: &'a Source<'a>, scanner: &'a Scanner<'a>) -> Self {
        Self { source, scanner }
    }

    fn create_output_dir() -> io::Result<()> {
        let dir = "output";
        let path = Path::new(dir);

        if path.exists() {
            eprintln!("error: `output` dir already exists");
            exit(1);
        }

        fs::create_dir(dir)
    }

    pub fn run(&mut self) {
        Self::create_output_dir().unwrap();
    }
}
