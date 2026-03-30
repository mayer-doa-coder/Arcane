# Learn Later Help (Arcane Compiler)

This file is your quick memory guide.
Use it when you come back later and forget the steps.

---

## 1) What this project does

You write Wizard language code in `.wiz` files.
The compiler converts it into C code, then into an `.exe` program.

Flow:
1. Lexer reads text and makes tokens.
2. Parser checks grammar.
3. Semantic checks validate meaning.
4. ICG makes intermediate code.
5. Optimizer improves that code.
6. Codegen writes `output/output.c`.
7. C compiler builds executable.

---

## 2) Most used commands

Run from project root: `D:/Arcane`

Build lexer:

```powershell
make lexer
```

Build parser/full compiler:

```powershell
make parser
```

Run lexer on sample file:

```powershell
make run
```

Save lexer output to file:

```powershell
make test
```

Run parser test on sample file:

```powershell
make test-parser
```

Clean generated files:

```powershell
make clean
```

---

## 3) Full run for your own program

Example file:
1. `input/my_program.wiz`

Example source:

```wiz
ENTER_HOGWARTS
HOUSE Gryffindor
DECLARE x AS INT := 5 + 4;
CAST x;
ENDHOUSE
EXIT_HOGWARTS
```

Run full pipeline:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_simple_decl_print.wiz
g++ -Wall ../output/output.c -o ../output/wizard_from_c.exe
../output/wizard_from_c.exe
Pop-Location
```

Expected runtime output:

```text
9
```

---

## 4) Where files go

Input files:
1. `input/*.wiz`

Generated C file:
1. `output/output.c`

Generated exe from C:
1. `output/wizard_from_c.exe`

Saved lexer tokens:
1. `output/output.txt`
2. `output/day2_tokens.txt`

---

## 5) Quick error meaning

Lexical error:
1. Token is invalid (lexer stage).
2. Message has `LEXICAL ERROR` with line and column.

Parse error:
1. Grammar mismatch (parser stage).
2. Message usually starts with `Parse error`.

Semantic error:
1. Meaning/type/scope issue.
2. Message contains `Semantic Error`.

---

## 6) Known project behavior

1. `input/sample.wiz` may fail parser in current grammar state.
2. Use grading-valid files first, like:
   - `input/grade_valid_simple_decl_print.wiz`
   - `input/grade_valid_medium_condition_assign.wiz`

---

## 7) Grading suite (batch run)

Run all grading tests:

```powershell
./run_grading_suite.ps1
```

Skip build and run tests only:

```powershell
./run_grading_suite.ps1 -SkipBuild
```

Artifacts are written under:
1. `grading_artifacts/<timestamp>/`
2. Logs: `grading_artifacts/<timestamp>/logs/`
3. Generated C copies: `grading_artifacts/<timestamp>/generated_c/`
4. Summary CSV: `grading_artifacts/<timestamp>/summary.csv`

---

## 8) If you forget where to continue

Continue in this order:
1. Day 1 file: `DAY1_IMPLEMENTATION_BEGINNER.md`
2. Day 2 file: `DAY2_IMPLEMENTATION_BEGINNER.md`
3. Then start parser learning in `parser/wizard.y`

---

## 9) 2-minute restart checklist

1. Open terminal in `D:/Arcane`.
2. Run `make lexer` and `make parser`.
3. Run lexer once (`make run`).
4. Run parser on known valid file:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_simple_decl_print.wiz
Pop-Location
```

5. If parse is successful, you are ready.
