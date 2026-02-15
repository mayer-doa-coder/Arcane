# Product Requirements Document: Arcane Lexical Analyzer

## 1. Project Overview

### 1.1 Purpose
Create a lexical analyzer (lexer) using Flex for the Arcane programming language - a wizard-inspired scripting language. The lexer will tokenize source code written in `.wiz` files and prepare tokens for subsequent parsing by Bison.

### 1.2 Project Context
- **Language Name**: Arcane (Wizard)
- **File Extension**: `.wiz`
- **Toolchain**: Flex (lexer) → Bison (parser)
- **Output**: C++ implementation (`lex.yy.cpp`)
- **Theme**: Spell-based, magical programming paradigm

### 1.3 Deliverables
- `wizard.l` - Flex specification file
- Token definitions compatible with Bison integration
- Tokenized output capability for debugging
- Support for `yyin` (input) and `yyout` (output) file redirection

---

## 2. Language Specification

### 2.1 Keywords (Reserved Words)

#### Control Flow
- `spell` - Function/procedure declaration (equivalent to `function`)
- `ritual` - Main program block (equivalent to `main`)
- `cast` - Function call/execution
- `if` - Conditional statement
- `else` - Alternative branch
- `elseif` - Chained conditional
- `while` - Loop construct
- `for` - Iteration construct
- `repeat` - Alternative loop syntax
- `until` - Loop termination condition
- `break` - Exit loop
- `continue` - Skip to next iteration
- `return` - Return from spell

#### Data Types
- `essence` - Integer type
- `spirit` - Floating-point type
- `rune` - Character type
- `scroll` - String type
- `oracle` - Boolean type
- `void` - No return type
- `array` - Array declaration
- `grimoire` - Structure/record type

#### Boolean Literals
- `true` - Boolean true
- `false` - Boolean false
- `null` - Null/undefined value

#### Input/Output
- `summon` - Input/read operation
- `manifest` - Output/print operation
- `scry` - Debug print/inspect

#### Other Keywords
- `const` - Constant declaration
- `let` - Variable declaration
- `enchant` - Modifier (like `static`)
- `bind` - Assignment/initialization
- `transmute` - Type casting

### 2.2 Operators

#### Arithmetic Operators
- `+` - Addition
- `-` - Subtraction
- `*` - Multiplication
- `/` - Division
- `%` - Modulus
- `**` - Power/exponentiation
- `++` - Increment
- `--` - Decrement

#### Relational Operators
- `==` - Equal to
- `!=` - Not equal to
- `>` - Greater than
- `<` - Less than
- `>=` - Greater than or equal to
- `<=` - Less than or equal to

#### Logical Operators
- `&&` - Logical AND
- `||` - Logical OR
- `!` - Logical NOT

#### Assignment Operators
- `=` - Simple assignment
- `+=` - Add and assign
- `-=` - Subtract and assign
- `*=` - Multiply and assign
- `/=` - Divide and assign
- `%=` - Modulus and assign

#### Bitwise Operators (Optional)
- `&` - Bitwise AND
- `|` - Bitwise OR
- `^` - Bitwise XOR
- `~` - Bitwise NOT
- `<<` - Left shift
- `>>` - Right shift

### 2.3 Delimiters and Punctuation
- `(` `)` - Parentheses (grouping, function calls)
- `{` `}` - Braces (code blocks)
- `[` `]` - Brackets (array indexing)
- `;` - Statement terminator
- `,` - Separator (parameters, arguments)
- `.` - Member access
- `:` - Type annotation/label
- `->` - Return type indicator

### 2.4 Identifiers
- **Pattern**: `[a-zA-Z_][a-zA-Z0-9_]*`
- **Rules**:
  - Must start with letter or underscore
  - Can contain letters, digits, underscores
  - Case-sensitive
  - Cannot be a reserved keyword
- **Examples**: `mana`, `spell_power`, `elementalForce`, `_privateVar`

