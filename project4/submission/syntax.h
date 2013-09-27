/*-----------------------------------------------------------
Version 1.0
Version 1.1
1. No stmtlistNode. T/F branches in conditionNode.
2. assignnode: char* id -> struct primaryNode* lvalue;
------------------------------------------------------------*/
/*-----------------------------------------------------------
  PARSE TREE TYPES
------------------------------------------------------------*/
struct programNode{
	struct var_sectionNode* var_section;
    struct bodyNode* body;
};

struct var_sectionNode{
	struct var_declNode* var_decl; 
};

struct var_declNode{
	struct id_listNode* id_list;
};

struct id_listNode{
	char* id;
	struct id_listNode* id_list;
};

struct bodyNode{
	struct stmtNode* stmt_list;
};

struct stmtNode{
	int stmtType;		//ASSIGN, WHILE, PRINT, IF, NOOP or GOTO   
	struct assign_stmtNode* assign_stmt;
	struct print_stmtNode* print_stmt;
	struct while_stmtNode* while_stmt;			
	struct if_stmtNode* if_stmt;
	struct stmtNode* next;
};

struct assign_stmtNode{
	struct primaryNode* lvalue;
	struct exprNode* expr;
};

struct exprNode{
	int tag; //EXPR or PRIMARY
	int op;	// PLUS , MINUS, MULT, DIV or NOOP
	struct primaryNode* leftOperand;
	struct primaryNode* rightOperand;
};

struct primaryNode{
	int tag; 	// NUM or ID
	int ival;
	char *id;
};

struct print_stmtNode{
	struct primaryNode* var;
};

struct while_stmtNode{
	struct conditionNode* condition;
	struct bodyNode* body;
};

struct if_stmtNode{
	struct conditionNode* condition;
	struct bodyNode* body;
};

struct conditionNode{
	int relop;	//GREATER, LESS or NOTEQUAL
	struct primaryNode* left_operand;
	struct primaryNode* right_operand;
};

/*------------------------------------------------------------------------
  PARSE TREE FUNCTIONS
--------------------------------------------------------------------------*/
struct programNode* make_programNode();
struct var_sectionNode* make_var_sectionNode();
struct var_declNode* make_var_declNode();
struct id_listNode* make_id_listNode();
struct bodyNode* make_bodyNode();
struct stmtNode* make_stmtNode();
struct assign_stmtNode* make_assign_stmtNode();
struct exprNode* make_exprNode();
struct primaryNode* make_primaryNode();
struct print_stmtNode* make_print_stmtNode();
struct while_stmtNode* make_while_stmtNode();
struct if_stmtNode* make_if_stmtNode();
struct conditionNode* make_conditionNode();

void print_parse_tree(struct programNode* program);
void print_var_section(struct var_sectionNode* varSection);
void print_var_decl(struct var_declNode* varDecl);
void print_id_list(struct id_listNode* idList);

void print_body(struct bodyNode* body);
void print_stmt_node(struct stmtNode* stmt);
void print_assign_stmt(struct assign_stmtNode* assign_stmt);
void print_expression_prefix(struct exprNode* expr);
void print_primary(struct primaryNode* primary);
void print_print_stmt(struct print_stmtNode* print_stmt);
void print_while_stmt(struct while_stmtNode* while_stmt);
void print_if_stmt(struct if_stmtNode* if_stmt);
void print_condition(struct conditionNode* condition);

/*------------------------------------------------------------------------
  PARSING FUNCTIONS
--------------------------------------------------------------------------*/
struct programNode* program();
struct var_sectionNode* var_section();
struct var_declNode* var_decl();
struct id_listNode* id_list();
struct bodyNode* body();
struct stmtNode* stmt();
struct assign_stmtNode* assign_stmt();
struct exprNode* expr();
struct primaryNode* primary();
struct print_stmtNode* print_stmt();
struct while_stmtNode* while_stmt();
struct if_stmtNode* if_stmt();
struct conditionNode* condition();

/*------------------------------------------------------------------------
  OTHER FUNCTIONS
--------------------------------------------------------------------------*/
int printToken(int ttype);
int skipSpace();
int isKeyword(char *s);
void ungetToken();
int scan_number();
int scan_id_or_keyword();
int getToken();
void syntax_error(char* NT, int line_no);
struct primaryNode* findVar(char* id);
void execute(struct stmtNode* stmt);
void linkNoop(struct bodyNode*, struct stmtNode*);
void main();