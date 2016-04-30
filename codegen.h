/*
BATCH-17
Priyanshu Bajpai-2013A7PS089P
Jayati Aggarwal-2013A7PS165P
*/

void codegenerator(FILE* fp, parseTree* root, idtable* globaltable, ftable* functable,typetable* types);
void GenerateCodeStmts(parseTree* stmt, FILE *fp,typetable* types);
void printNewline(FILE*fp);
void printDecimalNumber(FILE* fp);
void printNegPos(FILE*fp);
void takeUserInput(FILE* fp);
void DeclareVariablesInBss(idtable* table, FILE*fp,typetable* types);
int arithmeticCode(parseTree* root, FILE*fp,typetable* types);
void booleanCode(parseTree* root, FILE* fp,typetable* types);
void GenerateCodeStmts(parseTree* stmt, FILE *fp,typetable* types);