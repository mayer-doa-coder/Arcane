 # Day 1 Implementation Guide (Very Beginner Friendly)

This guide is for your first real hands-on day.

Goal of Day 1:
1. Build the project correctly.
2. Run lexer and parser at least once.
3. Understand what each output line means in simple words.
4. Confirm generated C code is created.

Estimated time: 3 to 4 hours.

---

## Before you start

Open PowerShell in project root folder:

D:/Arcane

You can check current folder with:

```powershell
Get-Location
```

Expected result:
1. It should show D:\Arcane.

If not:

```powershell
Set-Location D:\Arcane
```

---

## Step 1: Clean old generated files

Run:

```powershell
make clean
```

Why this step matters:
1. It removes old generated files from previous runs.
2. It prevents confusion from stale output.

How to read common output lines:
1. Cleaning generated files... : Make is starting cleanup.
2. rm -f ... : remove file command is running.
3. Clean complete! : cleanup is done.

If cleanup fails:
1. Close editors/terminals using generated exe files.
2. Run make clean again.

---

## Step 2: Build lexer only

Run:

```powershell
make lexer
```

What this builds:
1. lexer/wizard_lexer.exe

How to read common output lines:
1. Generating parser header from wizard.y... : bison makes token header first.
2. cd parser && bison -d wizard.y : parser header generation command.
3. Generating lexer from wizard.l... : flex starts lexer code generation.
4. cd lexer && flex wizard.l : flex reads lexer rules file.
5. Compiling lexer... : generated C lexer is being compiled.
6. Lexer built successfully! : lexer executable created.

Warnings may appear:
1. Warning lines are usually safe for now.
2. Build is considered successful if final line says lexer built successfully.

Quick verify:
1. Check file exists: lexer/wizard_lexer.exe

---

## Step 3: Build parser and full pipeline

Run:

```powershell
make parser
```

What this builds:
1. parser/wizard_parser.exe
2. It links parser + lexer + symbol table + ICG + optimizer + codegen.

How to read common output lines:
1. Generating parser from wizard.y... : bison generates parser source.
2. Generating lexer from wizard.l... : flex regenerates lexer source used by parser build.
3. Compiling parser... : g++ compiles and links all modules.
4. Parser built successfully! : full compiler executable is ready.

Quick verify:
1. Check file exists: parser/wizard_parser.exe

---

## Step 4: Run lexer on a small valid file

Use a small file first so output is short and easy:

input/grade_valid_simple_decl_print.wiz

Run:

```powershell
./lexer/wizard_lexer.exe ./input/grade_valid_simple_decl_print.wiz
```

Token line format:

<TOKEN_ID, "text", line:X, col:Y>

Meaning of each part:
1. TOKEN_ID : internal token number.
2. text : exact text read from source.
3. line:X : source line number.
4. col:Y : start column in that line.

Your task:
1. Copy first 8 token lines.
2. Write plain meaning for each line.
3. Example: DECLARE means variable declaration starts.

---

## Step 5: Run parser with known valid input

Important project detail:
1. input/sample.wiz currently shows syntax error with current grammar.
2. For Day 1 confidence, use a grading-valid input.

Run from root:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_simple_decl_print.wiz
Pop-Location
```

How to read expected output lines:
1. # ICG BEGIN : intermediate code section starts.
2. x=42 : IR assignment line.
3. print x : IR print instruction.
4. # ICG END : intermediate code section ends.
5. Parse successful. : syntax + semantic path succeeded for this input.

Why Push-Location parser is used:
1. This project expects some relative paths during code generation.
2. Running from parser folder avoids output path confusion.

---

## Step 6: Check generated C file

Open:

output/output.c

What to look for:
1. Variable declaration for x.
2. Print statement generated from wizard code.
3. Program structure in C style.

Small mapping exercise:
1. Find one wizard source line in grade_valid_simple_decl_print.wiz.
2. Find matching C line in output/output.c.
3. Write one sentence: wizard line -> generated C line.

---

## Step 7: Run one failure case on purpose

Run parser on a known negative/syntax case:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_negative_syntax_missing_endcheck.wiz
Pop-Location
```

Why we do this:
1. You learn what failure output looks like.
2. You learn parser error location reading.

Expected type of result:
1. Parse error message.
2. Not parse successful.

