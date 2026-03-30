# Formal Project Report

## Project Title
Arcane (Wizard64 Edition): End-to-End Compiler Pipeline with Automated Grading Evidence

## Date
March 30, 2026

## 1. Introduction
Arcane (Wizard64 Edition) is a compiler project that implements a full source-to-executable workflow for a themed programming language. The project begins with lexical analysis and syntax parsing, then continues through semantic validation, intermediate code generation, optimization, C code generation, native compilation, and runtime execution.

The project is designed for both implementation learning and academic demonstration. It provides not only compiler modules, but also repeatable testing, artifact collection, and report-ready evidence.

## 2. Objectives
The main objectives of this project are:

1. Build a reliable lexer and parser aligned to the Wizard64 keyword specification.
2. Implement semantic validation for declarations, types, function signatures, and control-flow correctness.
3. Generate intermediate code and an optimized intermediate form.
4. Translate intermediate representation into valid C code.
5. Compile generated C code into native executables for valid programs.
6. Provide structured negative and edge-case testing with reproducible artifacts.
7. Support demonstration through both command-line workflow and web API integration.

## 3. Project Scope
The implemented scope includes:

- Lexer rules and tokenization for Wizard64 language features.
- Bison parser grammar and parse execution flow.
- Symbol table management for variables, functions, and parameters.
- Semantic checks and categorized diagnostics.
- Intermediate code generation and optimization.
- C code emission from intermediate representation.
- Automated grading suite with logs, generated C snapshots, executable artifacts, and summary reports.
- Optional backend API to run compiler workflow from a browser interface.

## 4. Tools and Technologies Used
### 4.1 Core Compiler Toolchain
- Flex for lexical analyzer generation.
- Bison for parser generation.
- g++ for building parser executable and generated C programs.
- Make for build orchestration.

### 4.2 Runtime and Automation
- PowerShell for automated grading and artifact management.
- Windows command environment for parser execution and compilation tasks.

### 4.3 Web Integration
- Node.js with Express backend for compile-and-run API access.
- CORS and JSON request handling for browser-based interaction.

## 5. System Workflow
The project follows this pipeline:

1. Wizard source file (.wiz) is parsed after lexical tokenization.
2. Semantic checks validate declarations, types, conditions, and function calls.
3. Intermediate code is produced in raw form.
4. Optimizer transforms the intermediate code into an optimized form.
5. Code generator emits output/output.c.
6. g++ compiles generated C into a native executable.
7. Executable produces runtime output for valid programs.

## 6. Implementation Overview by Module
### 6.1 Lexer Module
The lexer supports Wizard64 keywords, operators, delimiters, literals, and comment forms, with line-aware token output formatting.

### 6.2 Parser Module
The parser processes language structure and orchestrates later stages including semantic handling, ICG, optimization, and codegen handoff.

### 6.3 Semantic Layer
Semantic validation includes undeclared identifier detection, duplicate declaration checks, type mismatch checks, condition validation, and function argument arity checks.

### 6.4 Intermediate Code and Optimizer
The system emits intermediate code and an optimized variant to make transformation effects inspectable and reportable.
The optimizer applies multiple passes, including constant folding, algebraic simplification, copy propagation, and dead code elimination.
In constant folding, pure integer expressions such as t1=5+3 are reduced to t1=8 when it is safe to do so.
The optimizer also protects correctness by skipping control-flow and call-sensitive lines (for example labels, gotos, ifFalse, arg/call, return, print, and function markers).

### 6.5 Code Generator
Code generation converts IR instructions into C statements, including function emission, call lowering, conditional/goto mapping, print handling, and main-body emission.

### 6.6 Grading Automation
A PowerShell grading suite runs categorized tests, captures parser/runtime outputs, copies generated C files per test, and exports summary.csv plus summary.txt.

## 7. Testing and Validation Strategy
The testing strategy is structured into three categories:

1. Valid tests: confirm parse success, C compilation success, and correct runtime behavior.
2. Negative tests: verify lexical, syntax, and semantic diagnostics.
3. Edge tests: evaluate boundary behavior and robustness.

For each test case, the suite collects:

- Parser log.
- Generated C file snapshot.
- Runtime log for executable cases.
- Structured summary row with classification flags.

## 8. Evidence-Based Results
Based on the latest grading run summary:

- Total tests executed: 15
- Category split: 3 valid, 7 negative, 5 edge
- Generated C compiled successfully: 5 cases
- Runtime exit code 0: 5 cases
- Lexical error cases detected: 1
- Parse error cases detected: 2
- Semantic error cases detected: 8

Interpretation:

- All valid programs passed the full pipeline through native execution.
- Negative and selected edge tests correctly stopped before runtime and were classified by error type.
- The summary format provides direct rubric mapping for lexical, syntax, semantic, and execution evidence.

### 8.1 Constant Folding and Optimization Evidence
Optimization behavior is observable in this project rather than hidden.

- Constant folding is implemented as a dedicated pass in the optimizer.
- Algebraic simplification, copy propagation, and dead code elimination are also applied in iterative optimization rounds.
- The project includes optimizer-focused tests, such as optimizer_constant_fold_test.wiz and optimizer_simplify_deadcode_test.wiz, to verify these transformations.
- Report evidence can show explicit before/after examples such as t1=5+3 to t1=8.

### 8.2 Edge Cases Handled
The grading suite explicitly covers edge-case behavior and records evidence per case.

