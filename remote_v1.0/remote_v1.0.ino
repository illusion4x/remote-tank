int prev_t = 0;
int pwm0_t = 0;

int prev1_t = 0;//方向
int pwm1_t = 0;//方向

#define PWM_mid 1512
#define PWM_max 2012
#define PWM_min 1012

#define SPEED_max 175
#define SPEED_min 30

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
}

void loop() {
  static int speed_a, speed_b , speed_sum, dir;
  speed_a = 0, speed_b = 0 , speed_sum = 0, dir = 0;
  speed_sum = map(pwm0_t - PWM_mid, -511, 511, -SPEED_max, SPEED_max);
  dir = map(pwm1_t - PWM_mid, -511, 511, -100, 100);
  speed_a = limit(speed_sum + dir);
  speed_b = limit(speed_sum - dir);
  A_move(speed_a);
  B_move(speed_b);
}

int limit(int x) {
  if ( x > -SPEED_min && x < SPEED_min)
    return 0;
  if ( x >= SPEED_max)
    return SPEED_max;
  if ( x <= -SPEED_max)
    return -SPEED_max;
  return x;
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
    speed = -speed;
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
  pwm0_t = micros() - prev_t;
  //Serial.println(pwm0_t);
}

void rising1() {
  attachInterrupt(1, falling1, FALLING);
  prev1_t = micros();
}

void falling1() {
  attachInterrupt(1, rising1, RISING);
  pwm1_t = micros() - prev1_t;
  //Serial.println(pwm1_t);
}