---

## Step 8: End-of-day note template

Write answers in very simple words:

1. What is lexer? (one sentence)
2. What is parser? (one sentence)
3. What is ICG? (one sentence)
4. One thing that build output tells you.
5. One difference between valid test and invalid test output.

---

## Step 9: Day 1 checklist

Mark done only when real:

- [ ] make clean ran successfully
- [ ] make lexer ran successfully
- [ ] make parser ran successfully
- [ ] lexer printed token output for a small input
- [ ] parser showed Parse successful on valid input
- [ ] parser showed error on invalid input
- [ ] output/output.c exists
- [ ] notes are written

---

## Quick troubleshooting (simple)

Problem: lex command not found
1. Flex is not in PATH.
2. Reopen terminal after adding Flex path.

Problem: parser cannot write output C file
1. Run parser from parser folder using Push-Location parser.

Problem: sample.wiz fails
1. Use grading-valid files first.
2. Return to sample.wiz later during grammar study.

---

## What comes next on Day 2

1. Learn wizard keywords deeply.
2. Learn token rules in lexer file line by line.
3. Connect sample input text to exact lexer rules.

---

## Appendix A: Full source to output mapping (your first valid file)

Source file:

input/grade_valid_simple_decl_print.wiz

Source lines:
1. ENTER_HOGWARTS
2. HOUSE Gryffindor
3. DECLARE x AS INT := 42;
4. CAST x;
5. ENDHOUSE
6. EXIT_HOGWARTS

Generated intermediate code (ICG):
1. x=42
2. print x

Generated C output:
1. int x;
2. x=42;
3. printf("%d\n", x);

Simple interpretation:
1. Wizard declaration became C int variable + assignment.
2. Wizard CAST became C printf.

---

## Appendix B: Token line dictionary

Token line format:

<token_id, "text", line:L, col:C>

Meaning:
1. token_id: internal number used by parser.
2. text: real word/symbol read from file.
3. line:L: line number in source.
4. col:C: starting column position.

Example from your run:

<273, "DECLARE", line:3, col:1>

Meaning in plain words:
1. Lexer saw DECLARE at line 3, first column.
2. It tagged it as declaration keyword token.

---

## Appendix C: Build output dictionary

Generating parser header from wizard.y...
1. Bison is creating token/type header for lexer/parser sharing.

Generating lexer from wizard.l...
1. Flex is converting lexer rules to C code.

Compiling lexer...
1. g++ is creating lexer executable.

Compiling parser...
1. g++ is compiling parser + all pipeline modules together.

Lexer built successfully!
1. Standalone lexer executable is ready.

Parser built successfully!
1. Full compiler executable is ready.

Parse successful.
1. File passed parse and semantic checks for this run.

Parse error: syntax error ...
1. Parser found grammar mismatch in input.

---

## Appendix D: Module cheat sheet (very short)

lexer/wizard.l
1. Reads characters.
2. Produces tokens.

parser/wizard.y
1. Reads tokens.
2. Validates program structure.
3. Triggers semantic checks + ICG + optimize + codegen.

symbol_table/symbol_table.h and symbol_table/symbol_table.c
1. Stores declared names.
2. Tracks type, scope, initialized status.
3. Catches duplicate/undeclared/type mismatch errors.

icg/icg.h and icg/icg.c
1. Builds intermediate instructions like x=42 and print x.

optimizer/optimizer.h and optimizer/optimizer.c
1. Improves ICG by simplifying calculations and removing waste.

codegen/codegen.h and codegen/codegen.c
1. Converts ICG + symbol table into C source file.

output/output.c
1. Final generated C program from your wizard source.

---

## Appendix E: Common beginner confusion and fixes

Why does sample.wiz fail but valid grade file works?
1. Current grammar and sample file are not fully aligned yet.
2. Use grading-valid files while learning foundations.

Why run parser from parser folder?
1. Some output paths are relative to parser location.
2. Running there avoids output write warnings.

Are warnings always errors?
1. No.
2. If executable is built and run succeeds, warning can be reviewed later.

How do I know which stage failed?
1. Token problem text usually means lexer stage.
2. Syntax error text means parser stage.
3. Semantic Error text means symbol/type stage.
4. Wrong output C means codegen stage.
