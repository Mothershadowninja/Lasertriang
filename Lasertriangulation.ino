/*
  This file contans the Software for the Lasertriangulationsensor LPE5854-X. LPE584-X is a Lastertrinagulationsensor with a Range of 0,3m-1m. It useing a 650nm Lasser.
  And a epc90 as a Detector. The Result of the Messering should be displayed on a SSD1306 monochromatic Display. For Connection I2C is used.
  Autor: Nicola Anesini 
  Company: Shadow Tech (FHGR)
  Date: 18.02.2021
  Version: 0.1 
  Changelog: 
  0.1            18.02.2021                   Nicola Anesini              Staring Sequence 



*/
/********************************************************************************************/
 /* Includes
/********************************************************************************************/
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include <avr/io.h>

/********************************************************************************************/
 /* Constants
/********************************************************************************************/

#define SCREEN_WIDTH 128 // OLED Display Breite in Pixel 
#define SCREEN_HEIGHT 64 // OLED Display Höhe in Pixel

#define OLED_RESET -1 // Reset pin Nummer Definition. -1, da Reset Pin mit Arduino geteilt wird
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaration des Displays ssd1306 an eine I2C Verbindung

#define Taster 1 
#define Data_RDY 2 
#define Clear_Pix 3
#define Shutter 4
#define READ 5
#define Laser_ON 6
#define Video_P A0

#define LED_Betrieb 13
#define LED_Messering_ON 12
#define LED_Out_of_Rage 11
#define LED_Alert 10



/********************************************************************************************/
 /* Globale Variables
/********************************************************************************************/

// Variable for the I2C address of the epc sesnor
byte epc_address = 0x15;
// Variable for the I2C address of the Display
byte ssd_addresse = 0xBD;
// Variable for the Register Adress for regulation the Frequenz of epc90
byte epc90_OSC_TRIM = 0x90;
// Response from the I2C command
byte error;
int value;                        //Variable für den Analogen Wertes
//Times in microseconds
int t_read_clk =20;
int t_read = 100;
int t_cds=100;
int t_flush=100;
int t_clr_pix=10;
int t_exposure=10;
int Distance=0+300;

/********************************************************************************************/
 /* Setup. Called once at programmstart
/********************************************************************************************/
void setup() {

int all_okay; // Checking for all Device on I2C-Bus

// Defingi Inputpins
pinMode(Taster,INPUT_PULLUP);
pinMode(Data_RDY,INPUT);

// Defining Outputpins 
pinMode(Clear_Pix,OUTPUT);
pinMode(Shutter,OUTPUT);
pinMode(Laser_ON,OUTPUT);

pinMode(LED_Betrieb,OUTPUT);
pinMode(LED_Messering_ON,OUTPUT);
pinMode(LED_Out_of_Rage,OUTPUT);
pinMode(LED_Alert,OUTPUT);



// Testing all LED for function 
digitalWrite(Laser_ON,LOW);
digitalWrite(LED_Betrieb,HIGH);
digitalWrite(LED_Messering_ON,HIGH);
digitalWrite(LED_Out_of_Rage,HIGH);
digitalWrite(LED_Alert,HIGH);
delay(500);
digitalWrite(LED_Betrieb,LOW);
digitalWrite(LED_Messering_ON,LOW);
digitalWrite(LED_Out_of_Rage,LOW);
digitalWrite(LED_Alert,LOW);

// Initialisation of the I2C communication
Wire.begin();
// Initialisation of the serial communication to the terminal
Serial.begin(9600);
// Start a trasmission to the device address
Wire.beginTransmission(epc_address);
// Device address is called. Result is an errorcode
// 0 = Device found, 4 = error, else no device found)
error = Wire.endTransmission();

// Device was found
if (error == 0){
  Serial.println("Epc90 found");
  all_okay ++;
}
// Error in communication
else if (error==4){
  Serial.println("Unknow error");
}
// No device found  
else
{
  Serial.println("Epc90 not found\n");
}
Wire.beginTransmission(ssd_addresse);
// Device address is called. Result is an errorcode
// 0 = Device found, 4 = error, else no device found
error = Wire.endTransmission();

// Device was found
if (error == 0){
  Serial.println("SSD1306 found");
  all_okay ++;
}
// Error in communication
else if (error==4){
  Serial.println("Unknow error");
}
// No device found  
else
{
  Serial.println("SSD1306 not found\n");
}
// Checker for all needed Devices 
if(all_okay == 2){
  digitalWrite(LED_Betrieb,HIGH);
}
else{
  digitalWrite(LED_Alert,HIGH);
}

// Disable charge pump and 5V regulator
Wire.beginTransmission(epc_address);
Wire.write(0xD0);
Wire.write(0x4a);
Wire.endTransmission();
Wire.beginTransmission(epc_address);
Wire.write(0xD1);
Wire.write(0x66);
Wire.endTransmission();
Wire.beginTransmission(epc_address);
Wire.write(0xD6);
Wire.write(0x03);
Wire.endTransmission();
Serial.println("Sensor Ready");

}

/********************************************************************************************/
 /* Main Loop
/********************************************************************************************/
void loop() {
   
  messering();
}
  
/*
*******************************************************************************************
Functionen
*******************************************************************************************
*/

int messering(){
 unsigned long current_micros=0;
 for(int z=0;z<3;z++){
  current_micros =micros();
  while(micros()<(current_micros+t_clr_pix)){
    digitalWrite(Clear_Pix,HIGH);
    
  }
   current_micros =micros();
  while(micros()<(current_micros+t_clr_pix)){
  digitalWrite(Clear_Pix,LOW);
 }}
current_micros =micros();
 while(micros()<(current_micros+t_clr_pix+t_exposure)){
  digitalWrite(Shutter,HIGH);
  
 }
 digitalWrite(Shutter,LOW);
 while(digitalRead(Data_RDY)==HIGH){
 current_micros = micros();
 while(micros()<(current_micros+t_read)){
   digitalWrite(READ,HIGH);
   
 }
  current_micros = micros();
 while(micros()<(current_micros+t_read)){
 digitalWrite(READ,LOW);
 }
 }
 
 for(int l=0;l<1026;l++){
 current_micros =micros();
 while(micros()<(current_micros+t_read_clk)){
  digitalWrite(READ,HIGH);
 }
  current_micros = micros();
 while(micros()<(current_micros+t_read_clk)){
  Distance=analogRead(Video_P);
  Serial.print(Distance);
  Serial.print("\n");
 digitalWrite(READ,LOW);
 }
 }
 
}
