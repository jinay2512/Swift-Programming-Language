# Swift-Programming-Language

SWIFT, is a combination of high-level programming language. SWIFT is a simple, but Turing complete , programming language. A SWIFT program must be wholly-contained in a single source file named with the extension .swift. SWIFT is a very strongly-typed  language which supports

•	explicitly defined variables and constants with data types integer, character, boolean, and string; 
•	integer, character, boolean, and string variables and constants structured as scalars
•	integer, character, and boolean variables (but no string variables and no constants of any data type) structured as one-dimensional associative arrays ;
•	literals for each of the scalar data types integer, character, boolean, and string;
•	statically-allocated global scope and program module local scope variables and constants; 
•	an eclectic list of unary and binary operators;
o	Unary may include
	The increment (++) and decrement (--) operators.
	The unary minus (-) operator.
	The logical not (!) operator.
o	Binary may include
	addition (+) operator, 
	subtraction (-) operator
	division (/) operator.
•	a multiple-assignment statement;
•	several flow-of-control statements (a “standard” pretest FOR loop statement, a very interesting midtest DO-WHILE-DO loop statement, a standard IF statement, but no GOTO statement and no statement labels);
•	PRINT and INPUT statements for interactive text input/output;
•	non-recursive and recursive calls of function and procedure subprogram modules (accomplished with an expression-resident prefix function referencing syntax, a CALL statement, and a RETURN statement);
•	a simplistic form of exception handling (accomplished with handler subprogram modules and a RAISE, an EXIT, and a RESUME statement).

SWIFT Context-Free Syntax

/*-----------------------------------------------------------
<SWIFTProgram> 			::= <variableDefinitions>
							{ <PROCEDUREDefinition> }*
							<ProgramDefinition> EOPC
							
<variableDefinitions> 	::= { VAR <identifier> -> <datatype> { , <identifier> -> <datatype> }* ; }*

<datatype>           		::= INT | BOOL

<ProgramDefinition> 		::=	BEGIN
							<variableDefinitions>
                             {  <statement>  } *
							ENDP
<PROCEDUREDefinition> ::= PROCEDURE <identifier> [ ( <formalParameter> { ,                                  <formalParameter > }* ) ]
                             <variableDefinitions>
                             { <statement> }*
                          END

<formalParameter>     ::= [ (( IN | OUT | IO | REF )) ] <identifier> : <datatype>


<statement>				::=	<PrintStatement>
							| <InputStatement>
							| <AssignmentStatement>
							| <IfStatement>
                       		| <DoWhileStatement>
							| <ForStatement>
							| <CallStatement>
							| <ReturnStatement>


<PrintStatement>			::=	WRITE _[_ ((<string> | <expession> | ENDL))
							{ , ((<string | <expession> | ENDL)) } * _]_ ;

<InputStatement> 	    		::= INPUT [ <string> ] <variable> ;

<assignmentStatement> 	::= <variable> { , <variable> }* := <expression> ;

<IfStatement>         	::= IF ( <expression> ) THEN    	                          { <statement> }*
    		                    { ELIF ( <expression> ) THEN
                 	            { <statement> }* }*
         	    	           [ ELSE
                             { <statement> }* ]
                     			ENDIF

<DoWhileStatement>  	  ::= DO
                           { <statement> }*
                          WHILE ( <expression> )
                            { <statement> }*
                          ENDDOWHILE

<FORStatement>        ::= FOR <variable> := <expression> TO <expression>
						[BY<expression> ]
                             { <statement> }*
                          ENDFOR
<CALLStatement>       ::= CALL <identifier> [ ( (( <expression> | <variable> )) 
{ , (( <expression> | <variable> )) }* ) ] ;

<RETURNStatement>     ::= RETURN ;


							
<expression>				::=	<conjunction> { (( OR | NOR | XOR )) <conjunction>}*


<conjunction>			::=	<negation> { (( AND | NAND )) <negation> }*

<negation>				::= [ NOT ] <comparison>

<comparison>				::=	<comparator> [ (( < | <= | = | > | >= |!= )) <comparator>]

<comparator>				::=	<term> { (( + | - )) <term> }*

<term>					::= <factor> { (( * | / | % )) <factor> }*

<factor>					::= [ (( ABS | + | - )) ] <secondary>

<secondary>				::= <primary> [ (( ^ )) <primary> ]

<primary>				::= <variable> | <integer> | <boolean> | ( <expression> )

<variable>            	::= <identifier>

<identifier>          	::= <letter> { (( <letter> | <digit> | _ )) }

<integer>				::= <digit> { <digit> }*

<digit>					::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

<boolean>				::= true | false

<string>					::=	“{  <ASCIICharacters>  }* “

<ASCIICharacters>			::=	|| All ASCIICharacters in range [32,126]

<Comments>				::=	@  {  <ASCIICharacters>  } * EOLC
							|  |> { (( <ASCIICharacter> | EOLC )) }* <|

-----------------------------------------------------------*/ 
Description of the Static Semantics of SWIFT
A SWIFT program which contains one or more syntax errors—context-free errors and/or static semantic errors—cannot be fully translated into machine code because the errors obscure the meaning.

