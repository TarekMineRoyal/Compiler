// Test Mangle 35: Complex Signature Mangling
// Purpose: Verifies that a subprogram with multiple, mixed-type
// parameters gets a correctly mangled name.

PROGRAM TestComplex;

PROCEDURE Display(id: INTEGER; amount: REAL; isValid: BOOLEAN);
BEGIN
  write('ID: ', id, ', Amount: ', amount, ', Valid: ', isValid);
  writeln;
END;

BEGIN
  // This call should resolve to p_Display_i_r_b
  Display(123, 45.67, TRUE);
END.

// Expected Output:
// ID: 123, Amount: 45.67, Valid: 1