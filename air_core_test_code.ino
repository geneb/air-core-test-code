//the mathematical constant pi, because we are using radians instead of degrees.
// 360 degrees = 2*pi, 180 = pi, 90 = pi/2, etc.
const float pi = 3.14159;

const float stepSize = 0.01; //This is the step size for incrementing the angle (in radians)

//this is the maximum resolution of the PWM output.
const int maxAnalogRes = 255;

/* Define the pins the meter is connected to. We need 1 PWM to vary the voltage in
 * each coil, and two more pins to reverse the polarity in that coil. So a total
 * of 6 pins (3 per coil).
 */
const int aircore1SinPin = 9; //this controls the voltage to the "sine" coil  (1,2EN)
const int aircore1CosPin = 10; //this controls the voltage to the "cosine" coil (3,4EN)
const int aircore1SinDirPin1 = 2;  //these two control the polarity to the "sine" coil (1IN)
const int aircore1SinDirPin2 = 4;  // 2IN
const int aircore1CosDirPin1 = 7;  //these two control the polarity to the "cosine" coil (3IN)
const int aircore1CosDirPin2 = 8;  // 4IN

// milliseconds between each angular step.
int delayTime = 5;
    
void setup()
{
  Serial.begin(9600);
  //Set the pins to OUTPUT
  pinMode(aircore1SinPin, OUTPUT);
  pinMode(aircore1CosPin, OUTPUT);
  pinMode(aircore1SinDirPin1, OUTPUT);
  pinMode(aircore1SinDirPin2, OUTPUT);
  pinMode(aircore1CosDirPin1, OUTPUT);
  pinMode(aircore1CosDirPin2, OUTPUT);
  
  // Set the initial direction as "forward". Of course it could be "reverse" depending on which lead you connected :-).
  digitalWrite(aircore1SinDirPin1, HIGH);
  digitalWrite(aircore1SinDirPin2, LOW);
  digitalWrite(aircore1CosDirPin1, HIGH);
  digitalWrite(aircore1CosDirPin2, LOW);
  
  Serial.println("Starting!");
}

void loop()                     // run over and over again
{
    //Rotate 0 through 360 degrees (OK, really 2*pi radians, but who cares?)
  //for(float i = 0; i < (2*pi); i = i + stepSize){
  for(float i = 0; i < 150; i+=10) {  // doing degrees
    setMeterPosition(aircore1SinPin, aircore1SinDirPin1, aircore1SinDirPin2, aircore1CosPin, aircore1CosDirPin1, aircore1CosDirPin2, i);
    delay(delayTime);           // waits for delayTime milliseconds    
  }

  //Rotate 360 through 0 degrees
  Serial.println("At zero.\n");
  //for(float i = (2*pi); i > 0; i = i - stepSize){
  for(float i = 360; i > 0; i--) { //doing degrees
    setMeterPosition(aircore1SinPin, aircore1SinDirPin1, aircore1SinDirPin2, aircore1CosPin, aircore1CosDirPin1, aircore1CosDirPin2, i);
    delay(delayTime);           // waits for delayTime milliseconds
  }
}


// setMeterPosition() - put it at the angle in radians
void setMeterPosition(int sinPin, int sinDirPin1, int sinDirPin2, int cosPin, int cosDirPin1, int cosDirPin2, float pos_degrees){
  // pos comes in as degrees and the code below only groks radians, so we need to convert.
  float pos_radians = pos_degrees * (pi / 180);
  
  // Calculate the voltage on the PWM pins based on the angle we want
  float sinCoilValue = maxAnalogRes*sin(pos_radians);
  float cosCoilValue = maxAnalogRes*cos(pos_radians);
  
  // change the polarity of the coil depending on the sign of the voltage level
  if (sinCoilValue<=0) {
    digitalWrite(sinDirPin1, LOW); //Change to LOW if using relays
    digitalWrite(sinDirPin2, HIGH); //comment this line out if using relays
  } else {
    digitalWrite(sinDirPin1, HIGH); //Change to HIGH if using relays
    digitalWrite(sinDirPin2, LOW); //comment this line out if using relays
  }
  if (cosCoilValue<=0) {
    digitalWrite(cosDirPin1, LOW); //Change to LOW if using relays
    digitalWrite(cosDirPin2, HIGH); //comment this line out if using relays
  } else {
    digitalWrite(cosDirPin1, HIGH); //Change to HIGH if using relays
    digitalWrite(cosDirPin2, LOW); //comment this line out if using relays
  }

  // take the absolute value because analogWrite doesn't take negatives
  sinCoilValue = abs(sinCoilValue);
  cosCoilValue = abs(cosCoilValue);

  analogWrite(sinPin, sinCoilValue);
  analogWrite(cosPin, cosCoilValue);
}
 
