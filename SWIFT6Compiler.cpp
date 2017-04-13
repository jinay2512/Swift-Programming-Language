//-----------------------------------------------------------
// JINAY JANI
// SWIFT6 Compiler Assignment #11
// SWIFT6Compiler.cpp
//-----------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>

using namespace std;

#define CALLBACKSUSED 2

//#define TRACEREADER
//#define TRACESCANNER
//#define TRACEPARSER
//#define	TRACEIDENTIFIERTABLE
#define TRACECOMPILER

const int SOURCELINELENGTH        = 512;
const int LOOKAHEAD               =   2;
const int LINESPERPAGE            =  60;
const int MAXIMUMLENGTHIDENTIFIER =  64;
const int MAXIMUMIDENTIFIERS      = 500;

#include ".\SWIFT.h"

/*-----------------------------------------------------------
<SWIFTProgram> 			::= <variableDefinitions>
							{ <PROCEDUREDefinition> }*
							<ProgramDefinition> EOPC
							
<variableDefinitions> ::= { VAR <identifier> : <datatype> { , <identifier> : <datatype> }* ; }*

<datatype>            ::= INT | BOOL

<ProgramDefinition> 	::=	BEGIN
							<variableDefinitions>
                             {  <statement>  } *
							ENDP

<statement>				::=	<PrintStatement>
							| <InputStatement>
							| <assignmentStatement>
							| <IfStatement>
							| <DoWhileStatement>
							| <ForStatment>
							| <CallStatement
							| <ReturnStatement>

<PrintStatement>		::=	WRITE _[_ ((<string> | <expession> | ENDL))
							{ , ((<string | <expession> | ENDL)) } * _]_ ;

<InputStatement> 	    ::= INPUT [ <string> ] <variable> ;

<assignmentStatement> 	::= <variable> { , <variable> }* := <expression> ;

<IFStatemnet>			::=	IF ( <expression> ) THEN
                             { <statement> }*
                        { ELSEIF ( <expression> ) THEN
                             { <statement> }* }*
                        [ ELSE
                             { <statement> }* ]
                          ENDIF

<DoWhileStatement>    ::= DO
                             { <statement> }*
                          WHILE ( <expression> )
                             { <statement> }*
                          ENDDOWHILE

<ForStatement>        ::= FOR <variable> := <expression> TO <expression> [ BY <expression> ]
                             { <statement> }*
                          ENDFOR
						
<CallStatement>       ::= CALL <identifier> [ ( (( <expression> | <variable> )) 
                                            { , (( <expression> | <variable> )) }* ) ] ;

<ReturnStatement>     ::= RETURN ;
							
<expression>			::=	<conjunction> { (( OR | NOR | XOR )) <conjunction> }*

<conjunction>			::=	<negation> { (( AND | NAND )) <negation> }*

<negation>				::= [ NOT ] <comparison>

<comparison>			::=	<comparator> [ (( < | <= | = | > | >= |!= )) <comparator>]

<comparator>			::=	<term> { (( + | - )) <term> }*

<term>					::= <factor> { (( * | / | % )) <factor> }*

<factor>				::= [ (( ABS | + | - )) ] <secondary>

<secondary>				::= <primary> [ (( ^ )) <primary> ]

<primary>				::= <variable> | <integer> | <boolean> | ( <expression> )

<variable>            	::= <identifier>

<identifier>          	::= <letter> { (( <letter> | <digit> | _ )) }

<integer>				::= <digit> { <digit> }*

<digit>					::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

<boolean>				::= true | false

<string>				::=	"{  <ASCIICharacters>  }* "

<ASCIICharacters>		::=	|| All ASCIICharacters in range [32,126]

<Comments>				::=	@  {  <ASCIICharacters>  } * EOLC
							|  |> { (( <ASCIICharacter> | EOLC )) }* <|

-----------------------------------------------------------*/

//-----------------------------------------------------------
typedef enum
//-----------------------------------------------------------
{
// pseudo-terminals
   IDENTIFIER,
   INTEGER,
   STRING,
   EOPTOKEN,
   UNKTOKEN,
// reserved words
   BEGIN,
   ENDP,
   WRITE,
   ENDL,
   OR,
   NOR,
   XOR,
   AND,
   NAND,
   NOT,
   TRUE,
   FALSE,
   VAR,
   INT,
   BOOL,
   INPUT,
   DO,
   WHILE,
   IF,
   THEN,
   ELSEIF,
   ELSE,
   FOR,
   TO,
   BY,
   PROCEDURE,
   IN,
   OUT,
   IO,
   REF,
   CALL,
   RETURN,
   END,
   ENDIF,
   ENDDOWHILE,
   ENDFOR,
// punctuation
   COMMA,
   SEMICOLON,
   OPENBRACKET,
   CLOSEBRACKET,
   OPENPAREN,
   CLOSEPAREN,
   COLON,
   COLONEQL,
// operators
   LESSTHEN,	// < 
   LESSTHENEQL, // <=
   EQUAL,		// =
   GREATERTHEN, // >
   GTEQUAL,		// >=
   NOTEQL,		// !=
   PLUS,		// +
   MINUS,		// -
   MULTIPLY,	// *
   DIVIDE,		// /
   MODULUS,		// %
   ABS,			// |x|
   POWER 		// ^
} TOKENTYPE;

//-----------------------------------------------------------
struct TOKENTABLERECORD
//-----------------------------------------------------------
{
   TOKENTYPE type;
   char description[12+1];
   bool isReservedWord;
};

//-----------------------------------------------------------
const TOKENTABLERECORD TOKENTABLE[] =
//-----------------------------------------------------------
{
   { IDENTIFIER  	,"IDENTIFIER"  		,false },
   { INTEGER     	,"INTEGER"     		,false },
   { STRING      	,"STRING"      		,false },
   { EOPTOKEN    	,"EOPTOKEN"    		,false },
   { UNKTOKEN    	,"UNKTOKEN"    		,false },
   { BEGIN       	,"BEGIN"       		,true  },
   { ENDP        	,"ENDP"        		,true  },
   { WRITE       	,"WRITE"       		,true  },
   { ENDL        	,"ENDL"        		,true  },
   { OR          	,"OR"          		,true  },
   { NOR         	,"NOR"         		,true  },
   { XOR         	,"XOR"         		,true  },
   { AND         	,"AND"         		,true  },
   { NAND        	,"NAND"        		,true  },
   { NOT         	,"NOT"         		,true  },
   { TRUE        	,"TRUE"        		,true  },
   { FALSE       	,"FALSE"       		,true  },
   { VAR         	,"VAR" 		        ,true  },
   { INT         	,"INT"     		 	,true  },
   { BOOL        	,"BOOL"    		    ,true  },
   { INPUT       	,"INPUT"       		,true  },
   { DO    	      	,"DO"      		    ,true  },
   { WHILE       	,"WHILE"       		,true  },
   { IF          	,"IF"          		,true  },
   { THEN        	,"THEN"        		,true  },
   { ELSEIF        	,"ELSEIF"        	,true  },
   { ELSE        	,"ELSE"        		,true  },
   { FOR         	,"FOR"         		,true  },
   { TO          	,"TO"          		,true  },
   { BY          	,"BY"	          	,true  },
   { PROCEDURE   	,"PROCEDURE"   		,true  },
   { IN          	,"IN"          		,true  },
   { OUT         	,"OUT"         		,true  },
   { IO          	,"IO"          		,true  },
   { REF         	,"REF"         		,true  },
   { CALL        	,"CALL"        		,true  },
   { RETURN      	,"RETURN"      		,true  },
   { END        	,"END"       		,true  },
   { ENDIF        	,"ENDIF"       		,true  },
   { ENDDOWHILE  	,"ENDDOWHILE"		,true  },
   { ENDFOR        	,"ENDFOR"        	,true  },
   { COMMA       	,"COMMA"       		,false },
   { SEMICOLON   	,"SEMICOLON"   		,false },
   { OPENBRACKET 	,"OPENBRACKET" 		,false },
   { CLOSEBRACKET 	,"CLOSEBRACKET"		,false },
   { OPENPAREN		,"OPENPAREN"		,false },
   { CLOSEPAREN		,"CLOSEPAREN"		,false },
   { COLON	       	,"COLON"	        ,false },
   { COLONEQL     	,"COLONEQL"    		,false },
   { LESSTHEN    	,"LESSTHEN"    		,false },
   { LESSTHENEQL 	,"LESSTHENEQL" 		,false },
   { EQUAL       	,"EQUAL"       		,false },
   { GREATERTHEN 	,"GREATERTHEN" 		,false },
   { GTEQUAL 		,"GTEQUAL" 			,false },
   { NOTEQL       	,"NOTEQL"       	,false },
   { PLUS        	,"PLUS"        		,false },
   { MINUS       	,"MINUS"       		,false },
   { MULTIPLY    	,"MULTIPLY"    		,false },
   { DIVIDE      	,"DIVIDE"      		,false },
   { MODULUS     	,"MODULUS"     		,false },
   { ABS         	,"ABS"         		,true  },
   { POWER       	,"POWER"       		,false }
};

//-----------------------------------------------------------
struct TOKEN
//-----------------------------------------------------------
{
   TOKENTYPE type;
   char lexeme[SOURCELINELENGTH+1];
   int sourceLineNumber;
   int sourceLineIndex;
};

