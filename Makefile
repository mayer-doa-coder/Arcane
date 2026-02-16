# Arcane Language Compiler Makefile
# Wizard-inspired programming language

# Compiler and tools
LEX = flex
# Use MinGW's bison to avoid path space issues with GnuWin32
YACC = C:/MinGW/msys/1.0/bin/bison.exe
CC = gcc
CXX = g++
RM = rm -f

# Directories
LEXER_DIR = lexer
PARSER_DIR = parser
INPUT_DIR = input
OUTPUT_DIR = output

# Files
LEXER_SOURCE = $(LEXER_DIR)/wizard.l
LEXER_OUTPUT = $(LEXER_DIR)/lex.yy.c
LEXER_EXEC = $(LEXER_DIR)/wizard_lexer.exe

PARSER_SOURCE = $(PARSER_DIR)/wizard.y
PARSER_LEXER = $(LEXER_DIR)/wizard_parser.l
PARSER_TAB_C = $(PARSER_DIR)/wizard.tab.c
PARSER_TAB_H = $(PARSER_DIR)/wizard.tab.h
PARSER_LEX_C = $(PARSER_DIR)/lex.yy.c
PARSER_EXEC = $(PARSER_DIR)/wizard_parser.exe

SAMPLE_INPUT = $(INPUT_DIR)/sample.wiz
SIMPLE_INPUT = $(INPUT_DIR)/simple_test.wiz
OUTPUT_FILE = $(OUTPUT_DIR)/output.txt

# Compiler flags
CFLAGS = -Wall
CXXFLAGS = -Wall

# Default target
.PHONY: all
all: parser

# Build lexer only
.PHONY: lexer
lexer: $(LEXER_EXEC)

$(LEXER_EXEC): $(LEXER_SOURCE)
	@echo "Generating lexer from wizard.l..."
	cd $(LEXER_DIR) && $(LEX) wizard.l
	@echo "Compiling lexer..."
	cd $(LEXER_DIR) && $(CXX) $(CXXFLAGS) lex.yy.c -o wizard_lexer.exe
	@echo "Lexer built successfully!"

# Build parser (Bison + Flex integration)
.PHONY: parser
parser: $(PARSER_EXEC)

$(PARSER_EXEC): $(PARSER_SOURCE) $(PARSER_LEXER)
	@echo "Generating parser from wizard.y..."
	cd $(PARSER_DIR) && $(YACC) -d wizard.y
	@echo "Generating lexer for parser..."
	cd $(PARSER_DIR) && $(LEX) ../lexer/wizard_parser.l
	@echo "Compiling parser..."
	cd $(PARSER_DIR) && $(CXX) $(CXXFLAGS) wizard.tab.c lex.yy.c -o wizard_parser.exe
	@echo "Parser built successfully!"

# Run lexer on sample input
.PHONY: test-lexer
test-lexer: $(LEXER_EXEC)
	@echo "Running lexer on sample.wiz..."
	$(LEXER_EXEC) $(SAMPLE_INPUT) $(OUTPUT_FILE)
	@echo "Output written to $(OUTPUT_FILE)"

# Run parser on sample input
.PHONY: test-parser
test-parser: $(PARSER_EXEC)
	@echo "Running parser on sample.wiz..."
	$(PARSER_EXEC) < $(SAMPLE_INPUT)

# Test parser with simple input
.PHONY: test-simple
test-simple: $(PARSER_EXEC)
	@echo "Running parser on simple_test.wiz..."
	$(PARSER_EXEC) < $(SIMPLE_INPUT)

# Alias for testing parser
.PHONY: test
test: test-parser

# Run lexer to stdout
.PHONY: run
run: $(LEXER_EXEC)
	@$(LEXER_EXEC) $(SAMPLE_INPUT)

# Clean generated files
.PHONY: clean
clean:
	@echo "Cleaning generated files..."
	-cd $(LEXER_DIR) && $(RM) lex.yy.c lex.yy.cc wizard_lexer.exe
	-cd $(PARSER_DIR) && $(RM) *.tab.c *.tab.h *.output lex.yy.c wizard_parser.exe
	-cd $(OUTPUT_DIR) && $(RM) output.txt
	@echo "Clean complete!"

# Help
.PHONY: help
help:
	@echo "Arcane Language Compiler - Makefile Help"
	@echo "========================================="
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build the parser (default)"
	@echo "  make parser       - Build the parser with Bison and Flex"
	@echo "  make lexer        - Build the lexer only"
	@echo "  make test         - Build and test parser with sample.wiz"
	@echo "  make test-parser  - Build and test parser with sample.wiz"
	@echo "  make test-simple  - Build and test parser with simple_test.wiz"
	@echo "  make test-lexer   - Build and test lexer with sample.wiz"
	@echo "  make run          - Run lexer and display output to console"
	@echo "  make clean        - Remove generated files"
	@echo "  make help         - Display this help message"
	@echo ""
	@echo "Manual usage:"
	@echo "  Lexer:  $(LEXER_EXEC) <input.wiz> [output.txt]"
	@echo "  Parser: $(PARSER_EXEC) < <input.wiz>"
	@echo ""