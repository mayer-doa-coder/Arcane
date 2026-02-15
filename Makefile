# Arcane Language Compiler Makefile
# Wizard-inspired programming language

# Compiler and tools
LEX = flex
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
SAMPLE_INPUT = $(INPUT_DIR)/sample.wiz
OUTPUT_FILE = $(OUTPUT_DIR)/output.txt

# Compiler flags
CFLAGS = -Wall
CXXFLAGS = -Wall

# Default target
.PHONY: all
all: lexer

# Build lexer only
.PHONY: lexer
lexer: $(LEXER_EXEC)

$(LEXER_EXEC): $(LEXER_SOURCE)
	@echo "Generating lexer from wizard.l..."
	cd $(LEXER_DIR) && $(LEX) wizard.l
	@echo "Compiling lexer..."
	cd $(LEXER_DIR) && $(CXX) $(CXXFLAGS) lex.yy.c -o wizard_lexer.exe
	@echo "Lexer built successfully!"

# Run lexer on sample input
.PHONY: test
test: $(LEXER_EXEC)
	@echo "Running lexer on sample.wiz..."
	$(LEXER_EXEC) $(SAMPLE_INPUT) $(OUTPUT_FILE)
	@echo "Output written to $(OUTPUT_FILE)"
	@echo ""
	@echo "First 50 lines of output:"
	@head -n 50 $(OUTPUT_FILE) || type $(OUTPUT_FILE) | more

# Run lexer to stdout
.PHONY: run
run: $(LEXER_EXEC)
	@$(LEXER_EXEC) $(SAMPLE_INPUT)

# Clean generated files
.PHONY: clean
clean:
	@echo "Cleaning generated files..."
	-cd $(LEXER_DIR) && $(RM) lex.yy.c lex.yy.cc wizard_lexer.exe
	-cd $(PARSER_DIR) && $(RM) *.tab.c *.tab.h
	-cd $(OUTPUT_DIR) && $(RM) output.txt
	@echo "Clean complete!"

# Help
.PHONY: help
help:
	@echo "Arcane Language Compiler - Makefile Help"
	@echo "========================================="
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the lexer (default)"
	@echo "  make lexer    - Build the lexer"
	@echo "  make test     - Build and test lexer with sample.wiz"
	@echo "  make run      - Run lexer and display output to console"
	@echo "  make clean    - Remove generated files"
	@echo "  make help     - Display this help message"
	@echo ""
	@echo "Manual usage:"
	@echo "  $(LEXER_EXEC) <input.wiz> [output.txt]"
	@echo ""