Description of the Syntax and the Dynamic Semantics of SWIFT1
SWIFT is a case-neutral, free format language, that is, any amount of white space characters (blanks, tabs, and end-of-lines) are allowed between source code tokens.

A single-line <comment> may be appended to the end of any source line; that is, a comment extends from the semicolon ; prefix (used to mark the beginning of the single-line comment) to the end of the line containing the semicolon. Single-line comments are treated like white space at the end of the source line in which the comment appears.

<comment>             ::= @ { <ASCIICharacter> }* EOLC

For example,

@--------------------
@Compute x
@--------------------
INPUT "x? " x.
x := ((y+z)*11 % 10) 45.
WRITE ["x is ",x,ENDL];      @ Display results

A multi-line <comment> may appear anyplace white-space may appear. The multi-line comment extends from the |- prefix (used to mark the beginning of the multi-line comment) to the -| suffix. Multi-line comments may be nested.

<comment>             ::= | |> { (( <ASCIICharacter> | EOLC )) }* <|

For example,

|>--------------------
@Compute x
--------------------<|
INPUT "x? " x;
x := ((y+z)*11 % 10) 45;
WRITE ["x is ",x,ENDL];      |> Display
                           	   Results <|


Literal constants may be <integer>, <boolean> or <string>. An integer literal must be unsigned and must be an element of the range [ 0, 215-1 ]; the reserved words true and false are the only two boolean literals, and a string literal is delimited by double quotes " and may contain zero or more printable characters 

<integer>             ::= <digit> { <digit> }*

<digit>               ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

<boolean>             ::= true | false

<string>              ::= " { <ASCIICharacter> }* "

For example, the WRITE statement shown below displays the line "Howdy", he exclaimed!

WRITE ["Howdy, he exclaimed!",ENDL];

An <identifier> is used to name global variables and program module (local) variables. A variable is defined when it is specified in a variable definitions section.

<identifier>          ::= <letter> { (( <letter> | <digit> | _ )) }*

A scalar variable must be defined with a data type (integer or boolean). Note No particular binary representation of the SPL data types is required by this specification, so the binary representation may be defined differently by each implementation (that is, the binary representation is implementation dependent).

Variables defined in the <variableDefinitions> block that precedes the program module definition have global scope. The scope of program module variable identifiers is local scope which extends from the point of definition to the last statement in the body of the module in which it is defined. It is a static semantic error to reference an undefined (or defined by not-in-scope) identifier. It is a static semantic error to multiply-define an identifier in the same scope (global or local); however, when a globally-scoped identifier and a locally-scoped identifier overlap, a reference to the shared identifier is a reference to the locally-scoped identifier. (Note There is no SPL syntax for referencing the globally-scoped identifier when it collides with a locally-scoped identifier.) 

The lifetime (or duration) of global variables and of program module local variables is the entire duration of the program’s execution. 

A scalar <variable> is referenced by specifying its identifier.

<variable>            ::= <identifier>

A <SWIFTProgram> is a required program module definition; (that is, each SWIFT program must consist of at least one module, the program module). When a SWIFT program begins execution, flow-of-control begins with the first statement in the program module’s list of statements and the flow-of-control continues until it terminates when the statement that ends the list completes execution.

<SWIFTProgram>          ::= <variableDefinition>
						{ <PROCEDUREDefinition> }*