1. Empty program boundary.
- Test: grade_edge_empty_program.wiz
- Handling: accepted as minimal valid structure and can complete pipeline execution.

2. Nested control structures.
- Test: grade_edge_nested_structures.wiz
- Handling: nested loop/condition flow is parsed and lowered with stable branch labels.

3. Multiple errors in one input.
- Test: grade_edge_multiple_errors.wiz
- Handling: semantic diagnostics are captured for review rather than only first-failure visibility.

4. Loop-control misuse outside loops.
- Test: grade_edge_break_continue_outside_loop.wiz
- Handling: semantic errors are raised for BREAK_SPELL and KEEP_CASTING used outside valid loop context.

5. Function call before declaration.
- Test: grade_edge_call_before_declaration.wiz
- Handling: semantic undeclared-function validation prevents invalid call lowering.

## 9. Unique and Exceptional Features
The following features are notable strengths of this project:

1. Wizard64 language identity with a strict implementation contract.
- The language design is specification-led, not only theme-led. The keyword contract, grammar, and samples are aligned to the same source of truth.
- This improves consistency between documentation, tokenization behavior, and parser expectations.

2. Full traceability across every compiler stage.
- The project makes each step visible: source program, parser behavior, semantic diagnostics, IR, optimized IR, generated C, and runtime output.
- This level of traceability is especially valuable for academic defense and debugging.

3. Dual IR observability (RAW and OPT forms).
- Both unoptimized and optimized intermediate code are explicitly exposed.
- This enables direct verification of optimization impact instead of relying on assumptions.

4. Evidence-first grading automation.
- The grading workflow generates timestamped artifacts including logs, generated C snapshots, executables, and summary tables.
- This creates a reproducible audit trail suitable for formal submission and external review.

5. Structured diagnostic categorization.
- Lexical, syntax, and semantic signals are distinguishable in automated summaries.
- This supports objective rubric mapping and rapid failure triage.

6. Maintainable code generation structure.
- Codegen responsibilities are separated into clear units such as line classification, pending argument buffering, function emission, and main emission.
- This modular approach supports onboarding, review, and extension.

7. Practical backend safety architecture.
- Browser-driven compilation is guarded by input-size limits, per-process timeouts, no-shell process spawning, temporary workspace isolation, and serialized execution.
- These controls reduce risk and improve runtime stability for repeated interactive use.

8. Advanced lexical behavior for a student compiler.
- The lexer supports nested multi-line comments in addition to single-line comments.
- Nested comment handling is a non-trivial lexer capability that strengthens language robustness.

### 9.1 Feasibility of Adding More Unique Features
The following candidate features were evaluated against the current architecture:

1. JSON diagnostic export per test case.
- Feasibility: High (can be added immediately).
- Why feasible: error classes already exist in logs, API responses, and summary output.

2. Automatic RAW-vs-OPT IR diff snippets in grading artifacts.
- Feasibility: High (can be added immediately).
- Why feasible: both IR versions are already emitted with stable markers.

3. Runtime expected-output assertions for valid tests.
- Feasibility: High (can be added immediately).
- Why feasible: runtime logs are already captured; adding expected-value checks is a small script enhancement.

4. Single-page HTML report generator from summary and logs.
- Feasibility: Medium (short implementation effort).
- Why feasible: the artifact structure is already normalized and machine-readable.

5. Optional control-flow graph export from IR labels and gotos.
- Feasibility: Medium.
- Why feasible: required graph nodes and edges already exist in current IR lines.

6. Incremental compile cache for repeated API submissions.
- Feasibility: Medium.
- Why feasible: possible with hashing and cache invalidation, but requires careful state management due to current per-run isolation design.

7. Grammar-aware fuzz testing for lexer/parser robustness.
- Feasibility: Medium to Low (larger effort).
- Why feasible: batch harness exists, but robust generator and triage tooling need additional implementation.

## 10. Limitations
Current limitations observed in the present implementation include:

- Build warnings from generated or helper code that are non-fatal but should be cleaned for stricter quality gates.
- Some edge/negative scenarios produce parse-success wording while semantic errors are still present, requiring interpretation by error flags.
- Platform assumptions currently favor the documented Windows setup and installed toolchain paths.

## 11. Recommendations and Future Work
Recommended next steps for improved maturity:

1. Standardize diagnostic messaging so semantic failure states are clearly separated from parse-success wording.
2. Expand optimizer evidence with before/after diff blocks per test case.
3. Add automated assertions on expected runtime output values for valid cases.
4. Add CI workflow to run grading suite on each change.
5. Extend language support with additional constructs and stronger type semantics.
6. Produce a formal benchmark section for compilation time and runtime comparison across test groups.

## 12. Conclusion
Arcane (Wizard64 Edition) demonstrates a complete and demonstrable compiler pipeline with reproducible evidence generation. The project meets its core educational and engineering objectives by combining language processing modules, robust test categorization, output traceability, and practical deployment support.

The current implementation is suitable for formal demonstration, grading submission, and iterative extension toward a more production-grade compiler framework.

## Appendix A: Evidence and Reference Files
- README.md
- QUICKSTART.md
- FULL_RUN_GUIDE.md
- LEXER_PRD.md
- TESTING_GRADING_MATRIX.md
- run_grading_suite.ps1
- grading_artifacts/2026-03-30_124720/summary.csv
