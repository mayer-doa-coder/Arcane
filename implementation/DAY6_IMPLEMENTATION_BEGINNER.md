# Day 6 Implementation Guide (Very Beginner Friendly)

Day 6 focus is optimizer internals.

You already know parser and ICG.
Now we learn how optimizer changes ICG lines before codegen.

Estimated time: 4 to 6 hours.

---

## Day 6 success goals

By end of Day 6, you should be able to:
1. Explain each optimizer pass in simple words.
2. Identify which ICG lines can be optimized.
3. Explain constant folding, algebraic simplification, copy propagation, and dead code elimination.
4. Compare before/after ICG behavior from real test files.
5. Explain optimizer limits (why some lines are not touched).

---

## 1) Files to study today

Read in this order:
1. `optimizer/optimizer.h`
2. `optimizer/optimizer.c`
3. `icg/icg.c` (quick revisit)
4. `codegen/codegen.c` (to see optimizer impact on final C)

What each file does today:
1. `optimizer.h`: optimizer API list.
2. `optimizer.c`: real optimization logic.
3. `icg.c`: helps you understand line format optimizer reads.
4. `codegen.c`: shows why cleaner ICG makes cleaner C.

---

## 2) Optimizer pass list (plain words)

From `optimizer/optimizer.c`, main passes are:

1. Constant folding (`pass_constant_folding`)
- If both sides are constants, compute early.
- Example: `t1=5+3` -> `t1=8`

2. Algebraic simplification (`pass_algebraic_simplification`)
- Math identity shortcuts.
- Example: `x=a+0` -> `x=a`
- Example: `x=b*1` -> `x=b`
- Example: `x=y*0` -> `x=0`

3. Copy propagation (`pass_copy_propagation`)
- If one variable copies another, later use original directly when safe.
- Example idea: `b=a` then `c=b` -> can become `c=a`

4. Dead code elimination (`pass_dead_code_elimination`)
- Remove temp assignment not used later.
- Only temp names (`t*`) are targeted.

5. `optimizer_run_all`
- Runs passes in loop until no more change.

---

## 3) Important safety idea (very important)

Optimizer does NOT touch risky lines.

`line_is_unsafe_for_optimization` protects lines like:
1. labels (`L1:`)
2. jumps (`goto`, `ifFalse`)
3. function markers (`func_begin`, `func_end`)
4. call/arg/return/print lines
5. math function calls (`POWO(...)`, `RADIX(...)`, etc.)

Simple reason:
1. Those lines control program flow or side effects.
2. Wrong optimization there could break behavior.

---

## 4) Real optimizer examples from your environment

### Example A: compare test

Command:
```powershell
./parser/wizard_parser.exe ./input/optimizer_compare_test.wiz
```

Observed ICG:
```text
# ICG BEGIN
a=0
b=0
c=0
t1=8
a=t1
b=t1
c=t1
# ICG END
```

What it shows:
1. Constant folding happened (`5+3` became `8`).
2. Same computed value reused in later lines.

### Example B: simplify deadcode test

Command:
```powershell
./parser/wizard_parser.exe ./input/optimizer_simplify_deadcode_test.wiz
```

Observed ICG:
```text
# ICG BEGIN
a=4
b=5
keep=0
drop=0
keep=a
keep=b
t3=a+b
drop=t3
t4=1
ifFalse t4 goto L1
t5=keep+1
keep=t5
L1:
L2:
# ICG END
```

What it shows:
1. Algebraic simplification happened (`+0`, `*1` style simplifications).
2. Branch condition with constants became simpler (`t4=1`).
3. Some lines remain because they are still meaningful for flow/use.

### Example C: constant fold test

Command:
```powershell
./parser/wizard_parser.exe ./input/optimizer_constant_fold_test.wiz
```

Easy observation:
1. You can see constant expressions already reduced in ICG output.

---

## 5) Step-by-step Day 6 learning workflow

Step 1: read helper functions in optimizer.c
1. `parse_assignment_parts`
2. `parse_binary_rhs`
3. `eval_binary_int`
4. `line_is_unsafe_for_optimization`

Goal:
1. Understand how optimizer reads one line and decides action.

Step 2: read each pass one by one
1. `pass_constant_folding`
2. `pass_algebraic_simplification`
3. `pass_copy_propagation`
4. `pass_dead_code_elimination`

Goal:
1. For each pass, write one before->after example.

Step 3: run optimizer test inputs
1. `optimizer_constant_fold_test.wiz`
2. `optimizer_compare_test.wiz`
3. `optimizer_simplify_deadcode_test.wiz`

Goal:
1. Match output lines to pass behavior.

Step 4: connect optimizer to codegen
1. Run parser from `parser/` directory and generate C.
2. Open `output/output.c`.
3. Observe cleaner expressions and fewer unnecessary operations.

---

## 6) Commands for Day 6

Run from project root:

```powershell
./parser/wizard_parser.exe ./input/optimizer_constant_fold_test.wiz
./parser/wizard_parser.exe ./input/optimizer_compare_test.wiz
./parser/wizard_parser.exe ./input/optimizer_simplify_deadcode_test.wiz
```

Run end-to-end with generated C:

```powershell
Push-Location parser
./wizard_parser.exe ../input/optimizer_compare_test.wiz
g++ -Wall ../output/output.c -o ../output/day6_run.exe
../output/day6_run.exe
Pop-Location
```

---

## 7) How to explain each pass in interview/simple viva

1. Constant folding:
- Compiler computes constant math early.

2. Algebraic simplification:
- Compiler removes math identities like +0 and *1.

3. Copy propagation:
- Compiler follows copied values to reduce redundant assignments.

4. Dead code elimination:
- Compiler removes temp calculations that are never used.

5. Safety guard:
- Compiler avoids changing control-flow/call/print lines.

---

## 8) Common confusion and simple fix

Confusion 1:
1. Why optimizer did not change some lines?
Fix:
1. Line may be unsafe to optimize or not a matching pattern.

Confusion 2:
1. Why Parse successful still appears even with issues?
Fix:
1. Parse is grammar success; semantic/other warnings can still show separately.

Confusion 3:
1. Why labels `L1`, `L2` remain?
Fix:
1. Labels are control-flow structure and usually preserved.

---

## 9) Day 6 worksheet

Write short answers:

1. What is constant folding?
2. Give one algebraic simplification example.
3. What is copy propagation in one sentence?
4. Why dead code elimination mainly targets temp variables?
5. Why optimizer avoids `ifFalse` and `goto` lines?

Mini table:

| Before ICG | After ICG | Pass |
|---|---|---|
| `t1=5+3` | `t1=8` | Constant folding |
| `x=a+0` | `x=a` | Algebraic simplification |
| `y=b*1` | `y=b` | Algebraic simplification |
| `z=t2` where `t2` is unused later | line removed | Dead code elimination |
| `ifFalse t1 goto L1` | unchanged | Safety rule |

---

## 10) End-of-day checklist

- [ ] I read optimizer.h and optimizer.c fully.
- [ ] I understood all four main optimization passes.
- [ ] I ran all optimizer test files listed.
- [ ] I mapped at least 5 before->after examples.
- [ ] I explained why some lines are intentionally unchanged.
- [ ] I generated C after optimization and inspected output.

---

## 11) What Day 7 will be

Day 7 focus:
1. Codegen deep dive.
2. How each ICG instruction is translated to C line-by-line.
3. Function signature emission, call argument buffering, and label/jump translation.

You are ready for Day 7 when:
1. You can tell which optimizer pass changed a given line.
2. You can explain optimized ICG without help.
