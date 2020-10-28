#include <xCore.h>

void setup() {
    pinMode(CORE_RED, OUTPUT);
    pinMode(CORE_GREEN, OUTPUT);
    pinMode(CORE_BLUE, OUTPUT);
}
void loop() {
    digitalWrite(CORE_RED, HIGH);
    delay(200);
    digitalWrite(CORE_RED, LOW);
    delay(200);

    digitalWrite(CORE_GREEN, HIGH);
    delay(200);
    digitalWrite(CORE_GREEN, LOW);
    delay(200);

    digitalWrite(CORE_BLUE, HIGH);
    delay(200);
    digitalWrite(CORE_BLUE, LOW);
    delay(200);
}