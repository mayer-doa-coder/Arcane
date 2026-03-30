# Day 4 Implementation Guide (Very Beginner Friendly)

You finished parser basics.
Now Day 4 is about semantic checking.

Semantic means:
1. Program structure may look correct,
2. but meaning can still be wrong.

Example:
- `x = true;` when `x` is `INT`.
- Grammar is valid, but meaning is wrong.

Estimated time: 3 to 5 hours.

---

## Day 4 success goals

By end of Day 4, you should be able to:
1. Explain what symbol table stores.
2. Explain undeclared, duplicate, and type mismatch errors.
3. Explain function argument count/type checking.
4. Explain BREAK_SPELL/KEEP_CASTING outside loop errors.
5. Explain CHECK_TYPES and CHECK_DECLARATIONS directives.

---

## 1) Core files for Day 4

Read in this order:
1. `symbol_table/symbol_table.h`
2. `symbol_table/symbol_table.c`
3. `parser/wizard.y` (semantic helper functions near top)
4. `parser/wizard.y` semantic-related grammar rules:
   - `declaration`
   - `assignment`
   - `if_stmt` and `if_else_part`
   - `call`
   - `break_stmt`, `continue_stmt`, `return_stmt`
   - `semantic_check`

---

## 2) What symbol table stores (simple)

For each symbol (name), it stores:
1. Name (like `x`, `add`)
2. Kind (variable, function, parameter)
3. Type (INT, FLOAT, BOOL, etc.)
4. Scope level
5. Owner function
6. Initialization status
7. Used status
8. Function signature (for functions)

So symbol table is memory of "who exists" and "what type/scope they have".

---

## 3) Key semantic checks and where they happen

### A) Undeclared variable

Idea:
1. You use a name before declaration.

Example:
```wiz
x = 10;
```

Error style:
1. Semantic Error: undeclared variable x

### B) Duplicate declaration

Idea:
1. Same variable declared again in same scope.

Example:
```wiz
DECLARE x AS INT := 1;
DECLARE x AS INT := 2;
```

Error style:
1. Semantic Error: duplicate variable x

### C) Type mismatch in assignment

Idea:
1. Left side type and right side type are not compatible.

Example:
```wiz
DECLARE x AS INT := 0;
x = true;
```

Error style:
1. Semantic Error: Type mismatch in assignment to 'x': INT <- BOOL

### D) Invalid condition type

Idea:
1. IF_CHARM condition must be boolean.
2. If expression becomes INT, it is semantic error.

Example:
```wiz
IF_CHARM x + 2 THEN
```

Error style:
1. Semantic Error: Condition must be boolean

### E) Wrong function arguments

Idea:
1. Function expects N args, call gives M args.
2. Or argument types do not match.

Error style:
1. Semantic Error: Function 'add' expects 2 argument(s) but got 1

### F) BREAK_SPELL / KEEP_CASTING outside loop

Idea:
1. BREAK_SPELL and KEEP_CASTING only work inside TIME_TURNER.

Error style:
1. Semantic Error: 'BREAK_SPELL' used outside loop
2. Semantic Error: 'KEEP_CASTING' used outside loop

### G) RETURN misuse

Idea:
1. RETURN outside function is invalid.
2. RETURN type must match function return type.

---

## 4) Guided Day 4 commands (run exactly)

Run from project root:

```powershell
./parser/wizard_parser.exe ./input/grade_negative_semantic_undeclared_var.wiz
./parser/wizard_parser.exe ./input/grade_negative_semantic_duplicate_decl.wiz
./parser/wizard_parser.exe ./input/grade_negative_semantic_type_mismatch.wiz
./parser/wizard_parser.exe ./input/grade_negative_semantic_invalid_condition.wiz
./parser/wizard_parser.exe ./input/grade_negative_semantic_wrong_function_args.wiz
./parser/wizard_parser.exe ./input/grade_edge_break_continue_outside_loop.wiz
```

Expected behavior:
1. You should see semantic errors.
2. Parser may still end with `Parse successful.`
3. That means grammar pass and semantic errors can both appear in same run.

---

## 5) Real output examples (from your environment)

### Undeclared variable

```text
Semantic Error (line 3): undeclared variable x
# ICG BEGIN
# ICG END
Parse successful.
```

### Duplicate declaration

```text
Semantic Error (line 4): duplicate variable x
# ICG BEGIN
x=1
# ICG END
Parse successful.
```

### Type mismatch

```text
Semantic Error (line 4): Type mismatch in assignment to 'x': INT <- BOOL
# ICG BEGIN
x=0
# ICG END
Parse successful.
```

### Invalid condition

```text
Semantic Error (line 4): Condition must be boolean
# ICG BEGIN
x=1
t1=x+2
ifFalse t1 goto L1
print x
L1:
# ICG END
Parse successful.
```

### Wrong function args

```text
Semantic Error (line 7): Function 'add' expects 2 argument(s) but got 1
# ICG BEGIN
a=1
func_begin add
param x
param y
t1=x+y
return t1
func_end add
arg a
call add 1
# ICG END
Parse successful.
```

### BREAK_SPELL/KEEP_CASTING outside loop

```text
Semantic Error (line 3): 'BREAK_SPELL' used outside loop
Semantic Error (line 4): 'KEEP_CASTING' used outside loop
# ICG BEGIN
# ICG END
Parse successful.
```

---

## 6) Semantic directives: CHECK_TYPES and CHECK_DECLARATIONS

These can be explicitly called using SORTING_HAT block.

Valid test:
```powershell
./parser/wizard_parser.exe ./input/semantic_directive_valid_test.wiz
```

You should see:
1. CHECK_TYPES completed message.
2. CHECK_DECLARATIONS completed message.

Invalid directive test:
```powershell
./parser/wizard_parser.exe ./input/semantic_directive_invalid_test.wiz
```

You should see:
1. Type mismatch error first.
2. Then directive summary messages.

Note:
1. Directive summary says "completed" even when earlier semantic errors exist.
2. It means directive pass itself ran to completion.

---

## 7) Why semantic errors can appear with Parse successful

Simple reason:
1. Parser accepted grammar shape.
2. Semantic checker found meaning problems.
3. Current implementation reports both.

So:
1. Parse successful does not always mean program is fully valid semantically.

---

## 8) Day 4 worksheet

Write short answers:

1. What is symbol table in one sentence?
2. What is undeclared variable error?
3. Difference between duplicate declaration and type mismatch?
4. Why can Parse successful appear with semantic error?
5. What do CHECK_TYPES and CHECK_DECLARATIONS do?

Mini table:

| Input pattern | Semantic result |
|---|---|
| x = 10; (without DECLARE x) | undeclared variable |
| DECLARE x ... then DECLARE x ... | duplicate variable |
| x is INT, assign true | type mismatch |
| IF_CHARM x + 2 THEN | condition must be boolean |
| SUMMON add WITH one arg but expects two | wrong function args |

---

## 9) End-of-day checklist

- [ ] I read symbol_table.h and symbol_table.c.
- [ ] I ran all semantic negative tests listed above.
- [ ] I understood undeclared/duplicate/type/condition errors.
- [ ] I understood wrong function args and loop control misuse.
- [ ] I ran semantic directive valid and invalid tests.
- [ ] I completed worksheet answers.

---

## 10) What Day 5 will be

Day 5 focus:
1. ICG internals (`icg/icg.h`, `icg/icg.c`)
2. Temp variables and labels
3. How each statement becomes ICG lines

You are ready for Day 5 when:
1. You can identify semantic error category quickly.
2. You can explain why a program parsed but still semantically failed.
