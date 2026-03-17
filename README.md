# Arcane (Wizard64 Edition)

Arcane is a Flex-based lexer and Bison parser foundation aligned to the **Final 64 keyword set** defined in `wizard_keyword.md`.

## Status

- ✅ Lexer keywords/operators converted to Wizard64
- ✅ Sample `.wiz` inputs converted
- ✅ Parser grammar (`parser/wizard.y`) is buildable and runnable for syntax validation

## Core Files

- `lexer/wizard.l` — Wizard64 lexer rules
- `parser/wizard.y` — Wizard64 parser grammar
- `input/sample.wiz` — main sample using Wizard64 syntax
- `input/another-sample.wiz` — minimal sample
- `wizard_keyword.md` — source-of-truth keyword list

## Build

```bash
make lexer
make parser
```

## Run

```bash
make run
```

Or write token output to file:

```bash
make test
```

Run parser on sample:

```bash
make test-parser
```

## Token Output Format

```text
<TOKEN_TYPE, "lexeme", line:X>
```

## Notes

- Comments use `##` (single-line) and `#* ... *#` (multi-line, nested).
- Relational tokens (`==`, `!=`, `>`, `<`, `>=`, `<=`) are supported for conditions.
