const int ledPin=12;
int sensorValue;
unsigned long temp_time;
int num=0;

#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
int Steps = 0;
boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=4095;
long time;


void setup() {
  
  Serial.begin(9600);
  Serial.println(".....");
  pinMode(ledPin,OUTPUT);
  pinMode(12, OUTPUT);


  //MOTOR:::::
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  Serial.println("Setup complete");
} 
 
void loop() {

//MOTOR::::::
//while(steps_left>0){
//    currentMillis = micros();
//    if(currentMillis-last_time>=1000){
//      stepper(1); 
//      time=time+micros()-last_time;
//      last_time=micros();
//      steps_left--;
//      }
//  }
//  Serial.println(time);
//  Serial.println("Wait...!");
//  delay(2000);
//  Direction=!Direction;
//  steps_left=4095;

  sensorValue = analogRead(A0);
//  if(temp != sensorValue)
//    Serial.println(sensorValue);


  
  if(sensorValue > 15) {
    //Serial.print(sensorValue);
    //Serial.print("Tapped: wait=");
    if(millis() - temp_time > 3000)
      Serial.println("**********************");
    Serial.println(millis() - temp_time);
    temp_time = millis();
    delay(50);
  }
  
  if(sensorValue==1023)
    {
    digitalWrite(ledPin,HIGH);
    }
  else
    {
    digitalWrite(ledPin,LOW);
    }
}


void stepper(int xw){
    for (int x=0;x<xw;x++){
  switch(Steps){
     case 0:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
     break; 
     case 1:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, HIGH);
     break; 
     case 2:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
     break; 
     case 3:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
     break; 
     case 4:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
     break; 
     case 5:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
     break; 
       case 6:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
     break; 
     case 7:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
     break; 
     default:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
     break; 
  }
  SetDirection();
  }
  } 
  void SetDirection(){
  if(Direction==1){ Steps++;}
  if(Direction==0){ Steps--; }
  if(Steps>7){Steps=0;}
  if(Steps<0){Steps=7; }
}


