// Test 37: Local Array Declaration and Access
// Purpose: This test verifies that the compiler can correctly handle the
// declaration, allocation, assignment to, and reading from an array that
// is declared locally within a procedure. It confirms that the stack frame
// for local variables (including array pointers) is allocated correctly
// and that symbols for local arrays are properly managed during code generation.

PROGRAM TestLocalArray;

PROCEDURE FillAndPrint;
  VAR
    myArr: ARRAY[0..2] OF INTEGER;
    i: INTEGER;
  BEGIN
    // First loop: Fill the array
    i := 0;
    WHILE i <= 2 DO
    BEGIN
      myArr[i] := i * 10;
      i := i + 1;
    END;

    // Second loop: Print the array elements
    i := 0;
    WHILE i <= 2 DO
    BEGIN
      writeln(myArr[i]);
      i := i + 1;
    END;
  END;

BEGIN // Main program body
  FillAndPrint;
END.

// Expected Output:
// 0
// 10
// 20