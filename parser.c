/*
GROUP: 17
Priyanshu Bajpai 2013A7PS089P
Jayati Aggarwal  2013A7PS165P
*/


/*
This file contains 2 main functions:

1.parseTree* createParseTree(FILE* fp, grammar*gr):
  This function tries to parse input and returns NULL in case of compilation error.
  In case top of stack symbol doesnt match with the next token, Error recovery used is panic mode and
  suitable errors messages are printed on console.
  HEURISTICS:
  FIRST SET and FOLLOW SET of a NT are considered in its synchronising set.
  1.If the next token in the stream is in the synchronising set of th NT then it is replaced with the RHS of rule.
  2.If the NT goes to epsilon in any of its rules then it is simply popped out of stack.
  3.If the next token in the stream is not in the synchronising set of th NT then print the expected terminals and get next token.
  4.If terminal on top of stack doesnt match with the next token, terminal on stack is popped.

2.void createParseTable(grammar*gr) :
  creates a table with Non terminals in rows and terminals as columns, a particular entry corresponds to the rule number,
  If any, which could generate the temrinal from this non temrinal.
*/

#include "parser.h"

char *Nontermsymbol[] = {"program", "mainFunction", "otherFunctions", "function", "input_par", "output_par", "parameter_list", "dataType", "primitiveDatatype", "constructedDatatype", "remaining_list", "stmts", "typeDefinitions", "typeDefinition", "fieldDefinitions", "fieldDefinition", "moreFields", "declarations", "declaration", "global_or_not", "otherStmts", "stmt", "assignmentStmt", "singleOrRecId", "C1", "funCallStmt"
, "outputParameters", "inputParameters", "iterativeStmt", "conditionalStmt", "B1", "ioStmt", "arithmeticExpression", "ex2", "term", "term2", "factor", "op1", "op2", "booleanExpression", "allVar", "logicalOp", "relationalOp", "returnStmt", "optionalReturn", "idList", "more_ids"};

char *TermSymbol[] = {"TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RECORDID", "TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_INT", "TK_REAL", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT", "TK_OUTPUT", "TK_SEM",
"TK_COLON", "TK_DOT", "TK_COMMA", "TK_ENDWHILE", "TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND", "TK_OR", "TK_NOT", "TK_LT", "TK_LE",
 "TK_EQ", "TK_GT", "TK_GE", "TK_NE", "TK_EOF", "TK_ERROR", "$1", "eps"};

/*********************************////HASHTABLE////*********************************/
hashtable_t *createHashtable( int size ) {
	hashtable_t *hashtable = NULL;
	int i;
	if( size < 1 ) return NULL;

	if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}
	if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ){
		hashtable->table[i] = NULL;
	}
	hashtable->size = size;

	return hashtable;
}

int hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval;
	int i = 0;

	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hashtable->size;
}

entry_t *Newpair( char *key, int value) {
	entry_t *newpair;

	if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
		return NULL;
	}
	if( ( newpair->key = strdup( key ) ) == NULL ) {
		return NULL;
	}
	newpair->value = value;
	newpair->next = NULL;

	return newpair;
}

