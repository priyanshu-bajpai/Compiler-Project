/*
BATCH-17
Priyanshu Bajpai-2013A7PS089P
Jayati Aggarwal-2013A7PS165P
*/

# include "parser.h"
# include "symbolTabledef.h"

int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("Usage: TestcaseFile, ParseTreeOutputFile\n" );
		return 0;
	}
		// printf("1.FIRST and FOLLOW Sets Automated\n");
		// printf("2.Both Lexical and Syntax Analysis Modules implemented, working for ALL TESTCASES\n");
		// printf("3.ERROR RECOVERY MODE: Panic Mode\n");

		int option;
		int*memforParseTree = malloc(sizeof(int));
		int*memforAST = malloc(sizeof(int));
		int numberNodesParseTree=0, NumberNodesAST=0;
		FILE*fp2 = fopen(argv[1],"r");
		printf("\nLEVEL  4:  All stage1 modules, AST(with operator pull up), Symbol table, Type Extractor and Checker, Semantic Analyzer, Code Generator modules work.\n");
		printf("\nENTER OPTION (1-7) :---- ");
		scanf("%d",&option);
		fflush(stdout);
	
		if(option==1)
		{
			tokenInfo * in = getNextToken(fp2);
			while(in->attribute!=55)
			{
				printf("%s %s %llu\n",getsymbol(in->attribute),in->str,in->lineNo);
				in = getNextToken(fp2);
			}
		}

		else
		{
			initializeHashTable();
			FILE *fp = fopen("grammarforparser.txt", "r");
			grammar *gr= getGrammar(fp);
			generateFirstSet(gr);//printFirst();
			generateFollowSet(gr);//printFollow();
			createParseTable(gr);
			parseTree*treeroot = createParseTree(fp2,gr);
			fclose(fp2);
			//check for treeroot being null (in case of compilation error)
			if(!treeroot){
				printf("CANNOT GENERATE PARSE TREE\n");
				return 0;
			}

			if(option==2||option==3)
			{
				printf("%20s %20s %20s %20s %20s %20s %20s\n", "LexemeCurrentNode","LineNo","Token","ValueIfNumber"
				,"ParentNodeSymbol","IsLeafNode(YES/NO)","NodeSymbol");
			}

			numberNodesParseTree = printTree(treeroot,"ROOT", &memforParseTree, option);
			
			if(option==2)
				return 0;
			
			createAST(treeroot);
			NumberNodesAST =  printTree(treeroot,"ROOT",&memforAST,option);

			if(option==3)
				return 0;

			if(option==4)
			{
				float compression =  ((float)(*memforParseTree)-(float)(*memforAST))/(float)(*memforParseTree)*(float)100;
				printf("Parse tree    Number of nodes = %d    Allocated Memory =  %d Bytes\n\
AST           Number of nodes = %d    Allocated Memory =  %d Bytes\n\
Compression percentage = %f percent \n",numberNodesParseTree,*memforParseTree,NumberNodesAST,*memforAST,compression);
				return 0;
			}



			typetable* recordtypetable = tcreateHashtable(10);
			idtable* globaltable = idcreateHashtable(50);
			ftable* functable = fcreateHashtable(20);

			int err = populateSymbolTables(recordtypetable,globaltable,functable,treeroot);

			if(option==5)
			{
				printf("%20s %20s %20s %20s\n","(identifier)Lexeme","type","scope","offset"); 
				// printRecordTable(recordtypetable);
				printGlobalTable(globaltable);
				printfuncTable(functable);
				return 0;
			}
		
			int err2 = checkDeclaration(treeroot, functable, globaltable, recordtypetable);
			int err3 = TypeChecker(treeroot,recordtypetable);

			//checks function and while semantics
			int err4 = checkFunctionSemantics(treeroot,globaltable, functable);
			if(!(err<0 || err2<0 || err3<0 || err4<0))
				printf("Code compiles successfully..........\n");
			
			if(option==6)
				return 0;
			
			if(err<0 || err2<0 || err3<0 || err4<0) return 0;
			FILE* outfile = fopen(argv[2],"w");
			codegenerator(outfile, treeroot, globaltable, functable,recordtypetable);
			fclose(outfile);
			}
	
	return 0;
}