/*----------------------------------------------------------------------------------------------
Name: Shihuan Shao	
ASUID: 1203060451 
Project4 CSE340
/*----------------------------------------------------------------------------------------------
Version 1.0
1. Parsing is done.
Version 1.3
1. Paring changed according prof's suggestions. Based on V1.0.
Version 1.4
1. Assign and Print are done.
Version 1.5
1. If is done.
Version 1.6
1. Add count > 0 before count--.
2. While is done.
----------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "syntax.h"

//----------------------------- token types ------------------------------
#define TRUE 1
#define FALSE 0
#define KEYWORDS 14
#define RESERVED 38
#define VAR 1
#define BEGIN 2
#define END 3
#define ASSIGN 4 
#define IF 5
#define WHILE 6 
#define DO 7
#define THEN 8
#define PRINT 9
#define INT 10
#define REAL 11
#define STRING 12
#define BOOLEAN 13
#define TYPE 14
#define PLUS 15
#define MINUS 16
#define DIV 17
#define MULT 18
#define EQUAL 19
#define COLON 20 
#define COMMA 21
#define SEMICOLON 22
#define LBRAC 23
#define RBRAC 24
#define LPAREN 25
#define RPAREN 26
#define NOTEQUAL 27
#define GREATER 28
#define LESS 29
#define LTEQ 30
#define GTEQ 31
#define DOT 32
#define ID 33
#define NUM 34
#define REALNUM 11//35
#define ERROR 36
#define LBRACE 37
#define RBRACE 38
#define NOOP 39

#define EXPR 1
#define PRIMARY 0

//------------------- reserved words and token strings -----------------------
char *reserved[] = 
	{	"",
		"VAR", 
		"BEGIN", 
		"END", 
		"ASSIGN", 
		"IF", 
		"WHILE", 
		"DO", 
		"THEN", 
		"print", 
		"INT",
		"REAL",
		"STRING",
		"BOOLEAN",
		"TYPE",
		"+",
		"-", 
		"/", 
		"*", 
		"=", 
		":", 
		",", 
		";", 
		"[", 
		"]", 
		"(", 
		")", 
		"<>", 
		">", 
		"<",
		"<=",
		">=",
		".",
		"ID",
		"NUM",
		"REALNUM",		
		"ERROR",
		"{",
		"}"
		};

int printToken(int ttype){
   if (ttype <= RESERVED){  
		printf("%s\n",reserved[ttype]);
		return 1;
   } 
   else
		return 0; 
}
//---------------------------------------------------------

//---------------------------------------------------------
// Global Variables associated with the next input token
#define MAX_TOKEN_LENGTH 100
#define VARNUM 1000
#define IFNUM 100
#define WHILENUM 100
#define STACKVOLUME 100

char token[MAX_TOKEN_LENGTH];      // token string
int  ttype;                        // token type
int  activeToken = FALSE;                  
int  tokenLength;
int line_no = 1;

struct stmtStack{
	int type;
	struct stmtNode* pointer;
}stack[STACKVOLUME];

struct primaryNode* varList[VARNUM];
int count = 0; 
int varCount = 0;

//----------------------------------------------------------
int skipSpace(){
   char c;

   c = getchar(); 
   line_no += (c == '\n');
   while (!feof(stdin) && isspace(c)){    
	   c = getchar(); 
        line_no += (c == '\n');
   }

   // return character to input buffer if eof is not reached
   if (!feof(stdin)) 
        ungetc(c,stdin);
}

int isKeyword(char *s){
     int i;

     for (i = 1; i <= KEYWORDS; i++)
	if (strcmp(reserved[i],s) == 0)
	   return i;
     return FALSE;
}

// ungetToken() simply sets a flag so that when getToken() is called
// the old ttype is returned and the old token is not overwritten 
// NOTE: BETWEEN ANY TWO SEPARATE CALLS TO ungetToken() THERE MUST BE
// AT LEAST ONE CALL TO getToken()
// CALLING TWO ungetToken() WILL NOT UNGET TWO TOKENS  
void ungetToken(){
    activeToken = TRUE;
}

int scan_number(){
	char c;
	
	c = getchar();
	if (isdigit(c)){	
		// First collect leading digits before dot
		// 0 is a nNUM by itself
		if (c == '0'){	
			token[tokenLength] = c;
			tokenLength++;
			token[tokenLength] = '\0';
		} 
		else{	
			while (isdigit(c)){	
				token[tokenLength] = c;
				tokenLength++;;
				c = getchar();
			}
			ungetc(c,stdin);
			token[tokenLength] = '\0';
		}

		// Check if leading digits are integer part of a REALNUM
		c = getchar();
		if (c == '.'){	
			c = getchar();
			if (isdigit(c)){	
				token[tokenLength] = '.';
				tokenLength++;
				while (isdigit(c)){	
					token[tokenLength] = c;
					tokenLength++;
					c = getchar();
				}
				token[tokenLength] = '\0';
				if (!feof(stdin)) 
					ungetc(c,stdin);
				return REALNUM;
			} 
			else{	
				ungetc(c, stdin);    // note that ungetc returns characters on a stack, so we first
				c = '.';             // return the second character and set c to '.' and return c again
				ungetc(c,stdin);				                                 
				return  NUM;         
            }
		} 
		else{	
			ungetc(c, stdin);
			return NUM;
		}
	}
	else
		return ERROR;   
}


int scan_id_or_keyword(){
	int ttype;
	char c;

	c = getchar();
	if (isalpha(c)){	
		while (isalnum(c)){	
			token[tokenLength] = c;
			tokenLength++;
			c = getchar();
		}
		if (!feof(stdin)) 
			ungetc(c,stdin); 
             
		token[tokenLength] = '\0';		                
		ttype = isKeyword(token); 
		if (ttype == 0) 
			ttype = ID;
		return ttype;
	} 
	else
		return ERROR;
}                            
   

int getToken(){	
	char c;
 
       if (activeToken){ 
			activeToken = FALSE;
			return ttype;
       }   // we do not need an else because the function returns in the body 
           // of the if
   
       skipSpace();   
       tokenLength = 0;
       c = getchar();
       switch (c){   
		   case '.': return DOT;
           case '+': return PLUS;
           case '-': return MINUS;
           case '/': return DIV;
           case '*': return MULT;
           case '=': return EQUAL;
           case ':': return COLON;
           case ',': return COMMA;
           case ';': return SEMICOLON;
           case '[': return LBRAC;
           case ']': return RBRAC;
           case '(': return LPAREN;
           case ')': return RPAREN;
           case '{': return LBRACE;
           case '}': return RBRACE;
           case '<':
               c = getchar();

               if (c == '>')
                   return NOTEQUAL;
               else{
                   ungetc(c,stdin);
                   return LESS;
               }
           case '>': return GREATER;
           default :
               if (isdigit(c)){	
				   ungetc(c,stdin);
               		return scan_number();
               }
               else if (isalpha(c)){	
					ungetc(c,stdin);
               		return scan_id_or_keyword();
               }
               else if (c == EOF)
               		return EOF;
               else
               		return ERROR;
	}
}

struct primaryNode* findVar(char* id){
	int i;
	struct primaryNode* p;
	for(i = 0; i < varCount; i++){
		if(strcmp(varList[i]->id, id) == 0){
			p = varList[i];
			return p;
		}
	}
}
/*----------------------------------------------------------------------------
SYNTAX ANALYSIS SECTION
----------------------------------------------------------------------------*/
#define PRIMARY 0
#define EXPR 1

