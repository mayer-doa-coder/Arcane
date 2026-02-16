%{
/* Arcane Language Parser */
/* Bison grammar for Wizard-inspired programming language */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations */
void yyerror(const char *s);
int yylex(void);

/* AST node structure */
typedef struct ASTNode {
    char *type;
    char *value;
    struct ASTNode **children;
    int num_children;
} ASTNode;

/* AST helper functions */
ASTNode* create_node(const char *type, const char *value);
ASTNode* create_node_with_children(const char *type, int num_children, ...);
void print_ast(ASTNode *node, int depth);
void free_ast(ASTNode *node);

extern int line_number;
ASTNode *root = NULL;

%}

/* Enable verbose error messages */
%error-verbose

/* Union for semantic values */
%union {
    char *string;
    int integer;
    double floating;
    struct ASTNode *node;
}

/* Token declarations - Keywords */
%token KEYWORD_SPELL KEYWORD_RITUAL KEYWORD_CAST
%token KEYWORD_IF KEYWORD_ELSE KEYWORD_ELSEIF
%token KEYWORD_WHILE KEYWORD_FOR KEYWORD_REPEAT KEYWORD_UNTIL
%token KEYWORD_BREAK KEYWORD_CONTINUE KEYWORD_RETURN
%token KEYWORD_ESSENCE KEYWORD_SPIRIT KEYWORD_RUNE KEYWORD_SCROLL
%token KEYWORD_ORACLE KEYWORD_VOID KEYWORD_ARRAY KEYWORD_GRIMOIRE
%token KEYWORD_TRUE KEYWORD_FALSE KEYWORD_NULL
%token KEYWORD_SUMMON KEYWORD_MANIFEST KEYWORD_SCRY
%token KEYWORD_CONST KEYWORD_LET KEYWORD_ENCHANT KEYWORD_BIND KEYWORD_TRANSMUTE

/* Token declarations - Operators */
%token OP_POWER OP_INCREMENT OP_DECREMENT
%token OP_EQUAL OP_NOT_EQUAL OP_GREATER_EQUAL OP_LESS_EQUAL
%token OP_AND OP_OR
%token OP_PLUS_ASSIGN OP_MINUS_ASSIGN OP_MULTIPLY_ASSIGN OP_DIVIDE_ASSIGN OP_MODULUS_ASSIGN
%token OP_LEFT_SHIFT OP_RIGHT_SHIFT
%token OP_PLUS OP_MINUS OP_MULTIPLY OP_DIVIDE OP_MODULUS
%token OP_GREATER OP_LESS OP_NOT OP_ASSIGN
%token OP_BITWISE_AND OP_BITWISE_OR OP_BITWISE_XOR OP_BITWISE_NOT

/* Token declarations - Delimiters */
%token DELIM_LPAREN DELIM_RPAREN DELIM_LBRACE DELIM_RBRACE
%token DELIM_LBRACKET DELIM_RBRACKET DELIM_SEMICOLON DELIM_COMMA
%token DELIM_DOT DELIM_COLON DELIM_ARROW

/* Token declarations - Literals and identifiers */
%token <string> IDENTIFIER STRING_LITERAL CHAR_LITERAL
%token <integer> INTEGER_LITERAL
%token <floating> FLOAT_LITERAL

/* Non-terminal types */
%type <node> program declaration_list declaration
%type <node> function_declaration parameter_list parameter
%type <node> type_specifier statement_list statement
%type <node> compound_statement expression_statement
%type <node> if_statement while_statement for_statement
%type <node> return_statement break_statement continue_statement
%type <node> declaration_statement assignment_statement
%type <node> expression primary_expression postfix_expression
%type <node> unary_expression multiplicative_expression
%type <node> additive_expression relational_expression
%type <node> equality_expression logical_and_expression
%type <node> logical_or_expression assignment_expression
%type <node> argument_list literal

