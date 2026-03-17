# Arcane Quick Start Memo (Wizard64)

This memo gets you from a clean checkout to running lexer and parser checks on Windows.

## 0) Prerequisites

- `make`
- `gcc/g++`
- `flex` and `bison` available at:
  - `C:/MinGW/msys/1.0/bin/flex.exe`
  - `C:/MinGW/msys/1.0/bin/bison.exe`

## 1) Open project root

Run from the repo root:

```bash
cd d:\Arcane
```

## 2) Build lexer

```bash
make lexer
```

Expected output artifact:
- `lexer/wizard_lexer.exe`

## 3) Run sample to console

```bash
make run
```

## 4) Generate token output file

```bash
make test
```

Output file:
- `output/output.txt`

## 5) Run your own `.wiz` file

PowerShell / CMD (Windows):

```bash
.\lexer\wizard_lexer.exe input\your_file.wiz output\output.txt
```

## 6) Build and run parser

```bash
make parser
make test-parser
```

Expected output artifact:
- `parser/wizard_parser.exe`

Manual parser run:

```bash
.\parser\wizard_parser.exe < input\sample.wiz
```

## 7) Clean generated artifacts

```bash
make clean
```

## Minimal Wizard64 Program

```wiz
ENTER_HOGWARTS HOUSE
    DECLARE x AS INT := 10;
    IF x > 0 HOUSE
        CAST("Ready");
    FI
EXIT_HOGWARTS ENDHOUSE
```