### 2.5 Literals

#### Integer Literals
- **Decimal**: `[0-9]+`
  - Examples: `0`, `42`, `1337`
- **Hexadecimal** (Optional): `0x[0-9a-fA-F]+`
  - Examples: `0xFF`, `0x2A`
- **Octal** (Optional): `0[0-7]+`
  - Examples: `077`, `052`

#### Floating-Point Literals
- **Pattern**: `[0-9]+\.[0-9]+([eE][+-]?[0-9]+)?`
- **Examples**: `3.14`, `2.71828`, `1.5e10`, `6.022e-23`

#### String Literals
- **Pattern**: `"([^"\\]|\\.)*"`
- **Rules**:
  - Enclosed in double quotes
  - Support escape sequences: `\"`, `\\`, `\n`, `\t`, `\r`
- **Examples**: `"Hello, Wizard!"`, `"Path: C:\\magic\\spells"`

#### Character Literals
- **Pattern**: `'([^'\\]|\\.)'`
- **Rules**:
  - Enclosed in single quotes
  - Single character or escape sequence
- **Examples**: `'A'`, `'\n'`, `'\''`

### 2.6 Comments

#### Single-Line Comments
- **Pattern**: `##.*`
- **Description**: From `##` to end of line
- **Example**: `## This is a comment`

#### Multi-Line Comments
- **Pattern**: `#*...*#`
- **Description**: Nestable block comments
- **Example**:
  ```
  #*
    This is a
    multi-line comment
  *#
  ```

### 2.7 Whitespace
- **Characters**: Space (` `), Tab (`\t`), Newline (`\n`), Carriage Return (`\r`)
- **Treatment**: Ignored (used as token separators)

---

## 3. Token Definitions

### 3.1 Token Categories
Each recognized pattern should produce a token with:
- **Token Type** (category)
- **Lexeme** (matched text)
- **Line Number** (for error reporting)
- **Column Number** (optional, for precise error location)

### 3.2 Token Type Enumeration
```
KEYWORD_SPELL, KEYWORD_RITUAL, KEYWORD_CAST, KEYWORD_IF, KEYWORD_ELSE, KEYWORD_ELSEIF,
KEYWORD_WHILE, KEYWORD_FOR, KEYWORD_REPEAT, KEYWORD_UNTIL, KEYWORD_BREAK, KEYWORD_CONTINUE,
KEYWORD_RETURN, KEYWORD_ESSENCE, KEYWORD_SPIRIT, KEYWORD_RUNE, KEYWORD_SCROLL,
KEYWORD_ORACLE, KEYWORD_VOID, KEYWORD_ARRAY, KEYWORD_GRIMOIRE, KEYWORD_TRUE,
KEYWORD_FALSE, KEYWORD_NULL, KEYWORD_SUMMON, KEYWORD_MANIFEST, KEYWORD_SCRY,
KEYWORD_CONST, KEYWORD_LET, KEYWORD_ENCHANT, KEYWORD_BIND, KEYWORD_TRANSMUTE,

IDENTIFIER,
INTEGER_LITERAL, FLOAT_LITERAL, STRING_LITERAL, CHAR_LITERAL,

OP_PLUS, OP_MINUS, OP_MULTIPLY, OP_DIVIDE, OP_MODULUS, OP_POWER,
OP_INCREMENT, OP_DECREMENT,
OP_EQUAL, OP_NOT_EQUAL, OP_GREATER, OP_LESS, OP_GREATER_EQUAL, OP_LESS_EQUAL,
OP_AND, OP_OR, OP_NOT,
OP_ASSIGN, OP_PLUS_ASSIGN, OP_MINUS_ASSIGN, OP_MULTIPLY_ASSIGN, OP_DIVIDE_ASSIGN, OP_MODULUS_ASSIGN,
OP_BITWISE_AND, OP_BITWISE_OR, OP_BITWISE_XOR, OP_BITWISE_NOT,
OP_LEFT_SHIFT, OP_RIGHT_SHIFT,

DELIM_LPAREN, DELIM_RPAREN, DELIM_LBRACE, DELIM_RBRACE, DELIM_LBRACKET, DELIM_RBRACKET,
DELIM_SEMICOLON, DELIM_COMMA, DELIM_DOT, DELIM_COLON, DELIM_ARROW,

COMMENT, WHITESPACE, UNKNOWN, END_OF_FILE
```