/*-------------------------------UTILITIE---------------------------*/
void syntax_error(char* NT, int line_no){
	printf("Syntax error while parsing %s line %d\n", NT, line_no);
	system("pause");
}

/*--------------------------------------------------------------------
  PRINTING PARSE TREE
---------------------------------------------------------------------*/
void print_parse_tree(struct programNode* program){	
	print_var_section(program->var_section); 
	print_body(program->body);
}

void print_var_section(struct var_sectionNode* varSection){
	printf("VAR\n");
	if (varSection->var_decl != NULL){	
		print_var_decl(varSection->var_decl);
	}
}

void print_var_decl(struct var_declNode* varDecl){
	if (varDecl->id_list != NULL)
		print_id_list(varDecl->id_list);
}

void print_id_list(struct id_listNode* idList){
	printf("%s ", idList->id);
	if (idList->id_list != NULL){
		printf(", ");
		print_id_list(idList->id_list);	
	}
	else{
		printf(";\n");
	}
}

void print_body(struct bodyNode* body){
	printf("{\n");
	print_stmt_node(body->stmt_list);
	printf("}\n");	
}

void print_stmt_node(struct stmtNode* stmt){
	if (stmt->stmtType == ASSIGN)
		print_assign_stmt(stmt->assign_stmt);
	else if(stmt->stmtType == WHILE)
		print_while_stmt(stmt->while_stmt);
	else if(stmt->stmtType == PRINT)
		print_print_stmt(stmt->print_stmt);
	else if(stmt->stmtType == IF)
		print_if_stmt(stmt->if_stmt);
	
	if(stmt->next!= NULL)
		print_stmt_node(stmt->next);
}

