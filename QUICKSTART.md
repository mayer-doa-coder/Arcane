# Arcane Lexer - Quick Start Guide

## What You Have

Your Arcane lexer is fully functional! Here's what's been implemented:

### Files Created/Updated
1. **[lexer/wizard.l](lexer/wizard.l)** - Complete Flex specification with:
   - 30+ wizard-themed keywords
   - All operators (arithmetic, logical, relational, bitwise, assignment)
   - Support for integers, floats, strings, characters
   - Single-line (`##`) and multi-line (`#* *#`) comments
   - Comprehensive error handling

2. **[input/sample.wiz](input/sample.wiz)** - Comprehensive test program demonstrating all features

3. **[LEXER_PRD.md](LEXER_PRD.md)** - Complete specification document for LLM reference

4. **[Makefile](Makefile)** - Build automation

5. **[README.md](README.md)** - Complete project documentation

6. **[output/output.txt](output/output.txt)** - Sample tokenization output

## Quick Commands

### Build the Lexer
```bash
cd lexer
flex wizard.l
g++ lex.yy.c -o wizard_lexer.exe
```

### Run on Sample File
```bash
# Output to file (from project root)
lexer\wizard_lexer.exe input/sample.wiz output/output.txt

# Output to console (from project root)
lexer\wizard_lexer.exe input/sample.wiz

# Or from lexer directory:
cd lexer
wizard_lexer.exe ../input/sample.wiz ../output/output.txt
```

**Note**: Use backslashes `\` for Windows paths, not forward slashes `/`.

### Using Your Own .wiz File
```bash
lexer\wizard_lexer.exe your_file.wiz output.txt
```
# How to Run the Bison Parser

## Quick Reference

### PowerShell (Recommended for Windows)
```powershell
# Run on simple_test.wiz
Get-Content input\simple_test.wiz | .\parser\wizard_parser.exe

# Run on any .wiz file
Get-Content input\YOUR_FILE.wiz | .\parser\wizard_parser.exe

# Run on parser_test.wiz (more complex example)
Get-Content input\parser_test.wiz | .\parser\wizard_parser.exe
```

### Using Makefile
```bash
# Test with simple_test.wiz
make test-simple

# Test with sample.wiz (full sample)
make test-parser

# Rebuild parser if needed
make parser

# Clean and rebuild
make clean
make parser
```

### Command Prompt (CMD)
```cmd
type input\simple_test.wiz | parser\wizard_parser.exe
```

## Token Types Reference

### Keywords (31 total)
```
KEYWORD_SPELL, KEYWORD_RITUAL, KEYWORD_CAST
KEYWORD_IF, KEYWORD_ELSE, KEYWORD_ELSEIF
KEYWORD_WHILE, KEYWORD_FOR, KEYWORD_REPEAT, KEYWORD_UNTIL
KEYWORD_BREAK, KEYWORD_CONTINUE, KEYWORD_RETURN
KEYWORD_ESSENCE, KEYWORD_SPIRIT, KEYWORD_RUNE, KEYWORD_SCROLL
KEYWORD_ORACLE, KEYWORD_VOID, KEYWORD_ARRAY, KEYWORD_GRIMOIRE
KEYWORD_TRUE, KEYWORD_FALSE, KEYWORD_NULL
KEYWORD_SUMMON, KEYWORD_MANIFEST, KEYWORD_SCRY
KEYWORD_CONST, KEYWORD_LET, KEYWORD_ENCHANT
KEYWORD_BIND, KEYWORD_TRANSMUTE
```

### Operators (30+ total)
- Arithmetic: `OP_PLUS`, `OP_MINUS`, `OP_MULTIPLY`, `OP_DIVIDE`, `OP_MODULUS`, `OP_POWER`
- Increment/Decrement: `OP_INCREMENT`, `OP_DECREMENT`
- Relational: `OP_EQUAL`, `OP_NOT_EQUAL`, `OP_GREATER`, `OP_LESS`, `OP_GREATER_EQUAL`, `OP_LESS_EQUAL`
- Logical: `OP_AND`, `OP_OR`, `OP_NOT`
- Assignment: `OP_ASSIGN`, `OP_PLUS_ASSIGN`, `OP_MINUS_ASSIGN`, `OP_MULTIPLY_ASSIGN`, `OP_DIVIDE_ASSIGN`, `OP_MODULUS_ASSIGN`
- Bitwise: `OP_BITWISE_AND`, `OP_BITWISE_OR`, `OP_BITWISE_XOR`, `OP_BITWISE_NOT`, `OP_LEFT_SHIFT`, `OP_RIGHT_SHIFT`

### Delimiters (11 total)
```
DELIM_LPAREN, DELIM_RPAREN          # ( )
DELIM_LBRACE, DELIM_RBRACE          # { }
DELIM_LBRACKET, DELIM_RBRACKET      # [ ]
DELIM_SEMICOLON, DELIM_COMMA         # ; ,
DELIM_DOT, DELIM_COLON, DELIM_ARROW  # . : ->
```

### Literals
- `INTEGER_LITERAL` - Decimal, hex (0xFF), octal (077)
- `FLOAT_LITERAL` - 3.14, 1.5e10
- `STRING_LITERAL` - "text"
- `CHAR_LITERAL` - 'c'

### Other
- `IDENTIFIER` - Variable/function names
- `UNKNOWN` - Unrecognized characters (errors)

## Sample Output Format

Input:
```arcane
spell fireball(power: essence) -> essence {
    return power ** 2;
}
```

Output:
```
=== Arcane Lexical Analysis ===
Tokenizing input...

