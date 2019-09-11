//增加功能：对PWM信号滤波，步进5

int prev_t = 0;
int pwm0_t_change = 0;//增加滤波变量
int pwm0_t = 0;

int prev1_t = 0;//方向
int pwm1_t_change = 0;//增加滤波变量
int pwm1_t = 0;//方向

int pwm0_mid =0;
int pwm1_mid =0;

#define pwm_max 2000
#define pwm_min 1100

#define base_speed_max 80
#define dir_speed_max 20

#define motor_speed_max 170
#define motor_speed_min 20

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
  attachInterrupt(0, rising, RISING);
  attachInterrupt(1, rising1, RISING);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //校准，获取中间值
  delay(100);
  byte i = 100;
  while(i--){
    if(abs(pwm0_t - pwm0_t_change) > 5){
      pwm0_t = pwm0_t_change;
    }
    //滤波
    if(abs(pwm1_t - pwm1_t_change) > 5){
      pwm1_t = pwm1_t_change;
    }
  }

  pwm0_mid = pwm0_t;
  pwm1_mid = pwm1_t;

// Serial.println(pwm0_mid);
// Serial.println(pwm1_mid);

}

void loop() {
  static int speed_a, speed_b, speed_dir , speed_base;
  
  //滤波
  if(abs(pwm0_t - pwm0_t_change) >20){
    pwm0_t = pwm0_t_change;
  }
  //滤波
  if(abs(pwm1_t - pwm1_t_change) > 20){
    pwm1_t = pwm1_t_change;
  }

  if(pwm0_t - pwm0_mid > 400){
    speed_base = base_speed_max;
  }
  else if(pwm0_t - pwm0_mid < -400){
    speed_base = -base_speed_max;
  }
  else{
    speed_base = map(pwm0_t - pwm0_mid, -400 , 400, -base_speed_max , base_speed_max);
  }

  if(pwm1_t - pwm1_mid > 400){
    speed_dir = dir_speed_max;
  }
  else if(pwm1_t - pwm1_mid < -400){
    speed_dir = -dir_speed_max;
  }
  else{
    speed_dir = map(pwm1_t - pwm1_mid, -400 , 400, -dir_speed_max , dir_speed_max);
  }
  speed_a = map(speed_base+speed_dir,-100,100,-150,150);
  speed_b = map(speed_base-speed_dir,-100,100,-150,150);

  if(speed_a < -5 ) speed_a -= 20;
  else if(speed_a > 5) speed_a += 20;
  else speed_a = 0;
  
  if(speed_b < -5) speed_b -= 20;
  else if(speed_b > 5) speed_b += 20;
  else speed_b = 0;
  A_move(speed_a);
  B_move(speed_b);
  
  Serial.print(speed_a);
  Serial.print("  ");
  Serial.println(speed_b);
/*
  Serial.print("speed_base");
  Serial.print(speed_base);
  Serial.print("   ");
  Serial.print("speed_dir");
  Serial.println(speed_dir);
*/

 // Serial.println(speed_a);
  /*

  Serial.print(pwm0_t);
  Serial.print("   ");
  Serial.println(pwm1_t);
*/
}


//A out1 out2
void A_move(int speed) {
  if (speed < 0) {
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
  }
  else {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
  }
  analogWrite(ENA, abs(speed));
}
//B out3 out4
void B_move(int speed) {
  if (speed > 0) {
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
  }
  else {
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
  }
  analogWrite(ENB, abs(speed));
}


//read PWM signal from receiver using interrupt on pin2
void rising() {
  attachInterrupt(0, falling, FALLING);
  prev_t = micros();
}

void falling() {
  attachInterrupt(0, rising, RISING);
  pwm0_t_change = micros() - prev_t;
  //Serial.println(pwm0_t);
}

void rising1() {
  attachInterrupt(1, falling1, FALLING);
  prev1_t = micros();
}

void falling1() {
  attachInterrupt(1, rising1, RISING);
  pwm1_t_change = micros() - prev1_t;
  //Serial.println(pwm1_t);
}
