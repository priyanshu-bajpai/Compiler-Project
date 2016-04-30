/*
GROUP: 17
Priyanshu Bajpai 2013A7PS089P
Jayati Aggarwal  2013A7PS165P
*/

# include "lexer.h"
ull lineNo = 1;
int end=0;
int bufferSize = -1;

tokenInfo* InitializeToken(char str[],terminalId attr)
{
tokenInfo* token = (tokenInfo*)malloc(sizeof(tokenInfo));
token->str = (char*)malloc(sizeof(char)*(strlen(str)+1));
strcpy(token->str,str);
token->attribute = attr;
token->lineNo = lineNo;
return token;
}

tokenInfo* InitializeErrorToken(char str[],terminalId attr, terminalId expected)
{
tokenInfo* token = (tokenInfo*)malloc(sizeof(tokenInfo));
token->str = (char*)malloc(sizeof(char)*(strlen(str)+1));
strcpy(token->str,str);
token->expectedToken = expected;
token->attribute = attr;
token->lineNo = lineNo;
return token;
}

/*
Fecthes the next character the buffer, buffer fetches 1024 characters at once from the file.
A second buffer is maintained to store the lexemes in case a part of the keyword/lexeme is in the current buffer and a part is to be fetched
*/
char NextCharacter(FILE *fp)
{
	if(end==1) return 26;
	if(bufferSize == -1 || bufferPos == bufferSize) {
        bufferSize = fread(buffer, sizeof(char), (size_t)MAX_BUFFER_SIZE, fp);
        bufferPos = 1;
        if(bufferSize == 0)
        {
        	end=1;
        	return ' ';
        }
        else
            return buffer[0];
    }

    if(bufferSize == 0)
    {
      end=1;
      return ' ';
    }
    else { return buffer[bufferPos++]; }

}


