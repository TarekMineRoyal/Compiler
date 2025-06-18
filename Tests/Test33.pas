// Test 33: Keyword Collision
// Purpose: Verifies that a function named after a VM keyword (e.g., 'add')
// can be declared and called correctly due to name mangling (e.g., to 'f_add_i_i').

PROGRAM TestKeyword;
VAR
  res: INTEGER;

FUNCTION add(a: INTEGER; b: INTEGER): INTEGER;
BEGIN
  RETURN a + b;
END;

BEGIN
  res := add(10, 5); // Should call f_add_i_i
  writeln('Result of custom add function:');
  writeln(res);
END.

// Expected Output:
// Result of custom add function:
// 15