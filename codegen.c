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
# include "symbolTabledef.h"
# include "codegen.h"
int iternum=0, conum=0, relnum=0;
int maxNumFields=0;
void GenerateCodeStmts(parseTree* stmt, FILE *fp,typetable* types);

void printNewline(FILE*fp)
{
	fprintf(fp,"_PrintNewLine:\n\
section .data\n\
.nl\t\t db\t\t10\n\
section .text\n\
\t\tpush\t\tecx\n\
\t\tpush\t\tedx\n\
\t\tmov\t\tecx, .nl\n\
\t\tmov\t\tedx, 1\n\
\t\t;print the string\n\
\t\tpush\t\teax\n\
\t\tpush\t\tebx\n\
\t\tmov\t\teax,4\n\
\t\tmov\t\tebx,1\n\
\t\tint\t\t0x80\n\
\t\tpop\t\tebx\n\
\t\tpop\t\teax\n\
\t\tpop\t\tedx\n\
\t\tpop\t\tecx\n\
\t\tret\n");
}


void printDecimalNumber(FILE* fp)
{
	fprintf(fp,"_DecimalPrint:\n\
section\t\t.bss\n\
.decstr\t\tresb\t\t10\n\
.ct1\t\tresd\t\t1\n\
section .text\n\
\t\tpushad\n\
\t\tmov\t\t dword[.ct1],0\n\
\t\tmov\t\t edi,.decstr\n\
\t\tadd\t\t edi,9\n\
\t\txor\t\t edx,edx\t\n\
.whileNotZero:\n\
\t\tmov\t\t ebx,10\t\n\
\t\tdiv\t\t ebx\t\n\
\t\tadd\t\t edx,'0'\n\
\t\tmov\t\t byte[edi],dl\n\
\t\tdec\t\t edi\n\
\t\tinc\t\t dword[.ct1] \n\
\t\txor\t\t edx,edx\n\
\t\tcmp\t\t eax,0\n\
\t\tjne\t\t .whileNotZero\n\
\t\tinc\t\t edi\n\
\t\tmov\t\t ecx, edi\t \n\
\t\tmov\t\t edx, [.ct1]\n\
\t\tmov\t\t eax, 4\n\
\t\tmov\t\t ebx, 1\n\
\t\tint\t\t 0x80\n\
\t\tpopad\n\
\t\tret\n");
}
void printNegPos(FILE*fp)
{
fprintf(fp,"_NegPosPrint:\n\
section .data\n\
\t\tminus db '-'\n\
section .text\n\
\t\tpushad\n\
\t\tcmp eax, 0000h\n\
\t\tjge X1\n\
\t\tneg eax\n\
\t\tpush eax\n\
\t\tmov eax,4\n\
\t\tmov ebx, 1\n\
\t\tmov ecx, minus\n\
\t\tmov edx, 1\n\
\t\tint 80h\n\
\t\tpop eax\n\
\t\tX1:\n\
\t\tcall _DecimalPrint\n\
\t\tpopad\n\
\t\tret\n");
}

void takeUserInput(FILE* fp)
{
	fprintf(fp,"_UserInput:\n\
section .bss\n\
buffer\tresb\t120\n\
intg\tresd\t1\n\
isneg\tresb\t1\t\n\
section .text\n\
\t\tpushad\t\t\n\
\t\tmov\tesi, buffer\n\
\t\tmov\tedi, 0\t\n\
.loop1:\t\n\
\t\tmov\teax, 03\t\n\
\t\tmov\tebx, 0\t\n\
\t\tmov\tecx, esi\n\
\t\tmov\tedx, 1\t\n\
\t\tint\t0x80\t\n\
\t\tcmp\tbyte[esi], 0\n\
\t\tje\t .parse\n\
\t\tcmp\tbyte[esi], 10\n\
\t\tje\t .parse\n\
\t\tinc\tesi\n\
\t\tinc\tedi\t\n\
\t\tjmp\t.loop1\n\
.parse:\n\
\t\tmov\tesi, buffer\t\n\
\t\tmov\tecx, edi\t\n\
\t\tmov\tdword[intg], 0\n\
\t\tmov\tbyte[isneg], 0\n\
.negativ:\t\n \
\t\tcmp\tbyte[esi], '-'\n\
\t\tjne\t.loop\n\
\t\tinc\tesi\n\
\t\tinc\tbyte[isneg]\n\
.loop:\t  mov\tebx, 0\n\
\t\tmov\tbl, byte[esi]\t\n \
\t\tcmp\tbl, 10\t\n  \
\t\tje\t .done\t\n\
\t\tcmp\tbl, '0'\n\
\t\tjb\t .done\n\
\t\tcmp\tbl, '9'\n\
\t\tja\t .done\t\n\
\t\tmov\tedx, 10\n\
\t\tmov\teax, dword[intg]\n\
\t\tmul\tedx\t\n \
\t\tsub\tbl, '0'\n\
\t\tadd\teax, ebx\n\
\t\tmov\tdword[intg], eax\n\
\t\tinc\tesi\n\
\t\tloop    .loop\n\
.done:\n  \
\t\tcmp\tbyte[isneg], 0\n\
\t\tje\t .return\n\
\t\tneg\teax\n\
\t\tmov\tdword [intg], eax\n\
.return:\n\
\t\tpopad\n\
\t\tmov\teax, [intg]\n\
\t\tret\n");
}