/* Operator precedence and associativity (lowest to highest) */
%right OP_ASSIGN OP_PLUS_ASSIGN OP_MINUS_ASSIGN OP_MULTIPLY_ASSIGN OP_DIVIDE_ASSIGN OP_MODULUS_ASSIGN
%left OP_OR
%left OP_AND
%left OP_BITWISE_OR
%left OP_BITWISE_XOR
%left OP_BITWISE_AND
%left OP_EQUAL OP_NOT_EQUAL
%left OP_LESS OP_LESS_EQUAL OP_GREATER OP_GREATER_EQUAL
%left OP_LEFT_SHIFT OP_RIGHT_SHIFT
%left OP_PLUS OP_MINUS
%left OP_MULTIPLY OP_DIVIDE OP_MODULUS
%right OP_NOT OP_BITWISE_NOT UNEG
%right OP_POWER
%left OP_INCREMENT OP_DECREMENT
%left DELIM_LPAREN DELIM_LBRACKET DELIM_DOT

%%

/* Grammar Rules */

program:
    declaration_list {
        $$ = create_node("Program", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->num_children = 1;
        root = $$;
    }
    | /* empty */ {
        $$ = create_node("Program", "empty");
        root = $$;
    }
    ;

declaration_list:
    declaration {
        $$ = create_node("DeclarationList", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->num_children = 1;
    }
    | declaration_list declaration {
        $$ = $1;
        $$->children = (ASTNode**)realloc($$->children, ($$->num_children + 1) * sizeof(ASTNode*));
        $$->children[$$->num_children] = $2;
        $$->num_children++;
    }
    ;

declaration:
    function_declaration { $$ = $1; }
    ;

function_declaration:
    KEYWORD_SPELL IDENTIFIER DELIM_LPAREN parameter_list DELIM_RPAREN DELIM_ARROW type_specifier compound_statement {
        $$ = create_node("FunctionDecl", $2);
        $$->children = (ASTNode**)malloc(3 * sizeof(ASTNode*));
        $$->children[0] = $4;  /* parameters */
        $$->children[1] = $7;  /* return type */
        $$->children[2] = $8;  /* body */
        $$->num_children = 3;
        free($2);
    }
    | KEYWORD_SPELL IDENTIFIER DELIM_LPAREN DELIM_RPAREN DELIM_ARROW type_specifier compound_statement {
        $$ = create_node("FunctionDecl", $2);
        $$->children = (ASTNode**)malloc(3 * sizeof(ASTNode*));
        $$->children[0] = create_node("ParameterList", "empty");
        $$->children[1] = $6;  /* return type */
        $$->children[2] = $7;  /* body */
        $$->num_children = 3;
        free($2);
    }
    | KEYWORD_RITUAL IDENTIFIER DELIM_LPAREN parameter_list DELIM_RPAREN compound_statement {
        $$ = create_node("RitualDecl", $2);
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $4;  /* parameters */
        $$->children[1] = $6;  /* body */
        $$->num_children = 2;
        free($2);
    }
    | KEYWORD_RITUAL IDENTIFIER DELIM_LPAREN DELIM_RPAREN compound_statement {
        $$ = create_node("RitualDecl", $2);
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = create_node("ParameterList", "empty");
        $$->children[1] = $5;  /* body */
        $$->num_children = 2;
        free($2);
    }
    ;

parameter_list:
    parameter {
        $$ = create_node("ParameterList", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->num_children = 1;
    }
    | parameter_list DELIM_COMMA parameter {
        $$ = $1;
        $$->children = (ASTNode**)realloc($$->children, ($$->num_children + 1) * sizeof(ASTNode*));
        $$->children[$$->num_children] = $3;
        $$->num_children++;
    }
    ;

parameter:
    IDENTIFIER DELIM_COLON type_specifier {
        $$ = create_node("Parameter", $1);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $3;
        $$->num_children = 1;
        free($1);
    }
    ;

type_specifier:
    KEYWORD_ESSENCE { $$ = create_node("Type", "essence"); }
    | KEYWORD_SPIRIT { $$ = create_node("Type", "spirit"); }
    | KEYWORD_RUNE { $$ = create_node("Type", "rune"); }
    | KEYWORD_SCROLL { $$ = create_node("Type", "scroll"); }
    | KEYWORD_ORACLE { $$ = create_node("Type", "oracle"); }
    | KEYWORD_VOID { $$ = create_node("Type", "void"); }
    ;

compound_statement:
    DELIM_LBRACE statement_list DELIM_RBRACE {
        $$ = create_node("CompoundStmt", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $2;
        $$->num_children = 1;
    }
    | DELIM_LBRACE DELIM_RBRACE {
        $$ = create_node("CompoundStmt", "empty");
    }
    ;

statement_list:
    statement {
        $$ = create_node("StatementList", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->num_children = 1;
    }
    | statement_list statement {
        $$ = $1;
        $$->children = (ASTNode**)realloc($$->children, ($$->num_children + 1) * sizeof(ASTNode*));
        $$->children[$$->num_children] = $2;
        $$->num_children++;
    }
    ;

statement:
    compound_statement { $$ = $1; }
    | expression_statement { $$ = $1; }
    | if_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    | return_statement { $$ = $1; }
    | break_statement { $$ = $1; }
    | continue_statement { $$ = $1; }
    | declaration_statement { $$ = $1; }
    | assignment_statement { $$ = $1; }
    ;

expression_statement:
    expression DELIM_SEMICOLON {
        $$ = create_node("ExprStmt", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->num_children = 1;
    }
    | DELIM_SEMICOLON {
        $$ = create_node("ExprStmt", "empty");
    }
    ;

if_statement:
    KEYWORD_IF DELIM_LPAREN expression DELIM_RPAREN statement {
        $$ = create_node("IfStmt", "");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $3;  /* condition */
        $$->children[1] = $5;  /* then body */
        $$->num_children = 2;
    }
    | KEYWORD_IF DELIM_LPAREN expression DELIM_RPAREN statement KEYWORD_ELSE statement {
        $$ = create_node("IfElseStmt", "");
        $$->children = (ASTNode**)malloc(3 * sizeof(ASTNode*));
        $$->children[0] = $3;  /* condition */
        $$->children[1] = $5;  /* then body */
        $$->children[2] = $7;  /* else body */
        $$->num_children = 3;
    }
    ;

while_statement:
    KEYWORD_WHILE DELIM_LPAREN expression DELIM_RPAREN statement {
        $$ = create_node("WhileStmt", "");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $3;  /* condition */
        $$->children[1] = $5;  /* body */
        $$->num_children = 2;
    }
    ;

for_statement:
    KEYWORD_FOR DELIM_LPAREN expression_statement expression_statement expression DELIM_RPAREN statement {
        $$ = create_node("ForStmt", "");
        $$->children = (ASTNode**)malloc(4 * sizeof(ASTNode*));
        $$->children[0] = $3;  /* init */
        $$->children[1] = $4;  /* condition */
        $$->children[2] = $5;  /* increment */
        $$->children[3] = $7;  /* body */
        $$->num_children = 4;
    }
    ;

return_statement:
    KEYWORD_RETURN expression DELIM_SEMICOLON {
        $$ = create_node("ReturnStmt", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $2;
        $$->num_children = 1;
    }
    | KEYWORD_RETURN DELIM_SEMICOLON {
        $$ = create_node("ReturnStmt", "empty");
    }
    ;

break_statement:
    KEYWORD_BREAK DELIM_SEMICOLON {
        $$ = create_node("BreakStmt", "");
    }
    ;

continue_statement:
    KEYWORD_CONTINUE DELIM_SEMICOLON {
        $$ = create_node("ContinueStmt", "");
    }
    ;

declaration_statement:
    type_specifier IDENTIFIER OP_ASSIGN expression DELIM_SEMICOLON {
        $$ = create_node("VarDeclStmt", $2);
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;  /* type */
        $$->children[1] = $4;  /* initializer */
        $$->num_children = 2;
        free($2);
    }
    | type_specifier IDENTIFIER DELIM_SEMICOLON {
        $$ = create_node("VarDeclStmt", $2);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $1;  /* type */
        $$->num_children = 1;
        free($2);
    }
    | KEYWORD_LET IDENTIFIER OP_ASSIGN expression DELIM_SEMICOLON {
        $$ = create_node("LetStmt", $2);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $4;  /* initializer */
        $$->num_children = 1;
        free($2);
    }
    | KEYWORD_CONST type_specifier IDENTIFIER OP_ASSIGN expression DELIM_SEMICOLON {
        $$ = create_node("ConstDeclStmt", $3);
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $2;  /* type */
        $$->children[1] = $5;  /* initializer */
        $$->num_children = 2;
        free($3);
    }
    ;

assignment_statement:
    IDENTIFIER OP_ASSIGN expression DELIM_SEMICOLON {
        $$ = create_node("AssignStmt", $1);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $3;
        $$->num_children = 1;
        free($1);
    }
    | IDENTIFIER OP_PLUS_ASSIGN expression DELIM_SEMICOLON {
        $$ = create_node("PlusAssignStmt", $1);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $3;
        $$->num_children = 1;
        free($1);
    }
    | IDENTIFIER OP_MINUS_ASSIGN expression DELIM_SEMICOLON {
        $$ = create_node("MinusAssignStmt", $1);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $3;
        $$->num_children = 1;
        free($1);
    }
    ;

/* Expression grammar with proper precedence */
expression:
    assignment_expression { $$ = $1; }
    ;

assignment_expression:
    logical_or_expression { $$ = $1; }
    ;

logical_or_expression:
    logical_and_expression { $$ = $1; }
    | logical_or_expression OP_OR logical_and_expression {
        $$ = create_node("BinaryOp", "||");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    ;

logical_and_expression:
    equality_expression { $$ = $1; }
    | logical_and_expression OP_AND equality_expression {
        $$ = create_node("BinaryOp", "&&");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    ;

equality_expression:
    relational_expression { $$ = $1; }
    | equality_expression OP_EQUAL relational_expression {
        $$ = create_node("BinaryOp", "==");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | equality_expression OP_NOT_EQUAL relational_expression {
        $$ = create_node("BinaryOp", "!=");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    ;

relational_expression:
    additive_expression { $$ = $1; }
    | relational_expression OP_LESS additive_expression {
        $$ = create_node("BinaryOp", "<");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | relational_expression OP_GREATER additive_expression {
        $$ = create_node("BinaryOp", ">");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | relational_expression OP_LESS_EQUAL additive_expression {
        $$ = create_node("BinaryOp", "<=");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | relational_expression OP_GREATER_EQUAL additive_expression {
        $$ = create_node("BinaryOp", ">=");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    ;

additive_expression:
    multiplicative_expression { $$ = $1; }
    | additive_expression OP_PLUS multiplicative_expression {
        $$ = create_node("BinaryOp", "+");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | additive_expression OP_MINUS multiplicative_expression {
        $$ = create_node("BinaryOp", "-");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    ;

multiplicative_expression:
    unary_expression { $$ = $1; }
    | multiplicative_expression OP_MULTIPLY unary_expression {
        $$ = create_node("BinaryOp", "*");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | multiplicative_expression OP_DIVIDE unary_expression {
        $$ = create_node("BinaryOp", "/");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | multiplicative_expression OP_MODULUS unary_expression {
        $$ = create_node("BinaryOp", "%");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    | multiplicative_expression OP_POWER unary_expression {
        $$ = create_node("BinaryOp", "**");
        $$->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->children[1] = $3;
        $$->num_children = 2;
    }
    ;

unary_expression:
    postfix_expression { $$ = $1; }
    | OP_MINUS unary_expression %prec UNEG {
        $$ = create_node("UnaryOp", "-");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $2;
        $$->num_children = 1;
    }
    | OP_NOT unary_expression {
        $$ = create_node("UnaryOp", "!");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $2;
        $$->num_children = 1;
    }
    | OP_INCREMENT IDENTIFIER {
        $$ = create_node("PreIncrement", $2);
        free($2);
    }
    | OP_DECREMENT IDENTIFIER {
        $$ = create_node("PreDecrement", $2);
        free($2);
    }
    ;

postfix_expression:
    primary_expression { $$ = $1; }
    | IDENTIFIER OP_INCREMENT {
        $$ = create_node("PostIncrement", $1);
        free($1);
    }
    | IDENTIFIER OP_DECREMENT {
        $$ = create_node("PostDecrement", $1);
        free($1);
    }
    | IDENTIFIER DELIM_LPAREN argument_list DELIM_RPAREN {
        $$ = create_node("FunctionCall", $1);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $3;
        $$->num_children = 1;
        free($1);
    }
    | IDENTIFIER DELIM_LPAREN DELIM_RPAREN {
        $$ = create_node("FunctionCall", $1);
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = create_node("ArgumentList", "empty");
        $$->num_children = 1;
        free($1);
    }
    | KEYWORD_MANIFEST DELIM_LPAREN expression DELIM_RPAREN {
        $$ = create_node("ManifestCall", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $3;
        $$->num_children = 1;
    }
    ;

argument_list:
    expression {
        $$ = create_node("ArgumentList", "");
        $$->children = (ASTNode**)malloc(sizeof(ASTNode*));
        $$->children[0] = $1;
        $$->num_children = 1;
    }
    | argument_list DELIM_COMMA expression {
        $$ = $1;
        $$->children = (ASTNode**)realloc($$->children, ($$->num_children + 1) * sizeof(ASTNode*));
        $$->children[$$->num_children] = $3;
        $$->num_children++;
    }
    ;

primary_expression:
    IDENTIFIER {
        $$ = create_node("Identifier", $1);
        free($1);
    }
    | literal { $$ = $1; }
    | DELIM_LPAREN expression DELIM_RPAREN { $$ = $2; }
    ;

literal:
    INTEGER_LITERAL {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", $1);
        $$ = create_node("IntegerLiteral", buf);
    }
    | FLOAT_LITERAL {
        char buf[32];
        snprintf(buf, sizeof(buf), "%f", $1);
        $$ = create_node("FloatLiteral", buf);
    }
    | STRING_LITERAL {
        $$ = create_node("StringLiteral", $1);
        free($1);
    }
    | CHAR_LITERAL {
        $$ = create_node("CharLiteral", $1);
        free($1);
    }
    | KEYWORD_TRUE {
        $$ = create_node("BooleanLiteral", "true");
    }
    | KEYWORD_FALSE {
        $$ = create_node("BooleanLiteral", "false");
    }
    | KEYWORD_NULL {
        $$ = create_node("NullLiteral", "null");
    }
    ;

%%

/* C code section */

/* Error handling */
void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", line_number, s);
}

/* AST helper functions */
ASTNode* create_node(const char *type, const char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = strdup(type);
    node->value = strdup(value);
    node->children = NULL;
    node->num_children = 0;
    return node;
}

void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    
    /* Print indentation */
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    /* Print node */
    printf("%s", node->type);
    if (strlen(node->value) > 0) {
        printf(": %s", node->value);
    }
    printf("\n");
    
    /* Print children */
    for (int i = 0; i < node->num_children; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    
    free(node->type);
    free(node->value);
    
    for (int i = 0; i < node->num_children; i++) {
        free_ast(node->children[i]);
    }
    
    free(node->children);
    free(node);
}

/* Main function */
int main(int argc, char **argv) {
    printf("=== Arcane Parser ===\n");
    printf("Parsing input...\n\n");
    
    /* Parse the input */
    int result = yyparse();
    
    if (result == 0) {
        printf("=== Parsing Successful ===\n\n");
        printf("Abstract Syntax Tree:\n");
        print_ast(root, 0);
        free_ast(root);
        return 0;
    } else {
        printf("=== Parsing Failed ===\n");
        return 1;
    }
}
