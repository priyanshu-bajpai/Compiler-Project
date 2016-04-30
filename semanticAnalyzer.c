/*
BATCH-17
Priyanshu Bajpai-2013A7PS089P
Jayati Aggarwal-2013A7PS165P
*/

#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include "parserdef.h"
#include "lexer.h"
#include "symbolTabledef.h"




int checkFuncCallStmtsUtil(parseTree* stmt,ftable* functable, func *fentry,int* assigned, parseTree* outputpar)
{
	func* callee;
	parseTree* outpar, *inpar;
	int i,j,k,error=0;
	switch(stmt->id)
	{
		case funCallStmt:

			callee = fhash_get(functable, stmt->children[1]->str);
			if(callee==NULL) error=-1;
			if(error==-1) return error;
			//A function definition for a function being used (say F1) by another (say F2) must precede the definition of the function using it(i.e. F2).
			
			if(callee->childNo>fentry->childNo){	
				printf("LineNo %llu A funtion <%s> cannot be called before its declaration\n",stmt->children[1]->lineNo,stmt->children[1]->str);
				error=-1;
			}
			//The function cannot be invoked recursively.
			if(callee->childNo==fentry->childNo)
			{
				printf("LineNo %llu A function <%s>  cannot be recursively\n",stmt->children[1]->lineNo,stmt->children[1]->str);
				error=-1;
			}

			//The types and  the number of parameters returned by a function must be the same as that of the parameters used in invoking the function.
			outpar = stmt->children[0];
			if(outpar->noChild!=callee->noOut)
			{
				printf("Line No: %llu Number of parameters required for function <%s> doesnt match number of returned paramters \n", stmt->children[1]->lineNo, callee->name);
				error=-1;
			}
			for(j=0;j<outpar->noChild;j++)
			{	if(outpar->children[j]->stEntry==NULL || outpar->children[j]->stEntry->type!=callee->outType[j])
				{
					printf("Line No: %llu type of output paramter in function <%s> doesnt match the returned type\n",outpar->children[j]->lineNo, stmt->children[1]->str);
					error=-1;
				}
				if(assigned!=NULL)
					for(k=1;k<outputpar->noChild;k+=2)
						if(outputpar->children[k]->stEntry==outpar->children[j]->stEntry) assigned[k/2]=1;
			}

			inpar = stmt->children[2];
			if(inpar->noChild!=callee->noIn)
			{
				printf("Line No: %llu Number of parameters required for function <%s> doesnt match number of returned paramters \n", stmt->children[1]->lineNo, callee->name);
				error=-1;
			}
			for(j=0;j<inpar->noChild;j++)
				if( inpar->children[j]->stEntry==NULL || inpar->children[j]->stEntry->type!=callee->inType[j])
				{
					printf("Line %llu type of input paramter in function <%s> doesnt match the expected input type\n",inpar->children[j]->lineNo, stmt->children[1]->str);
					error=-1;
				}
	
				return error;

		case iterativeStmt:
			for(i=1;i<stmt->noChild;i++)
				if(checkFuncCallStmtsUtil(stmt->children[i], functable, fentry, assigned, outputpar)==-1)
					error=-1;	
			if(WhileSemantics(stmt)==-1)
				{
					printf("Line No: %llu None of the variables participating in the iterations of the while loop gets updated.\n", stmt->children[0]->lineNo);
					error=-1;
				}
			return error;

		case conditionalStmt:
			for(i=1;i<stmt->noChild-1;i++){
				if(checkFuncCallStmtsUtil(stmt->children[i], functable, fentry,assigned, outputpar)==-1)
					error=-1;	
			}
			for(i=1;i<stmt->children[stmt->noChild-1]->noChild;i++){
				if(checkFuncCallStmtsUtil(stmt->children[stmt->noChild-1]->children[i], functable, fentry, assigned, outputpar)==-1)
					error=-1;		
			}
			return error;
//>>The parameters being returned by a function must be assigned a value. If a parameter does not get a value assigned within the function definition, it should be reported as an error.			
		case assignmentStmt:
			if(assigned==NULL) return 0;
			for(k=1;k<outputpar->noChild;k+=2)
				if(stmt->children[0]->children[0]->stEntry==outputpar->children[k]->stEntry){
					assigned[k/2]=1;
					break;
				}
			return 0;
//>>The parameters being returned by a function must be assigned a value. If a parameter does not get a value assigned within the function definition, it should be reported as an error.			
		case ioStmt:
			if(assigned==NULL) return 0;
			for(k=1;k<outputpar->noChild;k+=2)
				if(stmt->children[1]->children[0]->stEntry==outputpar->children[k]->stEntry){
					assigned[k/2]=1;
					break;
				}
			return 0;
		default:
			return 0;	
	}
}