void DeclareVariablesInBss(idtable* table, FILE*fp,typetable* types)
{
	int i, j;
	for(i=0;i<table->size;i++)
	{
		identifier* st = table->table[i];
		while(st!=NULL)
		{
			if(st->type==0)
				fprintf(fp,"%s resd %d\n",st->name,1);
			else
			{
				recordtype* rtype =  thash_get(types,st->tname);
				for(j=0;j<rtype->noField;j++)
				{
					fprintf(fp,"%s.%s resd %d\n",st->name,rtype->fieldid[j],1);
				}
				if(rtype->noField>maxNumFields) maxNumFields = rtype->noField;
			}
			st=st->next;
		}
	}
	return;
}


int arithmeticCode(parseTree* root, FILE*fp,typetable* types)
{
	int noField,i,nofield1,nofield2;
	switch(root->id)
	{
		case singleOrRecId:
		if(root->noChild==1)
			{
				if(root->children[0]->stEntry->type==0)
				{
					fprintf(fp,"push dword[%s]\n",root->children[0]->str);
					return 1;
				}
				else
				{
					recordtype * rtype = thash_get(types,root->children[0]->stEntry->tname);
					for(i=0;i<rtype->noField;i++)
						fprintf(fp,"push dword[%s.%s]\n",root->children[0]->str,rtype->fieldid[i] );
					return rtype->noField;
				}
			}
		else 
			fprintf(fp,"push dword[%s.%s]\n",root->children[0]->str, root->children[1]->str);   //if record types are also allowed change here
			return 1;

		case TK_NUM:
			fprintf(fp,"mov eax,%s\npush eax\n",root->str);
			return 1;

		case TK_ID:	
			if(root->stEntry->type==0)
				{
					fprintf(fp,"push dword[%s]\n",root->str);
					return 1;
				}
				else
				{
					recordtype * rtype = thash_get(types,root->stEntry->tname);
					for(i=0;i<rtype->noField;i++)
						fprintf(fp,"push dword[%s.%s]\n",root->str,rtype->fieldid[i] );
					return rtype->noField;
				}
		case TK_PLUS:
			noField = arithmeticCode(root->children[0],fp,types);
			arithmeticCode(root->children[1],fp,types);
			if(noField==1)
			{
				fprintf(fp,"pop ebx\npop eax\nadd eax,ebx\npush eax\n");
				return 	1;
			}
			for(i=noField-1;i>=0;i--)
				fprintf(fp, "pop dword[.bufferop1+%d]\n",4*i );
			for(i=noField-1;i>=0;i--)
				fprintf(fp, "pop eax\nadd eax, dword[.bufferop1+%d]\nmov dword[.bufferop1+%d], eax\n",4*i ,4*i);
			for(i=0;i<noField;i++)
				fprintf(fp,"push dword[.bufferop1+%d]\n", 4*i);
			return noField;
		case TK_MUL:
		nofield1= arithmeticCode(root->children[0],fp,types);
		nofield2=  arithmeticCode(root->children[1],fp,types);
		if(nofield1==1 && nofield2==1)
		{
			fprintf(fp,"pop ebx\npop eax\nimul ebx\npush eax\n");
			return 1;
		}
		if(nofield1>1)
		{
			fprintf(fp,"pop ebx\n");
			for(i=0;i<nofield1;i++)
				fprintf(fp,"pop eax\nimul eax,ebx\nmov dword[.bufferop1 + %d],eax\n",4*i);
			for(i=nofield1-1;i>=0;i--)
				fprintf(fp,"push dword[.bufferop1 + %d]\n",4*i);
			return nofield1;
		}
		if(nofield2>1)
		{
			for(i=0;i<nofield2;i++)
				fprintf(fp,"pop dword[.bufferop1 + %d]\n",4*i);

			fprintf(fp,"pop ebx\n");
			
			for(i=nofield2-1;i>=0;i--)
				fprintf(fp,"mov eax,dword[.bufferop1 + %d]\nimul eax,ebx\npush eax",4*i);
			return nofield2;
		}

		case TK_DIV:
		nofield1= arithmeticCode(root->children[0],fp,types);
		nofield2= arithmeticCode(root->children[1],fp,types);
		if(nofield2==1 && nofield1==1)
		{
			fprintf(fp,"pop ebx\npop eax\npush edx\nmov edx,0\nidiv ebx\npop edx\npush eax\n");
			return 1;
		}
		if(nofield1>1)
		{
			fprintf(fp,"pop ebx\n");
			for(i=0;i<nofield1;i++)
				fprintf(fp,"pop eax\npush edx\nmov edx,0\nidiv ebx\npop edx\nmov dword[.bufferop1 + %d],eax\n",4*i);
			for(i=nofield1-1;i>=0;i--)
				fprintf(fp,"push dword[.bufferop1 + %d]\n",4*i);
			return nofield1;
		}
		else
		{
			printf("Oops!!!Something went wrong...\n");
			return 1;
		}
		case TK_MINUS:
		if(root->noChild==2)
		{
			noField = arithmeticCode(root->children[0],fp,types);
			arithmeticCode(root->children[1],fp,types);
			if(noField==1)
			{	fprintf(fp,"pop ebx\npop eax\nsub eax,ebx\npush eax\n");
				return 	1;
			}
			for(i=noField-1;i>=0;i--)
				fprintf(fp, "pop dword[.bufferop1+%d]\n",4*i );
			for(i=noField-1;i>=0;i--)
				fprintf(fp, "pop eax\nsub eax, dword[.bufferop1+%d]\nmov dword[.bufferop1+%d], eax\n",4*i,4*i );
			for(i=0;i<noField;i++)
				fprintf(fp,"push dword[.bufferop1+%d]\n", 4*i);
			return noField;
		}
		else
		{
			noField = arithmeticCode(root->children[0],fp,types);
			if(noField==1)
			{
				fprintf(fp,"pop ebx\nmov eax,0\nsub eax,ebx\npush eax\n");
				return 1;
			}
			for(i=noField-1;i>=0;i--)
				fprintf(fp,"pop eax\nneg eax\nmov dword[.bufferop1+%d],eax\n",4*i );
			for(i=0;i<noField;i++)
				fprintf(fp, "push dword[.bufferop1 +%d]\n",4*i );
			return noField;
		}		
	}
}

