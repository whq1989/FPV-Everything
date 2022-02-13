//motor connected between A01 and A02

int STBY = 10; //standby   停止状态
int PWMA = 3; //Speed control  速度
int AIN1 = 9; //Direction  方向
int AIN2 = 8; //Direction  方向

void setup(){
pinMode(STBY, OUTPUT);

pinMode(PWMA, OUTPUT);
pinMode(AIN1, OUTPUT);
pinMode(AIN2, OUTPUT);

//pinMode(PWMB, OUTPUT);
//pinMode(BIN1, OUTPUT);
//pinMode(BIN2, OUTPUT);
}

void loop(){
move(1,90, 1); //motor 1, full speed, left
//move(2, 255, 1); //motor 2, full speed, left

delay(1000); //go for 1 second
stop(); //stop
delay(1250); //hold for 250ms until move again

move(1, 90, 0); //motor 1, half speed, right
//move(2, 128, 0); //motor 2, half speed, right

delay(1000);
stop();
delay(1250);
}

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A 选择电机
//speed: 0 is off, and 255 is full speed 选择速度
//direction: 0 clockwise, 1 counter-clockwise 选择旋转方向： 0表示顺时针，1表示逆时针

digitalWrite(STBY, HIGH); //disable standby取消空置状态

boolean inPin1 = LOW;
boolean inPin2 = HIGH;

if(direction == 1){
inPin1 = HIGH;
inPin2 = LOW;
}

if(motor == 1){
digitalWrite(AIN1, inPin1);
digitalWrite(AIN2, inPin2);
analogWrite(PWMA, speed);
}else{
//digitalWrite(BIN1, inPin1);
//digitalWrite(BIN2, inPin2);
//analogWrite(PWMB, speed);
}
}

void stop(){
//enable standby
digitalWrite(STBY, LOW);
}
