# Swift-Programming-Language

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