void booleanCode(parseTree* root, FILE* fp,typetable* types)
{
	switch(root->id)
	{
		case TK_OR:
		booleanCode(root->children[0],fp,types);
		booleanCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\nor eax, ebx\npush eax\n");
		return;
		case TK_AND:
		booleanCode(root->children[0],fp,types);
		booleanCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\nand eax, ebx\npush eax\n");
		return;
		case TK_LT:
		arithmeticCode(root->children[0],fp,types);
		arithmeticCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\njl .rel%d\npop eax\npush 0000h\n.rel%d:\n",relnum,relnum);
		relnum++;
		return;
		case TK_GT:
		arithmeticCode(root->children[0],fp,types);
		arithmeticCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\njg .rel%d\npop eax\npush 0000h\n.rel%d:\n",relnum,relnum);
		relnum++;
		return;
		case TK_LE:
		arithmeticCode(root->children[0],fp,types);
		arithmeticCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\njle .rel%d\npop eax\npush 0000h\n.rel%d:\n",relnum,relnum);
		relnum++;
		return;
		case TK_GE:
		arithmeticCode(root->children[0],fp,types);
		arithmeticCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\njge .rel%d\npop eax\npush 0000h\n.rel%d:\n",relnum,relnum);
		relnum++;
		return;
		case TK_NE:
		arithmeticCode(root->children[0],fp,types);
		arithmeticCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\njne .rel%d\npop eax\npush 0000h\n.rel%d:\n",relnum,relnum);
		relnum++;
		return;
		case TK_EQ:
		arithmeticCode(root->children[0],fp,types);
		arithmeticCode(root->children[1],fp,types);
		fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\nje .rel%d\npop eax\npush 0000h\n.rel%d:\n",relnum,relnum);
		relnum++;
		return;
		case TK_NOT:
		booleanCode(root->children[0],fp,types);
		fprintf(fp,"pop eax\nnot eax\npush eax\n");
		return;
	}
	return ;
}

