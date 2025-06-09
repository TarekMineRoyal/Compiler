// Test 24: Comprehensive Logical Operators
// Tests AND, OR, and NOT operators and their precedence.

PROGRAM Test24;
VAR
  a, b, c, d: BOOLEAN;
BEGIN
  // (TRUE OR FALSE) -> TRUE
  a := TRUE OR FALSE; 
  // (TRUE AND FALSE) -> FALSE
  b := TRUE AND FALSE;
  // NOT (TRUE AND FALSE) -> NOT FALSE -> TRUE
  c := NOT (TRUE AND FALSE); 
  // TRUE OR TRUE AND FALSE -> TRUE AND FALSE -> FALSE
  d := TRUE OR TRUE AND FALSE; 
  
  writeln(a);
  writeln(b);
  writeln(c);
  writeln(d);
END.

{
Expected Output:
1
0
1
1
}