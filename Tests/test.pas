PROGRAM Test;
VAR
  result: INTEGER;
FUNCTION GetNumber(a: INTEGER): INTEGER;
BEGIN
  RETURN 100;
END;

BEGIN
  result := GetNumber(1);
  writeln(result);
END.