<PROGRAMDefinition> EOPC

A <variableDefinitions> is a collection of comma-separated lists of identifiers used to define variables. An SPL variable must be defined before it can be legally referenced.

<variableDefinitions> ::= { VAR <identifier> : <datatype> { , <identifier> : <datatype> }* ; }*

<datatype>            ::= INT | BOOL

A <ProcedureDefinition> defines a globally-scoped, named procedure subprogram module by providing (1) a globally-unique identifier to name the module; (2) an optional list of formal parameters; (3) an optional variables definitions section for local variables; and (4) a set of zero or more executable statements procedure module body.

<ProcedureDefinition> ::= PROCEDURE <identifier> [ ( <formalParameter> { , < formalParameter >                            }* ) ]
                             [ <dataDefinitions> ]
                             { <statement> }*
                          END

A procedure module body consists of zero or more statements that are executed when the procedure is referenced with a CALL statement (see <CALLStatement> description below). Flow-of-control begins with the first statement in the procedure’s body and the flow-of-control terminates when the procedure executes a RETURN statement. After the procedure terminates, flow-of-control resumes with the statement immediately following the CALL statement that referenced the procedure. When a procedure body does not contain a RETURN statement, the flow-of-control terminates when the procedure “runs into” the procedure’s END keyword.

A <formalParameter> may be IN (which specifies call-by-value), OUT (call-by-result), IO (call-by-value-result), or REF (call-by-reference). A formal parameter defined without an IN, OUT, IO, or REF specifier is call-by-value (IN) by default. 

<formalParameter>     ::= [ (( IN | OUT | IO | REF )) ] <identifier> : <datatype>

Procedure subprogram module formal parameters may be IN, OUT, IO, and/or REF parameters. When a procedure subprogram module is defined with no formal parameters, the set of parentheses used to enclose the formal parameter list must not be specified.

An actual parameter corresponding to an IN formal parameter is an r-value which results from evaluation of an expression; an actual parameter corresponding to an OUT or an IO formal parameter must be scalar variable (an l-value). It is a static semantic error when the data type of an actual parameter does not exactly match the data type of its corresponding formal parameter; that is, no coercions (implicit cast operations) are supported.


A <ProgramDefinition> defines the program module by providing a set of zero or more executable statements which make up the program module body.

<ProgramDefinition>   	::= BEGIN
                            	{ <statement> }*
                            ENDP

The program module body consists of zero or more statements that begin execution when the program is run. Flow-of-control begins with the first statement in the program module’s body and the flow-of-control terminates when the last statement is executed.

A <statement> may be a <PrintStatement>.

<statement>        	   ::= <PrintStatement>
					| <INPUTStatement>
                        	| <AssignmentStatement>
					| <IfStatement>
					| <DoWhileStatement>

The <PRINTStatement> adds computed integer values, computed boolean values, and string literals to the output buffer using only the minimum number of characters required to represent the value being output. Note The boolean value true is output as "T" and false as "F". Output of ENDL causes the output buffer to be displayed as a single line on the user’s terminal and then empties the output buffer.

<PrintStatement>		::=	WRITE _[_ (( <string> | <expression> | ENDL))
							{ , ((< string> | <expression> | ENDL)) } * _]_ ;


For example, the PRINT statement shown below

WRITE ["Howdy, world","!",ENDL];

displays this line of output

Howdy, world!

WRITE [+11711,"+",-321,"=",11711+-321,".",ENDL];

displays this line of output

11711+-321=11390.

The WRITE statement shown below

WRITE ['\'',(2 <= 2),"' and ",(2 != 2),'.',ENDL];

displays this line of output

'T' and F.

The <InputStatement> prompts with the string specified (if any), then inputs a single carriage return-terminated record from the user’s screen and stores the integer- or boolean-equivalent of the characters contained in the line into the integer or boolean variable specified. The user’s input must be coded according to the syntax rules for SPL integer and boolean literals. It is a run-time error when the input characters cannot be converted to the appropriate data value.

<InputStatement>      ::= INPUT [ <string> ] <variable> ;

For example, the execution of the two INPUT statements shown below

INPUT "x? " x;
INPUT y;


The <assignmentStatement> has the semantics of a classic assignment statement; namely, the expression is evaluated yielding a single integer or boolean value, then the value is assigned to the variables on the left of the := operator. No coercion of right-hand side data type to left-hand side data type is permitted. 

