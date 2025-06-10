PROGRAM MultiSyntax;
VAR
  x INTEGER; // Missing ':'
BEGIN
  x := (10 + 5; // Missing ')'
  IF x > 10 THEN
    writeln('ok') // Missing ';'
END.

{
Error Test 11: Multiple Syntax Errors
Tests the parser's ability to report multiple syntax issues.
Expected Error(s):
Syntax Error (L:3, C:12): syntax error

}