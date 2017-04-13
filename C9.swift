|>----------------------
		C9.swift
----------------------<|

VAR x: INT,y: INT ;
VAR z: BOOL;

PROCEDURE PIN0
	WRITE[ "PIN0()",ENDL];
	RETURN;
END

PROCEDURE PIN1(x: INT)
	WRITE[ "PIN1(",x,")",ENDL];
	RETURN;
END

PROCEDURE PIN2(x: INT,IN y: INT)
	WRITE[ "PIN2(",x,",",y,")",ENDL];
END

PROCEDURE POUT1(OUT b: BOOL)
	WRITE[ "POUT1",ENDL];
	b := true;
END

PROCEDURE POUT2(OUT x: INT,OUT y: INT)
	WRITE[ "POUT2",ENDL];
	x := 2;
	y := 2*x;
	RETURN;
END

PROCEDURE PIN1R(x: INT)
	IF ( x > 0 ) THEN
	WRITE[ x,ENDL];
	CALL PIN1R(x-1);
	ENDIF 
END

PROCEDURE PIO1(IO x: INT)
	WRITE[ "PIO1",ENDL];
	x := x+2;
END

PROCEDURE PIO2(IO x1: INT,IO x2: INT)
	VAR T: INT;

	WRITE[ "PIO2",ENDL];
	T := x1;
	x1 := x2;
	x2 := T;
END

PROCEDURE PREF2(REF i: INT,REF x: BOOL)
	WRITE[  "PREF2 (i,x) = (",i,",",x,")",ENDL];
	x := NOT x;
	RETURN;
END

BEGIN
	VAR i: INT;

	CALL PIN0;

	CALL PIN1(1);

	x := 2;
	CALL PIN2(x,x+2);

	CALL POUT1(z);
	WRITE[ "z = ",z,ENDL];

	CALL POUT2(x,y);
	WRITE[  "x = ",x,", y = ",y,ENDL];

	WRITE[ "PIN1R(3)",ENDL];
	CALL PIN1R(3);


	y := 7;
	CALL PIO1(y);
	WRITE[ "y = ",y,ENDL];

	x := 3; y := 7;
	WRITE[ "(x,y) = (",x,",",y,")",ENDL];
	CALL PIO2(x,y);
	WRITE[ "(x,y) = (",x,",",y,")",ENDL];

	FOR i := 1 TO 2
	CALL PREF2(i,z);
	ENDFOR

ENDP