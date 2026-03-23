#define ENA 5
#define ENB 3

// Run this file and verify that both wheels spin.
// If not double-check the wiring.

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  analogWrite(ENA, 150);
  analogWrite(ENB, 150); 

  analogWrite(6, 150);
  analogWrite(9, 0);
  analogWrite(10, 150);
  analogWrite(11, 0);
}