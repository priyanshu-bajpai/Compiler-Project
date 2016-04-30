/*
GROUP: 17
Priyanshu Bajpai 2013A7PS089P
Jayati Aggarwal  2013A7PS165P
*/


# define NT_START 20000
# define NUMBER_OF_RULES 83
# define NUMBER_OF_TERMINALS 57
# define NO_OF_NONTERMINALS 47
# define NO_OF_TERMINALS 57
# define ull unsigned long long
#include<stdlib.h>
#include<stdio.h>
#include"symbolTable.h"
typedef enum
{
	program = NT_START,
  mainFunction,
	otherFunctions,
	function,
	input_par,
	output_par,
	parameter_list,
	dataType,
	primitiveDatatype,
	constructedDatatype,
	remaining_list,
	stmts,
	typeDefinitions,
	typeDefinition,
	fieldDefinitions,
	fieldDefinition,
	moreFields,
	declarations,
	declaration,
	global_or_not,
	otherStmts,
	stmt,
	assignmentStmt,
	singleOrRecId,
  C,
	funCallStmt,
	outputParameters,
	inputParameters,
	iterativeStmt,
	conditionalStmt,
	B,
	ioStmt,
	arithmeticExpression,
	ex2,
	term,
	term2,
	factor,
	op1,
	op2,
	booleanExpression,
  allVar,
	logicalOp,
	relationalOp,
	returnStmt,
	optionalReturn,
	idList,
	more_ids,
}Ids;

/**************HASHTABLE****************/
struct entry_s {
	char *key;
	int value;
	struct entry_s *next;
};

struct hashtable_s {
	int size;
	struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;
typedef struct entry_s entry_t;

/*************HASHTABLE-END***************/



/******************Parse tree node contains children and other details***********************/
struct parseTree
{
	int noChild;
	int ruleno;
	struct parseTree **children;
	char*str;
	ull lineNo;
	int id;
	struct parseTree* node,*inh,*syn;
	identifier* stEntry;
	//for creating AST reallocate the children to only useful nodes.
	//add link to address of corresponding symbol table.

};
typedef struct parseTree parseTree;



/******************STACK***********************/
struct stack {
   parseTree** arr;
   int top;
};


/******************RULE***********************/
struct node{
  struct node* next;
  int id;
};
typedef struct node rule;


/******************GRAMMAR***********************/
struct grammar{
    rule *gr[NUMBER_OF_RULES];
};
typedef struct grammar grammar;


/******************Data structure used for automating the first and followsets ***********************/

struct ruleNum
{
  int num;
  struct ruleNum* next;
};
typedef struct ruleNum ruleNum;


struct ruleNum *reference[NO_OF_NONTERMINALS], *reference2[NO_OF_NONTERMINALS];
int parseTable[NO_OF_NONTERMINALS][NO_OF_TERMINALS];
hashtable_t *hashtable;
ull* firstset;
ull* followset;
int rulelen;
ull doing;
int size;
