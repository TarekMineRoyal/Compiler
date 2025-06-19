// Test 31: Chained Procedure Calls
// Tests if multiple procedures can call each other in a sequence.

PROGRAM Test31;
PROCEDURE ProcC(val: INTEGER);
BEGIN
  write('C received: ', val);
END;

PROCEDURE ProcB(val: INTEGER);
BEGIN
  write('B received: ', val);
  ProcC(val * 2);
END;

PROCEDURE ProcA(val: INTEGER);
BEGIN
  write('A received: ', val);
  ProcB(val + 5);
END;

BEGIN
  ProcA(10);
END.

{
expectedOutput:
A received: 10
B received: 15
C received: 30
}