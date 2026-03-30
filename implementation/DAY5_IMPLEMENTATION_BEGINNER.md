# Day 5 Implementation Guide (Very Beginner Friendly)

Day 5 focus is the ICG stage.

ICG means Intermediate Code Generation.
It is the middle form between parser and final C code.

Simple flow today:
1. Wizard source
2. Parser builds ICG lines
3. Optimizer improves some lines
4. Codegen converts ICG into C
5. C compiler makes runnable exe

Estimated time: 4 to 6 hours.

---

## Day 5 success goals

By end of Day 5, you should be able to:
1. Explain what temp (`t1`, `t2`) and label (`L1`, `L2`) mean.
2. Read ICG lines and understand control flow.
3. Explain how function calls appear in ICG (`arg`, `call`, `return`).
4. Explain simple optimizer effect (like `5+3` becoming `8`).
5. Map one input statement to ICG and then to generated C.

---

## 1) Files to study today

Read in this order:
1. `icg/icg.h`
2. `icg/icg.c`
3. `optimizer/optimizer.h`
4. `optimizer/optimizer.c`
5. `codegen/codegen.c`

What each file does:
1. `icg/icg.h`: list of ICG API functions.
2. `icg/icg.c`: creates temp names, labels, and ICG lines.
3. `optimizer/optimizer.c`: simplifies ICG lines.
4. `codegen/codegen.c`: turns ICG into C statements.

---

## 2) Core ICG concepts in simple words

### A) Temp variables

Names like `t1`, `t2`, `t3`.

Use:
1. Store intermediate expression values.
2. Keep operations step-by-step.

Example:
1. `t2=score+3`
2. `score=t2`

### B) Labels

Names like `L1`, `L2`.

Use:
1. Mark jump points.
2. Used for IF/CHECK and LOOP branches.

Example:
1. `ifFalse t1 goto L1`
2. `L1:`

### C) Branch instruction

`ifFalse cond goto Label`

Meaning:
1. If condition is false, jump to that label.
2. Skip some block lines.

### D) Function call instructions

Common lines:
1. `func_begin name`
2. `param x`
3. `arg a`
4. `t3 = call inc 1`
5. `return t1`
6. `func_end name`

---

## 3) Important ICG emit functions (with plain meaning)

From `icg/icg.c`:

1. `icg_new_temp`
- Creates next temp name (`t1`, `t2`, ...)

2. `icg_new_label`
- Creates next label (`L1`, `L2`, ...)

3. `icg_emit_assign(target, source)`
- Adds assignment line like `x=42`

4. `icg_emit_binary(result, left, op, right)`
- Adds binary line like `t1=a+b`

5. `icg_emit_if_false(cond, label)`
- Adds branch line `ifFalse cond goto label`

6. `icg_emit_goto(label)`
- Adds unconditional jump

7. `icg_emit_label(label)`
- Adds label line like `L1:`

8. `icg_emit_call` and `icg_emit_call_assign`
- Add function call lines with or without assigned return value

9. `icg_emit_return`
- Adds return line

---

## 4) Real ICG examples (from your environment)

### Example 1: Simple declaration + print

Command:
```powershell
./parser/wizard_parser.exe ./input/grade_valid_simple_decl_print.wiz
```

ICG output:
```text
# ICG BEGIN
x=42
print x
# ICG END
```

Meaning:
1. Create value for x.
2. Print x.

### Example 2: Condition and assignment

Command:
```powershell
./parser/wizard_parser.exe ./input/grade_valid_medium_condition_assign.wiz
```

ICG output:
```text
# ICG BEGIN
score=7
t1=score>5
ifFalse t1 goto L1
t2=score+3
score=t2
L1:
L2:
print score
# ICG END
```

Meaning:
1. Initialize score.
2. Compute condition into `t1`.
3. If false, jump to L1.
4. If true, do addition and assign back.
5. Continue to print.

### Example 3: Function and loop

Command:
```powershell
./parser/wizard_parser.exe ./input/grade_valid_complex_loop_function_output.wiz
```

