#include <SparkFun_TB6612.h>
#include <Servo.h>

// ----------------- Standby -----------------
#define STBY 34

// ----------------- TB6612 #1 -----------------
#define AIN1_1 22
#define AIN2_1 23
#define PWMA_1 5
#define BIN1_1 24
#define BIN2_1 25
#define PWMB_1 6

// ----------------- TB6612 #2 -----------------
#define AIN1_2 26
#define AIN2_2 27
#define PWMA_2 7
#define BIN1_2 28
#define BIN2_2 29
#define PWMB_2 8

// ----------------- TB6612 #3 -----------------
#define AIN1_3 30
#define AIN2_3 31
#define PWMA_3 9
#define BIN1_3 32
#define BIN2_3 33
#define PWMB_3 10

#define OFFSET 1   // Set to -1 if a motor spins backward

// Create Motor Objects
Motor frontLeft(PWMA_1, AIN1_1, AIN2_1, OFFSET, STBY);
Motor frontRight(PWMB_1, BIN1_1, BIN2_1, OFFSET, STBY);

Motor midLeft(PWMA_2, AIN1_2, AIN2_2, OFFSET, STBY);
Motor midRight(PWMB_2, BIN1_2, BIN2_2, OFFSET, STBY);

Motor rearLeft(PWMA_3, AIN1_3, AIN2_3, OFFSET, STBY);
Motor rearRight(PWMB_3, BIN1_3, BIN2_3, OFFSET, STBY);

// ----------------- Servos -----------------
Servo servoFL;
Servo servoFR;
Servo servoRL;
Servo servoRR;

// ----------------- Setup -----------------
void setup()
{
  Serial.begin(115200);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  servoFL.attach(40);
  servoFR.attach(41);
  servoRL.attach(42);
  servoRR.attach(43);

  centerSteering();

  delay(2000);
}

// ----------------- Loop -----------------
void loop()
{
  Serial.println("Forward");

  driveAll(120);
  delay(2000);

  Serial.println("Stop");
  brakeAll();
  delay(1000);

  Serial.println("Turn Wheels");

  servoFL.write(70);
  servoFR.write(110);
  servoRL.write(70);
  servoRR.write(110);

  delay(1000);

  Serial.println("Forward With Turn");

  driveAll(120);
  delay(2000);

  Serial.println("Final Stop");
  brakeAll();

  while (1); // Halt forever
}

// ----------------- Helper Functions -----------------

void driveAll(int speed)
{
  frontLeft.drive(speed);
  frontRight.drive(speed);
  midLeft.drive(speed);
  midRight.drive(speed);
  rearLeft.drive(speed);
  rearRight.drive(speed);
}

void brakeAll()
{
  frontLeft.brake();
  frontRight.brake();
  midLeft.brake();
  midRight.brake();
  rearLeft.brake();
  rearRight.brake();
}

void centerSteering()
{
  servoFL.write(90);
  servoFR.write(90);
  servoRL.write(90);
  servoRR.write(90);
}