void print_assign_stmt(struct assign_stmtNode* assign_stmt){
	printf("%s ", assign_stmt->lvalue->id);
	printf("= ");
	print_expression_prefix(assign_stmt->expr);
}

void print_expression_prefix(struct exprNode* expr){
	if (expr->tag == EXPR){
		printf("%s ", reserved[expr->op]);
		print_primary(expr->leftOperand);
		print_primary(expr->rightOperand);
		printf("; \n");
	} 
	else if (expr->tag == PRIMARY){
		print_primary(expr->leftOperand);
		printf("; \n");
	}
}

void print_primary(struct primaryNode* primary){
	if (primary->tag == ID)
		printf("%s ", primary->id);
	else if (primary->tag == NUM)
		printf("%d ", primary->ival);
}

void print_print_stmt(struct print_stmtNode* print_stmt){
	printf("print ");
	printf("%s ", print_stmt->var->id);
	printf("; \n");
}

void print_while_stmt(struct while_stmtNode* while_stmt){
	printf("WHILE ");
	print_condition(while_stmt->condition);
	printf("\n");
	print_body(while_stmt->body);
}

void print_if_stmt(struct if_stmtNode* if_stmt){
	printf("IF ");
	print_condition(if_stmt->condition);
	printf("\n");
	print_body(if_stmt->body);
}

void print_condition(struct conditionNode* condition){
	print_primary(condition->left_operand);
	if(condition->relop == 27)
		printf("<> ");
	else if(condition->relop == 28)
		printf("> ");
	else if(condition->relop == 29)
		printf("< ");	
	if(condition->relop >= 27 && condition->relop <= 29)
		print_primary(condition->right_operand);
}

/*--------------------------------------------------------------------
  CREATING PARSE TREE NODE
---------------------------------------------------------------------*/
struct programNode* make_programNode(){	
	return (struct programNode*) malloc(sizeof(struct programNode));
}

struct var_sectionNode* make_var_sectionNode(){
	return (struct var_sectionNode*) malloc(sizeof(struct var_sectionNode));
}

struct var_declNode* make_var_declNode(){
	return (struct var_declNode*) malloc(sizeof(struct var_declNode));
}

struct id_listNode* make_id_listNode(){
	return (struct id_listNode*) malloc(sizeof(struct id_listNode));
}

struct bodyNode* make_bodyNode(){
	return (struct bodyNode*) malloc(sizeof(struct bodyNode));
}

struct stmtNode* make_stmtNode(){
	return (struct stmtNode*) malloc(sizeof(struct stmtNode));
}

struct assign_stmtNode* make_assign_stmtNode(){
	return (struct assign_stmtNode*) malloc(sizeof(struct assign_stmtNode));
}

struct exprNode* make_exprNode(){
	return (struct exprNode*) malloc(sizeof(struct exprNode));
}

struct primaryNode* make_primaryNode(){
	return (struct primaryNode*) malloc(sizeof(struct primaryNode));
}

struct print_stmtNode* make_print_stmtNode(){
	return (struct print_stmtNode*) malloc(sizeof(struct print_stmtNode));
}

struct while_stmtNode* make_while_stmtNode(){
	return (struct while_stmtNode*) malloc(sizeof(struct while_stmtNode));
}

struct if_stmtNode* make_if_stmtNode(){
	return (struct if_stmtNode*) malloc(sizeof(struct if_stmtNode));
}

struct conditionNode* make_conditionNode(){
	return (struct conditionNode*) malloc(sizeof(struct conditionNode));
}

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  PARSING AND BUILDING PARSE TREE
---------------------------------------------------------------------*/
struct primaryNode* primary(){
	struct primaryNode* primar;
	ttype = getToken();
	if (ttype == ID){   
		primar = make_primaryNode();
		primar->id = (char *) malloc((tokenLength+1)*sizeof(char));
		strcpy(primar->id, token);
		primar->tag = ID;
	}
	else if (ttype == NUM){
		primar = make_primaryNode();
		primar->ival = atoi(token);
		primar->tag = NUM;
	}
	else{
		syntax_error("primary. NUM or ID expected", line_no);
		exit(0);
	}
	return primar;
}

