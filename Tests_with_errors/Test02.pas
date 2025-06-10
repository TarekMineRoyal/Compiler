PROGRAM LexicalError2;
VAR
  x: INTEGER; { // This comment never ends
BEGIN
  x := 1;
END.

// Error Test 2: Lexical - Unterminated Comment
// Tests if the lexer reports an error for a comment that is never closed.
// Expected Error(s): Lexical Error (L:6, C:5): Unterminated comment that started at (L:3, C:15).