int checkFuncCallStmts(parseTree* funcnode, idtable* globaltable, ftable* functable)
{
	int i,j,k,error=0;
	parseTree* outputpar = funcnode->noChild==1?NULL:(funcnode->children[2]);

	int* assigned = NULL;
	if(funcnode->noChild!=1)
	{
		assigned = malloc( sizeof(int)* outputpar->noChild/2);
		memset(assigned, 0 , sizeof(int)* outputpar->noChild/2);
	}
	
	parseTree* stmtsnode=funcnode->children[funcnode->noChild-1];
	func* fentry = fhash_get(functable, funcnode->noChild==1?"_main": funcnode->children[0]->str);
	for(i=2;i<stmtsnode->noChild-1;i++)
	{
		if(checkFuncCallStmtsUtil(stmtsnode->children[i], functable,fentry,assigned,outputpar)==-1)
			error=-1;
	}
	if(outputpar!=NULL)
		for(i=0;i<outputpar->noChild/2;i++)
			if(assigned[i]==0)
			{
				printf("%s output parameter not assigned ay value before returning it in function %s\n",outputpar->children[2*i+1]->str, funcnode->children[0]->str );
				error=-1;
			}
	if(assigned!=NULL)
		free(assigned);
	return error;
}



int checkFunctionSemantics(parseTree* root,idtable* globaltable, ftable* functable)
{
	int i,j,k,error =0;
	for(i=0;i<root->noChild-1;i++)
	{
		//checking if the output parameters defined in the starting match with the type and number of paramters that the function is returning. 
		func* fentry =fhash_get(functable, root->children[i]->children[0]->str);
		idtable* localtable = fentry->localtable;

		parseTree* retnod = root->children[i]->children[3]->children[root->children[i]->children[3]->noChild-1];
		if(retnod->noChild!=fentry->noOut ) 
		{
			printf("error:In function %s, number of parameters returned does not match number of output parameters in function definition ",root->children[i]->children[0]->str );
			error=-1;
		}
		for(j=0;j<retnod->noChild;j++)
		{
			if( retnod->children[j]->stEntry==NULL)
				return -1;
			if( retnod->children[j]->stEntry->type!= fentry->outType[j])
			{
				printf("lineNo %llu returned type of %s does not match expected type\n",retnod->children[j]->lineNo, retnod->children[j]->str);
				error=-1;
			}
		}
		if(checkFuncCallStmts(root->children[i],globaltable, functable)==-1)
			error=-1;
	}
	func* fentry =fhash_get(functable, "_main");
		idtable* localtable = fentry->localtable;
		if(checkFuncCallStmts(root->children[root->noChild-1],globaltable, functable)==-1)
			error=-1;
	return error;
}

struct list
{
	identifier*stEntry;
	char*fieldid;
	struct list* next; 
};

struct list* getList(parseTree* root)
{
	if(root->id ==singleOrRecId)
	{
		struct list*nd1 = malloc(sizeof(struct list));
		nd1->stEntry = root->children[0]->stEntry;
		nd1->next=NULL;
		if(root->noChild==2)
			nd1->fieldid = strdup(root->children[1]->str);
		return nd1;	
	}
	else if(root->id==TK_ID)
	{
		struct list*nd1 = malloc(sizeof(struct list));
		nd1->stEntry = root->stEntry;
		nd1->next=NULL;
		return nd1;
	}
	struct list *head=NULL,*tail=NULL;
	int i;
	for(i=0;i<root->noChild;i++)
	{
		if(head==NULL)
		{
			head = getList(root->children[i]);
			tail=head;
		} 
		else 
		{
			while(tail->next!=NULL) tail= tail->next;
			tail->next=getList(root->children[i]);
		}
	}
	return head;
}



int checkListElements(struct list *head, parseTree* root)
{
	int i;
	parseTree*temp = root->children[0];
	struct list *temp2;
	switch(root->id)
	{
		case funCallStmt:
			
			for(i=0;i<temp->noChild;i++)
			{
				struct list *temp2 = head;
				while(temp2!=NULL)
				{
					if(temp->children[i]->stEntry==temp2->stEntry)
						return 1;
					temp2=temp2->next;
				}
				
			}
			return 0;	
		case assignmentStmt:
			temp2= head;
			if(root->children[0]->noChild==1)
			{
				while(temp2!=NULL)
				{
					if(root->children[0]->children[0]->stEntry==temp2->stEntry)
						return 1;
					temp2=temp2->next;
				}
				return 0;
			}	
			else
			{
				while(temp2!=NULL)
				{
					if(root->children[0]->children[0]->stEntry==temp2->stEntry)
						if(strcmp(temp->children[1]->str, temp2->fieldid)==0)
						return 1;
					temp2=temp2->next;
				}
				return 0;
			}	
		case ioStmt:
			if(root->children[0]->id==TK_READ)
			{
				temp2=head;
				while(temp2!=NULL)
				{
					if(temp2->stEntry==root->children[1]->children[0]->stEntry)
						if(root->children[1]->noChild==1 || (root->children[1]->noChild==2 && (strcmp(temp2->fieldid, root->children[1]->children[1]->str)==0)))
							return 1;
					temp2 = temp2->next;
				}
			}
			return 0;
		case iterativeStmt:
			for(i=1;i<root->noChild;i++)
				if(checkListElements(head, root->children[i])==1)
					return 1;
			return 0;
		case conditionalStmt:
			for(i=1;i<root->noChild-1;i++)
				if(checkListElements(head, root->children[i])==1)
					return 1;
			for(i=0;i<root->children[root->noChild-1]->noChild;i++)
				if(checkListElements(head, root->children[i])==1)
					return 1;
			return 0;
		default:
			return 0;
	}
}


int WhileSemantics(parseTree* iterative)
{
	struct list * head = getList(iterative->children[0]);
	int i;
	for(i=1;i<iterative->noChild;i++)
		if(checkListElements(head, iterative->children[i])==1)
			return 0;
	
	return -1;
}

