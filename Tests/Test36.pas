// Test: Function Overloading
// Purpose: This test verifies that the
// compiler can declare and correctly resolve calls to two functions
// that share the same name but have different parameter types.

PROGRAM TestOverloading;

VAR
  int_result: INTEGER;
  real_result: REAL;

// The compiler will store this function with the mangled key "f_Process_i".
FUNCTION Process(val: INTEGER): INTEGER;
BEGIN
  RETURN val * 2;
END;

FUNCTION Process(val: REAL): REAL;
BEGIN
  RETURN val * 2.0;
END;

BEGIN
  // The Semantic Analyzer sees Process(10) and looks up "f_Process_i".
  int_result := Process(10);
  
  // The Semantic Analyzer sees Process(5.5) and looks up "f_Process_r".
  real_result := Process(5.5);

  writeln('Calling Process(10) returned:');
  writeln(int_result);
  writeln('Calling Process(5.5) returned:');
  writeln(real_result);
END.

{
Calling Process(10) returned:
20
Calling Process(5.5) returned:
11.0
}