struct conditionNode* condition(){
	//*****************added code start*****************
	struct conditionNode* conditio;
	ttype = getToken();
	if (ttype == ID || ttype == NUM){	
		conditio = make_conditionNode();
		
		if(ttype == ID)	
			conditio->left_operand = findVar(token);
		else{
			ungetToken();
			conditio->left_operand = primary();
		}
		ttype = getToken();
		if(ttype >= 27 && ttype <= 29 ){
			conditio->relop = ttype;
			ttype = getToken();
			if (ttype == ID || ttype == NUM){	
				if(ttype == ID)	
					conditio->right_operand = findVar(token);
				else{
					ungetToken();
					conditio->right_operand = primary();
				}
			}
			else{
				syntax_error("condition. right_operand expected", line_no);
				exit(0);
			}
		}
		else{
			syntax_error("condition. relop expected", line_no);
			exit(0);		
		}
	}
	else{
		syntax_error("condition. primary expected", line_no);
		exit(0);
	}	
	return conditio;
}

struct if_stmtNode* if_stmt(){
	struct if_stmtNode* i;
	
	ttype = getToken();
	if(ttype == IF){
		i =  make_if_stmtNode();
		ttype = getToken();

		if (ttype == ID || ttype == NUM){
			ungetToken();
			i->condition = condition();			
			ttype = getToken();
			if (ttype == LBRACE){
				ungetToken();
				i->body = body();
			}
			else{
				syntax_error("if_stmt. body. LBRACE expected", line_no);
				exit(0);
			}
		}
		else{
			syntax_error("if_stmt. ID or primary expected", line_no);
			exit(0);
		}
	} 
	else{
		syntax_error("while_stmt. WHILE expected", line_no);
		exit(0);
	}	
	return i;
}

struct print_stmtNode* print_stmt(){
	struct print_stmtNode* prin;
	ttype = getToken();
	if(ttype == PRINT){
		prin = make_print_stmtNode();
		ttype = getToken();
		if (ttype == ID){   
			prin->var = findVar(token);
		}
		else{
			syntax_error("print. ID expected", line_no);
			exit(0);
		}
	}
	return prin;
}

struct exprNode* expr()
{
	struct exprNode* exp;	
	ttype = getToken();
	if ((ttype == ID)|(ttype == NUM)){	
		exp = make_exprNode();
		if(ttype == ID)
			exp->leftOperand = findVar(token);
		else{
			ungetToken();
			exp->leftOperand = primary();
		}

		ttype = getToken();
		if ((ttype == PLUS) | (ttype == MINUS) | (ttype == MULT) | (ttype == DIV)){	
			exp->op = ttype;
			exp->tag = EXPR;
			ttype = getToken();
			if ((ttype == ID)|(ttype == NUM)){	
				if(ttype == ID)
					exp->rightOperand = findVar(token);
				else{
					ungetToken();
					exp->rightOperand = primary();
				}
			}
			else{	
				syntax_error("expr.RPAREN. ID or NUM expected", line_no);
				exit(0);
			}
		}
		else if (ttype == SEMICOLON){
			ungetToken();
			exp->op = NOOP;
			exp->tag = PRIMARY;
			exp->rightOperand = NULL;
		}
		else {	
			syntax_error("expr. PLUS, MINUS, MULT, DIV or SEMICOLON expected", line_no);
			exit(0);
		}
	} 
	else{	
		syntax_error("expr. LPAREN. ID or NUM expected", line_no);
		exit(0);
	}
	return exp;
}

struct assign_stmtNode* assign_stmt(){	
	struct assign_stmtNode* assignStmt;

	ttype = getToken();
	if (ttype == ID) {	
		assignStmt = make_assign_stmtNode();
		assignStmt->lvalue = findVar(token);
		ttype = getToken();
		if (ttype == EQUAL){	
			assignStmt->expr = expr();
			ttype = getToken();
			if (ttype == SEMICOLON){
				ungetToken();
				return assignStmt;
			}
			else{	
				syntax_error("stmt. SEMICOLON expected", line_no);
				exit(0);
			}			
		}
		else{	
			syntax_error("assign_stmt. EQUAL expected", line_no);
			exit(0);
		}
	} 
	else{
		syntax_error("assign_stmt. ID expected", line_no);
		exit(0);
	}
}

struct while_stmtNode* while_stmt(){
	//*****************added code start*****************
	struct while_stmtNode* whileStmt;
	ttype = getToken();
	if (ttype == WHILE){	
		whileStmt = make_while_stmtNode();
		ttype = getToken();
		if (ttype == ID || ttype == NUM){
			ungetToken();
			whileStmt->condition = condition();
			
			ttype = getToken();
			if (ttype == LBRACE){
				ungetToken();
				whileStmt->body = body();
			}
			else{
				syntax_error("while_stmt. body. LBRACE expected", line_no);
				exit(0);
			}
		}
		else{
			syntax_error("while_stmt. ID or primary expected", line_no);
			exit(0);
		}
	} 
	else{
		syntax_error("while_stmt. WHILE expected", line_no);
		exit(0);
	}	
	return whileStmt;
	//*****************added code end*****************
}

