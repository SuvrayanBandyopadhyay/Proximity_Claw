//Including the nescessary libraries
#include<Servo.h>


 #define arrsize 10
int trigPin = 3;    // Trigger
int echoPin = 2;    // Echo
double duration, cm, inches;
const int steppin=5;
const int dirpin=4;

//array for storing last arrsize distances so that one rogue value does not mess up our calculation
double arr[arrsize];
//Function to get average value in such a way that it tries to ignore the effect of outliers
double powavg(double arr[],int size)
{
  double x = 0.2;
  double ret = 0;
  for(int i=0;i<size;i++)
  {
    ret += pow(arr[i],x);
  }
  
  ret = ret/(double)size;
  ret = pow(ret,1.0/x);



  return ret;
}

//Defining our Servo Object
Servo myservo;
//The open function and global bool variables
int oangle = 120;
int cangle = 80;
void openservo()
{
  for(int i=cangle;i<=oangle;i++)
  {
    myservo.write(i);
    delay(20);
  }
}
void closeservo()
{
  for(int i=oangle;i>=cangle;i--)
  {
    myservo.write(i);
    delay(20);
  }
}
void pick(){
  digitalWrite(dirpin,HIGH);
  for (int i=0;i<100;i++){
    for(int i=0;i<=800;i++){
    digitalWrite(steppin,HIGH);
    delayMicroseconds(100);
    digitalWrite(steppin,LOW);
    delayMicroseconds(100);
  }
  }
}
void descend(){
  digitalWrite(dirpin,LOW);
  for (int i=0;i<100;i++){
    for(int i=0;i<=800;i++){
    digitalWrite(steppin,HIGH);
    delayMicroseconds(100);
    digitalWrite(steppin,LOW);
    delayMicroseconds(100);
  }  
  }
}

bool open;

void setup() {
  open =0;
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(steppin,OUTPUT);
  pinMode(dirpin,OUTPUT);
  //Setting up the Servo Object
  myservo.attach(9);
  pick();
}
 //The looping variable
 int i = 0;
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  

 

  double average = powavg(arr,arrsize);

  Serial.print(cm);
  Serial.print("cm ");
  Serial.print(average);
  Serial.print("cm (wavg)");
  Serial.println();
   //Getting the last arrsize average value for better judgement 
  arr[i] = cm;
  i++;
  if(i==arrsize)
  {
    i=0;

  }
  //The servo logic
  //If ball is close and it is open, then close
  if(average>2.5 && average <9 && open == 1)
  {
    open =0;
    Serial.println("CLOSE");
    closeservo();
    pick();

  }

                                                                                                                                                                                                                                                                                    //If it is close and ball can no longer be detected,then open
  if(average>18 && open == 0)
  {
      open = 1;                                                                                                                                                                                                                                                                                                                                                                                                  
    Serial.println("OPEN");
    openservo();
    descend();

  }

  delay(50);
}