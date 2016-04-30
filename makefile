all:
		gcc -g lexer.c parser.c driver.c codegen.c AST.c symbolTables.c typeChecker.c semanticAnalyzer.c -o toycompiler