void hash_set( hashtable_t *hashtable, char *key, int value ) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	bin = hash( hashtable, key );
	next = hashtable->table[ bin ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

		next->value = value;

	} else {
		newpair = Newpair( key, value );

		if( next == hashtable->table[ bin ] ) {
			newpair->next = next;
			hashtable->table[ bin ] = newpair;

		} else if ( next == NULL ) {
			last->next = newpair;

		} else  {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

int hash_get( hashtable_t *hashtable, char *key ) {
	int bin = 0;
	entry_t *pair;

	bin = hash( hashtable, key );

	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		return -1;

	}
  else { return pair->value; }

}
/*********************************////HASHTABLE END////*********************************/

void initializeHashTable()
{
hashtable = createHashtable(1024);

//Inserting non terminals into the hashtable
	hash_set( hashtable, "program", 20000);
	hash_set( hashtable, "mainFunction", 20001);
	hash_set( hashtable, "otherFunctions", 20002);
	hash_set( hashtable, "function", 20003);
	hash_set( hashtable, "input_par", 20004);
	hash_set( hashtable, "output_par", 20005);
	hash_set( hashtable, "parameter_list", 20006);
	hash_set( hashtable, "dataType", 20007);
	hash_set( hashtable, "primitiveDatatype", 20008);
	hash_set( hashtable, "constructedDatatype", 20009);
	hash_set( hashtable, "remaining_list", 20010);
	hash_set( hashtable, "stmts", 20011);
	hash_set( hashtable, "typeDefinitions", 20012);
	hash_set( hashtable, "typeDefinition", 20013);
	hash_set( hashtable, "fieldDefinitions", 20014);
	hash_set( hashtable, "fieldDefinition", 20015);
	hash_set( hashtable, "moreFields", 20016);
	hash_set( hashtable, "declarations", 20017);
	hash_set( hashtable, "declaration", 20018);
	hash_set( hashtable, "global_or_not", 20019);
	hash_set( hashtable, "otherStmts", 20020);
	hash_set( hashtable, "stmt", 20021);
	hash_set( hashtable, "assignmentStmt", 20022);
	hash_set( hashtable, "singleOrRecId", 20023);
	hash_set( hashtable, "C1", 20024);
	hash_set( hashtable, "funCallStmt", 20025);
	hash_set( hashtable, "outputParameters", 20026);
	hash_set( hashtable, "inputParameters", 20027);
	hash_set( hashtable, "iterativeStmt", 20028);
	hash_set( hashtable, "conditionalStmt", 20029);
	hash_set( hashtable, "B1", 20030);
	hash_set( hashtable, "ioStmt", 20031);
	hash_set( hashtable, "arithmeticExpression", 20032);
	hash_set( hashtable, "ex2", 20033);
	hash_set( hashtable, "term", 20034);
	hash_set( hashtable, "term2", 20035);
	hash_set( hashtable, "factor", 20036);
	hash_set( hashtable, "op1", 20037);
	hash_set( hashtable, "op2", 20038);
	hash_set( hashtable, "booleanExpression", 20039);
	hash_set( hashtable, "allVar", 20040);
	hash_set( hashtable, "logicalOp", 20041);
	hash_set( hashtable, "relationalOp", 20042);
	hash_set( hashtable, "returnStmt", 20043);
	hash_set( hashtable, "optionalReturn", 20044);
	hash_set( hashtable, "idList", 20045);
	hash_set( hashtable, "more_ids", 20046);

//Inserting token into the hashtable
	hash_set( hashtable, "TK_ASSIGNOP", 0);
	hash_set( hashtable, "TK_COMMENT", 1);
	hash_set( hashtable, "TK_FIELDID", 2);
	hash_set( hashtable, "TK_ID", 3);
	hash_set( hashtable, "TK_NUM", 4);
	hash_set( hashtable, "TK_RNUM", 5);
	hash_set( hashtable, "TK_FUNID", 6);
	hash_set( hashtable, "TK_RECORDID", 7);
	hash_set( hashtable, "TK_WITH", 8);
	hash_set( hashtable, "TK_PARAMETERS", 9);
	hash_set( hashtable, "TK_END", 10);
	hash_set( hashtable, "TK_WHILE", 11);
	hash_set( hashtable, "TK_INT", 12);
	hash_set( hashtable, "TK_REAL", 13);
	hash_set( hashtable, "TK_TYPE", 14);
	hash_set( hashtable, "TK_MAIN", 15);
	hash_set( hashtable, "TK_GLOBAL", 16);
	hash_set( hashtable, "TK_PARAMETER", 17);
	hash_set( hashtable, "TK_LIST", 18);
	hash_set( hashtable, "TK_SQL", 19);
	hash_set( hashtable, "TK_SQR", 20);
	hash_set( hashtable, "TK_INPUT", 21);
	hash_set( hashtable, "TK_OUTPUT", 22);
	hash_set( hashtable, "TK_SEM", 23);
	hash_set( hashtable, "TK_COLON", 24);
	hash_set( hashtable, "TK_DOT", 25);
	hash_set( hashtable, "TK_COMMA", 26);
	hash_set( hashtable, "TK_ENDWHILE", 27);
	hash_set( hashtable, "TK_OP", 28);
	hash_set( hashtable, "TK_CL", 29);
	hash_set( hashtable, "TK_IF", 30);
	hash_set( hashtable, "TK_THEN", 31);
	hash_set( hashtable, "TK_ENDIF", 32);
	hash_set( hashtable, "TK_READ", 33);
	hash_set( hashtable, "TK_WRITE", 34);
	hash_set( hashtable, "TK_RETURN", 35);
	hash_set( hashtable, "TK_PLUS", 36);
	hash_set( hashtable, "TK_MINUS", 37);
	hash_set( hashtable, "TK_MUL", 38);
	hash_set( hashtable, "TK_DIV", 39);
	hash_set( hashtable, "TK_CALL", 40);
	hash_set( hashtable, "TK_RECORD", 41);
	hash_set( hashtable, "TK_ENDRECORD", 42);
	hash_set( hashtable, "TK_ELSE", 43);
	hash_set( hashtable, "TK_AND", 44);
	hash_set( hashtable, "TK_OR", 45);
	hash_set( hashtable, "TK_NOT", 46);
	hash_set( hashtable, "TK_LT", 47);
	hash_set( hashtable, "TK_LE", 48);
	hash_set( hashtable, "TK_EQ", 49);
	hash_set( hashtable, "TK_GT", 50);
	hash_set( hashtable, "TK_GE", 51);
	hash_set( hashtable, "TK_NE", 52);
	hash_set( hashtable, "TK_EOF", 53);
	hash_set( hashtable, "TK_ERROR", 54);
	hash_set( hashtable, "$1", 55);
	hash_set( hashtable, "eps", 56);

return ;
}


rule* initAtom(int id)
{
  rule* ret = (rule*) malloc(sizeof(rule));
  ret->id=id;
  ret->next=NULL;
	return ret;
}

//returns the string correspong to an integer tokenid, used for printing the parsetree and errors
char*getToken(int id){return TermSymbol[id];}

char*getsymbol(int id)
{
	if(id<20000)
			return getToken(id);
	return Nontermsymbol[id-20000];
}

//fetch a rule of non terminal and terminal ids in form of a linked list
rule* getRule(char*line)
{
  char *pch = strtok(line," \n\r");
  int temp=hash_get(hashtable,pch);
  if(temp==-1) printf(".....%s",pch);

  rule* ret = initAtom(temp);
  rule* cur= ret;
  pch = strtok(NULL," \n\r");

  while(pch!=NULL)
  {
  	int hashVal = hash_get(hashtable,pch);
  	if(hashVal==-1) printf(".....%s",pch);
    cur->next = initAtom(hashVal);
    cur = cur->next;
    pch = strtok(NULL," \n\r");
  }
  return ret;
}

//grammar is a linkedlist of rules fetched from file grammarforparser
grammar* getGrammar(FILE* fp)
{

	size_t len = 0;
	int i;
  grammar* ret = (grammar*) malloc (sizeof(grammar));
	char *buff = NULL;

  for(i=0;i<NO_OF_NONTERMINALS;i++)
  {
    reference[i]=NULL;
    reference2[i]=NULL;
  }


  for(i=0;i<NUMBER_OF_RULES;i++)
  {
		int read = getline(&buff,&len,fp);
		buff[read-1]='\0';
    ret->gr[i]=getRule(buff);

    ruleNum *temp = (ruleNum*)malloc(sizeof(ruleNum));
    temp->num = i;
    temp->next = reference[ret->gr[i]->id-20000];
    reference[ret->gr[i]->id-20000] = temp;
    rule* rhs = ret->gr[i]->next;

    while(rhs)
    {
      if(rhs->id>=20000)
      {
        int idx = rhs->id - 20000;
        ruleNum *temp2 = (ruleNum*)malloc(sizeof(ruleNum));
        temp2->num = i;
        temp2->next = reference2[idx];
        reference2[idx] = temp2;
      }
      rhs=rhs->next;
    }
  }
	fclose(fp);
	return ret;
}

//returns an unsigned long long which has the firstset of the RHS of a rule,
ull getFirst(rule* cur)
{
	ull ret=0, one=1;
	if(cur->id<20000)
	{
		ret = one<<(cur->id);
		return ret;
	}

	ret = firstset[cur->id -20000];

	if(firstset[cur->id-20000] & (one<<56))
		if(cur->next)
		{
			ret = ret & ~(one<<56);
			return ret|getFirst(cur->next);
		}

	return ret;
}


ull getFirst2(rule* cur,grammar*gr);

//Used to populate the firstset data structure for a particular ID if it has not been calculated yet.else return.
void generateFirstSetUtil(grammar*gr, int id)
{
    if(firstset[id-20000]!=0) return;
    ull ret=0;
    ruleNum* curRule = reference[id-20000];
    while(curRule)
    {
      int ruleidx = curRule->num;
      rule* rhs = gr->gr[ruleidx]->next;
      ret|= getFirst2(rhs,gr);
      curRule=curRule->next;
    }
    firstset[id-20000] = ret;
    return;
}


void generateFollowSetUtil(grammar *gr, int id)
{
  if(followset[id-20000]!=0) return;
  ull ret=0,one=1;
  doing|=(one<<(id-20000));
  ruleNum* curRule = reference2[id-20000];

  while(curRule)
  {
	int ruleidx = curRule->num;
    rule*lhs = gr->gr[ruleidx];
    rule* rhs = gr->gr[ruleidx]->next;
    while(rhs!=NULL && rhs->id!=id)
      {
      	rhs = rhs->next;
      }
  	if(rhs==NULL)
  			continue;
    if(rhs->next==0 && ((doing & one<<(lhs->id-20000))==0))
      {
        generateFollowSetUtil(gr, lhs->id);
        ret|=followset[lhs->id-20000];
      }
    else  if(rhs->next!=NULL){
      ret|= getFirst(rhs->next);
      if(ret&(one<<56))
      {
        ret&= (~(one<<56));
        if((doing & (one<<(lhs->id-20000)))==0)
		    {
		      generateFollowSetUtil(gr, lhs->id);
		      ret|=followset[lhs->id-20000];
		    }
      }
    }
    curRule=curRule->next;
  }
  followset[id-20000] = ret;
  doing &= ~(one<<(id-20000));

  return;
}

void generateFollowSet(grammar* gr)
{
  int i;
  doing=0;
  ull one=1;
  followset = (ull*) malloc(sizeof(ull)*NO_OF_NONTERMINALS);
  memset(followset,0,sizeof(ull)*NO_OF_NONTERMINALS);
  followset[0]= one<<55;

  for( i=1;i<NO_OF_NONTERMINALS;i++)
   	generateFollowSetUtil(gr,i+20000);
}


void printFollow()
{
  int i,j;
  ull one=1;
  for(i=0;i<NO_OF_NONTERMINALS;i++)
  {
    printf("%s-> ", getsymbol(i+20000));
    for( j=0;j<NO_OF_TERMINALS;j++)
      if(followset[i]&(one<<j))
        printf(" %s", getToken(j));

    printf("\n");
  }

}

void generateFirstSet(grammar* gr)
{
  int i;
  firstset = (ull*) malloc(sizeof(ull)*NO_OF_NONTERMINALS);
  memset(firstset,0,sizeof(ull)*NO_OF_NONTERMINALS);
  for( i=0;i<NO_OF_NONTERMINALS;i++)
  {
    generateFirstSetUtil(gr,i+20000);
  }
}

void printFirst()
{
  int i,j;
  ull one=1;
  for(i=0;i<NO_OF_NONTERMINALS;i++)
  {
    printf("%s-> ", getsymbol(i+20000));
    for( j=0;j<NO_OF_TERMINALS;j++)
      if(firstset[i]&(one<<j))
        printf(" %s", getToken(j));
    printf("\n");
  }
}

//calculates first set for a Non-Terminal on LHS of a rule,
//and recursively calculates the firstsets of all the non temrinals which ahevnt been calculated yet.
ull getFirst2(rule* cur, grammar*gr)
{
	//if terminal
	ull ret=0, one=1;
	if(cur->id<20000)
	{
		ret = one<<(cur->id);
		return ret;
	}

	generateFirstSetUtil(gr, cur->id);
  ret = firstset[cur->id-20000];
	if(ret & (one<<56))
		if(cur->next)
		{
			ret = ret & ~(one<<56);
			   return ret|getFirst2(cur->next,gr);
		}

	return ret;
}


//creates a table with Non terminals in rows and terminals as columns, a particular entry corresponds to the rule number,
//If any, which could generate the temrinal from this non temrinal.
void createParseTable(grammar*gr)
{
	int i,j;
	for(i=0;i<NO_OF_NONTERMINALS;i++)
		for(j=0;j<NO_OF_TERMINALS;j++)
				parseTable[i][j]=-1;
	ull one=1;
	for(i=0;i<NUMBER_OF_RULES;i++)
	{
		int lhs = (gr->gr[i]->id)-20000;
		ull first = getFirst(gr->gr[i]->next);

		for(j=0;j<NO_OF_TERMINALS-2;j++)
		{
			if(first&(one<<j))
			{
				if(parseTable[lhs][j]!=-1)
				{
					printf("Error:%d rule already present in cell non terminal %d %d\n",parseTable[lhs][j],lhs+20000,j);
					printf("%d\n",i);
				}
				parseTable[lhs][j]=i;
			}
		}

		if(first&(one<<56))
		{
			ull follow = followset[lhs];

			for(j=0;j<NO_OF_TERMINALS-2;j++)
			{
				if(follow&(one<<j))
				{
					if(parseTable[lhs][j]!=-1)
					{
						printf("\nError:%d rule already present in cell non terminal %d %d\n",parseTable[lhs][j],lhs+20000,j);
						printf("%d\n",i);
					}
					parseTable[lhs][j]=i;
				}
			}

			if(follow&(one<<55))
			{
				if(parseTable[lhs][j]!=-1)
				{
					printf("\nError:%d rule already present in cell non terminal %d %d\n",parseTable[lhs][j],lhs+20000,j);
					printf("%d\n",i);
				}
				parseTable[lhs][55]=i;
			}
		}
	}
}


/*********************stack********************/

//Initializing the stack(top=-1)
void initializeStack(struct stack *st)
{
  st->arr = (parseTree**) malloc(sizeof(parseTree*)*size);
  st->top = -1;
}

// Entering the elements into stack
void push(struct stack *st, parseTree* num) {
   if (st->top == size - 1) {
      size = size*2;
      st->arr = realloc(st->arr,size);
   }
   st->top++;
   st->arr[st->top] = num;
}
 parseTree* peek(struct stack* st){
return st->arr[st->top];
}
//Deleting an element from the stack.
parseTree* pop(struct stack *st) {
   parseTree* num;
   if (st->top == -1){
      printf("\nStack Underflow\n");
      return NULL;
   }
   num = st->arr[st->top];
   st->top--;
   return num;
}

/**********************************************/
//function to generate synchronzing set of a non temrinal- FIRTS AND FOLLOW SET are in the synchronizing set.
ull getSynchSet(int nt)
{ull one=1;
  ull ret=0;
  ret |= firstset[nt-20000];
  ret |= followset[nt-20000];
  return ret;
}

parseTree* initParseTree(int id,ull line,char* str)
{
	parseTree* ret = (parseTree*) malloc(sizeof(parseTree));
	ret->children=NULL;
	ret->noChild=0;
  ret->node = ret;
	ret->str = strdup(str);
	ret->lineNo=line;
	ret->id=id;
	return ret;
}

//utility function to put the rule from right to left in the stack.
 rule* reverseList(rule*head)
 {
	 rule*head2=NULL;
	 rule*pointer = head;
	 while(pointer)
	 {
		 rule*temp = (rule*)malloc(sizeof(rule));
		 temp->id = pointer->id;
		 temp->next = head2;
		 head2 = temp;
		 rulelen++;
		 pointer = pointer->next;
	 }
	 return head2;
 }


void printExpected(tokenInfo*cur,int row)
{
  printf("ERROR_5: The token <%s> for lexeme <%s>  does not match at line <%llu>. The expected token here is ",
  getsymbol(cur->attribute),cur->str,cur->lineNo);
  int i,count=0;
  for(i=0;i<NUMBER_OF_TERMINALS;i++)
    if(parseTable[row][i]!=-1)
    {
      count++;
      if(count==4)
      {
        printf("etc.");
        break;
      }
      printf("<%s>  ",getsymbol(i));
    }
  printf("\n");
}


parseTree* createParseTree(FILE* fp, grammar*gr)
{
  ull one=1;
  int compiled=1;
	size =50;
	struct stack* st = (struct stack*) malloc(sizeof(struct stack));
	initializeStack(st);
	push(st, initParseTree(56,0,"$1"));
	parseTree* root = initParseTree(20000,0,"---");
	push(st,root);
	int length = 2;
	tokenInfo* cur = getNextToken(fp);

	while(length>0 )
	{
		parseTree* top =pop(st);
    length--;
		if(cur->attribute==TK_ERROR)
		{
      compiled=0;
			printf("%s\n",cur->str);
      cur = InitializeToken(getsymbol(cur->expectedToken), cur->expectedToken);
      if(cur->attribute==TK_ERROR) cur = getNextToken(fp);
		}

		if(top->id<20000)
		{
			if(top->id==56)
			{
				top->str = strdup("");
				top->lineNo = cur->lineNo;
			}
			else if(top->id ==cur->attribute)
			{
				top->lineNo=cur->lineNo;
				top->str=strdup(cur->str);
				cur=getNextToken(fp);
			}
			else
			{
        compiled=0;
				printf("ERROR_5: The token <%s> for lexeme <%s>  does not match at line <%llu>. The expected token here is <%s>\n",
        getsymbol(cur->attribute),cur->str,cur->lineNo,getsymbol(top->id));
        continue;
			}
		}
		else
		{
			int row = top->id-20000;
			int col = cur->attribute;

			if(parseTable[row][col]==-1)
			{
        printExpected(cur,row);
        compiled =0;
        //if non terminal is nullable then reduce it to epsilon
        ull tempfirst = firstset[row];
        if(tempfirst& (one<<56))
          continue;

        //else skip tokens until token in synch of top is seen
        else {
          //ISSUE AN ERROR MSG
          ull synch = getSynchSet(top->id);
          ull one =1;
          while((synch&(one<<cur->attribute))==0 && cur->attribute!=TK_DOLLAR && cur->attribute!=TK_ERROR)
          {
            cur = getNextToken(fp);
          }
        }
			}
			else
			{
				parseTree* nt = top;
        nt->ruleno=parseTable[row][col];
				rule*lhs = gr->gr[parseTable[row][col]];
				rule*rhs = lhs->next;
				rulelen=0;

				rule* revrhs = reverseList(rhs);
				nt->noChild=rulelen;
        int idx=rulelen-1;
				nt->children = (parseTree**) malloc(sizeof(parseTree*)*(rulelen+1));

				while(revrhs)
				{
					rule* cur1 = revrhs;
					revrhs = revrhs->next;
					int id = cur1->id;
					free(cur1);
					parseTree *newNode ;
					newNode=initParseTree(id,0,"---");
					push(st,newNode);
					length++;
					nt->children[idx--] = newNode;
				}
			}
		}
	}

  if(!compiled)
  {
    printf("COULD NOT COMPILE SUCCESSFULLY\n");
    return NULL;
  }

return root;
}

int printTree(parseTree*root, char*parent, int**mem, int option)
{
	//printf("printing par\n");
  	int i, numberNodes =0;
  	(**mem) += sizeof(root);
	parseTree*pt = root;
	char*NodeSymbol = getsymbol(root->id);
	if(option==2||option==3)
	{
		printf("%20s ",root->str);
		if(root->id>=20000)
			printf("%20s ","---");
		else
			printf("%20llu ",root->lineNo);
		printf("%20s ",NodeSymbol);
		printf("%20s ",((root->id==TK_NUM)||(root->id==TK_RNUM))?root->str:"---");
		printf("%20s ", parent);
		printf("%20s ",(root->id<20000)?"YES":"NO");
		printf("%20s\n",NodeSymbol);
		fflush(stdout);
	}
	
	for(i=0;i<=pt->noChild-1;i++)
		numberNodes+= printTree(root->children[i], NodeSymbol, mem, option);
return numberNodes+1;
}
