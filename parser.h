/*
GROUP: 17
Priyanshu Bajpai 2013A7PS089P
Jayati Aggarwal  2013A7PS165P
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "parserdef.h"
#include "lexer.h"

/*************HASHTABLE-END***************/
hashtable_t *createHashtable( int size );
int hash( hashtable_t *hashtable, char *key );
entry_t *Newpair( char *key, int value);
void hash_set( hashtable_t *hashtable, char *key, int value );
int hash_get( hashtable_t *hashtable, char *key );

/******************STACK***********************/
void initializeStack(struct stack *st);
void push(struct stack *st, parseTree* num);
parseTree* pop(struct stack *st);

rule* reverseList(rule*head);
void printFirst();
void printFollow();
int printTree(parseTree*root, char*parent, int **mem, int option);

parseTree* createAST(parseTree* root);

void initializeHashTable(); //keys->strings containing non terminals or terminals, values->corresponding Enum values.
rule* initAtom(int id); //returns pointer to the rule with head pointing to the LHS of the rule.
char*getToken(int id); //returns the string having the terminal when TOKENID is provided.
char*getsymbol(int id); //returns the string having the non-terminal when TOKENID is provided.
rule* getRule(char*line); //return rule pointer when a line containing grammar rule is given

grammar* getGrammar(FILE* fp); //populate the grammar into the grammar structure rule by rule
ull getFirst(rule* cur);
ull getFirst2(rule* cur,grammar*gr);
void generateFirstSetUtil(grammar*gr, int id);
void generateFirstSet(grammar* gr);
void generateFollowSetUtil(grammar *gr, int id);
void generateFollowSet(grammar* gr);

void createParseTable(grammar*gr);
parseTree* initParseTree(int id,ull line,char* str);
parseTree* createParseTree(FILE* fp, grammar*gr);
void printExpected(tokenInfo*cur,int row);