tokenInfo* getNextToken(FILE *fp)
{
	int state=0,currBuffPos=-1;
	char cur=0;
	char buff[MAX_LENGTH];

		while(true)
		{
			cur = NextCharacter(fp);
			if(cur==26)
				{
					return InitializeToken("$1",TK_DOLLAR);
				}
				currBuffPos++;
				buff[currBuffPos]=cur;

			switch(state)
			{

				case 0:
					switch(cur)
					{
						case '\n':
							lineNo++;
						case '\t':
						case '\r':
						case ' ':
            currBuffPos=-1;
			        break;
						case '<':
							state=1;
							break;
						case '>':
							state=5;
							break;
						case '=':
							state = 8;
							break;
						case '!':
							state=10;
							break;
						case '&':
							state=12;
							break;
						case '@':
							state=15;
							break;
						case '#':
							state=18;
							break;
						case '_':
							state=20;
							break;
						case ']':
							return InitializeToken("]",TK_SQR);
						case '[':
							return InitializeToken("[",TK_SQL);
						case '/':
							return InitializeToken("/",TK_DIV);
						case '-':
							return InitializeToken("-",TK_MINUS);
						case '*':
							return InitializeToken("*",TK_MUL);
						case '(':
							return InitializeToken("(",TK_OP);

						case ')':
							return InitializeToken(")",TK_CL);

						case '.':
							return InitializeToken(".",TK_DOT);

						case ',':
							return InitializeToken(",",TK_COMMA);
						case ':':
							return InitializeToken(":",TK_COLON);
						case '+':
							return InitializeToken("+",TK_PLUS);
						case ';':
							return InitializeToken(";",TK_SEM);
						case '~':
							return InitializeToken("~",TK_NOT);

						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							state=23;
							break;

						case 'b':
						case 'c':
						case 'd':
							state=26;
							break;
						case 'a':
						case 'e':
						case 'f':
						case 'g':
						case 'h':
						case 'i':
						case 'j':
						case 'k':
						case 'l':
						case 'm':
						case 'n':
						case 'o':
						case 'p':
						case 'q':
						case 'r':
						case 's':
						case 't':
						case 'u':
						case 'v':
						case 'w':
						case 'x':
						case 'y':
						case 'z':
							state=29;
							break;
						case '%':
							state=42;
							break;
						default:
							buff[currBuffPos+1]='\0';
							sprintf(errormsg, "ERROR_2: Unknown Symbol <%s> at line number %llu", buff, lineNo);

							return InitializeErrorToken(errormsg,TK_ERROR,TK_ERROR);
				}
				break;

			case 1:
						switch(cur)
						{
							case '-':
								state=2;
								break;
							case '=':
							 return InitializeToken("<=",TK_LE);
							default:
							 	bufferPos--;
								return InitializeToken("<",TK_LT);
						}
						break;
			case 2:
					if(cur=='-')state=3;
					else
					{
						bufferPos--;
						buff[currBuffPos]='\0';
						sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '-' at line %llu",buff,lineNo);
						return InitializeErrorToken(errormsg,TK_ERROR,TK_ASSIGNOP);
					}
					break;

			case 3:
				if(cur=='-')return InitializeToken("<---",TK_ASSIGNOP);
				else
				{
					bufferPos--;
					buff[currBuffPos]='\0';
					sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '-' at line %llu",buff,lineNo);
					return InitializeErrorToken(errormsg,TK_ERROR,TK_ASSIGNOP);
				}
				break;
			case 5:
				if(cur=='=')return InitializeToken(">=",TK_GE);
				//else 	return InitializeToken("ERROR: = expected",TK_ERROR);
				else 	{
					bufferPos--;
					return InitializeToken(">",TK_GT);
				}
				break;
			case 8:
				if(cur=='=')return InitializeToken("==",TK_EQ);
				else
				{
					bufferPos--;
					buff[currBuffPos]='\0';
					sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '=' at line %llu",buff,lineNo);
					return InitializeErrorToken(errormsg,TK_ERROR,TK_EQ);
				}
				break;
			case 10:
				if(cur=='=')return InitializeToken("!=",TK_NE);
				else
				{
					bufferPos--;
					buff[currBuffPos]='\0';
					sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '=' at line %llu",buff,lineNo);
					return InitializeErrorToken(errormsg,TK_ERROR,TK_NE);
				}
				break;

			case 12:
				if(cur=='&')state=13;
				else
				{
					bufferPos--;
					buff[currBuffPos]='\0';
					sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '&' at line %llu",buff,lineNo);
					return InitializeErrorToken(errormsg,TK_ERROR,TK_AND);
				}
				break;

			case 13:
				if(cur=='&')return InitializeToken("&&&",TK_AND);
				else
				{
					bufferPos--;
					buff[currBuffPos]='\0';
					sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '&' at line %llu",buff,lineNo);
					return InitializeErrorToken(errormsg,TK_ERROR,TK_AND);
				}
				break;

			case 15:
					if(cur=='@')state=16;
					else
					{
						bufferPos--;
						buff[currBuffPos]='\0';
						sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '@' at line %llu",buff,lineNo);
						return InitializeErrorToken(errormsg,TK_ERROR,TK_OR);
					}
					break;

			case 16:
					if(cur=='@')return InitializeToken("@@@",TK_OR);
					else
					{
						bufferPos--;
						buff[currBuffPos]='\0';
						sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected '@' at line %llu",buff,lineNo);
						return InitializeErrorToken(errormsg,TK_ERROR,TK_OR);
					}
			case 18:
				switch(cur)
				{
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
					case 'f':
					case 'g':
					case 'h':
					case 'i':
					case 'j':
					case 'k':
					case 'l':
					case 'm':
					case 'n':
					case 'o':
					case 'p':
					case 'q':
					case 'r':
					case 's':
					case 't':
					case 'u':
					case 'v':
					case 'w':
					case 'x':
					case 'y':
					case 'z':
						state=19;
						break;
					default:
						bufferPos--;
						buff[currBuffPos]='\0';
						sprintf(errormsg,"ERROR_3: Unknown pattern <%s> ,a-z expected at line %llu",buff,lineNo);
						return InitializeErrorToken(errormsg,TK_ERROR,TK_RECORDID);

				break;
			}
			case 19:
				if(cur<'a'||cur>'z'){
					bufferPos--;
					buff[currBuffPos]='\0';
					return InitializeToken(buff,TK_RECORDID);
				}
				break;

			case 20:
				if((cur>='a'&&cur<='z')||(cur>='A'&&cur<='Z'))
					state=21;
				else
				{
						bufferPos--;
						buff[currBuffPos]='\0';
						sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected alphabet at line %llu",buff, lineNo);
						return InitializeErrorToken(errormsg,TK_ERROR,TK_FUNID);
				}
				break;

			case 21:
					if((cur>='a'&&cur<='z')||(cur>='A'&&cur<='Z'));
					else if(cur=='0'||cur=='1'||cur=='2'||cur=='3'||cur=='4'||cur=='5'||cur=='6'||cur=='7'||cur=='8'||cur=='9')
						state=22;
					else
					{
						bufferPos--;
						buff[currBuffPos]='\0';
					if(strcmp(buff,"_main")==0)
						return InitializeToken(buff,TK_MAIN);
					return InitializeToken(buff,TK_FUNID);
					}
					break;

			case 22:
			if(cur=='0'||cur=='1'||cur=='2'||cur=='3'||cur=='4'||cur=='5'||cur=='6'||cur=='7'||cur=='8'||cur=='9');
			else
			{
				bufferPos--;
				buff[currBuffPos]='\0';
				return InitializeToken(buff,TK_FUNID);
			}
			break;

			case 23:
					if(cur=='0'||cur=='1'||cur=='2'||cur=='3'||cur=='4'||cur=='5'||cur=='6'||cur=='7'||cur=='8'||cur=='9');
					else if(cur=='.')
						state=24;
					else
					{
						bufferPos--;
						buff[currBuffPos]='\0';
						return InitializeToken(buff,TK_NUM);
					}
					break;
			case 24:
				if(cur=='0'||cur=='1'||cur=='2'||cur=='3'||cur=='4'||cur=='5'||cur=='6'||cur=='7'||cur=='8'||cur=='9')
				state=25;
				else
				{
						bufferPos--;
						buff[currBuffPos]='\0';
						sprintf(errormsg,"ERROR_3: Unknown pattern <%s>, expected atleast one digit after '.' at line %llu",buff,lineNo);
						return InitializeErrorToken(errormsg,TK_ERROR,TK_RNUM);
				}
				break;

			case 25:
				if(cur=='0'||cur=='1'||cur=='2'||cur=='3'||cur=='4'||cur=='5'||cur=='6'||cur=='7'||cur=='8'||cur=='9')
				{
					buff[currBuffPos+1]='\0';
					return InitializeToken(buff,TK_RNUM);
				}
				else
				{
						bufferPos--;
						buff[currBuffPos]='\0';
						sprintf(errormsg,"ERROR_3: Unknown pattern <%s> at line %llu",buff,lineNo);
						return InitializeErrorToken(errormsg,TK_ERROR,TK_RNUM);
				}
				break;

			case 26:
				if(cur>='2'&&cur<='7')
						state=27;
				else if(cur>='a'&&cur<='z')
						state=29;
				else
				{
					bufferPos--;
					buff[currBuffPos]='\0';
					return InitializeToken(buff,TK_FIELDID);
				}
				break;

			case 27:

				if(currBuffPos==20)
				{
					while(cur>='b'&&cur<='d')cur = NextCharacter(fp);
					while(cur>='2'&&cur<='7')cur = NextCharacter(fp);
					bufferPos--;
					buff[20]='\0';
					sprintf(errormsg,"ERROR_1: identifier: %s at line %llu is longer than the prescribed length of 20 characters",buff,lineNo);
					return InitializeErrorToken(errormsg,TK_ERROR,TK_ID);
				}

				else if(cur>='2'&&cur<='7')
					state=28;
				else if(cur>='b'&&cur<='d');
				else
			  {
					bufferPos--;
					buff[currBuffPos]='\0';
					return InitializeToken(buff,TK_ID);
				}
				break;

			case 28:
				if(currBuffPos==20)
				{
					while(cur>='2'&&cur<='7')cur = NextCharacter(fp);
					bufferPos--;
					buff[20]='\0';
					sprintf(errormsg,"ERROR_1: identifier: %s at line %llu is longer than the prescribed length of 20 characters",buff,lineNo);
					return InitializeErrorToken(errormsg,TK_ERROR,TK_ID);
				}

				else if(cur>='2'&&cur<='7');
				else
				{
					bufferPos--;
					buff[currBuffPos]='\0';
					return InitializeToken(buff,TK_ID);
				}
				break;

			case 29:
			if(currBuffPos==20)
			{
				while(cur>='a'&&cur<='z')cur = NextCharacter(fp);
				bufferPos--;
				buff[20]='\0';
				sprintf(errormsg,"ERROR_1: identifier: %s at line %llu is longer than the prescribed length of 20 characters",buff,lineNo);
				return InitializeErrorToken(errormsg,TK_ERROR,TK_FIELDID);
			}
				else if(cur>='a'&&cur<='z');
				else
			  {
					bufferPos--;
					buff[currBuffPos]='\0';

					if(strcmp(buff,"with")==0)
						return InitializeToken(buff,TK_WITH);
				  else if(strcmp(buff,"parameters")==0)
						return InitializeToken(buff,TK_PARAMETERS);
					else if(strcmp(buff,"end")==0)
						return InitializeToken(buff,TK_END);
					else if(strcmp(buff,"while")==0)
						return InitializeToken(buff,TK_WHILE);
					else if(strcmp(buff,"int")==0)
						return InitializeToken(buff,TK_INT);
					else if(strcmp(buff,"real")==0)
						return InitializeToken(buff,TK_REAL);

					else if(strcmp(buff,"type")==0)
						return InitializeToken(buff,TK_TYPE);
					else if(strcmp(buff,"global")==0)
						return InitializeToken(buff,TK_GLOBAL);
					else if(strcmp(buff,"parameter")==0)
						return InitializeToken(buff,TK_PARAMETER);
					else if(strcmp(buff,"list")==0)
						return InitializeToken(buff,TK_LIST);
					else if(strcmp(buff,"input")==0)
						return InitializeToken(buff,TK_INPUT);
					else if(strcmp(buff,"output")==0)
						return InitializeToken(buff,TK_OUTPUT);

					else if(strcmp(buff,"endwhile")==0)
						return InitializeToken(buff,TK_ENDWHILE);
					else if(strcmp(buff,"if")==0)
						return InitializeToken(buff,TK_IF);
					else if(strcmp(buff,"endif")==0)
						return InitializeToken(buff,TK_ENDIF);
					else if(strcmp(buff,"then")==0)
						return InitializeToken(buff,TK_THEN);
					else if(strcmp(buff,"read")==0)
						return InitializeToken(buff,TK_READ);
					else if(strcmp(buff,"write")==0)
						return InitializeToken(buff,TK_WRITE);

					else if(strcmp(buff,"return")==0)
						return InitializeToken(buff,TK_RETURN);
					else if(strcmp(buff,"record")==0)
						return InitializeToken(buff,TK_RECORD);
					else if(strcmp(buff,"endrecord")==0)
						return InitializeToken(buff,TK_ENDRECORD);
					else if(strcmp(buff,"call")==0)
						return InitializeToken(buff,TK_CALL);
					else if(strcmp(buff,"else")==0)
						return InitializeToken(buff,TK_ELSE);
					else
						return InitializeToken(buff,TK_FIELDID);
				}
				break;

        case 42:
          currBuffPos=-1;
          if(cur=='\n')
					{
						lineNo++;
						state=0;
					}

          break;
				default:
				printf("WRONG STATE\n");
				;

			}
		}
}
