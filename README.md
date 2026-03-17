# Arcane (Wizard64 Edition)

Arcane is a Flex-based lexer for a wizard-themed language now fully aligned to the **Final 64 keyword set** defined in `wizard_keyword.md`.

## Status

- ✅ Lexer keywords/operators converted to Wizard64
- ✅ Sample `.wiz` inputs converted
- 🔄 Parser (`parser/wizard.y`) is still a placeholder and can be implemented next with these tokens

## Core Files

- `lexer/wizard.l` — Wizard64 lexer rules
- `input/sample.wiz` — main sample using Wizard64 syntax
- `input/another-sample.wiz` — minimal sample
- `wizard_keyword.md` — source-of-truth keyword list

## Build

```bash
make lexer
```

## Run

```bash
make run
```

Or write output to file:

```bash
make test
```

## Token Output Format

```text
<TOKEN_TYPE, "lexeme", line:X>
```

## Implemented Wizard64 Keywords

Program & Blocks:
- `ENTER_HOGWARTS`, `EXIT_HOGWARTS`, `HOUSE`, `ENDHOUSE`
- `Gryffindor`, `Slytherin`, `Hufflepuff`, `Ravenclaw`

Types:
- `INT`, `FLOAT`, `DOUBLE`, `LONG`, `CHAR`, `BOOL`, `VOID`

Declaration:
- `DECLARE`, `AS`, `=`, `:=`

Control:
- `IF`, `FI`, `ELSE`, `CHECK`, `THEN`, `ENDCHECK`

Loops:
- `LOOP`, `DO`, `ENDLOOP`, `BREAK`, `CONTINUE`

Functions:
- `SPELL`, `ENDSPELL`, `SUMMON`, `WITH`, `RETURN`

I/O:
- `CAST`, `PROPHECY`, `INPUT`

Scope/Semantics:
- `POTION`, `ENDPOTION`, `SORTING_HAT`, `ENDHAT`, `CHECK_TYPES`, `CHECK_DECLARATIONS`

Logical & Math:
- `AND`, `OR`, `NOT`, `XOR`
- `POWER`, `ROOT`, `FLOOR`, `CEIL`, `ABS`, `LOG`, `SIN`, `COS`, `TAN`

## Notes

- Comments use `##` (single-line) and `#* ... *#` (multi-line, nested).
- Relational tokens (`==`, `!=`, `>`, `<`, `>=`, `<=`) are supported for conditions.
- Parser generation is not yet implemented in `wizard.y`.
