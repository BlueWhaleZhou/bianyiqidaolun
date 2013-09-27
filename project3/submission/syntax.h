/*---------------------------------Student Information------------------------------------
Name: Shihuan Shao
ASU ID: 1203060451
/*-----------------------------------------------------------
  PARSE TREE TYPES
------------------------------------------------------------*/
struct programNode {
       struct declNode* decl;
       struct bodyNode* body;
};

struct declNode {
	// A NULL field means that the section is empty
	struct type_decl_sectionNode* type_decl_section;   
	struct var_decl_sectionNode* var_decl_section;      
};

struct type_decl_sectionNode {
	struct type_decl_listNode* type_decl_list;          
};

struct var_decl_sectionNode {
	struct var_decl_listNode* var_decl_list;          
};

struct  type_decl_listNode {
	struct type_declNode * type_decl;
        struct type_decl_listNode* type_decl_list;
};

struct  var_decl_listNode {
	struct var_declNode * var_decl;
        struct var_decl_listNode* var_decl_list;
};

struct type_declNode {
	struct id_listNode* id_list;
	struct type_nameNode* type_name;
};

struct var_declNode {
	struct id_listNode* id_list;
	struct type_nameNode* type_name;
};

struct type_nameNode {
	int type;     // INT, REAL, STRING, BOOLEAN, ID 
	char* id;     // actual string when type is ID
};

struct id_listNode {
	char * id;
	struct id_listNode* id_list;
};

struct bodyNode {
	struct stmt_listNode* stmt_list;
};

struct stmt_listNode {
	struct stmtNode* stmt;
	struct stmt_listNode * stmt_list;
};

struct stmtNode {
	int stmtType;           // WHILE or ASSIGN
	union {
		struct while_stmtNode* while_stmt;
		struct assign_stmtNode* assign_stmt;
	};
};
        
struct conditionNode {
	int relop;
	struct primaryNode* left_operand;
	struct primaryNode* right_operand;
};

struct while_stmtNode {
	struct conditionNode* condition;
	struct bodyNode* body;
};

struct assign_stmtNode {
	char* id;
	struct exprNode* expr;
};

struct exprNode {	
	int operator;	// PLUS , MINUS, MULT, ... 
                        // or NO-OP
        int tag;        // PRIMARY or EXPR
	struct primaryNode* primary;
	struct exprNode * leftOperand;
	struct exprNode * rightOperand;
};

struct primaryNode {
	int tag; 	// NUM, REALNUM or ID
	int ival;
	float fval;
	char *id;
};


/*------------------------------------------------------------------------
  PARSE TREE FUNCTIONS
--------------------------------------------------------------------------*/
struct programNode*				make_programNode();
struct declNode*						make_declNode();
struct type_decl_sectionNode* 	make_type_decl_sectionNode();
struct var_decl_sectionNode* 	make_var_decl_sectionNode();
struct type_decl_listNode*		make_type_decl_listNode();
struct type_declNode* 			make_type_declNode();
struct var_decl_listNode* 		make_var_decl_listNode();
struct var_declNode* 			make_var_declNode();
struct id_listNode* 			make_id_listNode();
struct type_nameNode* 			make_type_nameNode();
struct bodyNode* 				make_bodyNode();
struct stmt_listNode* 			make_stmt_listNode();
struct stmtNode* 				make_stmtNode();
struct while_stmtNode* 			make_while_stmtNode();
struct conditionNode* 			make_conditionNode();
struct assign_stmtNode* 		make_assign_stmtNode();
struct type_nameNode* 			make_type_nameNode();
struct exprNode* 				make_exprNode();
struct primaryNode* 			make_primaryNode();


void print_decl(struct declNode* dec);
void print_body(struct bodyNode* body);

void print_type_decl_section(struct type_decl_sectionNode* typeDeclSection);
void print_type_decl_list(struct type_decl_listNode* typeDeclList);
void print_type_decl(struct type_declNode* typeDecl);
void print_type_name(struct type_nameNode* typeName);

void print_var_decl_section(struct var_decl_sectionNode* varDeclSection);
void print_var_decl_list(struct var_decl_listNode* varDeclList);
void print_var_decl(struct var_declNode* varDecl);

void print_id_list(struct id_listNode* idList);
void print_id_list(struct id_listNode* idList);

void print_stmt_list(struct stmt_listNode* stmt_list);
void print_stmt(struct stmtNode* stmt);
void print_while_stmt(struct while_stmtNode* while_stmt);
void print_condition(struct conditionNode* condition);
void print_primary(struct primaryNode* primary);
void print_assign_stmt(struct assign_stmtNode* assign_stmt);
void print_expression_prefix(struct exprNode* expr);

/*------------------------------------------------------------------------
  PARSING FUNCTIONS
--------------------------------------------------------------------------*/
struct programNode* program();
struct declNode* decl();
struct type_decl_sectionNode* type_decl_section();
struct var_decl_sectionNode* var_decl_section();
struct type_decl_listNode* type_decl_list();
struct var_decl_listNode* var_decl_list();
struct var_declNode* var_decl();
struct type_declNode* type_decl();
struct id_listNode* id_list();
struct type_nameNode* type_name();
struct bodyNode* body();
struct stmt_listNode* stmt_list();
struct stmtNode* stmt();
struct while_stmtNode* while_stmt();
struct assign_stmtNode* assign_stmt();
struct exprNode* expr();
struct exprNode*term();
struct exprNode*factor();
struct conditionNode* condition();
struct primaryNode* primary();
