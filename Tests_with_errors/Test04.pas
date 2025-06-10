PROGRAM SyntaxError2;
BEGIN
  writeln('test');
END

// Error Test 4: Syntax - Missing Program Period
// Tests if the parser reports an error if the final 'END.' is missing its period.
// Expected Error(s): Syntax Error (L:8, C:61): syntax error