It is a static semantic error when the data type of one of more of the variables does not match the data type of the expression.

<AssignmentStatement> ::= <variable> { , <variable> }* := <expression> ;

For example,

VAR x: INT,y: INT;
VAR flag1: BOOL,flag2: BOOL,flag3: BOOL;

x,y := 1+x;

flag1,flag2,flag3 := false;

The <IfStatement> has the semantics of a classical if-statement; specifically, at most, only one set of statements is ever selected to be executed; all other sets of statements are bypassed. (Note Each “set of statements” is also called a clause. Clauses are usually distinguished by the reserved word which precedes it; namely, the IF-clause, an ELSEIF-clause (there may be several ELSEIF-clauses), and the ELSE-clause.) The boolean expressions are evaluated from top-to-bottom looking for the first IF or ELSEIF expression which evaluates to true. Only the clause associated with this first true expression is selected to be executed, then the if-statement terminates (all other clauses are bypassed). When all of the boolean expressions evaluate to false and the if-statement contains an ELSE clause, the ELSE-clause is executed. When all of the boolean expressions evaluate to false and the if-statement does not contain an ELSE-clause, all of the if-statement clauses are bypassed. It is a static semantic error when any of the IF and/or ELSEIF expressions are not boolean data type.

<IfStatement>         ::= IF ( <expression> ) THEN                              { <statement> }*
                        { ELSEIF ( <expression> ) THEN                              { <statement> }* }*
                        [ ELSE                              { <statement> }* ]
                          ENDIF

The <DoWhileStatement> is a combination of both of the classic unbounded loops; this is, the SWIFT unbounded loop is an unbounded mid-test loop which can also be used as either a pre-test loop or a post-test loop. It is a static semantic error when the expression is not a boolean data type. Here are its semantics.

   1. Execute the statements between DO and WHILE (if any).
   2. Evaluate the boolean expression.
   3. When the expression is false, terminate the loop.
   4. Execute the statements between the WHILE and the ENDDOWHILE (if any).
   5. Go to step 1.


<DoWhileStatement>    ::= DO
                             { <statement> }*
                          WHILE ( <expression> )
                             { <statement> }*
                          ENDDOWHILE

The <ForStatement> has the semantics of the classic, bounded pre-test loop (also called a counted loop). The variable specified between the reserved word FOR and the := is called the loop control variable.

   1. The three expressions are evaluated exactly one time as the loop begins. The first expression, E1, is a lower bound when the BY expression, E3, is positive or E1 is an upper bound when E3 is negative. The second expression, E2, is an upper bound when E3 is positive or E2 is a lower bound when E3 is negative. Note When the BY expression syntax is omitted, E3 always defaults to +1, so E1 is treated as a lower bound and E2 as an upper bound.
   2. The loop control variable is initialized with E1.
   3. When (E3 > 0) and the loop control variable > E2, terminate the loop.
       When (E3 < 0) and the loop control variable < E2, terminate the loop.
   4. Execute the statements in the body of the loop.
   5. Add E3 to the loop control variable.
   6. Go to step 3.

It is a static semantic error when (1) the loop control variable is not integer data type, or (2) any of the expressions is not integer data type.

<ForStatement>        ::= FOR <variable> := <expression> TO <expression> [ BY <expression> ]
                             { <statement> }* 
                          ENDFOR

For example,

VAR i: INT,iSquared: INT.

FOR i := 1 TO 10 BY 2
   iSquared := i*i.
   PRINT i,iSquared,ENDL.
ENDFOR

The procedure subprogram module <CallStatement> actual parameter expressions corresponding to IN formal parameters are evaluated (OUT and IO actual parameters must be l-values (that is, variables) so no evaluation is necessary). Each IN formal parameter is initialized with the value of the corresponding actual parameter expression specified in the CALL statement and each IO formal parameter is initialized with the value of the corresponding actual parameter variable specified in the CALL statement. Flow-of-control is transferred to the first statement of the procedure being referenced after parameter passing is completed. It is a static semantic error when the CALL statement actual parameters do not exactly match in number and data type with the formal parameters specified in the procedure definition.

