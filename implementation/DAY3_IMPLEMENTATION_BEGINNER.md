# Day 3 Implementation Guide (Very Beginner Friendly)

You finished Day 1 and Day 2.
Now Day 3 is about parser understanding.

Day 3 focus:
1. Understand how parser grammar is organized.
2. Understand how a valid token sequence becomes a valid program.
3. Understand why syntax errors happen.
4. Trace parser behavior using real test files.

Estimated time: 3 to 5 hours.

---

## Day 3 success goals

By the end of Day 3, you should be able to:
1. Explain what `program`, `statement`, `if_stmt`, and `expression` do.
2. Explain difference between `IF_CHARM ... THEN ... END_CHARM` and `IF_CHARM ... THEN ... ELSE_CHARM ... THEN ... OTHERWISE ... END_CHARM`.
3. Run parser on one valid file and one invalid file and explain result.
4. Point to grammar rule areas in parser/wizard.y for common statements.
5. Explain parse error message in simple words.

---

## 1) Before starting Day 3

Open PowerShell at root folder:

D:/Arcane

Run:

```powershell
Get-Location
```

If needed:

```powershell
Set-Location D:\Arcane
```

Build parser once:

```powershell
make parser
```

---

## 2) Big picture of parser/wizard.y

File to study:
1. parser/wizard.y

Simple structure:
1. Top C code/helper functions.
2. Token declarations and precedence.
3. Grammar rules (this is main Day 3 area).
4. Parser error function `yyerror`.
5. `main` function for parser executable.

Important: Grammar starts after the first `%%` marker.

---

## 3) Grammar map (read in this order)

These are key rule sections in parser/wizard.y:

1. `program` (starts around line 903)
2. `house_blocks`, `house_block` (around 927 to 932)
3. `function` and function helper rules (around 966 onward)
4. `statements` and `statement` (around 1029 to 1034)
5. `declaration` (around 1048)
6. `assignment` (around 1117)
7. `print_stmt` (around 1166)
8. `call` (around 1222)
9. `if_stmt` and `if_else_part` (around 1250 and 1273)
10. `loop_stmt` (around 1324)
11. `break_stmt`, `continue_stmt`, `return_stmt`
12. `expr` and `expression` (around 1467 and 1492)

How to use this map:
1. Read one rule block at a time.
2. Find one matching `.wiz` example for each block.
3. Run parser and compare behavior.

---

## 4) Most important syntax pairs (memorize these)

Program:
1. ENTER_HOGWARTS ... EXIT_HOGWARTS

Block wrappers:
1. HOUSE ... ENDHOUSE

Condition styles:
1. IF_CHARM expr THEN ... END_CHARM
2. IF_CHARM expr THEN ... OTHERWISE ... END_CHARM
3. IF_CHARM expr THEN ... ELSE_CHARM expr THEN ... OTHERWISE ... END_CHARM

Loops:
1. TIME_TURNER expr BEGIN_LOOP ... END_TIME_TURNER

Functions:
1. SPELL name ... ENDSPELL

Why this matters:
1. Most parse errors happen when one close keyword is missing or wrong.
2. Example: using `OTHERWISE` or `ELSE_CHARM` outside an IF_CHARM chain.

---

## 5) Guided parser run commands (do these exactly)

### Command A: Valid simple parse

```powershell
./parser/wizard_parser.exe ./input/grade_valid_simple_decl_print.wiz
```

Expected type of output:
1. ICG block lines.
2. Parse successful.

### Command B: Valid medium parse

```powershell
./parser/wizard_parser.exe ./input/grade_valid_medium_condition_assign.wiz
```

Expected type of output:
1. More ICG lines (assign + condition path).
2. Parse successful.

### Command C: Missing END_CHARM syntax error

```powershell
./parser/wizard_parser.exe ./input/grade_negative_syntax_missing_endcheck.wiz
```

Expected output style:
1. Parse error: syntax error (possible misplaced OTHERWISE/ELSE_CHARM or missing END_CHARM)

### Command D: OTHERWISE without IF_CHARM error

```powershell
./parser/wizard_parser.exe ./input/if_else_invalid_else_without_check.wiz
```

Expected output style:
1. Parse error
2. It fails because OTHERWISE is out of valid grammar context.

---

## 6) How to read parser output in simple way

When you see:

Parse successful.

It means:
1. Token sequence matched grammar.
2. Parser accepted structure.

When you see:

Parse error: syntax error (possible misplaced OTHERWISE/ELSE_CHARM or missing END_CHARM)

It means:
1. Parser expected some keyword/structure.
2. Actual input did not match next grammar rule.
3. Common reason: wrong close keyword or wrong OTHERWISE/ELSE_CHARM position.

---

## 7) Beginner tracing method (very practical)

Take one source file and trace with this checklist.

Example file:
1. input/grade_valid_medium_condition_assign.wiz

Trace steps:
1. Mark program start/end keywords.
2. Mark each statement line (declare, assign, cast, check, loop, etc).
3. For each statement, find matching rule name in parser/wizard.y.
4. Confirm close keywords are correct.
5. Run parser and verify parse success.

If parse fails:
1. Find nearest control keyword (`IF_CHARM`, `ELSE_CHARM`, `OTHERWISE`, `END_CHARM`, `TIME_TURNER`, `END_TIME_TURNER`).
2. Re-check open/close pairing.

---

## 8) Rule understanding with examples

### A) Declaration rule idea

Pattern:
1. DECLARE name AS type init ';'

