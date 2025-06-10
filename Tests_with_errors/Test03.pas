PROGRAM SyntaxError1;
BEGIN
  writeln('Line 1')
  writeln('Line 2');
END.

// Error Test 3: Syntax - Missing Semicolon
// Tests if the parser reports a syntax error when a semicolon is missing between statements.
// Expected Error(s): Syntax Error (L:4, C:3): syntax error