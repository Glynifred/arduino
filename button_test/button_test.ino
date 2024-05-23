
#define btn 2  //assuming we use D2 on Arduino

void setup() {
  pinMode(btn, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  Serial.println(digitalRead(btn));
  if (debounce()) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