ICG output:
```text
# ICG BEGIN
i=0
func_begin inc
param x
t1=x+1
return t1
func_end inc
L1:
t2=i<4
ifFalse t2 goto L2
arg i
t3 = call inc 1
i=t3
print i
goto L1
L2:
# ICG END
```

Meaning:
1. Function `inc` is defined in ICG block.
2. Loop starts at L1.
3. Condition checked each round.
4. Calls function with `arg i`.
5. Assigns function result back to i.
6. Jumps again to loop start.

### Example 4: Constant folding seen

Command:
```powershell
./parser/wizard_parser.exe ./input/optimizer_constant_fold_test.wiz
```

ICG output contains:
```text
t1=8
```

Meaning:
1. `5 + 3` was pre-computed by optimizer.
2. Runtime will not recompute it.

---

## 5) How optimizer helps (simple)

Optimizer runs before printing final ICG in parser pipeline.

Easy effects you can observe:
1. Constant math folding:
- `t1=5+3` becomes `t1=8`

2. Cleaner lines for later codegen.

Note:
1. Not every line will be optimized.
2. Control flow and function logic still stay clear.

---

## 6) How ICG becomes C code

In `codegen/codegen.c`:
1. ICG lines are read one by one.
2. Each known pattern becomes C statement.

Examples:
1. `x=42` -> `x=42;`
2. `print score` -> `printf("%d\n", score);`
3. `ifFalse t1 goto L1` -> `if (!(t1)) goto L1;`
4. `L1:` -> `L1: ;`
5. `t3 = call inc 1` with prior `arg i` -> `t3 = inc(i);`

---

## 7) End-to-end Day 5 command (ICG -> C -> runtime)

Run this from repo root:

```powershell
Push-Location parser
./wizard_parser.exe ../input/grade_valid_medium_condition_assign.wiz
g++ -Wall ../output/output.c -o ../output/day5_run.exe
../output/day5_run.exe
Pop-Location
```

Observed runtime output in your environment:
1. `10`

Meaning:
1. Source program logic was preserved.
2. ICG and generated C both worked correctly.

---

## 8) Detailed practice steps (easy workflow)

Step 1:
1. Run simple file and copy ICG lines.
2. Explain each line in one sentence.

Step 2:
1. Run medium condition file.
2. Circle all `t*` lines and all `L*` lines.
3. Explain each jump.

Step 3:
1. Run complex loop+function file.
2. Mark function area (`func_begin` to `func_end`).
3. Mark loop area (`L1`, `ifFalse`, `goto`).

Step 4:
1. Open `output/output.c` after parser run from parser folder.
2. Match 5 ICG lines to 5 C lines.

---

## 9) Day 5 worksheet

Write short answers:

1. What is temp variable in ICG?
2. What is label in ICG?
3. What does `ifFalse t1 goto L1` mean?
4. Why does optimizer turn `5+3` into `8`?
5. How does `print x` become C code?

Mini mapping table:

| ICG line | Meaning | C-style idea |
|---|---|---|
| `t1=score>5` | condition result | `t1 = score > 5;` |
| `ifFalse t1 goto L1` | false jump | `if (!t1) goto L1;` |
| `arg i` | push call arg | function arg list |
| `t3 = call inc 1` | call with return | `t3 = inc(i);` |
| `print score` | output value | `printf("%d\\n", score);` |

---

## 10) End-of-day checklist

- [ ] I read icg.h and icg.c.
- [ ] I read optimizer.c basics.
- [ ] I read codegen.c mapping flow.
- [ ] I ran 3 parser files and studied ICG output.
- [ ] I understood temps, labels, branches, and calls.
- [ ] I mapped ICG to C for at least 5 lines.
- [ ] I ran end-to-end and confirmed runtime output.

---

## 11) What Day 6 will be

Day 6 focus:
1. Deep optimizer internals.
2. Exactly which patterns are optimized.
3. How optimization changes final C output quality.

You are ready for Day 6 when:
1. You can read ICG lines confidently.
2. You can explain control flow jumps without guessing.