//--------------------------------------------------
// Global variables
//--------------------------------------------------
READER<CALLBACKSUSED> reader(SOURCELINELENGTH,LOOKAHEAD);
LISTER lister(LINESPERPAGE);
CODE code;
IDENTIFIERTABLE identifierTable(&lister,MAXIMUMIDENTIFIERS);

#ifdef TRACEPARSER
int level;
#endif

//-----------------------------------------------------------
void EnterModule(char module[])
//-----------------------------------------------------------
{
#ifdef TRACEPARSER
   char information[SOURCELINELENGTH+1];

   level++;
   sprintf(information,"   %*s>%s",level*2," ",module);
   lister.ListInformationLine(information);
#endif
}

//-----------------------------------------------------------
void ExitModule(char module[])
//-----------------------------------------------------------
{
#ifdef TRACEPARSER
   char information[SOURCELINELENGTH+1];

   sprintf(information,"   %*s<%s",level*2," ",module);
   lister.ListInformationLine(information);
   level--;
#endif
}

//--------------------------------------------------
void ProcessCompilerError(int sourceLineNumber,int sourceLineIndex,char errorMessage[])
//--------------------------------------------------
{
   char information[SOURCELINELENGTH+1];

// Use "panic mode" error recovery technique: report error message and terminate compilation!
   sprintf(information,"     At (%4d:%3d) %s",sourceLineNumber,sourceLineIndex,errorMessage);
   lister.ListInformationLine(information);
   lister.ListInformationLine("SWIFT compiler ending with compiler error!\n");
   throw( SWIFTEXCEPTION("SWIFT compiler ending with compiler error!") );
}

//-----------------------------------------------------------
int main()
//-----------------------------------------------------------
{
   void Callback1(int sourceLineNumber,char sourceLine[]);
   void Callback2(int sourceLineNumber,char sourceLine[]);
   void GetNextToken(TOKEN tokens[]);
   void ParseSWIFTProgram(TOKEN tokens[]);

   char sourceFileName[80+1];
   TOKEN tokens[LOOKAHEAD+1];
   char line[SOURCELINELENGTH+1];
   
   cout << "Source filename? ";
   cin >> sourceFileName;

   try
   {
      lister.OpenFile(sourceFileName);

// CODEGENERATION
      code.OpenFile(sourceFileName);

      code.EmitUnformattedLine(";--------------------------------------------------------------");
      sprintf(line,"; %s.stm",sourceFileName); code.EmitUnformattedLine(line);
      code.EmitUnformattedLine(";--------------------------------------------------------------");
      code.EmitUnformattedLine("; SVC numbers");
      code.EmitFormattedLine("SVC_DONOTHING"      ,"EQU","0D0","force context switch");
      code.EmitFormattedLine("SVC_TERMINATE"      ,"EQU","0D1");
      code.EmitFormattedLine("SVC_READ_INTEGER"   ,"EQU","0D10");
      code.EmitFormattedLine("SVC_WRITE_INTEGER"  ,"EQU","0D11");
      code.EmitFormattedLine("SVC_READ_FLOAT"     ,"EQU","0D20");
      code.EmitFormattedLine("SVC_WRITE_FLOAT"    ,"EQU","0D21");
      code.EmitFormattedLine("SVC_READ_BOOLEAN"   ,"EQU","0D30");
      code.EmitFormattedLine("SVC_WRITE_BOOLEAN"  ,"EQU","0D31");
      code.EmitFormattedLine("SVC_READ_CHARACTER" ,"EQU","0D40");
      code.EmitFormattedLine("SVC_WRITE_CHARACTER","EQU","0D41");
      code.EmitFormattedLine("SVC_WRITE_ENDL"     ,"EQU","0D42");
      code.EmitFormattedLine("SVC_READ_STRING"    ,"EQU","0D50");
      code.EmitFormattedLine("SVC_WRITE_STRING"   ,"EQU","0D51");
      code.EmitFormattedLine("SVC_INITIALIZE_HEAP","EQU","0D90");
      code.EmitFormattedLine("SVC_ALLOCATE_BLOCK" ,"EQU","0D91");
      code.EmitFormattedLine("SVC_FREE_BLOCK"     ,"EQU","0D92");
      code.EmitUnformattedLine("");
      code.EmitFormattedLine(""                   ,"ORG","0X0000");
      code.EmitUnformattedLine("");
      code.EmitFormattedLine(""                   ,"JMP","PROGRAMMAIN");
// ENDCODEGENERATION

      reader.SetLister(&lister);
      reader.AddCallbackFunction(Callback1);
      reader.AddCallbackFunction(Callback2);
      reader.OpenFile(sourceFileName);

   // Fill tokens[] and lexemes[][] for look-ahead
      for (int i = 0; i <= LOOKAHEAD; i++)
         GetNextToken(tokens);

#ifdef TRACEPARSER
      level = 0;
#endif
   
      ParseSWIFTProgram(tokens);
   }
   catch (SWIFTEXCEPTION swiftEXCEPTION)
   {
      cout << "SWIFT exception: " << swiftEXCEPTION.GetDescription() << endl;
   }
   lister.ListInformationLine("******* SWIFT4 Compiler ending");
   cout << "SWIFT4 Compiler ending\n";

   system("PAUSE");
   return( 0 );
   
}

//-----------------------------------------------------------
void ParseSWIFTProgram(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseVariableDefinitions(TOKEN tokens[],IDENTIFIERTYPE identifierType);
   void ParseProcedureDefinition(TOKEN tokens[]);
   void ParseProgramDefinition(TOKEN tokens[]);
   void GetNextToken(TOKEN tokens[]);
   
   EnterModule("SWIFTProgram");
   
   if ( tokens[0].type == VAR )
      ParseVariableDefinitions(tokens,GLOBAL_VARIABLE);

#ifdef TRACECOMPILER
   identifierTable.DisplayTableContents("Contents of identifier table after compilation of global variable definitions");
#endif

	while ( tokens[0].type == PROCEDURE )
      ParseProcedureDefinition(tokens);

   if ( tokens[0].type == BEGIN )
      ParseProgramDefinition(tokens);
   else
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                           "Expecting BEGIN");

   if ( tokens[0].type != EOPTOKEN )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                           "Expecting end-of-program");

   ExitModule("SWIFTProgram");
}

