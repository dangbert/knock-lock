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



  ///NEED TO RESET CUR AFTER EACH PATTERN ENTERED
  if(sensorValue > 15) { //if a knock occurred
    diff = millis() - temp_time; //time since last knock
    
    //when done recording mem
    if(diff > 3000 && mem[0] != 0) { //at least one knock was done
      learned = true;
      Serial.println("**********************");
    }

    //record mem
    if(!learned && diff<3000) { //don't save first time of knock sequence
      for(int i=0; i<10; i++) { //loop through 
        if(mem[i] == 0) { //if nothing is stored there
          mem[i] = diff; ///fix this for first time through???
          if(i == 9)
            learned = true;
          break;
        }
      }
    }

    //record cur
    if(learned && diff < 3000) { //don't save first time of knock sequence
      
      for(int i=0; i<10; i++) { //loop through
        if(!first_cur) {
          first_cur = true;
          break;
          }
        if(cur[i] == 0) { //if nothing is stored there
          cur[i] = diff; ///fix this for first time through???
          if(i == 9) {
            stored_cur = true;
          }
          break;
        }
      }
    }


    if(learned && stored_cur && !printed_cur) {
          Serial.println("Printing cur:");
          printArray(cur);
          printed_cur = true;
          checkValid();
    }

//    if(learned && !printed_cur) {
//      for(int i=0; i<10; i++) {
//        //if(mem[i] != 0 && cur[i]==0) ////////THIS IS IMPORTANT!!!
//          //break;
//        
//        if(i == 9) {
//          Serial.println("Printing cur:");
//          printArray(cur);
//          printed_cur = true;
//        }
//
//      }
//    }


    if(diff < 3000)
      Serial.println(diff);
    temp_time = millis();
    delay(50);

    if(learned && !printed_mem) {
      Serial.println("Printing mem");
      printArray(mem);
      printed_mem = true;
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
  Serial.println("checking unknock pattern"); //knock lock
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
    if(i==9)
      Serial.println("AYYYYYYY LMAO UNLOCKED");
      
  }
}


