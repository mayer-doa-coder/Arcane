# Day 2 Implementation Guide (Very Beginner Friendly)

You completed Day 1. Great.

Day 2 focus is only this:
1. Understand Wizard language keywords clearly.
2. Understand how lexer rules in lexer/wizard.l turn text into tokens.
3. Practice reading lexer output until you can explain each token line.

Estimated time: 3 to 4 hours.

---

## Day 2 success goals

By the end of Day 2, you should be able to do all of this:
1. Tell what each keyword group means.
2. Explain the difference between IDENTIFIER and NUMBER.
3. Explain why a lexical error happens.
4. Run lexer on valid and invalid files and explain output.
5. Point to the exact rule area in lexer/wizard.l for a token.

---

## Before starting Day 2

Open PowerShell at project root:

D:/Arcane

Run:

```powershell
Get-Location
```

If not in root, run:

```powershell
Set-Location D:\Arcane
```

Build lexer once:

```powershell
make lexer
```

---

## Study Block 1 (45-60 min): Keyword groups in plain words

Read these files first:
1. wizard_keyword.md
2. lexer/tokens.h

How to read them:
1. wizard_keyword.md tells language meaning.
2. lexer/tokens.h tells exact spellings used in code constants.

### Quick keyword map (simple)

Program shape:
1. ENTER_HOGWARTS: program start
2. EXIT_HOGWARTS: program end
3. HOUSE / ENDHOUSE: block begin/end

House names (semantic role):
1. Gryffindor: declaration area
2. Slytherin: function area
3. Hufflepuff: condition area
4. Ravenclaw: loop area

Data types:
1. INT
2. FLOAT
3. DOUBLE
4. LONG
5. CHAR
6. BOOL
7. VOID

Declaration and assign:
1. DECLARE
2. AS
3. :=
4. =

Control flow:
1. IF_CHARM / END_CHARM
2. OTHERWISE / ELSE_CHARM
3. IF_CHARM / THEN / END_CHARM

Loops:
1. TIME_TURNER / BEGIN_LOOP / END_TIME_TURNER
2. BREAK_SPELL
3. KEEP_CASTING

Functions:
1. SPELL / ENDSPELL
2. SUMMON
3. WITH
4. RETURN

Input output:
1. CAST
2. PROPHECY
3. INPUT

Math names:
1. POWO
2. RADIX
3. FLOORUS
4. CEILUS
5. ABSOLUTUS
6. LOGUS
7. SINUS
8. COSINUS
9. TANUS

Boolean words:
1. true
2. false

Your mini task:
1. Pick 15 keywords.
2. Write one plain sentence for each keyword meaning.

---

## Study Block 2 (60 min): Learn lexer file structure

Read:
1. lexer/wizard.l

Do not panic. Read by sections only.

### Section A: Top include and helper functions

What you will see:
1. include lines
2. helper functions like dup_text and dup_unquoted
3. column tracking variables

Why this matters:
1. The lexer stores token values in yylval.
2. It tracks line and column for useful errors.
3. Strings and chars are copied safely.

### Section B: Definitions area (before %% rules)

Important patterns:
1. DIGIT           [0-9]
2. IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_]*
3. INTEGER         {DIGIT}+
4. FLOAT_LITERAL   digits dot digits (optional exponent)
5. HEX             0x...
6. OCTAL           0...

Simple meaning:
1. These are pattern shortcuts.
2. Later rules reuse them.

### Section C: Token rules area (between %% and %%)

Core idea:
1. Left side is text pattern.
2. Right side is action (usually return token).

Examples:
1. "DECLARE" { return DECLARE; }
2. {IDENTIFIER} { ... return IDENTIFIER; }
3. {INTEGER} { ... return NUMBER; }

### Section D: Error and comments rules

Important lines:
1. "##".*                      { }  (single-line comments ignored)
2. #* ... *#                   (multi-line comment mode)
3. .                           { report_lexical_error(yytext); }

