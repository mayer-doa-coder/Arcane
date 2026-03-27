# Arcane Testing + Grading Validation Matrix

This document maps executable tests to rubric evidence and shows how to collect proof.

## 1. Test Suite Structure

All grading tests are under input/ with prefix grade_.

### A) Valid Programs

1. grade_valid_simple_decl_print.wiz
- Scope: declaration + output
- Expected: parse success, no semantic error, generated C compiles, runtime prints 42

2. grade_valid_medium_condition_assign.wiz
- Scope: condition + assignment + output
- Expected: parse success, valid IF/CHECK flow in IR, generated C compiles, runtime prints 10

3. grade_valid_complex_loop_function_output.wiz
- Scope: loop + function definition + function call + output
- Expected: parse success, IR includes func_begin/func_end/arg/call/return, generated C has real function and main call flow

### B) Negative Tests

1. grade_negative_invalid_token.wiz
- Scope: lexical analysis
- Expected: lexical error message for invalid symbol

2. grade_negative_syntax_missing_endcheck.wiz
- Scope: syntax analysis
- Expected: parse error for malformed CHECK block

3. grade_negative_semantic_undeclared_var.wiz
- Scope: semantic undeclared identifier
- Expected: semantic error for undeclared variable

4. grade_negative_semantic_duplicate_decl.wiz
- Scope: semantic duplicate declaration
- Expected: semantic error for duplicate variable declaration

5. grade_negative_semantic_type_mismatch.wiz
- Scope: semantic type checking
- Expected: assignment type mismatch semantic error

6. grade_negative_semantic_invalid_condition.wiz
- Scope: semantic condition validation
- Expected: condition must be boolean semantic error

7. grade_negative_semantic_wrong_function_args.wiz
- Scope: function signature validation
- Expected: function arity mismatch semantic error

### C) Edge Cases

1. grade_edge_empty_program.wiz
- Scope: minimal valid boundary
- Expected: parse success with minimal/no statements

2. grade_edge_nested_structures.wiz
- Scope: nested loop and condition flow
- Expected: parse success, stable labels/branches in IR

3. grade_edge_multiple_errors.wiz
- Scope: error recovery and multi-error reporting
- Expected: multiple semantic errors in one run where possible

4. grade_edge_break_continue_outside_loop.wiz
- Scope: loop control misuse
- Expected: semantic errors for BREAK and CONTINUE outside loop

5. grade_edge_call_before_declaration.wiz
- Scope: function lookup order edge case
- Expected: undeclared function semantic error at call site

## 2. Output Validation Workflow

For each test collect:

1. Console/parser output
- Contains lexical/syntax/semantic diagnostics and ICG dump
- Artifact: logs/<test>.log

2. Intermediate representation (IR)
- Captured in same parser log between # ICG BEGIN and # ICG END
- Also compare against output/icg_full_test_output.txt style snapshots if used in course report

3. Generated C output
- Artifact: generated_c/<test>.c
- Confirms codegen correctness and function lowering

4. Runtime output (valid tests)
- Compile generated C and execute
- Artifact: logs/<test>.runtime.log

### Expected Log Markers (Quick Checks)

Use these markers when validating parser logs:

| Test | Expected Marker |
| --- | --- |
| grade_valid_simple_decl_print | Parse successful. |
| grade_valid_medium_condition_assign | Parse successful. |
| grade_valid_complex_loop_function_output | func_begin inc and call inc |
| grade_negative_invalid_token | Parse error: syntax error |
| grade_negative_syntax_missing_endcheck | Parse error: syntax error |
| grade_negative_semantic_undeclared_var | Semantic Error and undeclared variable |
| grade_negative_semantic_duplicate_decl | Semantic Error and duplicate variable |
| grade_negative_semantic_type_mismatch | Semantic Error and Type mismatch in assignment |
| grade_negative_semantic_invalid_condition | Semantic Error and Condition must be boolean |
| grade_negative_semantic_wrong_function_args | Semantic Error and expects 2 argument(s) but got 1 |
| grade_edge_break_continue_outside_loop | Semantic Error and BREAK used outside loop / CONTINUE used outside loop |
| grade_edge_call_before_declaration | Semantic Error and undeclared function |

## 3. Rubric Mapping Table

| Rubric Area | Test Evidence |
| --- | --- |
| Lexical Analysis | grade_negative_invalid_token.wiz log with lexical error message |
| Syntax Analysis | grade_negative_syntax_missing_endcheck.wiz log with parse error |
| Semantic Analysis | undeclared/duplicate/type/condition/function-arg tests logs |
| Execution Correctness | valid tests runtime logs from compiled generated C |
| Advanced Features | complex valid test IR shows function IR + optimized form + generated C functions |

## 4. Evidence Collection for Submission

1. Keep the generated grading artifact folder (timestamped).
2. Include summary.csv and summary.txt in report appendix.
3. Attach representative logs:
- one lexical error log
- one syntax error log
- two semantic error logs
- one valid end-to-end runtime log
4. Include before/after optimization examples from parser log snippets.

Suggested optimization evidence snippets:
- Constant fold: t1=5+3 -> t1=8
- Copy propagation: b=a then c=b -> c=a (when safe)
- Dead temp elimination: dropped temp assignment with no use

## 5. Automation Command

Run all grading tests from repo root:

powershell -ExecutionPolicy Bypass -File run_grading_suite.ps1

Optional (skip rebuild):

powershell -ExecutionPolicy Bypass -File run_grading_suite.ps1 -SkipBuild

Artifacts produced under grading_artifacts/<timestamp>/:
- logs/
- generated_c/
- exe/
- summary.csv
- summary.txt

Quick inspection commands:

1. Open summary:
- Import-Csv .\grading_artifacts\<timestamp>\summary.csv | Format-Table

2. Filter semantic failures:
- Import-Csv .\grading_artifacts\<timestamp>\summary.csv | Where-Object { $_.HasSemanticError -eq "True" }

3. Show only valid runtime evidence:
- Import-Csv .\grading_artifacts\<timestamp>\summary.csv | Where-Object { $_.Category -eq "valid" -and $_.CCompiled -eq "True" }

## 6. Final Validation Checklist

- All core language features tested
- Lexical, syntax, and semantic error classes covered
- IR evidence captured for control flow, functions, and optimization impact
- Generated C validated for valid/edge-success tests
- Rubric areas linked to concrete executable evidence
