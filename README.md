# Arcane
Arcane is a wizard-inspired programming language developed using Flex and Bison. The project demonstrates core compiler design principles, including lexical analysis, syntax analysis, tokenization, and grammar construction, through a structured and spell-based language design.

## Overview

Arcane features a unique magical programming paradigm with wizard-themed keywords:
- `spell` - Function declarations
- `ritual` - Main program block
- `cast` - Function calls
- `essence` - Integer type
- `spirit` - Floating-point type
- `manifest` - Output operation
- `summon` - Input operation
- And many more mystical keywords!

## Project Structure

```
Arcane/
├── lexer/
│   ├── wizard.l          # Flex lexer specification
│   ├── lex.yy.c          # Generated lexer (after build)
│   └── wizard_lexer.exe  # Compiled lexer executable (after build)
├── parser/
│   └── wizard.y          # Bison parser specification (TBD)
├── input/
│   └── sample.wiz        # Sample Arcane program
├── output/
│   └── output.txt        # Tokenization output
├── LEXER_PRD.md         # Comprehensive lexer requirements document
├── Makefile             # Build automation
└── README.md            # This file
```

## Current Status

✅ **Phase 1: Lexical Analysis (COMPLETE)**
- Full lexer implementation with 30+ keywords
- Support for all operators (arithmetic, logical, relational, bitwise)
- Integer, float, string, and character literals
- Single-line (`##`) and multi-line (`#* *#`) comments
- Comprehensive error reporting with line numbers

🔄 **Phase 2: Parser (Upcoming)**
- Will be implemented using Bison
- Grammar rules for language constructs
- AST generation

## Building the Lexer

### Prerequisites
- Flex (lexical analyzer generator)
- GCC/G++ compiler
- Make (optional, for Makefile usage)

### Using Makefile

```bash
# Build the lexer
make lexer

# Test with sample input
make test

# Run and display output to console
make run

# Clean generated files
make clean
```

### Manual Build

```bash
# Generate lexer source
cd lexer
flex wizard.l

# Compile lexer
g++ lex.yy.c -o wizard_lexer.exe

# Run lexer
wizard_lexer.exe ../input/sample.wiz ../output/output.txt
```

## Usage

### Basic Usage
```bash
lexer\wizard_lexer.exe input/sample.wiz output/output.txt
```

### Read from file, output to stdout
```bash
lexer\wizard_lexer.exe input/sample.wiz
```

### Interactive mode (stdin)
```bash
lexer\wizard_lexer.exe
```

## Token Output Format

The lexer produces tokens in the following format:
```
<TOKEN_TYPE, "lexeme", line:LINE_NUMBER>
```

Example:
```
<KEYWORD_SPELL, "spell", line:9>
<IDENTIFIER, "fireball", line:9>
<DELIM_LPAREN, "(", line:9>
<IDENTIFIER, "power", line:9>
<DELIM_COLON, ":", line:9>
<KEYWORD_ESSENCE, "essence", line:9>
```

## Language Features

### Keywords
- **Control Flow**: `if`, `else`, `elseif`, `while`, `for`, `repeat`, `until`, `break`, `continue`, `return`
- **Data Types**: `essence`, `spirit`, `rune`, `scroll`, `oracle`, `void`, `array`, `grimoire`
- **Functions**: `spell` (declare), `cast` (call), `ritual` (main)
- **I/O**: `manifest` (output), `summon` (input), `scry` (debug)
- **Variables**: `let`, `const`, `bind`
- **Other**: `enchant`, `transmute`

### Operators
- **Arithmetic**: `+`, `-`, `*`, `/`, `%`, `**`, `++`, `--`
- **Relational**: `==`, `!=`, `>`, `<`, `>=`, `<=`
- **Logical**: `&&`, `||`, `!`
- **Assignment**: `=`, `+=`, `-=`, `*=`, `/=`, `%=`
- **Bitwise**: `&`, `|`, `^`, `~`, `<<`, `>>`

### Literals
- **Integers**: `42`, `0xFF` (hex), `077` (octal)
- **Floats**: `3.14`, `2.71e10`
- **Strings**: `"Hello, Wizard!"`
- **Characters**: `'A'`, `'\n'`
- **Booleans**: `true`, `false`, `null`

### Comments
```arcane
## This is a single-line comment

#*
  This is a
  multi-line comment
*#
```

## Example Program

```arcane
## Fireball spell demonstration

spell fireball(power: essence, range: spirit) -> essence {
    let damage = power ** 2;
    manifest("Casting fireball with power: ");
    manifest(damage);
    return damage;
}

ritual main() {
    essence mana = 10;
    spirit radius = 3.5;
    
    if (mana >= 5) {
        cast fireball(mana, radius);
    } else {
        manifest("Not enough mana!");
    }
}
```

## Documentation

See [LEXER_PRD.md](LEXER_PRD.md) for comprehensive lexer requirements and specifications.

## Development

### Testing
The `input/sample.wiz` file contains a comprehensive test program demonstrating all language features. Run the lexer on this file to verify tokenization.

### Next Steps
1. Implement Bison parser (`wizard.y`)
2. Define grammar rules
3. Build Abstract Syntax Tree (AST)
4. Add semantic analysis
5. Generate intermediate code

## License

See [LICENSE](LICENSE) file for details.

## Author

Arcane Language Compiler Project - A demonstration of compiler construction principles.
```
