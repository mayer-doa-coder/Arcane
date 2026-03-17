# Product Requirements Document: Wizard64 Lexical Analyzer

## 1. Goal

Build a Flex lexer for Arcane that fully follows the 64-keyword language contract in `wizard_keyword.md`.

## 2. Source of Truth

All reserved words and mappings come from:
- `wizard_keyword.md`

## 3. Required Token Support

### 3.1 Program and Block Keywords
- `ENTER_HOGWARTS`, `EXIT_HOGWARTS`, `HOUSE`, `ENDHOUSE`
- `Gryffindor`, `Slytherin`, `Hufflepuff`, `Ravenclaw`

### 3.2 Data Types
- `INT`, `FLOAT`, `DOUBLE`, `LONG`, `CHAR`, `BOOL`, `VOID`

### 3.3 Declarations
- `DECLARE`, `AS`, `=`, `:=`

### 3.4 Control Flow
- `IF`, `FI`, `ELSE`, `CHECK`, `THEN`, `ENDCHECK`

### 3.5 Loops
- `LOOP`, `DO`, `ENDLOOP`, `BREAK`, `CONTINUE`

### 3.6 Functions
- `SPELL`, `ENDSPELL`, `SUMMON`, `WITH`, `RETURN`

### 3.7 Input/Output
- `CAST`, `PROPHECY`, `INPUT`

### 3.8 Scope and Semantic Validation
- `POTION`, `ENDPOTION`
- `SORTING_HAT`, `ENDHAT`, `CHECK_TYPES`, `CHECK_DECLARATIONS`

### 3.9 Arithmetic Operators
- `+`, `-`, `*`, `/`, `%`

### 3.10 Logical Operators
- `AND`, `OR`, `NOT`, `XOR`

### 3.11 Math Function Keywords
- `POWER`, `ROOT`, `FLOOR`, `CEIL`, `ABS`, `LOG`, `SIN`, `COS`, `TAN`

### 3.12 Generic Tokens
- `IDENTIFIER` → `[a-zA-Z_][a-zA-Z0-9_]*`
- `NUMBER` → `[0-9]+` (plus float/hex/octal recognition in lexer)

## 4. Additional Lexical Support

### 4.1 Delimiters
- `(` `)` `{` `}` `[` `]` `;` `,` `:`

### 4.2 Relational Operators
- `==`, `!=`, `>`, `<`, `>=`, `<=`

### 4.3 Literals
- Number, string, char, bool literal tokens

### 4.4 Comments
- Single-line: `## ...`
- Multi-line nested: `#* ... *#`

## 5. Behavior Requirements

1. Longest-match priority.
2. Reserved keywords must win over identifiers.
3. Line number tracking for each token.
4. Unknown character recovery (emit `UNKNOWN` and continue).

## 6. Debug Output Format

```text
<TOKEN_TYPE, "lexeme", line:X>
```

## 7. Acceptance Criteria

- Lexer successfully tokenizes `input/sample.wiz` using Wizard64 keywords.
- Output in `output/output.txt` reflects converted tokens.
- No old Arcane-era keywords (e.g., `spell`, `ritual`, `essence`) remain in active docs/samples.
