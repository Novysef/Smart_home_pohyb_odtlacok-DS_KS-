const int MOTION_SENSOR_PIN = 32; 
const int LED_PIN = 25; 
const int TONE_OUTPUT_PIN = 26; 
const int TONE_PWM_CHANNEL = 14; 

int motionStateCurrent = LOW;
int motionStatePrevious = LOW;

void setup(){
  Serial.begin(9600);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT); 
}

void loop(){
  motionStatePrevious = motionStateCurrent;
  motionStateCurrent = digitalRead(MOTION_SENSOR_PIN);

  if (motionStatePrevious == LOW && motionStateCurrent == HIGH) {
Serial.println("Pohyb zaznamenany");
delay(5000);
digitalWrite(LED_PIN, HIGH);
ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 3);
delay(10);
ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
delay(50);
ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 5);
delay(50);
ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 6);
delay(50);
}
 else if (motionStatePrevious == HIGH && motionStateCurrent == LOW) {
   delay(2000);
   Serial.println("ziadny pohyb");
   digitalWrite(LED_PIN, LOW);
   ledcDetachPin(TONE_OUTPUT_PIN);
 }
}