void GenerateCodeStmts(parseTree* stmt, FILE *fp,typetable* types)
{
	int i, noField;
	switch(stmt->id)
	{
		case assignmentStmt:
			noField = arithmeticCode(stmt->children[1],fp,types);
			if(noField==1)
			{
				if(stmt->children[0]->noChild==1)
					fprintf(fp,"pop eax\nmov dword[%s],eax\n",stmt->children[0]->children[0]->str); //if record types are also allowed change here
				else
					fprintf(fp,"pop eax\nmov dword[%s.%s],eax\n",stmt->children[0]->children[0]->str, stmt->children[0]->children[1]->str);
				return;
			}
			else
			{
				recordtype* rtype = thash_get(types, stmt->children[0]->children[0]->stEntry->tname);
				for(i=noField-1;i>=0;i--)
					fprintf(fp,"pop eax\nmov dword[%s.%s],eax\n",stmt->children[0]->children[0]->str, rtype->fieldid[i]);
				return;
			}
		case iterativeStmt:
			fprintf(fp,".while%d:\n",iternum);
			booleanCode(stmt->children[0],fp,types);
			fprintf(fp,"pop eax\ncmp eax,0000h\nje .endwhile%d\n", iternum++);
			for(i=1;i<stmt->noChild;i++)
				GenerateCodeStmts(stmt->children[i],fp,types);
			fprintf(fp,"jmp .while%d\n", iternum-1);
			fprintf(fp,".endwhile%d:\n", iternum-1);
			return ;
		case conditionalStmt:
			booleanCode(stmt->children[0],fp,types);
			fprintf(fp,"pop eax\ncmp eax,0000h\nje .else%d\n", conum);
			for(i=1;i<stmt->noChild-1;i++)
				GenerateCodeStmts(stmt->children[i],fp,types);
			fprintf(fp,"jmp .endif%d\n", conum);
			fprintf(fp,".else%d:\n", conum);
			for(i=0;i<stmt->children[stmt->noChild-1]->noChild;i++)
				GenerateCodeStmts(stmt->children[stmt->noChild-1]->children[i],fp,types);
			fprintf(fp,".endif%d:\n", conum++);
			return;
		case ioStmt:
			if(stmt->children[0]->id==TK_READ)
				 {

				 	if(stmt->children[1]->noChild==1)
				 		{
				 			if(stmt->children[1]->children[0]->stEntry->type==0)
				 				fprintf(fp,"call _UserInput\nmov dword[%s], eax\n",stmt->children[1]->children[0]->str );
				 			else
				 			{
				 				recordtype* rtype= thash_get(types, stmt->children[1]->children[0]->stEntry->tname);
				 				for(i=0;i<rtype->noField;i++)
				 					fprintf(fp,"call _UserInput\nmov dword[%s.%s], eax\n",stmt->children[1]->children[0]->str, rtype->fieldid[i] );
				 			}
				 		}
				 	else
				 		fprintf(fp,"call _UserInput\nmov dword[%s.%s], eax\n",stmt->children[1]->children[0]->str,stmt->children[1]->children[1]->str );
				 }
			else
				 {
				 	if(stmt->children[1]->noChild==1)
				 		{
				 			if(stmt->children[1]->children[0]->stEntry->type==0)
				 				fprintf(fp,"mov eax, dword[%s]\ncall _NegPosPrint\ncall _PrintNewLine\n",stmt->children[1]->children[0]->str );
				 			else
				 			{
				 				recordtype* rtype= thash_get(types, stmt->children[1]->children[0]->stEntry->tname);
				 				for(i=0;i<rtype->noField;i++)
				 					fprintf(fp,"mov eax,dword[%s.%s]\ncall _NegPosPrint\ncall _PrintNewLine\n",stmt->children[1]->children[0]->str, rtype->fieldid[i] );
				 			}
				 		}
				 	else
				 		fprintf(fp,"mov eax, dword[%s.%s]\ncall _NegPosPrint\ncall _PrintNewLine\n",stmt->children[1]->children[0]->str,stmt->children[1]->children[1]->str );
				 }
		default :
			return;
	}
}



void codegenerator(FILE* fp, parseTree* root, idtable* globaltable, ftable* functable,typetable* types)
{
	// io functions 
	printNewline(fp);
	takeUserInput(fp);
	printDecimalNumber(fp);
	printNegPos(fp);

	fprintf(fp, "global _start\n_start:\nsection .bss\n");
	func* fentry = fhash_get(functable,"_main");
	idtable* localtable = fentry->localtable;
	
	//bss section
	DeclareVariablesInBss(globaltable,fp, types);
	DeclareVariablesInBss(localtable,fp,types);
	fprintf(fp,".bufferop1\tresd\t%d\n", maxNumFields); //used for computations on record identifiers

	//text section
	fprintf(fp, "section .text\n");
	parseTree*stmtsnode = root->children[0]->children[0];
	int i;
	for(i=2;i<stmtsnode->noChild;i++)
		GenerateCodeStmts(stmtsnode->children[i], fp,types);

	//exit statement
	fprintf(fp,"mov\tebx, 0\nmov\teax, 1\nint\t0x80\n");
}