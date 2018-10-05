PROGRAM aa(input, output, error); 
VAR a, b: INTEGER;
VAR c: REAL;

FUNCTION fact(x:INTEGER) : INTEGER;
BEGIN
    if(x = 1) then
        fact := x
    else 
	
        fact := x*fact(x-1)
	
END;

BEGIN
    a := fact(3);
	printInt(a);
 a := -8;
		
         
      while a < 5 do
      begin
	a   := a + 1; 
	while a<0 do
	begin
	a:=a+1;
	printInt(a);
	end;
	
      end;
END.
