PROGRAM LexicalError1;
BEGIN
  writeln('Hello');
  $ // Invalid character
END.

// Error Test 1: Lexical - Invalid Character
// Tests if the lexer correctly identifies and reports an invalid character.
// Expected Error(s): Lexical Error (L:4, C:3): Unexpected character '$'