//-----------------------------------------------------------
void ParseProcedureDefinition(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseFormalParameter(TOKEN tokens[],IDENTIFIERTYPE &identifierType,int &n);
   void ParseVariableDefinitions(TOKEN tokens[],IDENTIFIERTYPE identifierType);
   void ParseStatement(TOKEN tokens[]);
   void GetNextToken(TOKEN tokens[]);

   bool isInTable;
   char line[SOURCELINELENGTH+1];
   int index;
   char reference[SOURCELINELENGTH+1];
   int n,m;
   char label[SOURCELINELENGTH+1],operand[SOURCELINELENGTH+1],comment[SOURCELINELENGTH+1];

   EnterModule("ProcedureDefinition");

   GetNextToken(tokens);

   if ( tokens[0].type != IDENTIFIER )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting identifier");

   index = identifierTable.GetIndex(tokens[0].lexeme,isInTable);
   if ( identifierTable.IsInScope(index) )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Multiply-defined identifier");

   identifierTable.AddToTable(tokens[0].lexeme,PROCEDURE_MODULE,NOTYPE,tokens[0].lexeme);

// CODEGENERATION
   code.EnterModuleBody(PROCEDURE_MODULE,index);
   code.ResetFrameData();
   code.EmitUnformattedLine("; **** =========");
   sprintf(line,"; **** PROCEDURE module (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);
   code.EmitUnformattedLine("; **** =========");
   code.EmitFormattedLine(tokens[0].lexeme,"EQU","*");
// ENDCODEGENERATION

   identifierTable.EnterNestedStaticScope();

   GetNextToken(tokens);
   n = 0;
   if ( tokens[0].type == OPENPAREN )
   {
      do
      {
         IDENTIFIERTYPE identifierType;

         GetNextToken(tokens);
         ParseFormalParameter(tokens,identifierType,n);
      } while ( tokens[0].type == COMMA );

      if ( tokens[0].type != CLOSEPAREN )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ')'");
      GetNextToken(tokens);
   }

// CODEGENERATION
   code.IncrementFBOffset(2);
// ENDCODEGENERATION

   if ( tokens[0].type == VAR )
      ParseVariableDefinitions(tokens,MODULE_VARIABLE);

// CODEGENERATION
   m = code.GetFBOffset()-(n+2);
   code.EmitFormattedLine("","PUSHSP","","set PROCEDURE module FB = SP-on-entry + 2(n+2)");
   sprintf(operand,"#0D%d",2*(n+2));
   sprintf(comment,"n = %d",n);
   code.EmitFormattedLine("","PUSH",operand,comment);
   code.EmitFormattedLine("","ADDI");
   code.EmitFormattedLine("","POPFB");
   code.EmitFormattedLine("","PUSHSP","","PROCEDURE module SP = SP-on-entry + 2m");
   sprintf(operand,"#0D%d",2*m);
   sprintf(comment,"m = %d",m);
   code.EmitFormattedLine("","PUSH",operand,comment);
   code.EmitFormattedLine("","SUBI");
   code.EmitFormattedLine("","POPSP");
   code.EmitUnformattedLine("; statements to initialize frame data (if necessary)");
   code.EmitFrameData();
   sprintf(label,"MODULEBODY%04d",code.LabelSuffix());
   code.EmitFormattedLine("","CALL",label);
   code.EmitFormattedLine("","PUSHFB","","restore caller's SP-on-entry = FB - 2(n+2)");
   sprintf(operand,"#0D%d",2*(n+2));
   code.EmitFormattedLine("","PUSH",operand);
   code.EmitFormattedLine("","SUBI");
   code.EmitFormattedLine("","POPSP");
   code.EmitFormattedLine("","RETURN","","return to caller");
   code.EmitUnformattedLine("");
   code.EmitFormattedLine(label,"EQU","*");
   code.EmitUnformattedLine("; statements in body of PROCEDURE module (may include RETURN)");
// ENDCODEGENERATION

   while ( tokens[0].type != END )
      ParseStatement(tokens);

// CODEGENERATION
   code.EmitFormattedLine("","RETURN");
   code.EmitUnformattedLine("");
   code.EmitUnformattedLine("; **** =========");
   sprintf(line,"; **** END (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);
   code.EmitUnformattedLine("; **** =========");
   code.ExitModuleBody();
// ENDCODEGENERATION

   identifierTable.ExitNestedStaticScope();

#ifdef TRACECOMPILER
   identifierTable.DisplayTableContents("Contents of identifier table at end of compilation of PROCEDURE module definition");
#endif

   GetNextToken(tokens);

   ExitModule("ProcedureDefinition");
}

//-----------------------------------------------------------
void ParseFormalParameter(TOKEN tokens[],IDENTIFIERTYPE &identifierType,int &n)
//-----------------------------------------------------------
{
   void GetNextToken(TOKEN tokens[]);

   char lexeme[MAXIMUMLENGTHIDENTIFIER+1],reference[MAXIMUMLENGTHIDENTIFIER+1];
   bool isInTable;
   int index;
   DATATYPE datatype;

   EnterModule("FormalParameter");

// CODEGENERATION
   switch ( tokens[0].type )
   {
      case IN:
         identifierType = IN_PARAMETER;
         sprintf(reference,"FB:0D%d",code.GetFBOffset());
         code.IncrementFBOffset(1);
         n += 1;
         GetNextToken(tokens);
         break;
      case OUT:
         identifierType = OUT_PARAMETER;
         code.IncrementFBOffset(1);
         sprintf(reference,"FB:0D%d",code.GetFBOffset());
         code.IncrementFBOffset(1);
         n += 2;
         GetNextToken(tokens);
         break;
      case IO:
         identifierType = IO_PARAMETER;
         code.IncrementFBOffset(1);
         sprintf(reference,"FB:0D%d",code.GetFBOffset());
         code.IncrementFBOffset(1);
         n += 2;
         GetNextToken(tokens);
         break;
      case REF:
         identifierType = REF_PARAMETER;
         sprintf(reference,"@FB:0D%d",code.GetFBOffset());
         code.IncrementFBOffset(1);
         n += 1;
         GetNextToken(tokens);
         break;
      default:
         identifierType = IN_PARAMETER;
         sprintf(reference,"FB:0D%d",code.GetFBOffset());
         code.IncrementFBOffset(1);
         n += 1;
         break;
   }
// ENDCODEGENERATION

   if ( tokens[0].type != IDENTIFIER )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting identifier");
   strcpy(lexeme,tokens[0].lexeme);
   GetNextToken(tokens);

   if ( tokens[0].type != COLON )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ':'");
   GetNextToken(tokens);

   switch ( tokens[0].type )
   {
      case INT:
         datatype = INTEGERTYPE;
         break;
      case BOOL:
         datatype = BOOLEANTYPE;
         break;
      default:
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting INT or BOOL");
   }
   GetNextToken(tokens);

   index = identifierTable.GetIndex(lexeme,isInTable);
   if ( isInTable && identifierTable.IsInScope(index) )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Multiply-defined identifier");

   identifierTable.AddToTable(lexeme,identifierType,datatype,reference);

   ExitModule("FormalParameter");
}

//-----------------------------------------------------------
void ParseVariableDefinitions(TOKEN tokens[],IDENTIFIERTYPE identifierType)
//-----------------------------------------------------------
{
   void GetNextToken(TOKEN tokens[]);

   EnterModule("VariableDefinitions");

   while ( tokens[0].type == VAR )
   {
      do
      {
         char lexeme[MAXIMUMLENGTHIDENTIFIER+1];
         DATATYPE datatype;
         char reference[MAXIMUMLENGTHIDENTIFIER+1];
         bool isInTable,isInCurrentScope;
         int index;

         GetNextToken(tokens);
   
         if ( tokens[0].type != IDENTIFIER )
            ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting identifier");
         strcpy(lexeme,tokens[0].lexeme);
         GetNextToken(tokens);
   
         if ( tokens[0].type != COLON )
            ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ':'");
         GetNextToken(tokens);
   
         switch ( tokens[0].type )
         {
            case INT:
               datatype = INTEGERTYPE;
               break;
            case BOOL:
               datatype = BOOLEANTYPE;
               break;
            default:
               ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting INT or BOOL");
         }
         GetNextToken(tokens);
   
         index = identifierTable.GetIndex(lexeme,isInTable);
         if ( isInTable && identifierTable.IsInScope(index) )
            ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Multiply-defined identifier");

   // CODEGENERATION
      switch ( identifierType )
      {
         case GLOBAL_VARIABLE:
         case PROGRAM_VARIABLE:
            code.AddRWToStaticData(1,lexeme,reference);
            break;
         case MODULE_VARIABLE:
            sprintf(reference,"FB:0D%d",code.GetFBOffset());
            code.IncrementFBOffset(1);
            break;
      }
   // ENDCODEGENERATION
   
      identifierTable.AddToTable(lexeme,identifierType,datatype,reference);
         } while ( tokens[0].type == COMMA );

      if ( tokens[0].type != SEMICOLON )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ';'");
      GetNextToken(tokens);
   }

   ExitModule("VariableDefinitions");

}

//-----------------------------------------------------------
void ParseProgramDefinition(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseVariableDefinitions(TOKEN tokens[],IDENTIFIERTYPE identifierType);
   void ParseStatement(TOKEN tokens[]);
   void GetNextToken(TOKEN tokens[]);

   char line[SOURCELINELENGTH+1];
   char label[SOURCELINELENGTH+1];
   char reference[SOURCELINELENGTH+1];

   EnterModule("ProgramDefinition");
   
   GetNextToken(tokens);

   identifierTable.EnterNestedStaticScope();

   if ( tokens[0].type == VAR )
      ParseVariableDefinitions(tokens,PROGRAM_VARIABLE);

// CODEGENERATION
   code.EmitUnformattedLine("; **** =========");
   sprintf(line,"; **** PROGRAM module (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);
   code.EmitUnformattedLine("; **** =========");
   code.EmitFormattedLine("PROGRAMMAIN","EQU"  ,"*");
   code.EmitFormattedLine("","PUSH" ,"#RUNTIMESTACK","set SP");
   code.EmitFormattedLine("","POPSP");
   code.EmitFormattedLine("","PUSHA","STATICDATA","set SB");
   code.EmitFormattedLine("","POPSB");
   code.EmitFormattedLine("","PUSH","#HEAPBASE","initialize heap");
   code.EmitFormattedLine("","PUSH","#HEAPSIZE");
   code.EmitFormattedLine("","SVC","#SVC_INITIALIZE_HEAP");
   sprintf(label,"PROGRAMBODY%04d",code.LabelSuffix());
   code.EmitFormattedLine("","CALL",label);
   code.AddDSToStaticData("Normal program termination","",reference);
   code.EmitFormattedLine("","PUSHA",reference);
   code.EmitFormattedLine("","SVC","#SVC_WRITE_STRING");
   code.EmitFormattedLine("","SVC","#SVC_WRITE_ENDL");
   code.EmitFormattedLine("","PUSH","#0D0","terminate with status = 0");
   code.EmitFormattedLine("","SVC" ,"#SVC_TERMINATE");
   code.EmitUnformattedLine("");
   code.EmitFormattedLine(label,"EQU","*");
// ENDCODEGENERATION

   while ( tokens[0].type != ENDP )
      ParseStatement(tokens);

// CODEGENERATION
   code.EmitFormattedLine("","RETURN");

   code.EmitUnformattedLine("; **** =========");
   sprintf(line,"; **** END (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);
   code.EmitUnformattedLine("; **** =========");
   code.EmitUnformattedLine("");
   code.EmitUnformattedLine(";------------------------------------------------------------");
   code.EmitUnformattedLine("; Static allocation of PROGRAM module data and global data");
   code.EmitUnformattedLine(";------------------------------------------------------------");
   code.EmitFormattedLine("STATICDATA","EQU","*");
   code.EmitStaticData();
   code.EmitUnformattedLine("");
   code.EmitUnformattedLine(";------------------------------------------------------------");
   code.EmitUnformattedLine("; Heap space for dynamic memory allocation (to support future SPL syntax)");
   code.EmitUnformattedLine(";------------------------------------------------------------");
   code.EmitFormattedLine("HEAPBASE","EQU","*");
   code.EmitFormattedLine("HEAPSIZE","EQU","0B0001000000000000","8K bytes = 4K words");
   code.EmitUnformattedLine("");
   code.EmitUnformattedLine(";------------------------------------------------------------");
   code.EmitUnformattedLine("; Run-time stack");
   code.EmitUnformattedLine(";------------------------------------------------------------");
   code.EmitFormattedLine("RUNTIMESTACK","EQU","0XFFFE");
// ENDCODEGENERATION

#ifdef TRACECOMPILER
   identifierTable.DisplayTableContents("Contents of identifier table at end of compilation of PROGRAM module definition");
#endif

   identifierTable.ExitNestedStaticScope();

   GetNextToken(tokens);

   ExitModule("ProgramDefinition");
}

//-----------------------------------------------------------
void ParseStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParsePrintStatement(TOKEN tokens[]);
   void ParseInputStatement(TOKEN tokens[]);
   void ParseAssignmentStatement(TOKEN tokens[]);
   void ParseIfStatement(TOKEN tokens[]);
   void ParseDoWhileStatement(TOKEN tokens[]);
   void ParseForStatement(TOKEN token[]);
   void ParseCallStatement(TOKEN tokens[]);
   void ParseReturnStatement(TOKEN tokens[]);
   void GetNextToken(TOKEN tokens[]);
   
   EnterModule("Statement");

   switch ( tokens[0].type )
   {
      case WRITE:
         ParsePrintStatement(tokens);
         break;
	  case INPUT:
         ParseInputStatement(tokens);
         break;
      case IDENTIFIER:
         ParseAssignmentStatement(tokens);
         break;
      case IF:
         ParseIfStatement(tokens);
         break;
      case DO:
         ParseDoWhileStatement(tokens);
         break;
	  case FOR:
		ParseForStatement(tokens);
		break;
	  case CALL:
         ParseCallStatement(tokens);
         break;
      case RETURN:
         ParseReturnStatement(tokens);
         break;
      default:
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                              "Expecting beginning-of-statement");
         break;
   }

   ExitModule("Statement");
}

//-----------------------------------------------------------
void ParsePrintStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseExpression(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   char line[SOURCELINELENGTH+1];
   DATATYPE datatype;

   EnterModule("PrintStatement");

   sprintf(line,"; **** PRINT statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);
	
	GetNextToken(tokens);
	if ( tokens[0].type != OPENBRACKET )
        ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                             "Expecting [");
   do
   {
      GetNextToken(tokens);

      switch ( tokens[0].type )
      {
         case STRING:

// CODEGENERATION
            char reference[SOURCELINELENGTH+1];

            code.AddDSToStaticData(tokens[0].lexeme,"",reference);
            code.EmitFormattedLine("","PUSHA",reference);
            code.EmitFormattedLine("","SVC","#SVC_WRITE_STRING");
// ENDCODEGENERATION

            GetNextToken(tokens);
            break;
         case ENDL:

// CODEGENERATION
            code.EmitFormattedLine("","SVC","#SVC_WRITE_ENDL");
// ENDCODEGENERATION

            GetNextToken(tokens);
            break;
         default:
           {
            ParseExpression(tokens,datatype);

// CODEGENERATION
            switch ( datatype )
            {
               case INTEGERTYPE:
                  code.EmitFormattedLine("","SVC","#SVC_WRITE_INTEGER");
                  break;
               case BOOLEANTYPE:
                  code.EmitFormattedLine("","SVC","#SVC_WRITE_BOOLEAN");
                  break;
            }
// ENDCODEGENERATION
		   }
	  }		   
   } while ( tokens[0].type == COMMA );
   
   if ( tokens[0].type != CLOSEBRACKET )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                           "Expecting ]");
   GetNextToken(tokens);

   if ( tokens[0].type != SEMICOLON )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                           "Expecting ';'");

   GetNextToken(tokens);

   ExitModule("PrintStatement");
}

