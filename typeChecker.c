/*
BATCH-17
Priyanshu Bajpai-2013A7PS089P
Jayati Aggarwal-2013A7PS165P
*/


#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
# include "parserdef.h"
# include "lexer.h"
#include "symbolTabledef.h"


//0-int, 1-real, >1 reocord type, -1 boolean , -2 errortype
int TypeChecker(parseTree * root, typetable *typetable1)
{
	int type1, type2;
	int i,error=0;;
	switch(root->id)
	{
		case assignmentStmt:
			type1= TypeChecker(root->children[0],typetable1);
			type2= TypeChecker(root->children[1],typetable1);
			if(type2==-2 || type1==-2) return -2;
			else if(type2!=type1) 
			{
				printf("LineNo: %llu type of left variable <%s> not equal to type of right expression\n", root->children[0]->children[0]->lineNo,root->children[0]->children[0]->str);
				return -2;
			}
			else return type2;
		case 20023:
			if(root->noChild==1)
				return TypeChecker(root->children[0],typetable1);
			else
			{
				if(root->children[0]->stEntry==NULL)
					return -2;

				recordtype* rtype = thash_get(typetable1, root->children[0]->stEntry->tname);
				if(rtype==NULL) return -2;
				for(i=0;i<rtype->noField;i++)
				{
					if(strcmp(rtype->fieldid[i], root->children[1]->str)==0)
						return rtype->fieldtype[i];
				}
			}

		case TK_ID:
			//printf("id is %s %d\n", root->str, root->lineNo);
			if(root->stEntry==NULL) return -2;
			return root->stEntry->type;
		case TK_RNUM:
			return 1;
		case TK_NUM:
			return 0;

		case TK_PLUS:
		case TK_MINUS:
			
			if((type1 = TypeChecker(root->children[0],typetable1))==-2)
				return -2;
			if((type2 = TypeChecker(root->children[1],typetable1))==-2)
				return -2;
			if(type1!=type2)
			{
				printf("Line No: %llu types dont match\n",root->lineNo);
				return -2;
			}		
			if(type1==-1)
			{
				printf("Line No: %llu boolean expressions cannot be added or subtracted \n", root->lineNo);
				return -2;
			}	
			return type1; 
		case TK_MUL:
			if((type1 = TypeChecker(root->children[0],typetable1))==-2)
				return -2;
			if((type2 = TypeChecker(root->children[1],typetable1))==-2)
				return -2;
			if((type1>1&& (type2==0||type2==1))||(type2>1&&(type1==0||type1==1)))
				return type1>type2?type1:type2;
			if(type1==-1||type2==-1)
			{
				printf("Line No: %llu  Boolean expression cannot be multiplied\n", root->lineNo);
				return -2;
			}
			if(type1!=type2)
			{
				printf("Line No: %llu  types not matching \n",root->lineNo);
				return -2;
			}
			if(type1>1&&type2>1)
			{
				printf("Line No: %llu record types cannot be multiplied\n",root->lineNo);
				return-2;
			}	
			return type1;

		case TK_DIV:
			if((type1 = TypeChecker(root->children[0],typetable1))==-2)
				return -2;
			if((type2 = TypeChecker(root->children[1],typetable1))==-2)
				return -2;
			if(type2>1)
			{
				printf("Line No: %llu record type cannot divide\n",root->lineNo);
				return -2;
			}
			if(type1==-1||type2==-1)
			{
				printf("Line No: %llu Boolean expression cannot be divided\n", root->lineNo);
				return -2;
			}
			if(type1>1&&(type2==0||type2==1))
				return type1;
			if(type1!=type2)
			{
				printf("Line No: %llu types not matching \n",root->lineNo);
				return -2;
			}

			return type1;
		case TK_NOT:	
			if((type1=TypeChecker(root->children[0],typetable1))==-2)
				return -2;
			if(type1!=-1)
			{
				printf("%s\n", root->children[0]->str);
				printf("Line No: %llunot operator can only be applied to boolean types\n",root->lineNo );
			}
			return -1;
		case TK_LE:
		case TK_LT:
		case TK_GE:
		case TK_GT:
		case TK_NE:
		case TK_EQ:
			type1=TypeChecker(root->children[0],typetable1);
			type2=TypeChecker(root->children[1],typetable1);
			if((type1!=0&&type1!=1)||(type2!=0&&type2!=1))
			{
				printf("Line No: %llu Invalid expression. Relational operations possible only on int and real\n", root->lineNo);
				return -2;
			}
			if(type1!=type2)
			{
				printf("Line No: %llu types of expressions should be same while comparing\n", root->lineNo);
				return -2;
			}
			return -1;	
		case TK_AND:
		case TK_OR:
			if((type1 = TypeChecker(root->children[0],typetable1))!=-1)
			{
				printf("Line No: %llu logical operators can only be applied to boolean type.", root->lineNo);
				return -2;
			}
			if((type2 = TypeChecker(root->children[1],typetable1))!=-1)
			{
				printf("Line No: %llu logical operators can only be applied to boolean type.", root->lineNo);
				return -2;
			}
			return -1;

		default:
		for(i=0;i<root->noChild;i++)
			if((type1 = TypeChecker(root->children[i],typetable1))==-2)
				error=1;
		if(error) return -2;
		
		
	}
	return 0;
}

