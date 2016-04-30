/*
BATCH-17
Priyanshu Bajpai-2013A7PS089P
Jayati Aggarwal-2013A7PS165P
*/

# include "parserdef.h"
# include <stdlib.h>
# include <stdio.h>


parseTree* createAST(parseTree * root)
{
  if(root->noChild==0 )
    return root;
  int i;
  for(i=0;i<root->noChild;i++)
  {
    createAST(root->children[i]);
    if(root->ruleno==50 && i==0)
    {
      root->children[1]->inh= root->children[0]->node;
    }
    if(root->ruleno==51 && i==1)
    {//e->op1 term e
      root->children[2]->inh= root->children[0]->node;
      root->children[0]->node->noChild=2;
      root->children[0]->node->children = (parseTree**) malloc(2*sizeof(parseTree*));
      root->children[0]->node->children[0] = root->inh;
      root->children[0]->node->children[1]= root->children[1]->node;
    }
    if(root->ruleno==53 && i==0)
    {
      root->children[1]->inh= root->children[0]->node;
    }
    if(root->ruleno==54 && i==1)
    {
      root->children[2]->inh= root->children[0]->node;
      root->children[0]->node->noChild=2;
      root->children[0]->node->children = (parseTree**) malloc(2*sizeof(parseTree*));
      root->children[0]->node->children[0] = root->inh;
      root->children[0]->node->children[1]= root->children[1]->node;
    }

  }
switch(1+ root->ruleno)
{

  case 1://<program> ===> <otherFunctions> <mainFunction>
  {
    root->noChild = root->children[0]->noChild + 1;
    parseTree** temp=(parseTree**) malloc(sizeof(parseTree*)*root->noChild);

    for(i = 0 ;i< root->children[0]->noChild;i++) temp[i] = root->children[0]->children[i];
    temp[i]=root->children[1]->node;

    free(root->children[0]);
    free(root->children);
    root->children=temp;
    break;
  }
  case 2: // <mainFunction>===> TK_MAIN <stmts> TK_END
  {
    root->noChild=1;
    parseTree*temp = root->children[1]->node;
    free(root->children[2]);
    free(root->children[0]);
    free(root->children);
    root->children= (parseTree**) malloc(sizeof(parseTree*));
    root->children[0]=temp;
    break;
  }

  case 3:{// <otherFunctions> ===> <function> <otherFunctions1>

    //root->children=concatList (function.node,otherFunctions1 .children)
    //free(otherFunctions1)

    root->noChild = root->children[1]->noChild + 1;
    parseTree** temp=(parseTree**) malloc(sizeof(parseTree*)*root->noChild);

    temp[0] = root->children[0]->node;
    for(i = 0 ;i< root->children[1]->noChild;i++)
      temp[i+1] = root->children[1]->children[i];

    free(root->children[1]);
    //free(root->children);
    root->children = temp;
    break;
  }


  case 4:{// <otherFunctions> ===>eps

  //OtherFunctions.children=NULL
  //free(OtherFuntions)
  root->noChild=0;
  free(root->children[0]);
  free(root->children);
    break;
  }

  case 5: {//<function> ===> TK_FUNID <input_par> <output_par> TK_SEM  <stmts>  TK_END
  //Function.children = concatList( TK_FUNID.node, <input_par>.node, <output_par>.node,   <stmts>.node)
    root->noChild = 4;
    parseTree**temp = malloc(sizeof(parseTree*)*4);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[1]->node;
    temp[2] = root->children[2]->node;
    temp[3] = root->children[4]->node;
    free(root->children[3]);
    free(root->children[5]);
    free(root->children);
    root->children = temp;
  break;
  }

  case 6:{// <input_par> ===> TK_INPUT  TK_PARAMETER  TK_LIST TK_SQL  <parameter_list>  TK_SQR

  //<input_par>.children = <parameter_list>.children
  //free(parameterlist)
    parseTree**temp =root->children[4]->children;
    root->noChild = root->children[4]->noChild;
    for(i=0;i<6;i++)
        free(root->children[i]);

    free(root->children);
    root->children = temp;
  break;
  }

  case 7:{// <output_par> ===> TK_OUTPUT  TK_PARAMETER  TK_LIST TK_SQL  <parameter_list> TK_SQR
  //<output_par>.children = <parameter_list> .children
  //free(parameterlist)
  parseTree**temp =root->children[4]->children;
  root->noChild = root->children[4]->noChild;
  for(i=0; i<6; i++)
      free(root->children[i]);

  free(root->children);
  root->children = temp;
  break;

  }


  case 8:{//  <output_par> ===> eps

  //<output_par>.children=NULL
    root->noChild =0;
    root->children[0]=NULL;
    free(root->children);
  break;
  }

  case 9:// <parameter_list> ===> <dataType>  TK_ID  <remaining_list>
  //parameter_list.children=concatList(<dataType>.node, TK_ID.node,remaining_list.children)
  //free(dataType)
  //free(remaining_list)
  {
    root->noChild= 2+ root->children[2]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[1]->node;
    for(i=2;i<root->noChild;i++) temp[i] = root->children[2]->children[i-2];
    free(root->children[0]);
    free(root->children[2]);
    free(root->children);
    root->children=temp;
    break;
  }
  case 10:
  // <dataType> ===>  <primitiveDatatype>
  //<dataType>.node = <primitiveDatatype>.node
  //free(primitiveDatatype)
  {
    root->noChild = 1;
    root->node = root->children[0]->node;
    free(root->children[0]);
    free(root->children);
  break;
  }

  case 11:// <dataType> ===> <constructedDatatype>
  //<dataType>.node = <ConstructedDatatype>.node
  //free( ConstructedDatatype)
  {
    root->noChild = 1;
    root->node = root->children[0]->node;
    free(root->children[0]);
    free(root->children);
  break;
  }

  case 12:// <primitiveDatatype> ===> TK_INT
  //<primitiveDatatype>.node=TK_INT.node
  {
    root->noChild = 1;
    root->node = root->children[0]->node;
    free(root->children);
  break;
  }

  case 13:// <primitiveDatatype> ===> TK_REAL
  //<primitiveDatatype> .node=TK_REAL.node
  {
    root->noChild = 1;
    root->node = root->children[0]->node;
    free(root->children);
  break;
  }


  case 14:// <constructedDatatype> ===> TK_RECORD  TK_RECORDID
  //<constructedDatatype>.node = TK_RECORDID.node
  {
    root->noChild = 1;
    root->node = root->children[1]->node;
    free(root->children[0]);
    free(root->children);
  break;
  }

  case 15: //<remaining_list> ===> TK_COMMA <parameter_list>
  //<remaining_list>.children=parameter_list.children
  //free(parameter_list)
  {
    parseTree** temp  = root->children[1]->children;
    root->noChild=root->children[1]->noChild;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 16: //<remaining_list> ===> eps
  //<remaining_list>.children=NULL
  {
    root->noChild = 0;
    free(root->children[0]);
    free(root->children);
    root->children=NULL;
  break;
  }

  case 17: // <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
    //Stmt.children = concatList(<typeDefinitions>.node, <declarations>.node, <otherStmts>.children, <returnStmt>.node)
    //free(otherStmts)
  {
    root->noChild = 3 + root->children[2]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[1]->node;
    temp[root->noChild-1] = root->children[3]->node;

    for(i=0;i<root->children[2]->noChild;i++)
      temp[i+2] = root->children[2]->children[i];
    free(root->children[2]);
    free(root->children);
    root->children = temp;
    break;
  }

  case 18: // <typeDefinitions> ===> <typeDefinition> <typeDefinitions1>
  //<typeDefinitions>.children = concatList(<typeDefinition>.node, typeDefinitions1>.children)
  //free(<typeDefinitions1>)
  {
    root->noChild = 1 + root->children[1]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    for(i=0;i<root->children[1]->noChild;i++)
      temp[i+1] = root->children[1]->children[i];
    free(root->children[1]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 19:// <typeDefinitions> ===>  eps
  //<typeDefinitions>.children=NULL
  {
  root->noChild = 0;
  free(root->children[0]);
  free(root->children);
  root->children=NULL;
  break;
  }

  case 20:// <typeDefinition> ===> TK_RECORD TK_RECORDID <fieldDefinitions> TK_ENDRECORD TK_SEM
  //<typeDefinition>.children = concatList(TK_RECORDID.node, <fieldDefinitions>.children)
  //free(fieldDefinitions)
  {
    root->noChild = 1 + root->children[2]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[1]->node;
    for(i=0;i<root->children[2]->noChild;i++)
      temp[i+1] = root->children[2]->children[i];
    free(root->children[0]);
    free(root->children[2]);
    free(root->children[3]);
    free(root->children[4]);
    free(root->children);
    root->children = temp;
    break;
  }


  case 21: // <fieldDefinitions> ===> <fieldDefinition1> <fieldDefinition2> <moreFields>
  //<fieldDefinitions>.children = concatList(<fieldDefinition1>.node, <fieldDefinition2>.node, <moreFields>.children)
  //free(morefields)
  {
   root->noChild = 2 + root->children[2]->noChild;
   parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
   temp[0] = root->children[0]->node;
   temp[1] = root->children[1]->node;
   for(i=0;i<root->children[2]->noChild;i++)
     temp[i+2] = root->children[2]->children[i];

   free(root->children[2]);
   free(root->children);
   root->children = temp;
   break;
  }

  case 22:// <fieldDefinition> ===> TK_TYPE <primitiveDatatype> TK_COLON TK_FIELDID TK_SEM
  //<fieldDefinition>.children = concatList(<primitiveDatatype>.node, TK_FIELDID.node)
  //free(<primitiveDatatype>)
  {
   root->noChild = 2;
   parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
   temp[0] = root->children[1]->node;
   temp[1] = root->children[3]->node;

   free(root->children[0]);
   free(root->children[2]);
   free(root->children[4]);
   free(root->children);
   root->children = temp;
   break;
  }


  case 23:// <moreFields> ===> <fieldDefinition> <moreFields1>
  //<moreFields>.children = concatList(<fieldDefinition>.Node,<moreFields1>.children)
  //free(morefields1)
  {
     root->noChild = 1 + root->children[1]->noChild;
     parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
     temp[0] = root->children[0]->node;
     for(i=0;i<root->children[1]->noChild;i++)
       temp[i+1] = root->children[1]->children[i];

     free(root->children[1]);
     free(root->children);
     root->children = temp;
     break;
  }

   case 24: //<moreFields> ===> eps
  //<morefields>.children=NULL
  {
    root->noChild=0;
    free(root->children[0]);
    free(root->children);
    root->children=NULL;
    break;
  }
  case 25:// <declarations> ===> <declaration> <declarations1>
  //Declarations.children= concatList(<declaration>.node,  declarations1.children)
  //free( declarations1)
  {
    root->noChild = 1 + root->children[1]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    for(i=0;i<root->children[1]->noChild;i++)
      temp[i+1] = root->children[1]->children[i];

    free(root->children[1]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 26:// <declarations> ===> eps
  //Declarations.children=NULL
  {
      root->noChild=0;
      free(root->children[0]);
      free(root->children);
      root->children=NULL;
      break;
  }

  case 27:// <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID  <global_or_not> TK_SEM
  //Declaration.children = concatList( dataType.node,  TK_ID.node, global_or_not.node)
  //free(datatype)
  //free(global_or_not)
  {

     root->noChild = 2 + (root->children[4]->node==NULL?0:1);
     parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
     temp[0] = root->children[1]->node;
     temp[1] = root->children[3]->node;
     if(root->noChild==3)
     	temp[2] = root->children[4]->node;
     free(root->children[0]);
     free(root->children[1]);
     free(root->children[2]);
     free(root->children[4]);
     free(root->children[5]);
     free(root->children);
     root->children = temp;
     break;
  }
  case 28: //<global_or_not> ===> TK_COLON  TK_GLOBAL
  // global_or_not.node=TK_GLOBAL.node
  {
    root->node = root->children[1]->node;
    free(root->children[0]);
    free(root->children);
    break;
  }

    case 29: //<global_or_not> ===> eps
  // global_or_not.node=NULL
  {
    root->noChild=0;
    root->node=NULL;
    free(root->children[0]);
    free(root->children);
    root->children=NULL;
    break;
  }

    case 30: // <otherStmts> ===> <stmt> <otherStmts1>
  // OtherStmts.children = concatList(stmt.node, otherstmts1.children)
  // free(stmt)
  // free(otherstmts1)
  {
    root->noChild = 1+ root->children[1]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    for(i=0;i<root->children[1]->noChild;i++)
      temp[i+1] = root->children[1]->children[i];

    free(root->children[0]);
    free(root->children[1]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 31:// <otherStmts> ===> eps
  // Otherstmts.children=NULL
  {
    root->noChild=0;
    free(root->children[0]);
    free(root->children);
    root->children=NULL;
    break;
  }
  case 32:// <stmt> ===> <assignmentStmt>
  // Stmt.node= assignmentStmt.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }
  case 33:// <stmt> ===> <funCallStmt>
  // Stmt.node=funcallstmt.node
  {
      root->node = root->children[0]->node;
      free(root->children);
      break;
  }
  case 34:// <stmt> ===> <iterativeStmt>
  // Stmt.node=iterativestmt.node
  {
      root->node = root->children[0]->node;
      free(root->children);
      break;
  }
  case 35:// <stmt> ===> <conditionalStmt>
  // Stmt.node=conditionalstmt.node
  {
      root->node = root->children[0]->node;
      free(root->children);
      break;
  }
  case 36: //<stmt> ===> <ioStmt>
  // Stmt.node=iostmt.node
  {
      root->node = root->children[0]->node;
      free(root->children);
      break;
  }

  case 37:// <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP  <arithmeticExpression> TK_SEM
  // AssignmentStmt.children = concatList( SingleOrRecId.node, arithmeticExpression.node)
  {
    root->noChild = 2;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[2]->node;
    //printf("arith-> node%d",root->children[2]->node );
    free(root->children[1]);
    free(root->children[3]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 38:// <singleOrRecId> ===>TK_ID  <C1>
  // SingleOrRecId.children = concatList(TK_ID.node, C1.node)
  // free(c1)
  {
      root->noChild = 1+(root->children[1]->node==NULL?0:1);
      parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
      temp[0] = root->children[0]->node;
      if(root->noChild==2)
      	temp[1] = root->children[1]->node;
      free(root->children[1]);
      free(root->children);
      root->children = temp;
      break;
  }
  case 39:// <C1> ===> TK_DOT  TK_FIELDID
  // C1.node = TK_FIELDID.node
  {
    root->node = root->children[1]->node;
    free(root->children[0]);
    free(root->children);
    break;
  }
  case 40:// <C1> ===> eps
  // C1.node= NULL
  {
    root->noChild=0;
    root->children[0]=NULL;
    free(root->children);
    root->children=NULL;
    root->node=NULL;
    break;
  }

  case 41:// <funCallStmt> ===> <outputParameters> TK_CALL  TK_FUNID  TK_WITH TK_PARAMETERS <inputParameters>  TK_SEM
  // FunCallStmt.children = concatList( outputParameters.node, TK_FUNID.node, inputParameters.node)
  {
    root->noChild = 3;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[2]->node;
    temp[2] = root->children[5]->node;
    free(root->children[1]);
    free(root->children[3]);
    free(root->children[4]);
    free(root->children[6]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 42:// <outputParameters> ==> TK_SQL <idList> TK_SQR TK_ASSIGNOP
  // OutputParameters.children = concatList( idList.children)
  // free(idlist)
  {
    root->noChild = root->children[1]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    for(i=0;i<root->children[1]->noChild;i++)
      temp[i] = root->children[1]->children[i];

    free(root->children[0]);
    free(root->children[2]);
    free(root->children[3]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 43://<outputParameters> ==> eps
  // OutputParameters.children=NULL
  {
    root->noChild=0;
    root->children[0]=NULL;
    free(root->children);
    root->children=NULL;
    break;
  }
  case 44://<inputParameters> ===> TK_SQL <idList> TK_SQR
  // InputParameters.children = idlist.children
  // free(idlist)
  {
    root->noChild = root->children[1]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    for(i=0;i<root->children[1]->noChild;i++)
      temp[i] = root->children[1]->children[i];

    free(root->children[0]);
    free(root->children[2]);
    free(root->children);
    root->children = temp;
    break;
  }

  case 45://. <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
  // IterativeStmt.children = concatList( booleanExpression.node,stmt.node,  otherStmts.children)
  // free(stmt)
  // free(booleanexpression)
  {
    root->noChild = 2+root->children[5]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[2]->node;
    temp[1] = root->children[4]->node;
    for(i=0;i<root->children[5]->noChild;i++)
      temp[i+2] = root->children[5]->children[i];

    free(root->children[0]);
    free(root->children[1]);
    free(root->children[2]);
    free(root->children[3]);
    free(root->children[4]);
    free(root->children[6]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 46:// <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <B1>
  // Conditionalstmt.children = concatlist( booleanExpression.node,stmt.node, otherStmts.children,b1.node)
  // free(stmt)
  // free(booleanexpression)
  {
    root->noChild = 3+root->children[6]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[2]->node;

    temp[1] = root->children[5]->node;
    for(i=0;i<root->children[6]->noChild;i++)
      temp[i+2] = root->children[6]->children[i];
    temp[i+2] = root->children[7]->node;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children[2]);
    free(root->children[3]);
    free(root->children[4]);
    free(root->children[6]);
    free(root->children);
    root->children = temp;
    //printTree(root,NULL,"pareting");
    //exit(0);
    break;
  }
  case 47://<B1> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
  // B1.children = concatList(stmt.node, otherstmts.children)
  // free(stmt)
  {
    root->noChild = 1+root->children[2]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[1]->node;
    for(i=0;i<root->children[2]->noChild;i++)
      temp[i+1] = root->children[2]->children[i];

    free(root->children[0]);
    free(root->children[1]);
    free(root->children[3]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 48://<B1> ===> TK_ENDIF
  // B1.children=NULL
  {
    root->noChild=0;
    free(root->children[0]);
    free(root->children);
    root->children=NULL;
    break;
  }
  case 49:// <ioStmt> ===> TK_READ TK_OP <singleOrRecId> TK_CL TK_SEM
  // Iostmt.children = concatList(read.node, singleOrrecid.node)
  {
    root->noChild = 2;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[2]->node;

    free(root->children[1]);
    free(root->children[3]);
    free(root->children[4]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 50:// <ioStmt> ===>  TK_WRITE TK_OP <allVar> TK_CL TK_SEM
  // Iostmt.children = concatList(write.node, allvar.node)
  {
    root->noChild = 2;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[2]->node;
    free(root->children[1]);
    free(root->children[3]);
    free(root->children[4]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 51:// <arithmeticExpression> ===> <term> <ex2>
  {

    root->node = root->children[1]->syn;
    break;
  }
  case 52:// <ex2> ===> <op1> <term> <ex2>
  {
    root->syn=root->children[2]->syn;
    break;
  }
  case 53:// <ex2> ===> eps
  {
    root->syn=root->inh;
    break;
  }
  case 54:// <term> ===> <factor> <term2>
  {
    root->node=root->children[1]->syn;
    break;
  }
  case 55:// <term2> ===> <op2> <factor> <term2>
  {
    root->syn=root->children[2]->syn;
    break;
  }
  case 56:// <term2> ===> eps
{
  root->syn=root->inh;
  break;
}

case 57: //<factor> ===> <allVar>
  // Factor.node=allvar.node
  // free(Allavr)
  {
    root->node = root->children[0]->node;
    free(root->children[0]);
    free(root->children);
    break;
  }
  case 58:// <factor> ===> TK_MINUS <factor1>
  // Factor.node= tk_minus.node
  // tk_minus.children = factor1.node
  // free(factor1)
  {
    root->node=root->children[0]->node;
    root->children[0]->noChild=1;
    root->children[0]->node->children = malloc(sizeof(parseTree*));
    root->children[0]->node->children[0] = root->children[1]->node;
    free(root->children[1]);
    break;
  }
  case 59:// <factor> ===> TK_OP <arithmeticExpression> TK_CL
  // Factor.node=arithmetcexpression.node
  // free( arithmetcexpression)
  {
    root->node=  root->children[1]->node;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children[2]);
    free(root->children);
    break;
  }

  case 60: // <op1> ===> TK_MINUS
  // Op1.node=tk_minus.node
  {
    root->node = root->children[0]->node;
    break;
  }
  case 61: // <op1> ===> TK_PLUS
  // Op1.node=tk_plus.node
  {
    root->node = root->children[0]->node;
    break;
  }
  case 62: //<op2> ===> TK_MUL
  // Op2.node=tk_mul.node
  {
    root->node = root->children[0]->node;
    break;
  }

  case 63:// <op2> ===> TK_DIV
  // Op2.node=tk_div.node
  {
    root->node = root->children[0]->node;
    break;
  }
  case 64:// <booleanExpression> ===> TK_OP  <booleanExpression1> TK_CL <logicalOp> TK_OP <booleanExpression2> TK_CL
  // booleanExpression.node= logicalOp.node
  // logicalOp.children = concatList( booleanExpression1.node, booleanExpression2.node)
  // free(logicalOp)
  {
    root->node = root->children[3]->node;

    parseTree** temp = malloc(sizeof(parseTree*)*2);
    temp[0] = root->children[1]->node;
    temp[1] = root->children[5]->node;
    root->node->children=temp;
    root->node->noChild=2;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children[2]);
    free(root->children[3]);
    free(root->children[4]);
    free(root->children[5]);
    free(root->children[6]);
    free(root->children);
    //root->children = temp;
    break;
  }

  case 65: //<booleanExpression> ===> <allVar> <relationalOp> <allVar>
  // BooleanExpression.node=  relationalOp.node
  // relationalOp.children = concatList(allvar1.node,allvar2.node)
  // free(Allvar)
  // free(allvar)
  // free(relationalop)
  {
    root->noChild=0;
    root->children[1]->node->noChild = 2;
    root->node = root->children[1]->node;
    parseTree** temp = malloc(sizeof(parseTree*)*2);
    temp[0] = root->children[0]->node;
    temp[1] = root->children[2]->node;
    root->node->children=temp;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children[2]);
    free(root->children);
    break;
  }
  case 66: // <booleanExpression> ===> TK_NOT TK_OP <booleanExpression1>TK_CL
  // BooleanExpression.node=tk_not.node
  // tk_not.children= (booleanExpression1.node)
  // free( booleanExpression1)
  {
    root->noChild=0;
    root->children[0]->noChild = 1;
    root->node = root->children[0]->node;
    parseTree** temp = malloc(sizeof(parseTree*)*1);
    temp[0] = root->children[2]->node;
    root->node->children=temp;

    free(root->children[1]);
    free(root->children[2]);
    free(root->children[3]);
    free(root->children);
    break;
  }
  case 67:// <allVar> ===> TK_NUM
  // Allvar.node=tk_num.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }
  case 68:// <allVar> ===> TK_RNUM
  // Allvar.node=tk_rnum.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 69:// <allVar> ===> <singleOrRecId>
  // Allvar.node = singleorrecid.node
  {
    root->node = root->children[0]->node;
   // printf("69");
   // printTree(root->node,NULL,"parenting");
    free(root->children);
    break;
  }

  case 70:// <logicalOp> ===> TK_AND
  // LogicalOp.node= tk_and.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 71:// <logicalOp> ===> TK_OR
  // LogicalOp.node= tk_or.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 72:// <relationalOp> ===> TK_LT
  // Relationop.node=tk_lt.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 73:// <relationalOp> ===> TK_LE
  // Relationop.node=tk_le.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 74:// <relationalOp> ===> TK_EQ
  // Relationop.node=tk_eq.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 75:// <relationalOp> ===> TK_GT
  // Relationop.node=tk_gt.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 76:// <relationalOp> ===> TK_GE
  // Relationop.node=tk_ge.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 77:// <relationalOp> ===> TK_NE
  // Relationop.node=tk_ne.node
  {
    root->node = root->children[0]->node;
    free(root->children);
    break;
  }

  case 78:// <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
  // ReturnStmt.children  =  optionalReturn.children
  // free(optionalreturn)
  {
    parseTree** temp = root->children[1]->children;
    root->noChild=root->children[1]->noChild;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children[2]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 79: // <optionalReturn> ===> TK_SQL <idList> TK_SQR
  // OptionalReturn.children=idlist.children
  // free(idlist)
  {
    parseTree** temp = root->children[1]->children;
    root->noChild=root->children[1]->noChild;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children[2]);
    free(root->children);
    root->children = temp;

   // printf("completing 79\n");
    break;
  }
  case 80:// <optionalReturn> ===> eps
  // OptionalReturn.children=NULL
  {
    root->noChild=0;
    root->children[0]=NULL;
    free(root->children[0]);
    free(root->children);
    root->children=NULL;
    break;
  }
  case 81:// <idList> ===> TK_ID <more_ids>
  // Idlist.children = concatList(TK_ID.node, moreid.children)
  // free(moreid)
  {
    root->noChild = 1+root->children[1]->noChild;
    parseTree** temp = malloc(sizeof(parseTree*)*root->noChild);
    temp[0] = root->children[0]->node;
    for(i=0;i<root->children[1]->noChild;i++)
      temp[i+1] = root->children[1]->children[i];

    free(root->children[1]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 82:// <more_ids> ===> TK_COMMA <idList>
  // Moreids.children=idlist.children
  // free(idlist)
  {
    parseTree** temp = root->children[1]->children;
    root->noChild=root->children[1]->noChild;
    free(root->children[0]);
    free(root->children[1]);
    free(root->children);
    root->children = temp;
    break;
  }
  case 83:// <more_ids> ===> eps
  // Moreids.children=NULL
  {
    root->noChild=0;
    root->children[0]=NULL;
    free(root->children[0]);
    free(root->children);
    root->children=NULL;
    break;
  }

}//end of switch statement
  return root;
}