Meaning:
1. Unknown character goes to lexical error.
2. Comments are ignored, not tokenized.

### Section E: Standalone lexer main

Bottom section has main function only when not using parser mode.

Meaning:
1. lexer/wizard_lexer.exe can run alone.
2. It prints token lines.

Your mini task:
1. Find rule for DECLARE.
2. Find rule for identifiers.
3. Find rule for unknown tokens.
4. Find rule for comments.
5. Note line locations in your notes.

---

## Study Block 3 (45-60 min): Guided command walkthrough

Use these exact commands.

### Command 1: Small valid input

```powershell
./lexer/wizard_lexer.exe ./input/grade_valid_simple_decl_print.wiz
```

What to observe:
1. Enter/start keywords appear first.
2. Declaration tokens appear in logical order.
3. CAST and variable token appear.
4. End keywords appear at end.

### Command 2: Bigger valid input

```powershell
./lexer/wizard_lexer.exe ./input/grade_valid_medium_condition_assign.wiz
```

What to observe:
1. More control-flow tokens (IF_CHARM/THEN/END_CHARM).
2. More operators and punctuation tokens.
3. Identifier reuse in many places.

### Command 3: Invalid token input

```powershell
./lexer/wizard_lexer.exe ./input/grade_negative_invalid_token.wiz
```

What to observe:
1. Look for lexical error message.
2. Note line and column from error.
3. Open source file and confirm exact position.

### Command 4: Save output to a file

```powershell
./lexer/wizard_lexer.exe ./input/grade_valid_simple_decl_print.wiz ./output/day2_tokens.txt
```

Then open:
1. output/day2_tokens.txt

Why this helps:
1. You can annotate token lines one by one.

---

## Study Block 4 (30-45 min): How token matching really works

Simple rule: first matching rule in lexer order wins.

What this means for this project:
1. Keyword rules are above identifier rule.
2. So DECLARE becomes DECLARE token, not IDENTIFIER.
3. Any name not in keyword list falls to IDENTIFIER rule.

Important examples:
1. DECLARE -> keyword token
2. score -> IDENTIFIER token
3. 42 -> NUMBER token
4. true -> BOOL_LITERAL token

Potential confusion:
1. FLOAT_LITERAL currently stores using integer conversion in action code.
2. Do not fix now in Day 2; just notice behavior.

---

## Study Block 5 (30 min): Error-reading practice

Lexical error style in this project:

LEXICAL ERROR: line X, col Y: Unknown token '...'

How to debug quickly:
1. Go to that line in input file.
2. Move to that column.
3. Check if character/word has a lexer rule.
4. If no rule exists, that is expected error.

Practice files to try:
1. input/grade_negative_invalid_token.wiz
2. input/grade_edge_multiple_errors.wiz

---

## Day 2 beginner worksheet (fill this)

Write short answers:

1. What is the job of lexer in one sentence?
2. What is IDENTIFIER pattern in one sentence?
3. Why does DECLARE not become IDENTIFIER?
4. What does line and col in token output help with?
5. Give one example of lexical error and why it happened.

Table exercise:

| Source Text | Token Kind | Why |
|-------------|------------|-----|
| ENTER_HOGWARTS | keyword | exact keyword rule exists |
| x | IDENTIFIER | matches identifier pattern |
| 42 | NUMBER | matches integer pattern |
| @ | lexical error | no rule for this character |

---

## End-of-day checklist

Mark done only if true:

- [ ] I read wizard_keyword.md fully.
- [ ] I read lexer/tokens.h fully.
- [ ] I read lexer/wizard.l in sections.
- [ ] I ran lexer on at least 2 valid files.
- [ ] I ran lexer on at least 1 invalid file.
- [ ] I explained one lexical error with line and column.
- [ ] I saved token output to output/day2_tokens.txt.
- [ ] I wrote worksheet answers.

---

## Common beginner problems and direct fix

Problem 1: lexer executable not found
1. Run make lexer again.
2. Confirm lexer/wizard_lexer.exe exists.

