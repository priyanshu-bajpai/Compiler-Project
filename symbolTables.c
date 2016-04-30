#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
# include "parserdef.h"
# include "lexer.h"
# define SIZE_OF_INT 4
# define SIZE_OF_REAL 8



/*********************************////HASHTABLE////*********************************/
idtable *idcreateHashtable( int size ) {
	idtable *hashtable = NULL;
	int i;
	if( size < 1 ) return NULL;

	if( ( hashtable = malloc( sizeof( idtable) ) ) == NULL ) {
		return NULL;
	}
	if( ( hashtable->table = malloc( sizeof( identifier* ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ){
		hashtable->table[i] = NULL;
	}
	hashtable->size = size;

	return hashtable;
}

int idhash( idtable *hashtable,char *str ) {

	unsigned long hashval = 0;
        int c;

        while (c = *str++)
            hashval = c + (hashval << 6) + (hashval << 16) - hashval;

	return hashval % hashtable->size;
}

identifier *idNewpair( char* name, int offset, int type, int size, char *tname) {
	identifier *newpair;

	if( ( newpair = malloc( sizeof( identifier) ) ) == NULL ) {
		return NULL;
	}
	if( ( newpair->name = strdup( name ) ) == NULL ) {
		return NULL;
	}
	if( ( newpair->tname = strdup( tname ) ) == NULL ) {
		return NULL;
	}
	newpair->offset= offset;
	newpair->type=type;
	newpair->size=size;
	newpair->next = NULL;

	return newpair;
}

void idhash_set( idtable *hashtable, char* name, int offset, int type, int size, char *tname) {
	int bin = 0;
	identifier *newpair = NULL;
	bin = idhash( hashtable, name );
	newpair = idNewpair( name, offset, type, size, tname);
	newpair->next=hashtable->table[bin];
	hashtable->table[bin]=newpair;

}

identifier* idhash_get( idtable *hashtable, char *name ) {
	int bin = 0;
	identifier *pair;

	bin = idhash( hashtable, name );

	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->name != NULL && strcmp( name, pair->name ) !=0 ) {
		pair = pair->next;
	}
	if( pair == NULL || pair->name == NULL || strcmp( name, pair->name ) != 0 ) {
		return NULL;

	}
  else { return pair; }

}
/*********************************////HASHTABLE END////*********************************/

/*********************************************///funtable//************************************/
ftable *fcreateHashtable( int size ) {
	ftable *hashtable = NULL;
	int i;
	if( size < 1 ) return NULL;

	if( ( hashtable = malloc( sizeof( ftable ) ) ) == NULL ) {
		return NULL;
	}
	if( ( hashtable->table = malloc( sizeof( func * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ){
		hashtable->table[i] = NULL;
	}
	hashtable->size = size;

	return hashtable;
}

int fhash( ftable *hashtable, char *str ) {

	unsigned long hashval = 0;
        int c;

        while (c = *str++)
            hashval = c + (hashval << 6) + (hashval << 16) - hashval;

	return hashval % hashtable->size;
}

func *fNewpair( char* name, int offset,int noIn, int noOut, int* in, int* out,idtable* localtable,int size,int childNo) {
	func *newpair;

	if( ( newpair = malloc( sizeof( func) ) ) == NULL ) {
		return NULL;
	}
	if( ( newpair->name = strdup( name ) ) == NULL ) {
		return NULL;
	}
	newpair->offset= offset;
	newpair->noOut=noOut;
	newpair->noIn=noIn;
	newpair->inType=in;
	newpair->outType=out;
	newpair->localtable=localtable;
	newpair->size=size;
	newpair->childNo=childNo;
	newpair->next = NULL;

	return newpair;
}

void fhash_set( ftable *hashtable, char* name,int offset,int noIn, int noOut, int* in,int* out,idtable* localtable,int size ,int childNo) {
	int bin = 0;
	func *newpair = NULL;
	bin = fhash( hashtable, name );
	newpair = fNewpair(name,offset, noIn,  noOut, in, out ,localtable,size,childNo);
	newpair->next=hashtable->table[bin];
	hashtable->table[bin]=newpair;

}

func* fhash_get( ftable *hashtable, char *name )
{

	int bin = 0;
	func *pair;

	bin = fhash( hashtable, name );

	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->name != NULL && strcmp( name, pair->name ) != 0 ) {
		pair = pair->next;
	}
	if( pair == NULL || pair->name == NULL || strcmp( name, pair->name ) != 0 ) {
		return NULL;

	}
  else { return pair; }

}
/*********************************////HASHTABLE END////*********************************/

/*********************************************///RECORDtypetable//************************************/
typetable *tcreateHashtable( int size )
{
	typetable *hashtable = NULL;
	int i;
	if( size < 1 ) return NULL;

	if( ( hashtable = malloc( sizeof( typetable ) ) ) == NULL ) {
		return NULL;
	}
	if( ( hashtable->table = malloc( sizeof( recordtype * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ){
		hashtable->table[i] = NULL;
	}
	hashtable->size = size;

	return hashtable;
}

int thash( typetable *hashtable, char *str ) {

	unsigned long hashval = 0;
        int c;

        while (c = *str++)
            hashval = c + (hashval << 6) + (hashval << 16) - hashval;

	return hashval % hashtable->size;
}

recordtype* tNewpair( char* name, int type, int size, int noField, int* fieldtype, char** fieldid) {
	recordtype*newpair;

	if( ( newpair = malloc( sizeof( recordtype) ) ) == NULL ) {
		return NULL;
	}
	if( ( newpair->name = strdup( name ) ) == NULL ) {
		return NULL;
	}
	newpair->type=type;
	newpair->size=size;
	newpair->noField = noField;
	newpair->fieldtype=fieldtype;
	newpair->fieldid=fieldid;
	newpair->next = NULL;

	return newpair;
}

void thash_set( typetable *hashtable, char* name,int type, int size, int noField, int* fieldtype, char** fieldid ) {
	int bin = 0;
	recordtype *newpair = NULL;
	bin = thash( hashtable, name );
	newpair = tNewpair( name, type, size,noField,fieldtype,fieldid );
	newpair->next=hashtable->table[bin];
	hashtable->table[bin]=newpair;

}


recordtype* thash_get( typetable *hashtable, char *name ) {
	int bin = 0;
	recordtype *pair;

	bin = thash( hashtable, name );

	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->name != NULL && strcmp( name, pair->name ) != 0 ) {
		pair = pair->next;
	}
	if( pair == NULL || pair->name == NULL || strcmp( name, pair->name ) != 0 ) {
		return NULL;

	}
  else { return pair; }

}

/*********************************////HASHTABLE END////*********************************/
int getType( parseTree* nd, typetable* types)
{
	//printf(" got id %d\n", nd->id);
	if(nd->id==TK_INT){return 0;}
	else if(nd->id==TK_REAL) return 1;
	recordtype* temp = thash_get(types,nd->str);
	if(temp==NULL) return -1;
	else return
		temp->type;
}
int getsize( parseTree* nd, typetable* types)
{
	//printf(" got id %d\n", nd->id);
	if(nd->id==TK_INT) { return SIZE_OF_INT;}
	else if(nd->id==TK_REAL) return SIZE_OF_REAL;
	recordtype* temp = thash_get(types,nd->str);
	if(temp==NULL) return -1;
	else return
		temp->size;
}

void printRecordTable(typetable* table)
{
	int i;
	for(i=0;i<table->size;i++)
	{
		recordtype* st = table->table[i];
		while(st!=NULL)
		{
			printf("%s %d %d %d\n",st->name,st->type,st->size,st->noField);
			st=st->next;
		}
	}
}


int populateSymbolTables(typetable* typetable1, idtable* globaltable,ftable* functable, parseTree *root)
{
		int error=0;
		int globalOffset=0;
		int i,j,k;
		char* name;
		int type;
		int size;
		int noField;
		int* fieldtype;
		char** fieldid;
		int typestart = 1;

		for(i=0; i<root->noChild-1;i++)
		{
				//typedifintions
				parseTree* typenode = root->children[i]->children[3]->children[0];

				for(j=0;j<typenode->noChild;j++)
				{
						typestart++;
						size=0;
						parseTree* temp = typenode->children[j];
						char*typename, *attrname;
						name = strdup(temp->children[0]->str);
						noField = temp->noChild-1;
						fieldtype = malloc(sizeof(int)*noField);
						fieldid = malloc(sizeof(char*)*noField);
						for(k=1;k<temp->noChild;k++)
						{
							int m;
							for(m=1;m<k-1;m++)
								if(strcmp(temp->children[k]->children[1]->str,temp->children[m]->children[1]->str)==0)
								{
									printf("Line No %llu field name repeated \n",temp->children[k]->children[1]->lineNo);
									error=-1;
								}
							
							fieldid[k-1]= strdup(temp->children[k]->children[1]->str);
							fieldtype[k-1]=(temp->children[k]->children[0]->id==TK_INT)?0:1;
							if(fieldtype[k-1]==0)
									size += SIZE_OF_INT;
							else
									size += SIZE_OF_REAL;
						}
						if(thash_get(typetable1, name)==NULL)
								thash_set(typetable1, name, typestart, size,noField,fieldtype,fieldid);
						else
								{printf("Line No: %llu same recordtype defined previously\n",temp->children[0]->lineNo);error=-1;}
				}
		}


		//MAIN FUNCTION typerecords
		parseTree *typenode = root->children[i]->children[0]->children[0];
		for(j=0;j<typenode->noChild;j++)
		{
				typestart++;
				size=0;
				parseTree* temp = typenode->children[j];
				char*typename, *attrname;
				name = strdup(temp->children[0]->str);
				noField = temp->noChild-1;
				fieldtype = malloc(sizeof(int)*noField);
				fieldid = malloc(sizeof(char*)*noField);
				
				for(k=1;k<temp->noChild;k++)
				{
						fieldid[k-1]= strdup(temp->children[k]->children[1]->str);
						fieldtype[k-1]=(temp->children[k]->children[0]->id==TK_INT)?0:1;
						if(fieldtype[k-1]==0)
								size+= SIZE_OF_INT;
						else
								size += SIZE_OF_REAL;
				}
				if(thash_get(typetable1, name)==NULL)
						thash_set(typetable1, name, typestart, size,noField,fieldtype,fieldid);
				else
						{printf("Line No: %llu same recordtype defined previously \n",temp->children[0]->lineNo);error=-1;}
		}
		//checking global declarations of other functions
		for(i=0;i<root->noChild-1;i++)
		{
			parseTree* decnode = root->children[i]->children[3]->children[1];
			for(k=0;k<decnode->noChild;k++)
			{
				if(decnode->children[k]->noChild==2) continue;
				idtable* temptable = globaltable;
				if(idhash_get(temptable, decnode->children[k]->children[1]->str)==NULL)
				{
						int type =  getType( decnode->children[k]->children[0],typetable1);
						if(type==-1)
						{
							printf("Line No: %llu type difinition for global variable <%s> not found\n",decnode->children[k]->children[0]->lineNo,decnode->children[k]->children[0]->str );
							return -1;
						}
						int size = getsize( decnode->children[k]->children[0],typetable1);
						idhash_set(temptable,decnode->children[k]->children[1]->str,globalOffset, type, size, decnode->children[k]->children[0]->str);
						globalOffset+=size;
				}
				else
						{printf("Line No: %llu  The global identifier <%s> is declared more than once.\n",decnode->children[k]->children[1]->lineNo, decnode->children[k]->children[1]->str);error=-1;}
			}
		}
		//checking global declarations for main function
		parseTree* decnode = root->children[root->noChild-1]->children[0]->children[1];
		for(k=0;k<decnode->noChild;k++)
		{
			if(decnode->children[k]->noChild==2) continue;
			idtable* temptable = globaltable;
			if(idhash_get(temptable, decnode->children[k]->children[1]->str)==NULL)
			{
					int type ;
					if((type =  getType( decnode->children[k]->children[0],typetable1))==-1)
					{
						printf("Line No: %llu type difinition for %s not found \n",decnode->children[k]->children[0]->lineNo,decnode->children[k]->children[0]->str );
						error=-1;
					}
					else
					{
						int size = getsize( decnode->children[k]->children[0],typetable1);
						idhash_set(temptable,decnode->children[k]->children[1]->str,globalOffset, type, size, decnode->children[k]->children[0]->str);
						globalOffset+=size;
					}
			}
			else
					{printf("Line No: %llu same variable <%s> declared previously\n", decnode->children[k]->children[1]->lineNo,decnode->children[k]->children[1]->str);error=-1;}
		}

		//Inserting decalrations of variables for all functions
		for(i=0; i<root->noChild-1;i++)
		{
			//printRecordTable(typetable1);

			if(fhash_get(functable, root->children[i]->children[0]->str)==NULL)
			{
					parseTree*funcnode = root->children[i];
					int noIn = (funcnode->children[1]->noChild) /2;
					int noOut =  (funcnode->children[2]->noChild) /2;
					int * inType =(int*) malloc(sizeof(int)*noIn);
					int * outType = (int*) malloc(sizeof(int)*noOut);
					idtable * localtable = idcreateHashtable(50);
					int k, startOffset=0;

					//adding input type parameter info to functable struct and its symbol table
					for(k=0;k<2*noIn;k=k+2)
					{

							inType[k/2]=getType(funcnode->children[1]->children[k],typetable1);
							if(inType[k/2]==-1)
							{
								printf("Line No: %llu type difinition for %s not found \n",funcnode->children[1]->children[k]->lineNo,funcnode->children[1]->children[k]->str );
								error=-1;
							}
							else if( idhash_get(globaltable, funcnode->children[1]->children[k+1]->str)!=NULL)
							{
								printf("Line No: %llu  Variable <%s> is a global variable and cannot be declared again.\n",funcnode->children[1]->children[k+1]->lineNo,funcnode->children[1]->children[k+1]->str );
							}
							else if(idhash_get(localtable, funcnode->children[1]->children[k+1]->str)==NULL)
							{
									int size = getsize(funcnode->children[1]->children[k],typetable1);
									idhash_set(localtable,funcnode->children[1]->children[k+1]->str, startOffset, 	inType[k/2], size, funcnode->children[1]->children[k]->str);
									startOffset+=size;
							}
							else
								{printf("Line No: %llu same inputParameter defined previously \n",funcnode->children[1]->children[k+1]->lineNo);error = -1;}
					}

					//adding output parametr type info
					for(k=0;k<2*noOut;k=k+2)
					{
							outType[k/2]=getType(funcnode->children[2]->children[k],typetable1);
							if(outType[k/2]==-1)
							{
								printf("Line No: %llu type difinition for <%s> not found \n",funcnode->children[2]->children[k]->lineNo, funcnode->children[2]->children[k]->str );
								error=1;
							}
							if(idhash_get(localtable, funcnode->children[2]->children[k+1]->str)==NULL)
							{
									int size = getsize(funcnode->children[2]->children[k],typetable1);
									idhash_set(localtable,funcnode->children[2]->children[k+1]->str, startOffset, 	outType[k/2], size, funcnode->children[2]->children[k]->str);
									startOffset+=size;
							}
							else
								{printf("Line No: %llu same inputParameter defined previously \n",funcnode->children[2]->children[k+1]->lineNo);error=-1;}
					}

					//declarations
					decnode = root->children[i]->children[3]->children[1];
					for(k=0;k<decnode->noChild;k++)
					{
						if(decnode->children[k]->noChild==3) continue;
						if(idhash_get(globaltable,decnode->children[k]->children[1]->str)!=NULL)
						{
							printf("Line No: %llu global variable <%s> cannot be declared locally \n", decnode->children[k]->children[1]->lineNo,decnode->children[k]->children[1]->str);
							error=-1;
						}
						idtable* temptable = localtable;
						if(idhash_get(temptable, decnode->children[k]->children[1]->str)==NULL)
						{
								int type =  getType( decnode->children[k]->children[0],typetable1);
								if(type==-1)
								{
									printf("Line No: %llu type difinition for <%s> not found \n",decnode->children[k]->children[0]->lineNo ,decnode->children[k]->children[0]->str);
									error=-1;
								}
								else
								{int size = getsize( decnode->children[k]->children[0],typetable1);
								idhash_set(temptable,decnode->children[k]->children[1]->str,startOffset, type, size, decnode->children[k]->children[0]->str);
								startOffset+=size;}
								
						}
						else
								{printf("Line No: %llu same variable <%s> declared previously \n",decnode->children[k]->children[1]->lineNo, decnode->children[k]->children[1]->str);error=-1;}
					}
					fhash_set(functable,funcnode->children[0]->str,0,noIn, noOut, inType, outType,localtable,startOffset,i);
			}
			else
				{printf("Line No: %llu Function overloading not allowed <%s> defined previously \n", root->children[i]->children[0]->lineNo,root->children[i]->children[0]->str);error=-1;}
	}



		//MAIN FUNCTION Declarations
		if(fhash_get(functable, root->children[root->noChild-1]->children[0]->str)!=NULL)
		{
			printf("Function overloading not allowed. Main function defined twice\n");
			error=-1;
		}
		int startOffset=0;
		idtable* localtable = idcreateHashtable(100);
		decnode = root->children[root->noChild-1]->children[0]->children[1];

		for(k=0;k<decnode->noChild;k++)
		{
			if(decnode->children[k]->noChild==3) continue;
			idtable* temptable =localtable;
			if(idhash_get(globaltable,decnode->children[k]->children[1]->str)!=NULL)
			{
				printf(" Line No: %llu global variable <%s> cannot be declared locally\n", decnode->children[k]->children[1]->lineNo,decnode->children[k]->children[1]->str);
				error=-1;
			}
			if(idhash_get(temptable, decnode->children[k]->children[1]->str)==NULL)
			{
					int type ;
					if((type =  getType(decnode->children[k]->children[0],typetable1))==-1)
					{
						printf("Line No: %llu type difinition for <%s> not found \n" , decnode->children[k]->children[0]->lineNo,decnode->children[k]->children[0]->str);
						error=-1;
					}
					else
					{int size = getsize( decnode->children[k]->children[0],typetable1);
					idhash_set(temptable,decnode->children[k]->children[1]->str,startOffset, type, size, decnode->children[k]->children[0]->str);
					startOffset+=size;}

			}
			else
					{printf("Line No: %llu same variable <%s> declared previously \n",decnode->children[k]->children[1]->lineNo,decnode->children[k]->children[1]->str);error=-1;}
		}
		fhash_set(functable,"_main",0,0, 0, NULL, NULL, localtable,startOffset,root->noChild-1);

		return error==-1?error:globalOffset;
}





int checkDeclarationUtil(parseTree*root, idtable*localtable, typetable* typetable1, idtable* globaltable){
	
	int i, error=0;
	if(root->id==20023)//SingleOrRecId
	{
		if(root->noChild==1)
			return checkDeclarationUtil(root->children[0],localtable,typetable1, globaltable);
		else
		{
			identifier*temp = idhash_get( localtable,root->children[0]->str);
			if(temp==NULL) temp=idhash_get(globaltable, root->children[0]->str);
			if(temp==NULL) 
			{
				printf("record type %s not defined\n", root->children[0]->str);
				return -1;
			}
			int type = temp->type;
			recordtype* rtype = thash_get(typetable1, temp->tname);
			if(rtype==NULL) return -1;
			for(i=0;i<rtype->noField;i++)
				if(strcmp(root->children[1]->str,rtype->fieldid[i])==0)
					break;
			if(i==rtype->noField)
			{
				printf("No such field like %s in the record %s\n",root->children[1]->str, temp->tname);
				return	-1;
			}
			root->children[0]->stEntry = temp;
			return 0;
		}
	}

	if(root->id==TK_ID)
	{
		root->stEntry = idhash_get(localtable, root->str);
		if(root->stEntry==NULL) root->stEntry =idhash_get(globaltable,root->str);
		if(root->stEntry==NULL)
		{
			printf("Line No: %llu identifier <%s> not declared\n",root->lineNo, root->str);
			return -1;
		}
	}

	for(i=0;i<root->noChild;i++)
		if(checkDeclarationUtil(root->children[i], localtable,typetable1, globaltable)==-1)error=-1;
	
	return error;
}


int checkDeclaration(parseTree* root, ftable* functable, idtable* globaltable, typetable* typetable1)
{
	int i,j,error=0;
	//for all declared functions 
	for(i=0;i<root->noChild-1;i++)
	{
		parseTree* funcnode = root->children[i];
		parseTree* stmtsnode=  funcnode->children[3];
		 idtable* localtable = fhash_get(functable, funcnode->children[0]->str)->localtable;
		if(checkDeclarationUtil(funcnode, localtable, typetable1, globaltable)==-1)
			error=-1;
	}

	//for main functions
	parseTree* funcnode = root->children[root->noChild-1];
	parseTree* stmtsnode=  funcnode->children[0];
	idtable* localtable = fhash_get(functable, "_main")->localtable;
	if(checkDeclarationUtil(funcnode, localtable, typetable1, globaltable)==-1)
		error =-1;
	return error;	
}




void printGlobalTable(idtable* globaltable)
{
	int i;
	for(i=0;i<globaltable->size;i++)
	{
		identifier* st = globaltable->table[i];
		while(st!=NULL)
		{
			printf("%20s %20s %20s %20d\n",st->name,st->tname,"global",st->offset);
			st=st->next;
		}
	}
}

void printfuncTable(ftable* functable)
{
	int i;
	for(i=0;i<functable->size;i++)
	{
		func* st = functable->table[i];
		while(st!=NULL)
		{
			int j;
			int *inType = st->inType, *outType = st->outType;
			idtable* localtable = st->localtable;
			//printf("Function name = %s size = %d numInputs= %d numOutputs= %d\n",st->name,st->size,st->noIn, st->noOut);
			// printf("Input parameters ");
			// for(j=0;j<st->noIn;j++)
			// 	printf("%d ",inType[j]);
			// printf("\nOutput Parameters ");
			// for(j=0;j<st->noOut;j++)
			// 	printf("%d ",outType[j]);
			// printf("\nDeclarations within this function\n");
			for(j=0;j<localtable->size;j++)
			{
				identifier* st1 = localtable->table[j];
				while(st1!=NULL)
				{
					printf("%20s %20s %20s %20d\n",st1->name,st1->tname,st->name,st1->offset);
					st1=st1->next;
				}
			}

			st=st->next;
		}
	}
}