<CALLStatement>       ::= CALL <identifier> [ ( (( <expression> | <variable> )) 
                                            { , (( <expression> | <variable> )) }* ) ] ;


A procedure module <ReturnStatement> assigns the value of each OUT and IO formal parameter variable to the corresponding actual parameter variable specified in the CALL statement, then flow-of-control is transferred to the statement which immediately follows the CALL statement that referenced the procedure. IN formal parameters are not part of the semantics of the procedure RETURN statement.

<RETURNStatement>     ::= RETURN ;


An <expression> computes a single integer or boolean value. 

<expression>          ::= <conjunction> { (( OR | NOR | XOR )) <conjunction> }*

<conjunction>         ::= <negation> { (( AND | NAND )) <negation> }*

<negation>            ::= [ NOT ] <comparison>

<comparison>          ::= <comparator> [ (( < | <= | = | > | >= | (( != )) )) <comparator> ]

<comparator>          ::= <term> { (( + | - )) <term> }*

<term>                ::= <factor> { (( * | / | % )) <factor> }*

<factor>              ::= [ (( ABS | + | - )) ] <secondary>

<secondary>           ::= <primary> [ (( ^ )) <primary> ]

<primary>             ::= <integer> | <boolean> | ( <expression> )

SWIFT binary arithmetic operators are addition (lexeme +); subtraction ( ); multiplication (*); division (/); integer remainder (%); and exponentiation (^). The unary arithmetic operators are identity (+); negate (-); absolute value (ABS) (Note ABS is a unary operator, not a function.) The evaluation of each binary arithmetic operator always requires two integer operands and always results in a single integer value. The signatures of the arithmetic operators are shown below. It is a static semantic error when arithmetic operator operands do not match one of the allowed signatures.

•	  +: integer  integer  integer
•	  -: integer  integer  integer
•	  *: integer  integer  integer
•	  /: integer  integer  integer
•	  %: integer  integer  integer
•	  ^: integer  integer  integer
•	  +: integer  integer
•	  -: integer  integer
•	ABS: integer  integer

Boolean values may be combined using the boolean binary conjunctive operators (AND, NAND); the boolean binary disjunctive operators (OR, NOR, XOR); and the boolean unary negation operator (NOT). The evaluation of the AND, NAND, OR, NOR, and XOR binary boolean operation always requires two boolean operands and always results in a single boolean value. The evaluation of the NOT unary boolean operation always requires one boolean operand and always results in a single boolean value. The signatures of the boolean operators are shown below. It is a static semantic error when the boolean operator operands do not match one of the allowed signatures.

•	 AND: boolean  boolean  boolean
•	NAND: boolean  boolean  boolean
•	  OR: boolean  boolean  boolean
•	 NOR: boolean  boolean  boolean
•	 XOR: boolean  boolean  boolean
•	 NOT: boolean  boolean

Integer values can be compared to each other using the less than (<), the less than or equal (<=), the equal (=), the greater than (>), the greater than or equal (>=), and the not equal (!=)binary relational or comparison operators. The relational operators always compute in a single boolean value. The signatures of the binary comparison operators are shown below. It is a static semantic error when the comparison operator operands do not match one of the allowed signatures.

•	       <: integer  integer  boolean
•	      <=: integer  integer  boolean
•	       >: integer  integer  boolean
•	      >=: integer  integer  boolean
•	       =: integer  integer  boolean
•	      !=: integer  integer  boolean

SWIFT is like most other programming languages in that it allows the use of paired parentheses in an expression to change the order of operator evaluation from that based solely on the operators’ predefined precedences and/or associativities. For example, the two expressions shown below have two different operator evaluation orders, - * and * -, respectively.

(12 34)*54
12 34*54

When an integer overflow occurs during the run-time evaluation of an expression, the SWIFT program terminates with an appropriate error message displayed on the user’s terminal.


SPL Operators
Precedence	Associativity
 ^	 1 = highest	 Non-associative
 ABS + - (unary)	 2	 Non-associative
 * / %	 3	 Left to right
 + - (binary)	 4	 Left to right
 <  <=  =  >  >= !=)	 5	 Left-to-right
 NOT	 6	 Left-to-right
 AND NAND	 7	 Left-to-right
 OR NOR XOR	 8 = lowest	 Left-to-right