Example:
1. DECLARE x AS INT := 42;

### B) Assignment rule idea

Pattern:
1. name = expression ;
2. name := expression ;

### C) IF_CHARM style rule idea

Pattern:
1. IF_CHARM expr THEN statements END_CHARM

### D) ELSE_CHARM / OTHERWISE style rule idea

Pattern:
1. IF_CHARM expr THEN statements OTHERWISE statements END_CHARM
2. IF_CHARM expr THEN statements ELSE_CHARM expr THEN statements OTHERWISE statements END_CHARM

### E) Loop rule idea

Pattern:
1. TIME_TURNER expr BEGIN_LOOP statements END_TIME_TURNER

---

## 9) Common parse mistakes and fix

Mistake 1: Missing END_CHARM
1. Fix by closing IF_CHARM block with END_CHARM.

Mistake 2: OTHERWISE without IF_CHARM
1. OTHERWISE must be inside IF_CHARM ... THEN ... END_CHARM form.

Mistake 3: Mixing old and new close keywords
1. IF_CHARM block uses END_CHARM.
2. Old FI/ENDCHECK are no longer accepted.

Mistake 4: Missing BEGIN_LOOP after TIME_TURNER
1. TIME_TURNER requires `BEGIN_LOOP` before statements.

Mistake 5: Wrong block ending order
1. Close inner block first, then outer block.

---

## 10) Parser + generated C workflow (optional but useful)

To also get C file output reliably:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_simple_decl_print.wiz
g++ -Wall ../output/output.c -o ../output/day3_run.exe
../output/day3_run.exe
Pop-Location
```

This gives:
1. parser result
2. generated C compile
3. runtime output

---

## 11) Day 3 worksheet

Write short answers:

1. What does `program` rule do?
2. Difference between simple IF_CHARM and ELSE_CHARM/OTHERWISE chain?
3. Why can OTHERWISE appear only inside an IF_CHARM chain?
4. Give one parse error case and reason.
5. Give one valid file and why it passes.

Mini mapping task:

| Source Pattern | Rule Name in parser/wizard.y |
|---|---|
| DECLARE x AS INT := 1; | declaration |
| x = x + 1; | assignment |
| CAST x; | print_stmt |
| IF_CHARM x > 0 THEN ... END_CHARM | if_stmt + if_else_part |
| TIME_TURNER x > 0 BEGIN_LOOP ... END_TIME_TURNER | loop_stmt |

---

## 12) End-of-day checklist

- [ ] I read parser/wizard.y grammar section slowly.
- [ ] I identified key rule names.
- [ ] I ran at least 2 valid parser tests.
- [ ] I ran at least 2 invalid parser tests.
- [ ] I explained one parse error clearly.
- [ ] I can explain IF_CHARM / ELSE_CHARM / OTHERWISE / END_CHARM flow.
- [ ] I completed worksheet answers.

---

## 13) Suggested files for Day 3 practice

Valid:
1. input/grade_valid_simple_decl_print.wiz
2. input/grade_valid_medium_condition_assign.wiz
3. input/if_without_else_valid_check.wiz

Invalid syntax-focused:
1. input/grade_negative_syntax_missing_endcheck.wiz
2. input/if_else_invalid_else_without_check.wiz

Good challenge file:
1. input/grade_valid_complex_loop_function_output.wiz

---

## 14) What Day 4 will be

Day 4 focus:
1. Symbol table and semantic checks.
2. Why undeclared/type mismatch errors happen.
3. Connect semantic errors to symbol table code.

You are ready for Day 4 when:
1. You can explain parse success/failure without guessing.
2. You can point to parser rules for major statements.

---

## Appendix A: Real Day 3 parser outputs

These outputs are from actual runs.

### A1) Valid simple parse output

Command:

```powershell
./parser/wizard_parser.exe ./input/grade_valid_simple_decl_print.wiz
```

Output:

```text
# ICG BEGIN
x=42
print x
# ICG END
Code generation warning: failed to write output C file
Parse successful.
```

Meaning:
1. Grammar parse passed.
2. ICG is generated.
3. Warning is path-related, not syntax failure.

### A2) Valid medium parse output

Command:

```powershell
./parser/wizard_parser.exe ./input/grade_valid_medium_condition_assign.wiz
```

Output (shortened):

```text
# ICG BEGIN
score=7
t1=score>5
ifFalse t1 goto L1
t2=score+3
score=t2
L1:
print score
# ICG END
Code generation warning: failed to write output C file
Parse successful.
```

Meaning:
1. IF_CHARM condition became branch instructions.
2. Parser accepted file successfully.

### A3) Missing END_CHARM error output

Command:

```powershell
./parser/wizard_parser.exe ./input/grade_negative_syntax_missing_endcheck.wiz
```

Output:

```text
Parse error: syntax error (possible misplaced OTHERWISE/ELSE_CHARM or missing END_CHARM)
```

### A4) OTHERWISE without IF_CHARM error output

Command:

```powershell
./parser/wizard_parser.exe ./input/if_else_invalid_else_without_check.wiz
```

Output:

```text
Parse error: syntax error (possible misplaced OTHERWISE/ELSE_CHARM or missing END_CHARM)
```

---

## Appendix B: Why code generation warning appears in some runs

If you run parser from project root, you may see:

Code generation warning: failed to write output C file

Reason:
1. Parser code writes C file to `../output/output.c`.
2. That path is designed for parser working directory.

Simple fix:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_simple_decl_print.wiz
Pop-Location
```

Then code generation usually writes successfully.