//-----------------------------------------------------------
void ParseInputStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseVariable(TOKEN tokens[],bool asLValue,DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   char reference[SOURCELINELENGTH+1];
   char line[SOURCELINELENGTH+1];
   DATATYPE datatype;

   EnterModule("InputStatement");

   sprintf(line,"; **** INPUT statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);

   GetNextToken(tokens);

   if ( tokens[0].type == STRING )
   {

// CODEGENERATION
      code.AddDSToStaticData(tokens[0].lexeme,"",reference);
      code.EmitFormattedLine("","PUSHA",reference);
      code.EmitFormattedLine("","SVC","#SVC_WRITE_STRING");
// ENDCODEGENERATION

      GetNextToken(tokens);
   }

   ParseVariable(tokens,true,datatype);

// CODEGENERATION
   switch ( datatype )
   {
      case INTEGERTYPE:
         code.EmitFormattedLine("","SVC","#SVC_READ_INTEGER");
         break;
      case BOOLEANTYPE:
         code.EmitFormattedLine("","SVC","#SVC_READ_BOOLEAN");
         break;
   }
   code.EmitFormattedLine("","POP","@SP:0D1");
   code.EmitFormattedLine("","DISCARD","#0D1");
// ENDCODEGENERATION

   if ( tokens[0].type != SEMICOLON )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ';'");

   GetNextToken(tokens);

   ExitModule("INPUTStatement");
}

//-----------------------------------------------------------
void ParseAssignmentStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseVariable(TOKEN tokens[],bool asLValue,DATATYPE &datatype);
   void ParseExpression(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   char line[SOURCELINELENGTH+1];
   DATATYPE datatypeLHS,datatypeRHS;
   int n;

   EnterModule("AssignmentStatement");

   sprintf(line,"; **** assignment statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);

   ParseVariable(tokens,true,datatypeLHS);
   n = 1;

   while ( tokens[0].type == COMMA )
   {
      DATATYPE datatype;

      GetNextToken(tokens);
      ParseVariable(tokens,true,datatype);
      n++;

      if ( datatype != datatypeLHS )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Mixed-mode variables not allowed");
   }
   if ( tokens[0].type != COLONEQL )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ':='");
   GetNextToken(tokens);

   ParseExpression(tokens,datatypeRHS);

   if ( datatypeLHS != datatypeRHS )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Data type mismatch");

// CODEGENERATION
   for (int i = 1; i <= n; i++)
   {
      code.EmitFormattedLine("","MAKEDUP");
      code.EmitFormattedLine("","POP","@SP:0D2");
      code.EmitFormattedLine("","SWAP");
      code.EmitFormattedLine("","DISCARD","#0D1");
   }
   code.EmitFormattedLine("","DISCARD","#0D1");
// ENDCODEGENERATION

   if ( tokens[0].type != SEMICOLON )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ';'");
   GetNextToken(tokens);

   ExitModule("AssignmentStatement");
}
//-----------------------------------------------------------
void ParseIfStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseExpression(TOKEN tokens[],DATATYPE &datatype);
   void ParseStatement(TOKEN tokens[]);
   void GetNextToken(TOKEN tokens[]);

   char line[SOURCELINELENGTH+1];
   char Ilabel[SOURCELINELENGTH+1],Elabel[SOURCELINELENGTH+1];
   DATATYPE datatype;

   EnterModule("IfStatement");

   sprintf(line,"; **** IF statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);

   GetNextToken(tokens);

   if ( tokens[0].type != OPENPAREN )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting '('");
   GetNextToken(tokens);
   ParseExpression(tokens,datatype);
   if ( tokens[0].type != CLOSEPAREN )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ')'");
   GetNextToken(tokens);
   if ( tokens[0].type != THEN )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting THEN");
   GetNextToken(tokens);

   if ( datatype != BOOLEANTYPE )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean expression");

// CODEGENERATION
/* 
   Plan for the generalized IF statement with n ELIFs and 1 ELSE (*Note* n
      can be 0 and the ELSE may be missing and the plan still "works.")

   ...expression...           ; boolean expression on top-of-stack
      SETT
      DISCARD   #0D1
      JMPNT     I???1
   ...statements...
      JMP       E????
I???1 EQU       *             ; 1st ELSEIF clause
   ...expression...
      SETT
      DISCARD   #0D1
      JMPNT     I???2
   ...statements...
      JMP       E????
      .
      .
I???n EQU       *             ; nth ELSEIF clause
   ...expression...
      SETT
      DISCARD   #0D1
      JMPNT     I????
   ...statements...
      JMP       E????
I???? EQU       *             ; ELSE clause
   ...statements...
E???? EQU       *
*/
   sprintf(Elabel,"E%04d",code.LabelSuffix());
   code.EmitFormattedLine("","SETT");
   code.EmitFormattedLine("","DISCARD","#0D1");
   sprintf(Ilabel,"I%04d",code.LabelSuffix());
   code.EmitFormattedLine("","JMPNT",Ilabel);
// ENDCODEGENERATION

   while ( (tokens[0].type != ELSEIF) && 
           (tokens[0].type != ELSE) && 
           (tokens[0].type != ENDIF) )
      ParseStatement(tokens);

// CODEGENERATION
   code.EmitFormattedLine("","JMP",Elabel);
   code.EmitFormattedLine(Ilabel,"EQU","*");
// ENDCODEGENERATION

   while ( tokens[0].type == ELSEIF )
   {
      GetNextToken(tokens);
      if ( tokens[0].type != OPENPAREN )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting '('");
      GetNextToken(tokens);
      ParseExpression(tokens,datatype);
      if ( tokens[0].type != CLOSEPAREN )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ')'");
      GetNextToken(tokens);
      if ( tokens[0].type != THEN )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting THEN");
      GetNextToken(tokens);

// CODEGENERATION
      code.EmitFormattedLine("","SETT");
      code.EmitFormattedLine("","DISCARD","#0D1");
      sprintf(Ilabel,"I%04d",code.LabelSuffix());
      code.EmitFormattedLine("","JMPNT",Ilabel);
// ENDCODEGENERATION

      while ( (tokens[0].type != ELSEIF) && 
              (tokens[0].type != ELSE) && 
              (tokens[0].type != ENDIF) )
         ParseStatement(tokens);

// CODEGENERATION
      code.EmitFormattedLine("","JMP",Elabel);
      code.EmitFormattedLine(Ilabel,"EQU","*");
// ENDCODEGENERATION

   }
   if ( tokens[0].type == ELSE )
   {
      GetNextToken(tokens);
      while ( tokens[0].type != ENDIF )
         ParseStatement(tokens);
   }

   GetNextToken(tokens);

// CODEGENERATION
      code.EmitFormattedLine(Elabel,"EQU","*");
// ENDCODEGENERATION

   ExitModule("IfStatement");
}

//-----------------------------------------------------------
void ParseDoWhileStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseExpression(TOKEN tokens[],DATATYPE &datatype);
   void ParseStatement(TOKEN tokens[]);
   void GetNextToken(TOKEN tokens[]);

   char line[SOURCELINELENGTH+1];
   char Dlabel[SOURCELINELENGTH+1],Elabel[SOURCELINELENGTH+1];
   DATATYPE datatype;

   EnterModule("DoWhileStatement");

   sprintf(line,"; **** DO-WHILE statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);

   GetNextToken(tokens);

// CODEGENERATION
/*
D???? EQU       *
   ...statements...
   ...expression...
      SETT
      DISCARD   #0D1
      JMPNT     E????
   ...statements...
      JMP       D????
E???? EQU       *
*/

   sprintf(Dlabel,"D%04d",code.LabelSuffix());
   sprintf(Elabel,"E%04d",code.LabelSuffix());
   code.EmitFormattedLine(Dlabel,"EQU","*");
// ENDCODEGENERATION

   while ( tokens[0].type != WHILE )
      ParseStatement(tokens);
   GetNextToken(tokens);
   if ( tokens[0].type != OPENPAREN )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting '('");
   GetNextToken(tokens);
   ParseExpression(tokens,datatype);
   if ( tokens[0].type != CLOSEPAREN )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ')'");
   GetNextToken(tokens);

   if ( datatype != BOOLEANTYPE )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean expression");

// CODEGENERATION
   code.EmitFormattedLine("","SETT");
   code.EmitFormattedLine("","DISCARD","#0D1");
   code.EmitFormattedLine("","JMPNT",Elabel);
// ENDCODEGENERATION

   while ( tokens[0].type != ENDDOWHILE )
      ParseStatement(tokens);

   GetNextToken(tokens);

// CODEGENERATION
   code.EmitFormattedLine("","JMP",Dlabel);
   code.EmitFormattedLine(Elabel,"EQU","*");
// ENDCODEGENERATION

   ExitModule("DoWhileStatement");
}

//-----------------------------------------------------------
void ParseForStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void ParseVariable(TOKEN tokens[],bool asLValue,DATATYPE &datatype);
   void ParseExpression(TOKEN tokens[],DATATYPE &datatype);
   void ParseStatement(TOKEN tokens[]);
   void GetNextToken(TOKEN tokens[]);

   char line[SOURCELINELENGTH+1];
   char Dlabel[SOURCELINELENGTH+1],Llabel[SOURCELINELENGTH+1],
        Clabel[SOURCELINELENGTH+1],Elabel[SOURCELINELENGTH+1];
   DATATYPE datatype;

   EnterModule("ForStatement");

   sprintf(line,"; **** FOR statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);

   GetNextToken(tokens);

   ParseVariable(tokens,true,datatype);

   if ( datatype != INTEGERTYPE )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer variable");

/*
; v := e1
   ...v...                    ; &v = run-time stack (bottom to top)
   ...e1...                   ; &v,e1
      POP       @SP:0D1       ; &v
   ...e2...                   ; &v,e2
   ...e3...                   ; &v,e2,e3
D???? SETNZPI
; if   ( e3 > 0 ) then
      JMPN      L????
      SWAP                    ; &v,e3,e2
      MAKEDUP                 ; &v,e3,e2,e2
      PUSH      @SP:0D3       ; &v,e3,e2,e2,v 
      SWAP                    ; &v,e3,e2,v,e2
;    if ( v <= e2 ) continue else end
      CMPI                    ; &v,e3,e2 (set LEG)
      JMPLE     C????
      JMP       E????  
; else ( e3 < 0 )
L???? SWAP                    ; &v,e3,e2
      MAKEDUP                 ; &v,e3,e2,e2
      PUSH      @SP:0D3       ; &v,e3,e2,e2,v 
      SWAP                    ; &v,e3,e2,v,e2
;    if ( v >= e2 ) continue else end
      CMPI                    ; &v,e3,e2 (set LEG)
      JMPGE     C????
      JMP       E????  
; endif
C???? EQU       *
   ...statements...
      SWAP                    ; &v,e2,e3
      MAKEDUP                 ; &v,e2,e3,e3
; v := e3+v
      PUSH      @SP:0D3       ; &v,e2,e3,e3,v
      ADDI                    ; &v,e2,e3,(e3+v)
      POP       @SP:0D3       ; &v,e2,e3
      JMP       D????
E???? DISCARD   #0D3          ; now run-time stack is empty
*/

   if ( tokens[0].type != COLONEQL )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ':='");
   GetNextToken(tokens);

   ParseExpression(tokens,datatype);
   if ( datatype != INTEGERTYPE )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer data type");

// CODEGENERATION
   code.EmitFormattedLine("","POP","@SP:0D1");
// ENDCODEGENERATION

   if ( tokens[0].type != TO )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting TO");
   GetNextToken(tokens);

   ParseExpression(tokens,datatype);
   if ( datatype != INTEGERTYPE )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer data type");

   if ( tokens[0].type == BY )
   {
      GetNextToken(tokens);

      ParseExpression(tokens,datatype);
      if ( datatype != INTEGERTYPE )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer data type");
   }
   else
   {

// CODEGENERATION
      code.EmitFormattedLine("","PUSH","#0D1");
// ENDCODEGENERATION

   }

// CODEGENERATION
   sprintf(Dlabel,"D%04d",code.LabelSuffix());
   sprintf(Llabel,"L%04d",code.LabelSuffix());
   sprintf(Clabel,"C%04d",code.LabelSuffix());
   sprintf(Elabel,"E%04d",code.LabelSuffix());
   code.EmitFormattedLine(Dlabel,"SETNZPI");
   code.EmitFormattedLine("","JMPN",Llabel);
   code.EmitFormattedLine("","SWAP");
   code.EmitFormattedLine("","MAKEDUP");
   code.EmitFormattedLine("","PUSH","@SP:0D3");
   code.EmitFormattedLine("","SWAP");
   code.EmitFormattedLine("","CMPI");
   code.EmitFormattedLine("","JMPLE",Clabel);
   code.EmitFormattedLine("","JMP",Elabel);
   code.EmitFormattedLine(Llabel,"SWAP");
   code.EmitFormattedLine("","MAKEDUP");
   code.EmitFormattedLine("","PUSH","@SP:0D3");
   code.EmitFormattedLine("","SWAP");
   code.EmitFormattedLine("","CMPI");
   code.EmitFormattedLine("","JMPGE",Clabel);
   code.EmitFormattedLine("","JMP",Elabel);
   code.EmitFormattedLine(Clabel,"EQU","*");
// ENDCODEGENERATION

   while ( tokens[0].type != ENDFOR )
      ParseStatement(tokens);

   GetNextToken(tokens);

// CODEGENERATION
   code.EmitFormattedLine("","SWAP");
   code.EmitFormattedLine("","MAKEDUP");
   code.EmitFormattedLine("","PUSH","@SP:0D3");
   code.EmitFormattedLine("","ADDI");
   code.EmitFormattedLine("","POP","@SP:0D3");
   code.EmitFormattedLine("","JMP",Dlabel);
   code.EmitFormattedLine(Elabel,"DISCARD","#0D3");
// ENDCODEGENERATION

   ExitModule("ForStatement");
}

//-----------------------------------------------------------
void ParseCallStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void GetNextToken(TOKEN tokens[]);
   void ParseVariable(TOKEN tokens[],bool asLValue,DATATYPE &datatype);
   void ParseExpression(TOKEN tokens[],DATATYPE &datatype);

   char line[SOURCELINELENGTH+1];
   bool isInTable;
   int index,p;

   EnterModule("CallStatement");

   sprintf(line,"; **** CALL statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);

   GetNextToken(tokens);

   if ( tokens[0].type != IDENTIFIER )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting identifier");

// STATICSEMANTICS
   index = identifierTable.GetIndex(tokens[0].lexeme,isInTable);
   if ( !isInTable )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Undefined identifier");
   if ( identifierTable.GetType(index) != PROCEDURE_MODULE )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting PROCEDURE identifier");
// ENDSTATICSEMANTICS

   GetNextToken(tokens);
   p = 0;
   if ( tokens[0].type == OPENPAREN )
   {
      DATATYPE expressionDatatype,variableDatatype;

      do
      {
         GetNextToken(tokens);
         p++;

// CODEGENERATION   
// STATICSEMANTICS
         switch ( identifierTable.GetType(index+p) )
         {
            case IN_PARAMETER:
               ParseExpression(tokens,expressionDatatype);
               if ( expressionDatatype != identifierTable.GetDatatype(index+p) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                     "Actual parameter data type does not match formal parameter data type");
               break;
            case OUT_PARAMETER:
               ParseVariable(tokens,true,variableDatatype);
               if ( variableDatatype != identifierTable.GetDatatype(index+p) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                     "Actual parameter data type does not match formal parameter data type");
               code.EmitFormattedLine("","PUSH","#0X0000");
               break;
            case IO_PARAMETER:
               ParseVariable(tokens,true,variableDatatype);
               if ( variableDatatype != identifierTable.GetDatatype(index+p) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                     "Actual parameter data type does not match formal parameter data type");
               code.EmitFormattedLine("","PUSH","@SP:0D0");
               break;
            case REF_PARAMETER:
               ParseVariable(tokens,true,variableDatatype);
               if ( variableDatatype != identifierTable.GetDatatype(index+p) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                     "Actual parameter data type does not match formal parameter data type");
               break;
         }
// ENDSTATICSEMANTICS
// ENDCODEGENERATION
      } while ( tokens[0].type == COMMA );

      if ( tokens[0].type != CLOSEPAREN )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting )");

      GetNextToken(tokens);
   }
                     
// STATICSEMANTICS
   if ( identifierTable.GetCountOfFormalParameters(index) != p )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
         "Number of actual parameters does not match number of formal parameters");
// ENDSTATICSEMANTICS

// CODEGENERATION
   code.EmitFormattedLine("","PUSHFB");
   code.EmitFormattedLine("","CALL",identifierTable.GetReference(index));
   code.EmitFormattedLine("","POPFB");
   for (int p = identifierTable.GetCountOfFormalParameters(index); p >= 1; p--)
   {
      switch ( identifierTable.GetType(index+p) )
      {
         case IN_PARAMETER:
            code.EmitFormattedLine("","DISCARD","#0D1");
            break;
         case OUT_PARAMETER:
            code.EmitFormattedLine("","POP","@SP:0D1");
            code.EmitFormattedLine("","DISCARD","#0D1");
            break;
         case IO_PARAMETER:
            code.EmitFormattedLine("","POP","@SP:0D1");
            code.EmitFormattedLine("","DISCARD","#0D1");
            break;
         case REF_PARAMETER:
            code.EmitFormattedLine("","DISCARD","#0D1");
            break;
      }
   }
// ENDCODEGENERATION

   if ( tokens[0].type != SEMICOLON )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ;");

   GetNextToken(tokens);

   ExitModule("CallStatement");
}

