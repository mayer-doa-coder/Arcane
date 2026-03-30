# Day 7 Implementation Guide (Very Beginner Friendly)

Day 7 focus is Code Generation.

Today you will learn how compiler ICG lines become real C code.
You already understand parser, semantic checks, ICG, and optimizer.
Now you will connect everything to final C output and executable output.

Simple flow today:
1. Wizard source
2. Parser builds ICG
3. Optimizer improves ICG
4. Codegen converts ICG to C file
5. C compiler builds exe
6. Exe prints final output

Estimated time: 4 to 6 hours.

---

## Day 7 success goals

By end of Day 7, you should be able to:
1. Explain how one ICG line becomes one C line.
2. Explain how labels and jumps are converted.
3. Explain how print and return lines are converted.
4. Explain how function call lines are converted.
5. Read output C file and match it with ICG.

---

## 1) Files to study today

Read in this order:
1. codegen/codegen.h
2. codegen/codegen.c
3. icg/icg.c (quick revisit)
4. parser/wizard.y (program end action where codegen is called)

What each file does today:
1. codegen.h: codegen API.
2. codegen.c: full translation logic from ICG to C.
3. icg.c: helps you understand ICG line format.
4. wizard.y: shows pipeline order and output path.

---

## 2) Big idea of codegen (simple words)

Codegen reads ICG lines one by one.
For each known pattern, it writes a C statement.

Think like this:
1. ICG is a recipe.
2. codegen.c is a translator.
3. output/output.c is the final translated recipe.

---

## 3) Important patterns to learn first

### A) Assignments

ICG:
1. x=42
2. t1=a+b

C idea:
1. x = 42;
2. t1 = a + b;

### B) Print lines

ICG:
1. print x
2. print "Hello"

C idea:
1. printf("%d\\n", x);
2. printf("%s\\n", "Hello");

### C) Branching with labels

ICG:
1. ifFalse t1 goto L1
2. goto L2
3. L1:

C idea:
1. if (!(t1)) goto L1;
2. goto L2;
3. L1: ;

### D) Function calls

ICG:
1. arg i
2. t3 = call inc 1

C idea:
1. push i into pending call args list
2. t3 = inc(i);

### E) Return lines

ICG:
1. return t1

C idea:
1. return t1;

---

## 4) Key codegen parts to inspect

Inside codegen/codegen.c, focus on these logical parts:
1. Line classification and parsing helpers.
2. ICG line to C line converter.
3. Pending args buffer for call conversion.
4. Function definition emission.
5. Main function emission.

Easy reading path:
1. Find where output file is opened.
2. Find where headers are written.
3. Find where functions are emitted.
4. Find where main block is emitted.
5. Find the function that converts one IR line to one C line.

---

## 5) Real command workflow for Day 7

Run from project root.

### Step 1: Build parser (if needed)

```powershell
make parser
```

### Step 2: Run parser on valid medium file

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_medium_condition_assign.wiz
Pop-Location
```

What to observe:
1. RAW ICG section markers.
2. OPT ICG section markers.
3. Parse successful message.

### Step 3: Open generated C file

Open output/output.c and inspect line-by-line.

### Step 4: Compile generated C and run

```powershell
g++ -Wall ./output/output.c -o ./output/day7_run.exe
./output/day7_run.exe
```

Expected idea:
1. Runtime output should match source program logic.

---

## 6) ICG to C mapping exercise (must do)

Use file: input/grade_valid_medium_condition_assign.wiz

Do this table manually while reading output/output.c:

| ICG line | Generated C line idea | Why |
|---|---|---|
| score=7 | score = 7; | direct assignment |
| t1=score>5 | t1 = score > 5; | condition compute |
| ifFalse t1 goto L1 | if (!(t1)) goto L1; | false jump |
| t2=score+3 | t2 = score + 3; | temp for expression |
| score=t2 | score = t2; | write back result |
| print score | printf("%d\\n", score); | output integer |

Goal:
1. Map at least 8 lines correctly.

---

## 7) Function call translation exercise

Use file: input/grade_valid_complex_loop_function_output.wiz

Run:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_complex_loop_function_output.wiz
Pop-Location
```

Then inspect output/output.c.

Find and explain:
1. Where function inc is emitted in C.
2. How arg i and call inc 1 becomes inc(i).
3. How loop labels become goto-based C flow.

---

## 8) Common confusion and easy fix

Confusion 1:
1. Why C has temp vars I did not write?
Fix:
1. Temps come from ICG to keep expressions step-by-step.

Confusion 2:
1. Why goto and labels appear in generated C?
Fix:
1. They are direct low-level control flow representation from ICG.

Confusion 3:
1. Why function call args are not visible near arg lines?
Fix:
1. Codegen collects arg lines and emits them at call line.

Confusion 4:
1. Why output/output.c not updated sometimes?
Fix:
1. Run parser from parser folder so relative path is correct.

---

## 9) Day 7 worksheet

Write short answers:
1. What is code generation in one sentence?
2. How does ifFalse become C code?
3. Why are labels needed in generated C?
4. How does print line choose %d or %s?
5. How is function call translated from arg + call lines?

Mini challenge:
1. Pick any 10 ICG lines.
2. Write matching C lines in your own words.
3. Verify from output/output.c.

---

## 10) End-of-day checklist

- [ ] I read codegen.h and codegen.c.
- [ ] I understood assignment, print, label, goto conversion.
- [ ] I understood call arg buffering and call emission.
- [ ] I ran at least 2 valid input files.
- [ ] I mapped ICG to C for at least 10 lines.
- [ ] I compiled output/output.c and ran executable.
- [ ] I explained final runtime output from source logic.

---

## 11) What Day 8 will be

Day 8 focus:
1. Full testing and grading strategy.
2. Negative tests and error category verification.
3. End-to-end report style for academic demo.

You are ready for Day 8 when:
1. You can explain generated C without confusion.
2. You can trace source -> ICG -> C -> runtime output clearly.
