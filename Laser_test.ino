int switch1=1;
int Laser_ON=6;
int LED_grun_1=10;
int LED_grun_2=11;
int LED_orange=12;
int LED_rot=13;



void setup() {


pinMode(switch1,INPUT_PULLUP);
pinMode(Laser_ON,OUTPUT);



digitalWrite(Laser_ON,LOW);
}

void loop() {
  digitalWrite(Laser_ON,HIGH);

}