struct stmtNode* stmt(){
	struct stmtNode* stm;
	
	struct stmtNode* p;
	ttype = getToken();
	stm = make_stmtNode();
	if (ttype == ID){ // assign_stmt
		ungetToken();
		stm->assign_stmt = assign_stmt();
		stm->stmtType = ASSIGN;
		ttype = getToken();
		if (ttype == SEMICOLON){	
			ttype = getToken();
			if(ttype == ID || ttype == IF || ttype == WHILE || ttype == PRINT){
				ungetToken();
				stm->next = stmt();
			}
			else if(ttype == RBRACE){
				ungetToken();
				stm->next = NULL;
			}
			else{
				syntax_error("stmt. ID, IF, WHILE, PRINT or RBRACE expected", line_no);
			}
			return stm;
		}
		else{	
			syntax_error("stmt. SEMICOLON expected", line_no);
			exit(0);
		}
	} 
	else if (ttype == WHILE){ // while_stmt
		ungetToken();
		stm->while_stmt = while_stmt();
		stm->stmtType = WHILE;
		ttype = getToken();
		if(ttype == ID || ttype == IF || ttype == WHILE || ttype == PRINT){
			ungetToken();
			stm->next = stmt();
		}
		else if(ttype == RBRACE){
			ungetToken();
			stm->next = NULL;
		}
		else{
			syntax_error("stmt. ID, IF, WHILE, PRINT or RBRACE expected", line_no);
		}
		return stm;
	} 
	else if (ttype == PRINT){ // print_stmt
		ungetToken();
		stm->print_stmt = print_stmt();
		stm->stmtType = PRINT;
		ttype = getToken();
		if (ttype == SEMICOLON){	
			ttype = getToken();
			if(ttype == ID || ttype == IF || ttype == WHILE || ttype == PRINT){
				ungetToken();
				stm->next = stmt();
			}
			else if(ttype == RBRACE){
				ungetToken();
				stm->next = NULL;
			}
			else{
				syntax_error("stmt. ID, IF, WHILE, PRINT or RBRACE expected", line_no);
			}
			return stm;
		}
		else{	
			syntax_error("stmt. SEMICOLON expected", line_no);
			exit(0);
		}
	}
	else if (ttype == IF){ // if_stmt
		ungetToken();
		stm->if_stmt = if_stmt();
		stm->stmtType = IF;
		ttype = getToken();
		if(ttype == ID || ttype == IF || ttype == WHILE || ttype == PRINT){
			ungetToken();
			stm->next = stmt();
		}
		else if(ttype == RBRACE){
			ungetToken();
			stm->next = NULL;
		}
		else{
			syntax_error("stmt. ID, IF, WHILE, PRINT or RBRACE expected", line_no);
		}
		return stm;
	}
	else{ // syntax error	
		syntax_error("stmt. ID/WHILE/print/IF expected", line_no);
		exit(0);
	}	
}

struct bodyNode* body(){	
	struct bodyNode* bod;

	ttype = getToken();
	if (ttype == LBRACE){	
		bod = make_bodyNode();
		bod->stmt_list = stmt();
		ttype = getToken();
		if (ttype == RBRACE)
			return bod;
		else{	
			syntax_error("body. RBRACE expected", line_no);
			exit(0); 
		}
	} 
	else{	
		syntax_error("body. LBRACE expected", line_no);
		exit(0); 
	}
}

struct id_listNode* id_list(){
	struct id_listNode* idList;
	idList = make_id_listNode();
	ttype = getToken();
	if (ttype == ID){	
		idList->id = (char*) malloc(tokenLength+1);
		strcpy(idList->id, token);
		//---------------------------Added codes----------------------------
		strcpy(varList[varCount]->id, token);
		varList[varCount]->ival = 0;
		varList[varCount]->tag = ID;
		varCount++;
		//---------------------------Added codes end----------------------------
				
		ttype = getToken();
		if (ttype == COMMA){
			idList->id_list = id_list();
			return idList;			
		} 
		else if (ttype == SEMICOLON){	
			ungetToken();
			idList->id_list = NULL;
			return idList;
		}
		else{	
			syntax_error("id_list. COMMA or SEMICOLON expected", line_no);
			exit(0);
		}
	}
	else{	
		syntax_error("id_list. ID expected", line_no);
		exit(0);
	}
}

