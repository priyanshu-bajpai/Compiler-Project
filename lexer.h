/*
GROUP: 17
Priyanshu Bajpai 2013A7PS089P
Jayati Aggarwal  2013A7PS165P
*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <stdbool.h>
# include "lexerdef.h"

tokenInfo* getNextToken(FILE *fp);
tokenInfo* InitializeToken(char str[],terminalId attr);
char getNextCharacter(FILE *fp);
