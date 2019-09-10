int prev_t = 0;
int pwm0_t = 0;

#define PWM_mid 1512
#define PWM_max 2012
#define PWM_min 1012

#define IN1 12
#define IN2 13
#define ENA 11
#define PWMA 11

#define IN3 8
#define IN4 7
#define ENB 10
#define PWMB 10

#define forward 0
#define back    1


void setup() {
    Serial.begin(9600);
    attachInterrupt(0,rising,RISING);

    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);
    pinMode(ENA,OUTPUT);
    pinMode(ENB,OUTPUT);
}

void loop() {
   
}
//A out1 out2
void A_move(int dir, int speed){
  if(dir == forward){
    digitalWrite(IN1,1);
    digitalWrite(IN2,0);
  }
  else if(dir == back){
    digitalWrite(IN1,0);
    digitalWrite(IN2,1); 
  }
  analogWrite(ENA,speed);
}
//B out3 out4
void B_move(int dir, int speed){
  if(dir == forward){
    digitalWrite(IN3,1);
    digitalWrite(IN4,0);
  }
  else if(dir == back){
    digitalWrite(IN3,0);
    digitalWrite(IN4,1); 
  }
  analogWrite(ENB,speed);
}


//read PWM signal from receiver using interrupt on pin2 
void rising(){
    attachInterrupt(0,falling,FALLING);
    prev_t = micros();  
}

void falling(){
    attachInterrupt(0,rising,RISING);
    pwm0_t = micros() - prev_t;
    Serial.println(pwm0_t);  
}