struct var_declNode* var_decl(){
	struct var_declNode* varDecl;
	varDecl = make_var_declNode();
	ttype = getToken();
	if (ttype == ID){	
		ungetToken();

		varDecl->id_list = id_list();
		ttype = getToken();
		if (ttype == SEMICOLON){	
			return varDecl;
		}
		else{	
			syntax_error("var_decl. SEMICOLON expected", line_no);
			exit(0);
		} 
	}
	return varDecl;
}	

struct var_sectionNode* var_section(){
	struct var_sectionNode* varDeclList;
	varDeclList = make_var_sectionNode();

	ttype = getToken();
	if (ttype == ID){	
		ungetToken();
		varDeclList->var_decl = var_decl();
		return varDeclList; 
	} 
	else{
		syntax_error("var_section. ID expected", line_no);
		exit(0);
	} 
}

struct programNode* program(){	
	struct programNode* prog;
	prog = make_programNode();
	ttype = getToken();
	if(ttype == VAR){	 
		prog->var_section = var_section();
		prog->body = body();
		return prog;
	} 
	else{	
		syntax_error("program. VAR expected", line_no);
		exit(0);		// stop after first syntax error
	}
}

void main(){
	int i, j;	
	struct programNode* parseTree;
	for(i=0; i < VARNUM; i++){
		varList[i] = (struct primaryNode*)malloc(sizeof(struct primaryNode));
		varList[i]->id = (char*)malloc(sizeof(char)*MAX_TOKEN_LENGTH);
	}
	parseTree = program();
	execute(parseTree->body->stmt_list);
//	print_parse_tree(parseTree);
/*	printf("\nSUCCESSFULLY PARSED INPUT!\n");
	system("pause");*/
}

