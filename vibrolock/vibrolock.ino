#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;


const int ledPin=12;
int sensorValue;
unsigned long temp_time;
unsigned long diff=0;
int num=0;

boolean learned = false;
boolean printed_mem = false;
boolean printed_cur = false;
boolean stored_cur = false;
boolean first_cur = false;
int prenum = 0;

int mem[] = {0,0,0,0,0,0,0,0,0,0}; //lock sequence
int cur[] = {0,0,0,0,0,0,0,0,0,0}; //current sequence

#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
int Steps = 0;
boolean Direction = true;
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=4095;
long time;


void setup() {
  Serial.begin(9600);
  Serial.println(".....");
  pinMode(ledPin,OUTPUT);
  pinMode(12, OUTPUT);

    for(int i=2; i<=6; i++) 
      pinMode(i, OUTPUT);


  //MOTOR:::::
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);

  //LCD:::::
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  write_LCD("LOCKED");
  lcd.setRGB(255,0,0);
  Serial.println("Setup complete");
} 
 
void loop() {

  //motor_loop();
  sensorValue = analogRead(A0);


    if(millis()-temp_time > 3000 && mem[0] != 0 && !learned) { //at least one knock was done
      learned = true;
      Serial.println("**********************");
    }

    if(learned && !printed_mem) {
      Serial.println("Printing mem");
      printArray(mem);
      printed_mem = true;
      Serial.println("Unlock pattern recorded. Please wait...");
      delay(3000);
      Serial.println("ready to be unlocked");
    }

    if(checkSize()) {
      //Serial.println("FLAG 1");
    //if(learned && stored_cur && !printed_cur) {
      Serial.println("Printing cur:");
      printArray(cur);
      printed_cur = true;
      checkValid();
      resetArray(cur);
      printed_cur = false;
      stored_cur = false;
      Serial.println("Please wait...");
      delay(3000);
      Serial.println("ready to be unlocked");
    }

    
  
  if(sensorValue > 17) { //if a knock occurred
    //Serial.print("knocked "); Serial.print("diff="); Serial.println(diff);
    
    diff = millis() - temp_time; //time since last knock
    if(prenum > 0) {
      //Serial.println("FLAG3");
      //record mem
      if(!learned) { //don't save first time of knock sequence
        for(int i=0; i<10; i++) { //loop through mem
          if(mem[i] == 0) { //if nothing is stored there
            mem[i] = diff; ///fix this for first time through???
            Serial.println(diff);
            if(i == 9) {
              learned = true;
              Serial.println("**********************");
            }
            break;
          }
        }
      }
  
  
      //record cur
      if(learned && diff < 3000) { //don't save first time of knock sequence
        
        for(int i=0; i<10; i++) { //loop through
//          if(!first_cur) {
//            first_cur = true;
//            break;
//            }
          if(cur[i] == 0) { //if nothing is stored there
            cur[i] = diff; ///fix this for first time through???
            Serial.println(diff);
            if(i == 9) {
              stored_cur = true;
            }
            break;
          }
        }
      }
  
  
  
//      if(diff < 3000)
//        Serial.println(diff);
      temp_time = millis();
      delay(50);
    }
    else {
      delay(50);
      prenum++;
      temp_time = millis();
    }
  }
  
}


void motor_loop() {

  while(1) {
  while(steps_left>0){
    currentMillis = micros();
    if(currentMillis-last_time>=1000){
      stepper(1); 
      time=time+micros()-last_time;
      last_time=micros();
      steps_left--;
      }
  }
  //Serial.println(time);
  //Serial.println("Wait...!");
  delay(2000);
  Direction=!Direction;
  steps_left=4095;

  ///LEDS:
  for(int i=2; i<=6; i++) {
    digitalWrite(i, HIGH);
    delay(80);
    digitalWrite(i, LOW);
    delay(80);
  }
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

void printArray(int ar[]) {
  Serial.println("\nPRINTING ARRAY[]");
  for(int i=0; i<10; i++) {
    Serial.println(ar[i]);
    
  }
  Serial.println("\n");
}

void checkValid() {
  Serial.println("CHECKING UNLOCK PATTERN"); //knock lock
  for(int i=0; i<10; i++) {
//    double error = (double) abs((cur[i]-mem[i])/mem[i]);
    double error = (double) (cur[i]-mem[i])/mem[i];
    error = abs(error);
    error = 100 * error;
    Serial.print("error="); Serial.println(error);
    Serial.print(cur[i]); Serial.print("\t"); Serial.print(mem[i]); Serial.print("\t");
    Serial.print((double) error); Serial.println("%");
    
    if(error > 40.0 || error < -40.0 ) {///.4 for now
      Serial.print(cur[i]); Serial.print(" !~ "); Serial.println(mem[i]);
      Serial.println("INCORRECT");
      break;
    }
    if(i==9) {
      Serial.println("UNLOCKED!!!!!!!!!!!!!!");
      write_LCD("UNLOCKED");
      lcd.setRGB(0,255,0);
      motor_loop();
    }
      
  }
}

void resetArray(int ar[]) {
  for(int i=0; i<10; i++) {
    ar[0] = 0;
  }
}

boolean checkSize() {
  int mem_length=0;
  int cur_length=0;
  for(int i=0; i<10; i++) {
    if(mem[i]==0)
      break;
    mem_length++;
  }

  for(int i=0; i<10; i++) {
    if(cur[i]==0)
      break;
    cur_length++;
  }
  

  if(mem_length != cur_length || mem_length==0)
    return false;
  return true;
}

void write_LCD(char s[]) {
  lcd.clear();
  lcd.write(s);
}