//-----------------------------------------------------------
void ParseReturnStatement(TOKEN tokens[])
//-----------------------------------------------------------
{
   void GetNextToken(TOKEN tokens[]);

   char line[SOURCELINELENGTH+1];

   EnterModule("ReturnStatement");

   sprintf(line,"; **** RETURN statement (%4d)",tokens[0].sourceLineNumber);
   code.EmitUnformattedLine(line);

   GetNextToken(tokens);

// STATICSEMANTICS
   if ( !code.IsInModuleBody(PROCEDURE_MODULE) )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
         "RETURN without expression only allowed in PROCEDURE module body");

// CODEGENERATION
   code.EmitFormattedLine("","RETURN");
// ENDCODEGENERATION
// ENDSTATICSEMANTICS

   if ( tokens[0].type != SEMICOLON )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting ;");

   GetNextToken(tokens);

   ExitModule("ReturnStatement");
}

//-----------------------------------------------------------
void ParseExpression(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
// CODEGENERATION
/*
   An expression is composed of a collection of one or more operands (SPL calls them
      primaries) and operators (and perhaps sets of parentheses to modify the default 
      order-of-evaluation established by precedence and associativity rules).
      Expression evaluation computes a single value as the expression's result.
      The result has a specific data type. By design, the expression result is 
      "left" at the top of the run-time stack for subsequent use.
   
   SPL expressions must be single-mode with operators working on operands of
      the appropriate type (for example, boolean AND boolean) and not mixing
      modes. Static semantic analysis guarantees that operators are
      operating on operands of appropriate data type.
*/
// ENDCODEGENERATION

   void ParseConjunction(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   DATATYPE datatypeLHS,datatypeRHS;

   EnterModule("Expression");

   ParseConjunction(tokens,datatypeLHS);

   if ( (tokens[0].type ==  OR) ||
        (tokens[0].type == NOR) ||
        (tokens[0].type == XOR) )
   {
      while ( (tokens[0].type ==  OR) ||
              (tokens[0].type == NOR) ||
              (tokens[0].type == XOR) )
      {
         TOKENTYPE operation = tokens[0].type;

         GetNextToken(tokens);
         ParseConjunction(tokens,datatypeRHS);
   
// CODEGENERATION
         switch ( operation )
         {
            case OR:

// STATICSEMANTICS
               if ( !((datatypeLHS == BOOLEANTYPE) && (datatypeRHS == BOOLEANTYPE)) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean operands");
// ENDSTATICSEMANTICS
   
               code.EmitFormattedLine("","OR");
               datatype = BOOLEANTYPE;
               break;
            case NOR:
   
// STATICSEMANTICS
               if ( !((datatypeLHS == BOOLEANTYPE) && (datatypeRHS == BOOLEANTYPE)) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean operands");
// ENDSTATICSEMANTICS
   
               code.EmitFormattedLine("","NOR");
               datatype = BOOLEANTYPE;
               break;
            case XOR:
   
// STATICSEMANTICS
               if ( !((datatypeLHS == BOOLEANTYPE) && (datatypeRHS == BOOLEANTYPE)) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean operands");
// ENDSTATICSEMANTICS
   
               code.EmitFormattedLine("","XOR");
               datatype = BOOLEANTYPE;
               break;
         }
      }
// CODEGENERATION

   }
   else
      datatype = datatypeLHS;

   ExitModule("Expression");
}


//-----------------------------------------------------------
void ParseConjunction(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void ParseNegation(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   DATATYPE datatypeLHS,datatypeRHS;

   EnterModule("Conjunction");

   ParseNegation(tokens,datatypeLHS);

   if ( (tokens[0].type ==  AND) ||
        (tokens[0].type == NAND) )
   {
      while ( (tokens[0].type ==  AND) ||
              (tokens[0].type == NAND) )
      {
         TOKENTYPE operation = tokens[0].type;
  
         GetNextToken(tokens);
         ParseNegation(tokens,datatypeRHS);
   
         switch ( operation )
         {
            case AND:
               if ( !((datatypeLHS == BOOLEANTYPE) && (datatypeRHS == BOOLEANTYPE)) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean operands");
               code.EmitFormattedLine("","AND");
               datatype = BOOLEANTYPE;
               break;
            case NAND:
               if ( !((datatypeLHS == BOOLEANTYPE) && (datatypeRHS == BOOLEANTYPE)) )
                  ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean operands");
               code.EmitFormattedLine("","NAND");
               datatype = BOOLEANTYPE;
               break;
         }
      }
   }
   else
      datatype = datatypeLHS;

   ExitModule("Conjunction");
}

//-----------------------------------------------------------
void ParseNegation(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void ParseComparison(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   DATATYPE datatypeRHS;

   EnterModule("Negation");

   if ( tokens[0].type == NOT )
   {
      GetNextToken(tokens);
      ParseComparison(tokens,datatypeRHS);

      if ( !(datatypeRHS == BOOLEANTYPE) )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting boolean operand");
      code.EmitFormattedLine("","NOT");
      datatype = BOOLEANTYPE;
   }
   else
      ParseComparison(tokens,datatype);

   ExitModule("Negation");
}


//-----------------------------------------------------------
void ParseComparison(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void ParseComparator(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   DATATYPE datatypeLHS,datatypeRHS;

   EnterModule("Comparison");

   ParseComparator(tokens,datatypeLHS);
   if ( (tokens[0].type ==	LESSTHEN) 		||
        (tokens[0].type ==	LESSTHENEQL)	||
        (tokens[0].type ==	EQUAL)			||
        (tokens[0].type ==	GREATERTHEN)	||
        (tokens[0].type ==	GTEQUAL) ||
        (tokens[0].type ==	NOTEQL)
      )
   {
      TOKENTYPE operation = tokens[0].type;

      GetNextToken(tokens);
      ParseComparator(tokens,datatypeRHS);

      if ( (datatypeLHS != INTEGERTYPE) || (datatypeRHS != INTEGERTYPE) )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer operands");
/*
      CMPI
      JMPXX     T????         ; XX = L,E,G,LE,NE,GE (as required)
      PUSH      #0X0000       ; push FALSE
      JMP       E????         ;    or 
T???? PUSH      #0XFFFF       ; push TRUE (as required)
E???? EQU       *
*/
      char Tlabel[SOURCELINELENGTH+1],Elabel[SOURCELINELENGTH+1];

      code.EmitFormattedLine("","CMPI");
      sprintf(Tlabel,"T%04d",code.LabelSuffix());
      sprintf(Elabel,"E%04d",code.LabelSuffix());
      switch ( operation )
      {
         case LESSTHEN:
            code.EmitFormattedLine("","JMPL",Tlabel);
            break;
         case LESSTHENEQL:
            code.EmitFormattedLine("","JMPLE",Tlabel);
            break;
         case EQUAL:
            code.EmitFormattedLine("","JMPE",Tlabel);
            break;
         case GREATERTHEN:
            code.EmitFormattedLine("","JMPG",Tlabel);
            break;
         case GTEQUAL:
            code.EmitFormattedLine("","JMPGE",Tlabel);
            break;
         case NOTEQL:
            code.EmitFormattedLine("","JMPNE",Tlabel);
            break;
      }
      datatype = BOOLEANTYPE;
      code.EmitFormattedLine("","PUSH","#0X0000");
      code.EmitFormattedLine("","JMP",Elabel);
      code.EmitFormattedLine(Tlabel,"PUSH","#0XFFFF");
      code.EmitFormattedLine(Elabel,"EQU","*");
   }
   else
      datatype = datatypeLHS;

   ExitModule("Comparison");
}


//-----------------------------------------------------------
void ParseComparator(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void ParseTerm(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   DATATYPE datatypeLHS,datatypeRHS;

   EnterModule("Comparator");

   ParseTerm(tokens,datatypeLHS);

   if ( (tokens[0].type ==  PLUS) ||
        (tokens[0].type == MINUS) )
   {
      while ( (tokens[0].type ==  PLUS) ||
              (tokens[0].type == MINUS) )
      {
         TOKENTYPE operation = tokens[0].type;
         
         GetNextToken(tokens);
         ParseTerm(tokens,datatypeRHS);

         if ( (datatypeLHS != INTEGERTYPE) || (datatypeRHS != INTEGERTYPE) )
            ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer operands");

         switch ( operation )
         {
            case PLUS:
               code.EmitFormattedLine("","ADDI");
               break;
            case MINUS:
               code.EmitFormattedLine("","SUBI");
               break;
         }
         datatype = INTEGERTYPE;
      }
   }
   else
      datatype = datatypeLHS;
   
   ExitModule("Comparator");
}


//-----------------------------------------------------------
void ParseTerm(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void ParseFactor(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   DATATYPE datatypeLHS,datatypeRHS;

   EnterModule("Term");

   ParseFactor(tokens,datatypeLHS);
   if ( (tokens[0].type == MULTIPLY) ||
        (tokens[0].type ==   DIVIDE) ||
        (tokens[0].type ==  MODULUS) )
   {
      while ( (tokens[0].type == MULTIPLY) ||
              (tokens[0].type ==   DIVIDE) ||
              (tokens[0].type ==  MODULUS) )
      {
         TOKENTYPE operation = tokens[0].type;
         
         GetNextToken(tokens);
         ParseFactor(tokens,datatypeRHS);

         if ( (datatypeLHS != INTEGERTYPE) || (datatypeRHS != INTEGERTYPE) )
            ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer operands");

         switch ( operation )
         {
            case MULTIPLY:
               code.EmitFormattedLine("","MULI");
               break;
            case DIVIDE:
               code.EmitFormattedLine("","DIVI");
               break;
            case MODULUS:
               code.EmitFormattedLine("","REMI");
               break;
         }
         datatype = INTEGERTYPE;
      }
   }
   else
      datatype = datatypeLHS;

   ExitModule("Term");
}


//-----------------------------------------------------------
void ParseFactor(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void GetNextToken(TOKEN tokens[]);
   void ParseSecondary(TOKEN tokens[],DATATYPE &datatype);

   EnterModule("Factor");

   if ( (tokens[0].type ==   ABS) ||
        (tokens[0].type ==  PLUS) ||
        (tokens[0].type == MINUS)
      )
   {
      DATATYPE datatypeRHS;
      TOKENTYPE operation = tokens[0].type;

      GetNextToken(tokens);
      ParseSecondary(tokens,datatypeRHS);

      if ( datatypeRHS != INTEGERTYPE )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer operand");

      switch ( operation )
      {
         case ABS:
/*
      SETNZPI
      JMPNN     E????
      NEGI                    ; NEGI or NEGF (as required)
E???? EQU       *
*/
            {
               char Elabel[SOURCELINELENGTH+1];
         
               sprintf(Elabel,"E%04d",code.LabelSuffix());
               code.EmitFormattedLine("","SETNZPI");
               code.EmitFormattedLine("","JMPNN",Elabel);
               code.EmitFormattedLine("","NEGI");
               code.EmitFormattedLine(Elabel,"EQU","*");
            }
            break;
         case PLUS:
         // Do nothing (identity operator)
            break;
         case MINUS:
            code.EmitFormattedLine("","NEGI");
            break;
      }
      datatype = INTEGERTYPE;
   }
   else
      ParseSecondary(tokens,datatype);

   ExitModule("Factor");
}

//-----------------------------------------------------------
void ParseSecondary(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void ParsePrimary(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   DATATYPE datatypeLHS,datatypeRHS;

   EnterModule("Secondary");

   ParsePrimary(tokens,datatypeLHS);

   if ( tokens[0].type == POWER )
   {
      GetNextToken(tokens);

      ParsePrimary(tokens,datatypeRHS);

      if ( (datatypeLHS != INTEGERTYPE) || (datatypeRHS != INTEGERTYPE) )
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting integer operands");

      code.EmitFormattedLine("","POWI");
      datatype = INTEGERTYPE;
   }
   else
      datatype = datatypeLHS;

   ExitModule("Secondary");
}

//-----------------------------------------------------------
void ParsePrimary(TOKEN tokens[],DATATYPE &datatype)
//-----------------------------------------------------------
{
   void ParseVariable(TOKEN tokens[],bool asLValue,DATATYPE &datatype);
   void ParseExpression(TOKEN tokens[],DATATYPE &datatype);
   void GetNextToken(TOKEN tokens[]);

   EnterModule("Primary");

   switch ( tokens[0].type )
   {
      case INTEGER:
         {
            char operand[SOURCELINELENGTH+1];
            
            sprintf(operand,"#0D%s",tokens[0].lexeme);
            code.EmitFormattedLine("","PUSH",operand);
            datatype = INTEGERTYPE;
            GetNextToken(tokens);
         }
         break;
      case TRUE:
         code.EmitFormattedLine("","PUSH","#0XFFFF");
         datatype = BOOLEANTYPE;
         GetNextToken(tokens);
         break;
      case FALSE:
         code.EmitFormattedLine("","PUSH","#0X0000");
         datatype = BOOLEANTYPE;
         GetNextToken(tokens);
         break;
      case OPENPAREN:
         GetNextToken(tokens);
         ParseExpression(tokens,datatype);
         if ( tokens[0].type != CLOSEPAREN )
            ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting )");
         GetNextToken(tokens);
         break;
	  case IDENTIFIER:
         ParseVariable(tokens,false,datatype);
         break;
      
	  default:
         ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,
                              "Expecting integer, true, false, '(', variable");
         break;
   }

   ExitModule("Primary");
}

//-----------------------------------------------------------
void ParseVariable(TOKEN tokens[],bool asLValue,DATATYPE &datatype)
//-----------------------------------------------------------
{
/*
Syntax "locations"                 l- or r-value
---------------------------------  -------------
<expression>                       r-value
<INPUTStatement>                   l-value
LHS of <assignmentStatement>       l-value
<FORStatement>                     l-value
OUT <formalParameter>              l-value
IO <formalParameter>               l-value
REF <formalParameter>              l-value

r-value (read-only): value is pushed on run-time stack
l-value (read and/or write): address of value is pushed on run-time stack
*/
   void GetNextToken(TOKEN tokens[]);

   bool isInTable;
   int index;

   EnterModule("Variable");

   if ( tokens[0].type != IDENTIFIER )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting identifier");

// STATICSEMANTICS
   index = identifierTable.GetIndex(tokens[0].lexeme,isInTable);
   if ( !isInTable )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Undefined identifier");
   if ( !((identifierTable.GetType(index) == GLOBAL_VARIABLE)  ||
          (identifierTable.GetType(index) == PROGRAM_VARIABLE) ||
          (identifierTable.GetType(index) == MODULE_VARIABLE)  ||
          (identifierTable.GetType(index) == IN_PARAMETER)     ||
          (identifierTable.GetType(index) == OUT_PARAMETER)    ||
          (identifierTable.GetType(index) == IO_PARAMETER)     ||
          (identifierTable.GetType(index) == REF_PARAMETER)) )
      ProcessCompilerError(tokens[0].sourceLineNumber,tokens[0].sourceLineIndex,"Expecting variable identifier");
   datatype = identifierTable.GetDatatype(index);
// ENDSTATICSEMANTICS

// CODEGENERATION
   if ( asLValue )
      code.EmitFormattedLine("","PUSHA",identifierTable.GetReference(index));
   else
      code.EmitFormattedLine("","PUSH",identifierTable.GetReference(index));
// ENDCODEGENERATION

   GetNextToken(tokens);

   ExitModule("Variable");
}

//-----------------------------------------------------------
void Callback1(int sourceLineNumber,char sourceLine[])
//-----------------------------------------------------------
{
   cout << setw(4) << sourceLineNumber << " " << sourceLine << endl;
}

//-----------------------------------------------------------
void Callback2(int sourceLineNumber,char sourceLine[])
//-----------------------------------------------------------
{
   char line[SOURCELINELENGTH+1];

// CODEGENERATION
   sprintf(line,"; %4d %s",sourceLineNumber,sourceLine);
   code.EmitUnformattedLine(line);
// ENDCODEGENERATION
}

//-----------------------------------------------------------
void GetNextToken(TOKEN tokens[])
//-----------------------------------------------------------
{
   int i;
   TOKENTYPE type;
   char lexeme[SOURCELINELENGTH+1];
   int sourceLineNumber;
   int sourceLineIndex;
   char information[SOURCELINELENGTH+1];

//============================================================
// Move look-ahead "window" to make room for next token/lexeme
//============================================================
   for (int i = 1; i <= LOOKAHEAD; i++)
      tokens[i-1] = tokens[i];

   char nextCharacter = reader.GetLookAheadCharacter(0).character;

//============================================================
// "Eat" white space and comments
//============================================================
   do
   {
//    "Eat" any white-space (blanks and EOLCs and TABCs) 
      while ( (nextCharacter == ' ')
           || (nextCharacter == READER<CALLBACKSUSED>::EOLC)
           || (nextCharacter == READER<CALLBACKSUSED>::TABC) )
         nextCharacter = reader.GetNextCharacter().character;

//    "Eat" line comment
      if ( nextCharacter == '@' )
      {

#ifdef TRACESCANNER
   sprintf(information,"At (%4d:%3d) begin line comment",
      reader.GetLookAheadCharacter(0).sourceLineNumber,
      reader.GetLookAheadCharacter(0).sourceLineIndex);
   lister.ListInformationLine(information);
#endif

         do
            nextCharacter = reader.GetNextCharacter().character;
         while ( nextCharacter != READER<CALLBACKSUSED>::EOLC );
      }

//    "Eat" block comments (nesting allowed)
      if ( (nextCharacter == '|') && (reader.GetLookAheadCharacter(1).character == '>') )
      {
         int depth = 0;

         do
         {
            if ( (nextCharacter == '|') && (reader.GetLookAheadCharacter(1).character == '>') )
            {
               depth++;

#ifdef TRACESCANNER
   sprintf(information,"At (%4d:%3d) begin block comment depth = %d",
      reader.GetLookAheadCharacter(0).sourceLineNumber,
      reader.GetLookAheadCharacter(0).sourceLineIndex,
      depth);
   lister.ListInformationLine(information);
#endif

               nextCharacter = reader.GetNextCharacter().character;
               nextCharacter = reader.GetNextCharacter().character;
            }
            else if ( (nextCharacter == '<') && (reader.GetLookAheadCharacter(1).character == '|') )
            {

#ifdef TRACESCANNER
   sprintf(information,"At (%4d:%3d)   end block comment depth = %d",
      reader.GetLookAheadCharacter(0).sourceLineNumber,
      reader.GetLookAheadCharacter(0).sourceLineIndex,
      depth);
   lister.ListInformationLine(information);
#endif

               depth--;
               nextCharacter = reader.GetNextCharacter().character;
               nextCharacter = reader.GetNextCharacter().character;
            }
            else
               nextCharacter = reader.GetNextCharacter().character;
         }
         while ( (depth != 0) && (nextCharacter != READER<CALLBACKSUSED>::EOPC) );
         if ( depth != 0 ) 
            ProcessCompilerError(reader.GetLookAheadCharacter(0).sourceLineNumber,
                                 reader.GetLookAheadCharacter(0).sourceLineIndex,
                                 "Unexpected end-of-program");
      }
   } while ( (nextCharacter == ' ')
          || (nextCharacter == READER<CALLBACKSUSED>::EOLC)
          || (nextCharacter == READER<CALLBACKSUSED>::TABC) );

//============================================================
// Scan token
//============================================================
   sourceLineNumber = reader.GetLookAheadCharacter(0).sourceLineNumber;
   sourceLineIndex = reader.GetLookAheadCharacter(0).sourceLineIndex;

// reserved word (or <identifier> ***BUT NOT YET***)
   if ( isalpha(nextCharacter) )
   {
      char UCLexeme[SOURCELINELENGTH+1];

      i = 0;
      lexeme[i++] = nextCharacter;
      nextCharacter = reader.GetNextCharacter().character;
      while ( isalpha(nextCharacter) || isdigit(nextCharacter) )
      {
         lexeme[i++] = nextCharacter;
         nextCharacter = reader.GetNextCharacter().character;
      }
      lexeme[i] = '\0';
      for (i = 0; i <= (int) strlen(lexeme); i++)
         UCLexeme[i] = toupper(lexeme[i]);

      bool isFound = false;

      i = 0;
      while ( !isFound && (i <= (sizeof(TOKENTABLE)/sizeof(TOKENTABLERECORD))-1) )
      {
         if ( TOKENTABLE[i].isReservedWord && (strcmp(UCLexeme,TOKENTABLE[i].description) == 0) )
            isFound = true;
         else
            i++;
      }
      if ( isFound )
         type = TOKENTABLE[i].type;
      else
         type = IDENTIFIER;
   }
 // <integer>
   else if ( isdigit(nextCharacter) )
   {
      i = 0;
      lexeme[i++] = nextCharacter;
      nextCharacter = reader.GetNextCharacter().character;
      while ( isdigit(nextCharacter) )
      {
         lexeme[i++] = nextCharacter;
         nextCharacter = reader.GetNextCharacter().character;
      }
      lexeme[i] = '\0';
      type = INTEGER;
   }
   else
   {
      switch ( nextCharacter )
      {
// <string>
         case '"': 
            i = 0;
            nextCharacter = reader.GetNextCharacter().character;
            while ( nextCharacter != '"' )
            {
               if ( nextCharacter == '\\' )
                  nextCharacter = reader.GetNextCharacter().character;
               else if ( nextCharacter == READER<CALLBACKSUSED>::EOLC )
                  ProcessCompilerError(sourceLineNumber,sourceLineIndex,
                                       "Invalid string literal");
               lexeme[i++] = nextCharacter;
               nextCharacter = reader.GetNextCharacter().character;
            }
            reader.GetNextCharacter();
            lexeme[i] = '\0';
            type = STRING;
            break;
         case READER<CALLBACKSUSED>::EOPC: 
            {
               static int count = 0;
   
               if ( ++count > (LOOKAHEAD+1) )
                  ProcessCompilerError(sourceLineNumber,sourceLineIndex,
                                       "Unexpected end-of-program");
               else
               {
                  type = EOPTOKEN;
                  reader.GetNextCharacter();
                  lexeme[0] = '\0';
               }
            }
            break;
		 case '[':
         	type = OPENBRACKET;
         	lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case ']':
         	type = CLOSEBRACKET;
         	lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case ',':
            type = COMMA;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case ';': 
            type = SEMICOLON;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case '(': 
            type = OPENPAREN;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case ')': 
            type = CLOSEPAREN;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
		  case ':': 
            lexeme[0] = nextCharacter;
            nextCharacter = reader.GetNextCharacter().character;
            if ( nextCharacter == '=' )
            {
               type = COLONEQL;
               lexeme[1] = nextCharacter; lexeme[2] = '\0';
               reader.GetNextCharacter();
            }
            else
            {
               type = COLON;
               lexeme[1] = '\0';
            }
            break;
         case '<': 
            lexeme[0] = nextCharacter;
            nextCharacter = reader.GetNextCharacter().character;
            if ( nextCharacter == '=' )
            {
               type = LESSTHENEQL;
               lexeme[1] = nextCharacter; lexeme[2] = '\0';
               reader.GetNextCharacter();
            }
            else
            {
               type = LESSTHEN;
               lexeme[1] = '\0';
            }
            break;
         case '=': 
            type = EQUAL;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case '>':
            lexeme[0] = nextCharacter;
            nextCharacter = reader.GetNextCharacter().character;
            if ( nextCharacter == '=' )
            {
               type = GTEQUAL;
               lexeme[1] = nextCharacter; lexeme[2] = '\0';
               reader.GetNextCharacter();
            }
            else
            {
               type = GREATERTHEN;
               lexeme[1] = '\0';
            }
            break;
      // use character look-ahead to "find" '='
         case '!':
            lexeme[0] = nextCharacter;
            if ( reader.GetLookAheadCharacter(1).character == '=' )
            {
               nextCharacter = reader.GetNextCharacter().character;
               lexeme[1] = nextCharacter; lexeme[2] = '\0';
               reader.GetNextCharacter();
               type = NOTEQL;
            }
            else
            {
               type = UNKTOKEN;
               lexeme[1] = '\0';
               reader.GetNextCharacter();
            }
            break;
         case '+': 
            type = PLUS;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case '-': 
            type = MINUS;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
      // use character look-ahead to "find" other '*'
         case '*': 
            lexeme[0] = nextCharacter;
            if ( reader.GetLookAheadCharacter(1).character == '*' )
            {
               nextCharacter = reader.GetNextCharacter().character;
               lexeme[1] = nextCharacter; lexeme[2] = '\0';
               type = POWER;
            }
            else
            {
               type = MULTIPLY;
               lexeme[0] = nextCharacter; lexeme[1] = '\0';
            }
            reader.GetNextCharacter();
            break;
         case '/': 
            type = DIVIDE;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case '%': 
            type = MODULUS;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         case '^': 
            type = POWER;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
         default:  
            type = UNKTOKEN;
            lexeme[0] = nextCharacter; lexeme[1] = '\0';
            reader.GetNextCharacter();
            break;
      }
   }

   tokens[LOOKAHEAD].type = type;
   strcpy(tokens[LOOKAHEAD].lexeme,lexeme);
   tokens[LOOKAHEAD].sourceLineNumber = sourceLineNumber;
   tokens[LOOKAHEAD].sourceLineIndex = sourceLineIndex;

#ifdef TRACESCANNER
   sprintf(information,"At (%4d:%3d) token = %12s lexeme = \\%s\\",
      tokens[LOOKAHEAD].sourceLineNumber,
      tokens[LOOKAHEAD].sourceLineIndex,
      TOKENTABLE[type].description,lexeme);
   lister.ListInformationLine(information);
#endif

}

