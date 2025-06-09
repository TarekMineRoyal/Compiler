// Test 20: Name Scoping (Shadowing)
// Tests that a local variable correctly 'shadows' a global variable of the same name.

PROGRAM Test20;
VAR
  x: INTEGER; // Global x

PROCEDURE ShadowTest;
  VAR
    x: INTEGER; // Local x
  BEGIN
    x := 99; // Should modify local x
    writeln(x);
  END;

BEGIN
  x := 1; // Modify global x
  ShadowTest; // Should print 99
  writeln(x);   // Should print 1 (global x is unchanged)
END.

// Expected Output:
// 99
// 1