void execute(struct stmtNode* stmt){
	struct stmtNode* stmtPointer = stmt;	
	int i = 0;
	for (i = 0; i < STACKVOLUME; i++){
		stack[i].pointer = NULL;
		stack[i].type = NOOP;
	}
	while (stmtPointer != NULL || stack[0].pointer != NULL){
		//------------------------------------------------------------------------Assign------------------------------------------------------------------------
		if(stmtPointer->stmtType == ASSIGN){
			if(stmtPointer->assign_stmt->expr->op == PLUS){
				stmtPointer->assign_stmt->lvalue->ival = stmtPointer->assign_stmt->expr->leftOperand->ival + stmtPointer->assign_stmt->expr->rightOperand->ival; 
			}
			else if(stmtPointer->assign_stmt->expr->op == MINUS){
				stmtPointer->assign_stmt->lvalue->ival = stmtPointer->assign_stmt->expr->leftOperand->ival - stmtPointer->assign_stmt->expr->rightOperand->ival; 
			}
			else if(stmtPointer->assign_stmt->expr->op == MULT){
				stmtPointer->assign_stmt->lvalue->ival = stmtPointer->assign_stmt->expr->leftOperand->ival * stmtPointer->assign_stmt->expr->rightOperand->ival; 
			}
			else if(stmtPointer->assign_stmt->expr->op == DIV){
				stmtPointer->assign_stmt->lvalue->ival = stmtPointer->assign_stmt->expr->leftOperand->ival / stmtPointer->assign_stmt->expr->rightOperand->ival; 
			}
			else if(stmtPointer->assign_stmt->expr->op == NOOP){
				stmtPointer->assign_stmt->lvalue->ival = stmtPointer->assign_stmt->expr->leftOperand->ival; 
			}
			//Move the pointer to the next stmtNode
			if(stmtPointer->next != NULL)
				stmtPointer = stmtPointer->next;
			else{
				if(stack[0].pointer == NULL){
					stmtPointer = stmtPointer->next;
				}
				else{
					if(stack[count].pointer == NULL && count > 0)
						count--;	
					if(stack[count].type == WHILE){
						stmtPointer = stack[count].pointer;
						stack[count].pointer = NULL;
						if(count > 0)
							count--;
					}
					else if(stack[count].type == IF){
						while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
							if(stack[count].type == IF)
								stmtPointer = stack[count].pointer->next;
							else if(stack[count].type == WHILE)
								stmtPointer = stack[count].pointer;
							stack[count].pointer = NULL;
							if(count > 0)
								count--;
							if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
								break;											  //then in next loop stack[count]->next will be NULL->next
						}
						if(stack[0].pointer != NULL){
							if(stack[count].type == IF)
								stmtPointer = stack[count].pointer->next;
							else if(stack[count].type == WHILE)
								stmtPointer = stack[count].pointer;
							stack[count].pointer = NULL;
							count--;
						}
					}
				}
			}
		}
		//------------------------------------------------------------------------Print------------------------------------------------------------------------
		else if(stmtPointer->stmtType == PRINT){
			printf("%d\n", stmtPointer->print_stmt->var->ival);
			if(stmtPointer->next != NULL)
				stmtPointer = stmtPointer->next;
			else{
				if(stack[0].pointer == NULL){
					stmtPointer = stmtPointer->next;
				}
				else{
					if(stack[count].pointer == NULL && count > 0)
						count--;			
					if(stack[count].type == WHILE){
						stmtPointer = stack[count].pointer;
						stack[count].pointer = NULL;
						if(count > 0)
							count--;
					}
					else if(stack[count].type == IF){
						while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
							if(stack[count].type == IF)
								stmtPointer = stack[count].pointer->next;
							else if(stack[count].type == WHILE)
								stmtPointer = stack[count].pointer;
							stack[count].pointer = NULL;
							if(count > 0)
								count--;
							if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
								break;											  //then in next loop stack[count]->next will be NULL->next
						}
						if(stack[0].pointer != NULL){
							if(stack[count].type == IF)
								stmtPointer = stack[count].pointer->next;
							else if(stack[count].type == WHILE)
								stmtPointer = stack[count].pointer;
							stack[count].pointer = NULL;
							count--;
						}
					}
				}
			}
		}
		//------------------------------------------------------------------------If------------------------------------------------------------------------
		else if(stmtPointer->stmtType == IF){
			if(stmtPointer->if_stmt->condition->relop == GREATER){
				if(stmtPointer->if_stmt->condition->left_operand->ival > stmtPointer->if_stmt->condition->right_operand->ival){
					if(count < 0)
						count = 0;
					stack[count].pointer = stmtPointer;
					stack[count].type = IF;
					count++;
					stmtPointer = stmtPointer->if_stmt->body->stmt_list;
				}
				else{
					if(stmtPointer->next != NULL)
						stmtPointer = stmtPointer->next;
					else{
						if(stack[0].pointer == NULL){
							stmtPointer = stmtPointer->next;
						}
						else{
							if(stack[count].pointer == NULL && count > 0)
								count--;			
							if(stack[count].type == WHILE){
								stmtPointer = stack[count].pointer;
								stack[count].pointer = NULL;
								if(count > 0)
									count--;
							}
							else if(stack[count].type == IF){
								while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									if(count > 0)
										count--;
									if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
										break;											  //then in next loop stack[count]->next will be NULL->next
								}
								if(stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									count--;
								}
							}
						}
					}
				}
			}
			else if(stmtPointer->if_stmt->condition->relop == LESS){
				if(stmtPointer->if_stmt->condition->left_operand->ival < stmtPointer->if_stmt->condition->right_operand->ival){
					if(count < 0)
						count = 0;
					stack[count].pointer = stmtPointer;
					stack[count].type = IF;
					count++;
					stmtPointer = stmtPointer->if_stmt->body->stmt_list;
				}
				else{
					if(stmtPointer->next != NULL)
						stmtPointer = stmtPointer->next;
					else{
						if(stack[0].pointer == NULL){
							stmtPointer = stmtPointer->next;
						}
						else{
							if(stack[count].pointer == NULL && count > 0)
								count--;			
							if(stack[count].type == WHILE){
								stmtPointer = stack[count].pointer;
								stack[count].pointer = NULL;
								if(count > 0)
									count--;
							}
							else if(stack[count].type == IF){
								while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									if(count > 0)
										count--;
									if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
										break;											  //then in next loop stack[count]->next will be NULL->next
								}
								if(stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									count--;
								}
							}
						}
					}
				}
			}
			else if(stmtPointer->if_stmt->condition->relop == NOTEQUAL){
				if(stmtPointer->if_stmt->condition->left_operand->ival != stmtPointer->if_stmt->condition->right_operand->ival){
					if(count < 0)
						count = 0;
					stack[count].pointer = stmtPointer;
					stack[count].type = IF;
					count++;
					stmtPointer = stmtPointer->if_stmt->body->stmt_list;
				}
				else{
					if(stmtPointer->next != NULL)
						stmtPointer = stmtPointer->next;
					else{
						if(stack[0].pointer == NULL){
							stmtPointer = stmtPointer->next;
						}
						else{
							if(stack[count].pointer == NULL && count > 0)
								count--;			
							if(stack[count].type == WHILE){
								stmtPointer = stack[count].pointer;
								stack[count].pointer = NULL;
								if(count > 0)
									count--;
							}
							else if(stack[count].type == IF){
								while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									if(count > 0)
										count--;
									if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
										break;											  //then in next loop stack[count]->next will be NULL->next
								}
								if(stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									count--;
								}
							}
						}
					}
				}
			}
		}
		//------------------------------------------------------------------------While------------------------------------------------------------------------
		else if(stmtPointer->stmtType == WHILE){
			if(stmtPointer->while_stmt->condition->relop == GREATER){
				if(stmtPointer->while_stmt->condition->left_operand->ival > stmtPointer->while_stmt->condition->right_operand->ival){
					if(count < 0)
						count = 0;
					if(stack[count].pointer != NULL)
						count++;
					stack[count].pointer = stmtPointer;
					stack[count].type = WHILE;
					count++;
					stmtPointer = stmtPointer->while_stmt->body->stmt_list;
				}
				else{
					if(stmtPointer->next != NULL)
						stmtPointer = stmtPointer->next;
					else{
						if(stack[0].pointer == NULL){
							stmtPointer = stmtPointer->next;
						}
						else{
							if(stack[count].pointer == NULL && count > 0)
								count--;			
							if(stack[count].type == WHILE){
								stmtPointer = stack[count].pointer;
								stack[count].pointer = NULL;
								if(count > 0)
									count--;
							}
							else if(stack[count].type == IF){
								while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									if(count > 0)
										count--;
									if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
										break;											  //then in next loop stack[count]->next will be NULL->next
								}
								if(stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									count--;
								}
							}
						}
					}
				}
			}			
			else if(stmtPointer->while_stmt->condition->relop == LESS){
				if(stmtPointer->while_stmt->condition->left_operand->ival < stmtPointer->while_stmt->condition->right_operand->ival){
					if(count < 0)
						count = 0;
					stack[count].pointer = stmtPointer;
					stack[count].type = WHILE;
					count++;
					stmtPointer = stmtPointer->while_stmt->body->stmt_list;
				}
				else{
					if(stmtPointer->next != NULL)
						stmtPointer = stmtPointer->next;
					else{
						if(stack[0].pointer == NULL){
							stmtPointer = stmtPointer->next;
						}
						else{
							if(stack[count].pointer == NULL && count > 0)
								count--;			
							if(stack[count].type == WHILE){
								stmtPointer = stack[count].pointer;
								stack[count].pointer = NULL;
								if(count > 0)
									count--;
							}
							else if(stack[count].type == IF){
								while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									if(count > 0)
										count--;
									if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
										break;											  //then in next loop stack[count]->next will be NULL->next
								}
								if(stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									count--;
								}
							}
						}
					}
				}
			}
			else if(stmtPointer->while_stmt->condition->relop == NOTEQUAL){
				if(stmtPointer->while_stmt->condition->left_operand->ival != stmtPointer->while_stmt->condition->right_operand->ival){
					if(count < 0)
						count = 0;
					stack[count].pointer = stmtPointer;
					stack[count].type = WHILE;
					count++;
					stmtPointer = stmtPointer->while_stmt->body->stmt_list;
				}
				else{
					if(stmtPointer->next != NULL)
						stmtPointer = stmtPointer->next;
					else{
						if(stack[0].pointer == NULL){
							stmtPointer = stmtPointer->next;
						}
						else{
							if(stack[count].pointer == NULL && count > 0)
								count--;			
							if(stack[count].type == WHILE){
								stmtPointer = stack[count].pointer;
								stack[count].pointer = NULL;
								if(count > 0)
									count--;
							}
							else if(stack[count].type == IF){
								while(stack[count].pointer->next == NULL && stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									if(count > 0)
										count--;
									if(count <= 0 && stack[count].pointer == NULL) //if no stack[count] == 0, than it is a bug when count = 0 and stack[count] = NULL
										break;											  //then in next loop stack[count]->next will be NULL->next
								}
								if(stack[0].pointer != NULL){
									if(stack[count].type == IF)
										stmtPointer = stack[count].pointer->next;
									else if(stack[count].type == WHILE)
										stmtPointer = stack[count].pointer;
									stack[count].pointer = NULL;
									count--;
								}
							}
						}
					}
				}
			}
		}
/*		printf("count = %d\n", count);
		if(stmtPointer == NULL)
			printf("NULL\n");
		else{
			printf("Not Null\n");
		}*/
//		system("pause");
	}
}