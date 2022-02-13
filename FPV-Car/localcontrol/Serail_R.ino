void setup() {
  Serial.begin(9600); 
  Serial.println("BT is ready!");
}

void loop() {
  if (Serial.available()) {
    //char serialData=Serial.read();//读取第一个字符
    int n=Serial.parseInt();//读取串口中的完整整数信息
    //Serial.println(serialData);
    Serial.println(n);
  }
}
