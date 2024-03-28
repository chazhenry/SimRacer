int r_pwm, l_pwm, r_en, l_en; // pin numbers for power and enable pins

int power = 120;
int power_max = 180;
int power_min = 50;
int direction = 0;  // 0 = forward, 1 = reverse
int factor = 1;  // 1 = increase power, -1 = decrease power
int loopcount = 0;

int pot1Value = 0;  // raw Value from potentiometer on A0
int pot2Value = 0;  // raw Value from potentiometer on A1
float pot1Angle = 0.0;  // converted to 0-360 angle
float pot2Angle = 0.0;
int currentMotor = 0;

void setMotor(int m)
{
  analogWrite(r_pwm, 0);   // shut down existing movement
  analogWrite(l_pwm, 0);
  if (m==0) // left motor
    {
    // pins
    r_pwm = 4;
    l_pwm = 10;
    r_en = 5;  // Enable pin for both directions
    l_en = 5;  // Same as above, considering they're wired as one
    }
  else  // right motor
    {
    // pins
    r_pwm = 2;
    l_pwm = 9;
    r_en = 3;  // Enable pin for both directions
    l_en = 3;  // Same as above, considering they're wired as one      
    }
  pinMode(r_en, OUTPUT);
  pinMode(l_en, OUTPUT);
  pinMode(r_pwm, OUTPUT);
  pinMode(l_pwm, OUTPUT);
  digitalWrite(r_en, HIGH);
  digitalWrite(l_en, HIGH);
}

void setup() 
{
  Serial.begin(9600);  // Initialize serial communication
  Serial.println("Setup!");
  setMotor(0);  
  delay(3000);
}

void loop() 
{
    // Check if data is available to read
    if (Serial.available() > 0) 
    {
      char incomingByte = Serial.read();  // Read the incoming byte:
      if (incomingByte == 'a') 
        {
          if (power < power_max)
            power += 10;
          Serial.println("Accelerate: " + String(power));
        } 
      else if (incomingByte == 'd') 
        {
          if (power > power_min)  // 50 will be minimum
            power -= 10; 
          Serial.println("Decelerate:" + String(power));
        }
    }

    delay(1000);
    pot1Value = analogRead(A0);
    pot2Value = analogRead(A1);
    pot1Angle = (pot1Value / 1023.0) * 360.0;
    pot2Angle = (pot2Value / 1023.0) * 360.0;
    Serial.println("Motor:" + String(currentMotor) + " Power:" + String(power) + ", Direction:" + (direction == 0 ? "Forward" : "Reverse") + ", Pot1:" + String(pot1Value) + ", Pot2:" + String(pot2Value) + ", PotAngle1: " + String(pot1Angle, 1) + ", PotAngle2: " + String(pot2Angle, 1) + "Pins:" + String(r_pwm) + "/" + String(l_pwm));
    loopcount++;

    // Conditions for changing direction or adjusting power
    if (loopcount == 20) // both directions completed. switch motors
      {
        Serial.println("Motor Switch!!"); 
        currentMotor ^= 1;  // swaps between 0 & 1
        setMotor(currentMotor);
        loopcount = 0;
        direction = 0;
      }

    if (loopcount == 10) 
      {
      Serial.println("Halt. Motor Reverse!");
      direction = !direction;  // Change direction
      }

    // Apply power and direction adjustments
    if (direction == 0) 
      {  // Forward
        analogWrite(l_pwm, 0);  // Ensure the other direction is off
        analogWrite(r_pwm, power);
      } 
    else 
      {  // Reverse
        analogWrite(r_pwm, 0);  // Ensure the other direction is off
        analogWrite(l_pwm, power);
      }
}
