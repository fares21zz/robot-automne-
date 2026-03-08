 #include <QTRSensors.h>
//#include <NewPing.h>

#define SETPOINT    3000
#define KP          0.8
#define KD          6
#define  ENA    6
#define ENB     3
#define MAX_SPEED   200
#define SET_SPEED   190
#define MIN_SPEED   0
#define NUM_SENSORS 8
#define NUM_SAMPLES_PER_SENSOR 2

#define echo 4
#define trigger 3
#define maxDist 5

#define S3 5
#define S4 6
#define OUT 7

int lastError = 0;

QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];

//NewPing sonar(trigger, echo, maxDist);

long currentMillis;
long lastMillis(0);


void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]) {
    A0, A1, A2, A3, A4, A5, A6, A7
  }, NUM_SENSORS);

  qtr.setSamplesPerSensor(NUM_SAMPLES_PER_SENSOR);
  
  qtr.calibrate();
  Serial.begin(9600);
  for (uint8_t i = 0; i < NUM_SENSORS; i++)
  {
    qtr.calibrationOn.minimum[i] = 800;
    qtr.calibrationOn.maximum[i] = 950;

  }

  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {

  currentMillis = millis();
  
   /* if (currentMillis - lastMillis > 30 ){
      int distance = sonar.ping_cm();

      Serial.print("Distance");
      Serial.print(distance);
      Serial.println(" cm");
      if (distance <= 10 & distance != 0){
        stopMotors();
        delay(1000);  
      }
    }*/
 
  if (digitalRead(A0) == HIGH || digitalRead(A1) == HIGH ||digitalRead(A2) == HIGH ||digitalRead(A3) == HIGH ||digitalRead(A4) == HIGH ||digitalRead(A5) == HIGH ||digitalRead(A6) == HIGH || digitalRead(A7) == HIGH  ){
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    delay(10);
  }
  if (linePos > 6500) {
    while (1) {
      linePos = qtr.readLineBlack(sensorValues);
      analogWrite(ENA, 100);
      analogWrite(ENB, 0);
      Serial.println("gauche");
      if (linePos > 2000 && linePos < 5000) {
        break;
      }
    }
  }
   if (linePos < 2500) {
    while (1) {
      linePos = qtr.readLineBlack(sensorValues);
      analogWrite(ENA, 0);
      analogWrite(ENB, 100);
      Serial.println("droite");
      if (linePos > 2000 && linePos < 5000) {
        break;
      }
    }

  }

  int error = SETPOINT - linePos;
  Serial.print(error); Serial.print('\t');

  int adjust = error * KP + KD * (error - lastError);
  Serial.print(adjust); Serial.println('\t');

  lastError = error;

  analogWrite(ENA, constrain(SET_SPEED - adjust, MIN_SPEED, MAX_SPEED));
  analogWrite(ENB, constrain(SET_SPEED + adjust, MIN_SPEED, MAX_SPEED));
}