<KEYWORD_SPELL, "spell", line:1>
<IDENTIFIER, "fireball", line:1>
<DELIM_LPAREN, "(", line:1>
<IDENTIFIER, "power", line:1>
<DELIM_COLON, ":", line:1>
<KEYWORD_ESSENCE, "essence", line:1>
<DELIM_RPAREN, ")", line:1>
<DELIM_ARROW, "->", line:1>
<KEYWORD_ESSENCE, "essence", line:1>
<DELIM_LBRACE, "{", line:1>
<KEYWORD_RETURN, "return", line:2>
<IDENTIFIER, "power", line:2>
<OP_POWER, "**", line:2>
<INTEGER_LITERAL, "2", line:2>
<DELIM_SEMICOLON, ";", line:2>
<DELIM_RBRACE, "}", line:3>

=== Tokenization Complete ===
Total lines processed: 3
```

## Integration with Bison (Future)

The lexer is designed for easy Bison integration:

1. Token types can be used in `wizard.y`
2. `yylex()` function returns tokens sequentially
3. `yyin` and `yyout` are compatible with Bison
4. Line number tracking built-in for error reporting

### Example Bison Token Declaration
```c
%token KEYWORD_SPELL KEYWORD_RITUAL KEYWORD_CAST
%token KEYWORD_IF KEYWORD_ELSE KEYWORD_WHILE
%token KEYWORD_ESSENCE KEYWORD_SPIRIT KEYWORD_ORACLE
%token IDENTIFIER INTEGER_LITERAL FLOAT_LITERAL STRING_LITERAL
%token OP_PLUS OP_MINUS OP_MULTIPLY OP_DIVIDE
// ... etc
```

## Testing Your Lexer

### Test Cases to Try

1. **All Keywords**
```arcane
spell ritual cast if else while for
essence spirit rune scroll oracle
manifest summon scry
```

2. **All Operators**
```arcane
+ - * / % ** ++ --
== != > < >= <=
&& || !
= += -= *= /=
& | ^ ~ << >>
```

3. **Literals**
```arcane
42          ## integer
0xFF        ## hex
077         ## octal
3.14        ## float
1.5e-10     ## scientific
"hello"     ## string
'A'         ## char
```

4. **Comments**
```arcane
## Single line comment

#*
  Multi-line
  comment
*#
```

5. **Error Handling**
```arcane
@#$%        ## Should produce UNKNOWN tokens
"unterminated string
```

## Next Steps

### For Bison Integration
1. Create `parser/wizard.y`
2. Define grammar rules for:
   - Spell declarations
   - Ritual (main) block
   - Expressions
   - Statements
   - Control structures
3. Generate parser: `bison -d wizard.y`
4. Link lexer and parser: `g++ lex.yy.c wizard.tab.c -o arcane`

### Enhancements
- Add column number tracking
- Implement symbol table
- Add preprocessor directives
- Support for Unicode identifiers
- Better error recovery

## Troubleshooting

### Compilation Errors
- Ensure Flex is installed: `flex --version`
- Ensure GCC/G++ is installed: `g++ --version`
- Check file paths are correct

### Runtime Errors
- Verify input file exists
- Check output directory is writable
- Ensure file path uses correct slashes for your OS

### Unexpected Tokens
- Check keyword spelling (case-sensitive)
- Verify operator patterns
- Review comment syntax (`##` and `#* *#`)

## Support Files

- **Full Specification**: See [LEXER_PRD.md](LEXER_PRD.md)
- **Project README**: See [README.md](README.md)
- **Sample Program**: See [input/sample.wiz](input/sample.wiz)
- **Sample Output**: See [output/output.txt](output/output.txt)

---

**Your lexer is production-ready for the next phase (Bison parser)!**
