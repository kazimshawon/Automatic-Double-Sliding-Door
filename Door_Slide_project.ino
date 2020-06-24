#define pirSensor1 12
#define pirSensor2 5
int ForwardLimitSwitch = 2;
int BackwardLimitSwitch = 3;
int ENAV = 100;
int ENBV = 100;
int ForwardLimitStatus = 0;
int BackwardLimitStatus = 0;
int ENA = 6;  //ENA connected to digital pin 6
int ENB = 11;  //ENB connected to digital pin 11
#define IN_11  7    // L298N #1 in 1 motor Right 
#define IN_12  8    // L298N #1 in 2 motor Right 
#define IN_13  9    // L298N #1 in 3 motor Left 
#define IN_14  10    // L298N #1 in 4 motor Left 
void setup() {
  pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(ForwardLimitSwitch, INPUT_PULLUP);
  pinMode(BackwardLimitSwitch, INPUT_PULLUP);
  pinMode(pirSensor1, INPUT);
  pinMode(pirSensor2, INPUT);
  pinMode(IN_11, OUTPUT);
  pinMode(IN_12, OUTPUT);
  pinMode(IN_13, OUTPUT);
  pinMode(IN_14, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ForwardLimitSwitch), ForwardLimit, RISING);
  attachInterrupt(digitalPinToInterrupt(BackwardLimitSwitch), BackwardLimit, RISING);
  Serial.begin(9600);

}

void loop() {

  if (digitalRead(pirSensor1) == HIGH) {
    startAgain:
    Serial.println("Motion");
    Serial.println("Opening");
    for(int x = 0; x < 100000; x++) {
      if(ForwardLimitStatus == 0){
        goForward();
        BackwardLimitStatus = 0;
      }
      else{
        goBackward();
        stopAll();
        delay(5000);
        break;
      }
    }    
    for(int x = 0; x < 100000; x++) {
      if(digitalRead(pirSensor1) == HIGH){
        stopAll();
        goto startAgain;
      }
      else{
        if(BackwardLimitStatus == 0){
          goBackward();
          ForwardLimitStatus = 0;
        }
        else{
          goForward();
          stopAll();
          break;
        }
      }
    }
  }
  else {
    Serial.println("Idle");
    stopAll();
  }

}

void goForward(){ 
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(IN_11, HIGH);
  digitalWrite(IN_12, LOW);

  digitalWrite(IN_13, HIGH);
  digitalWrite(IN_14, LOW);
}

void goBackward(){ 
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(IN_11, LOW);
  digitalWrite(IN_12, HIGH);

  digitalWrite(IN_13, LOW);
  digitalWrite(IN_14, HIGH);
}
void stopAll(){  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN_11, LOW);
  digitalWrite(IN_12, LOW);

  digitalWrite(IN_13, LOW);
  digitalWrite(IN_14, LOW);

}
void ForwardLimit(){
  ForwardLimitStatus = 1;
}
void BackwardLimit(){
  BackwardLimitStatus = 1;
}