Problem 2: too much token output feels confusing
1. Use smallest file first.
2. Annotate first 10 lines only.
3. Then continue next 10 lines.

Problem 3: cannot map token id numbers
1. Focus on text part first.
2. Token number is internal and less important at start.

Problem 4: invalid token file still prints some tokens before error
1. That is normal.
2. Lexer reads left-to-right and errors when bad token appears.

---

## Optional advanced observation (safe for beginner)

You may notice in lexer/wizard.l:
1. Rule for FLOAT_LITERAL uses strtol in current code.

Simple interpretation:
1. Float text is recognized.
2. Stored numeric conversion behavior may be simplified right now.
3. This can be improved later during quality/refactor phase.

---

## What Day 3 will do

Day 3 focus:
1. parser/wizard.y grammar blocks.
2. How token sequence becomes valid or invalid syntax.
3. Why errors like missing END_CHARM happen.

You are ready for Day 3 when:
1. You can explain token output without help.
2. You can point to lexer rules for common keywords.
3. You can explain one lexical failure clearly.

---

## Appendix A: Real walkthrough outputs from your environment

These are real outputs from your recent runs.

### A1) Small valid input output

Command:

```powershell
./lexer/wizard_lexer.exe ./input/grade_valid_simple_decl_print.wiz
```

Output:

```text
<258, "ENTER_HOGWARTS", line:1, col:1>
<260, "HOUSE", line:2, col:1>
<262, "Gryffindor", line:2, col:7>
<273, "DECLARE", line:3, col:1>
<314, "x", line:3, col:9>
<274, "AS", line:3, col:11>
<266, "INT", line:3, col:14>
<275, ":=", line:3, col:18>
<318, "42", line:3, col:21>
<59, ";", line:3, col:23>
<292, "CAST", line:4, col:1>
<314, "x", line:4, col:6>
<59, ";", line:4, col:7>
<261, "ENDHOUSE", line:5, col:1>
<259, "EXIT_HOGWARTS", line:6, col:1>
```

Simple interpretation:
1. Program starts.
2. A house block starts.
3. One integer variable x is declared and initialized to 42.
4. x is printed.
5. Block and program close.

### A2) Medium valid input output

Command:

```powershell
./lexer/wizard_lexer.exe ./input/grade_valid_medium_condition_assign.wiz
```

Important lines to notice:
1. IF_CHARM token appears for condition start.
2. > token appears as comparison operator.
3. THEN and END_CHARM mark condition block boundaries.
4. score = score + 3 is split into identifier, assign, identifier, plus, number.

### A3) Invalid token output

Command:

```powershell
./lexer/wizard_lexer.exe ./input/grade_negative_invalid_token.wiz
```

Important error line:

```text
LEXICAL ERROR: line 4, col 1: Unknown token '@'
```

Simple interpretation:
1. Lexer found @.
2. No lexer rule handles @ in this grammar.
3. So lexical error is correct behavior.

Extra observation:
1. Some tokens after the error still printed.
2. This lexer reports error but can continue scanning remaining input.

### A4) Saved token file output

Command:

```powershell
./lexer/wizard_lexer.exe ./input/grade_valid_simple_decl_print.wiz ./output/day2_tokens.txt
```

Result:
1. File output/day2_tokens.txt is created.
2. It contains token lines for annotation practice.

---

## Appendix B: 20-minute self-test for Day 2

Time box: 20 minutes.

Part 1 (5 min):
1. Run lexer on grade_valid_simple_decl_print.wiz.
2. Explain first 5 token lines in plain words.

Part 2 (5 min):
1. Run lexer on grade_valid_medium_condition_assign.wiz.
2. Find IF_CHARM and END_CHARM lines and explain them.

Part 3 (5 min):
1. Run lexer on grade_negative_invalid_token.wiz.
2. Explain line and column in the lexical error.

Part 4 (5 min):
1. Open lexer/wizard.l.
2. Point to the fallback error rule using dot pattern.

If you can do all parts without help, Day 2 is complete.