---

## 4. Lexer Behavior Requirements

### 4.1 Pattern Matching Priority
1. **Longest Match**: Always match the longest possible token
2. **Keyword vs Identifier**: Keywords take precedence over identifiers
3. **Operator Precedence**: Multi-character operators (e.g., `**`, `==`) before single-character

### 4.2 Error Handling
- Unrecognized characters should be reported as `UNKNOWN` tokens
- Print error message with line and column number
- Continue lexing (don't abort on first error)
- Example: `Error: Unrecognized character '@' at line 5, column 12`

### 4.3 Line Tracking
- Maintain line number counter (starting at 1)
- Increment on newline character (`\n`)
- Include line number in token output

### 4.4 Token Output Format
For debugging purposes, each token should be output in the format:
```
<TOKEN_TYPE, "lexeme", line:X>
```

Example output:
```
<KEYWORD_SPELL, "spell", line:1>
<IDENTIFIER, "fireball", line:1>
<DELIM_LPAREN, "(", line:1>
<IDENTIFIER, "power", line:1>
<DELIM_COLON, ":", line:1>
<KEYWORD_ESSENCE, "essence", line:1>
<DELIM_RPAREN, ")", line:1>
<DELIM_LBRACE, "{", line:1>
```

---

## 5. Flex Implementation Requirements

### 5.1 File Structure
The `wizard.l` file should include:

1. **Definitions Section**:
   - C/C++ includes
   - Global variables (line counter)
   - Helper function declarations
   - Option declarations

2. **Rules Section**:
   - Pattern-action pairs for all tokens
   - Ordered from most specific to least specific
   - Comment handling
   - Whitespace handling

3. **User Code Section**:
   - `main()` function
   - File I/O setup (`yyin`, `yyout`)
   - Helper function implementations

### 5.2 Required Options
```
%option noyywrap
%option yylineno
%option c++
```

### 5.3 Input/Output Handling
```cpp
int main(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
    }
    
    if (argc > 2) {
        yyout = fopen(argv[2], "w");
        if (!yyout) {
            fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
            return 1;
        }
    }
    
    // Tokenization loop
    while (yylex()) {
        // Token output handled in rules
    }
    
    if (yyin) fclose(yyin);
    if (yyout) fclose(yyout);
    
    return 0;
}
```

### 5.4 Output Generation
- Write token information to `yyout` (or stdout if not specified)
- Include token type, lexeme, and line number
- Use consistent formatting for easy parsing

---

## 6. Example Usage

### 6.1 Sample Input File (`sample.wiz`)
```
## Arcane Spell Example

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

### 6.2 Expected Output Format
```
<KEYWORD_SPELL, "spell", line:3>
<IDENTIFIER, "fireball", line:3>
<DELIM_LPAREN, "(", line:3>
<IDENTIFIER, "power", line:3>
<DELIM_COLON, ":", line:3>
<KEYWORD_ESSENCE, "essence", line:3>
<DELIM_COMMA, ",", line:3>
<IDENTIFIER, "range", line:3>
<DELIM_COLON, ":", line:3>
<KEYWORD_SPIRIT, "spirit", line:3>
<DELIM_RPAREN, ")", line:3>
<DELIM_ARROW, "->", line:3>
<KEYWORD_ESSENCE, "essence", line:3>
<DELIM_LBRACE, "{", line:3>
...
```

### 6.3 Command-Line Usage
```bash
# Generate lexer
flex wizard.l

# Compile
g++ lex.yy.cpp -o wizard_lexer

# Run with input file
./wizard_lexer input/sample.wiz output/output.txt

# Or with just input (output to stdout)
./wizard_lexer input/sample.wiz
```

---

## 7. Bison Integration Preparation

### 7.1 Token Declarations for Bison
The lexer should be designed to easily integrate with Bison. Token values should be:
- Defined in a way compatible with Bison's token declarations
- Return integer token codes from `yylex()`
- Support `yylval` for storing token attributes (semantic values)

### 7.2 Suggested Structure for Integration
```cpp
// For future Bison integration
typedef struct {
    char* string_value;
    int int_value;
    double float_value;
    int line_number;
} TokenValue;

// This will be used by Bison
extern TokenValue yylval;
```

---

## 8. Testing Requirements

### 8.1 Test Categories
1. **Keywords**: Verify all keywords are recognized
2. **Identifiers**: Test valid and edge-case identifiers
3. **Literals**: Test all literal types with various formats
4. **Operators**: Test all operators including multi-character ones
5. **Comments**: Test single-line and multi-line comments
6. **Edge Cases**: Empty file, whitespace-only, unterminated strings
7. **Error Cases**: Invalid characters, malformed literals

### 8.2 Success Criteria
- All valid tokens correctly recognized
- Correct line number tracking
- Proper error reporting for invalid input
- No crashes on malformed input
- Consistent output format

---

## 9. Technical Constraints

### 9.1 Environment
- **Flex Version**: 2.6.x or higher
- **Compiler**: GCC/G++ or MSVC with C++11 support
- **Platform**: Cross-platform (Windows, Linux, macOS)

### 9.2 Performance
- Should handle files up to several MB without performance degradation
- Linear time complexity relative to input size

### 9.3 Memory
- Minimal memory footprint
- No memory leaks
- Proper cleanup of file handles

---

## 10. Future Considerations

### 10.1 Phase 2 (Parser Integration)
- Token definitions will be shared with `wizard.y`
- `yylval` will carry semantic values to parser
- Error recovery mechanisms

### 10.2 Potential Extensions
- Unicode identifier support
- Additional escape sequences
- Preprocessor directives
- Include file mechanism
- Conditional compilation

---

## 11. Implementation Checklist

- [ ] Create `wizard.l` with proper structure (definitions, rules, user code)
- [ ] Define all keywords with patterns
- [ ] Define all operators with correct precedence
- [ ] Implement identifier pattern
- [ ] Implement all literal patterns (integer, float, string, char)
- [ ] Implement comment handling (single and multi-line)
- [ ] Implement whitespace handling
- [ ] Add line number tracking
- [ ] Implement token output formatting
- [ ] Add error handling for unknown characters
- [ ] Implement file I/O with `yyin` and `yyout`
- [ ] Create main() function with command-line argument handling
- [ ] Test with sample input file
- [ ] Verify output format correctness
- [ ] Ensure compilation produces `lex.yy.cpp`
- [ ] Document compilation and usage instructions

---

## 12. Glossary

- **Lexer**: Lexical analyzer; tokenizes source code
- **Token**: Smallest meaningful unit in source code
- **Lexeme**: Actual character sequence matched by a pattern
- **Pattern**: Regular expression defining token structure
- **Flex**: Fast lexical analyzer generator
- **Bison**: Parser generator (works with Flex)
- **yyin**: Flex input file pointer
- **yyout**: Flex output file pointer
- **yylex()**: Main lexer function that returns next token

---

## 13. References

- Flex Manual: https://github.com/westes/flex
- Bison Manual: https://www.gnu.org/software/bison/manual/
- Compiler Design Principles
- Regular Expression Patterns

---

**Document Version**: 1.0  
**Last Updated**: February 16, 2026  
**Status**: Ready for Implementation
