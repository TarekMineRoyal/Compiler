PROGRAM MixedErrors;
VAR
  my_val: INTEGER;
  is_valid BOOLEAN; // Missing ':'
BEGIN
  my_val := 10 % 2; // Invalid lexical character '%'
  IF is_valid THEN
    result := my_val; // 'result' is undeclared
END.

{
Error Test 9: Mixed Lexical, Syntax, and Semantic Errors
Tests the compiler's ability to handle a cascade of different error types.
Expected Error(s):
Lexical Error (L:6, C:16): Unexpected character '%'
Syntax Error (L:4, C:19): syntax error
}