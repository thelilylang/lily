# Git

## Commit

Here's a template of what a commit message looks like:

```
[action]<topic>: <short description>

[additional content]
```

### Actions

- build: Changes that affects the building of the code.
- chore: Changes that do not affect the program or the user (.gitignore, .gitattributes, ...).
- ci: Changes that affect scripts or configuration files.
- feat: Add a new feature.
- fix: Fix a bug.
- docs: Changes affecting documentation.
- perf: Improve performance.
- refactor: A code that doesn't fix bugs or add new features.
- style: Changes do not affect the meaning of the code.
- test: Add a new test or modify an existing one.

```
build
chore
feat
fix
docs
refactor
perf
test
```

### Topics

```
global

base

bin/ci
bin/lily
bin/lilyc

cli/lily
cli/lilyc

command/lily
command/lilyc

cc
cc/ci

cpp

lib/builtin
lib/local
lib/std
lib/sys

lily
lily/analysis
lily/compiler
lily/diagnostic
lily/functions
lily/interpreter
lily/mir
lily/package
lily/parser
lily/precompiler
lily/preparser
lily/scanner
lily/shared

lsp

scripts
shared
...
```
