#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include<LiquidCrystal.h>
 
#define APDS9960_INT    2 // Needs to be an interrupt pin

LiquidCrystal lcd(8,7,6,5,4,3);
 
// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0, a = 0;
int UHL = 0, LHL = 0, WP = 0, RL = 0;
 
void setup() {
 
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

 lcd.begin(16,2);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(" Gesture Control ");
 lcd.setCursor(0,2);
 lcd.print("  Detection  ");
 delay(6000);
 
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
 
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
     lcd.setCursor(0,2);
     lcd.print("  Standby Mode  ");
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
    lcd.setCursor(0,2);
    lcd.print("Something failed");
  }
 
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
    lcd.setCursor(0,2);
    lcd.print("  Standby Mode  ");
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
    lcd.setCursor(0,2);
    lcd.print("Something failed");
  }
}
 
void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    unlockingGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}
 
void interruptRoutine() {
  isr_flag = 1;
}

void unlockingGesture() {
    lcd.setCursor(0,1);
    lcd.print("Gesture Activated");
    lcd.setCursor(0,2);
    lcd.print("  LOCKED !!!       ");
   if(apds.readGesture()==DIR_RIGHT){
    lcd.setCursor(0,1);
    lcd.print("Gesture Activated");
    lcd.setCursor(0,2);
    lcd.print(" Recognizing...... ");
     a=0;
     handleGesture();
    } 
  }


void handleGesture() {
    while( apds.isGestureAvailable()==0 && a!=1 ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        lcd.setCursor(0,2);
        switch(UHL){
          case 0:
           lcd.print("Upper HLight ON   ");
           UHL=1;
          break;

          case 1:
           lcd.print("Upper HLight OFF   ");
           UHL=0;
          break;

          default:
          lcd.print("Try Again!!         ");
          
          }
          
     
          delay(3000);
          a=1;
        break;
        
      case DIR_DOWN:
        lcd.setCursor(0,2);
        Serial.println("DOWN");
         switch(LHL){
          case 0:
           lcd.print("Lower HLight ON   ");
           LHL=1;
          break;

          case 1:
            lcd.print("Lower HLight OFF   ");
           LHL=0;
          break;

          default:
          lcd.print("Try Again!!         ");
          
          }

       
          delay(3000);
         a=1;
        break;
      case DIR_LEFT:
        lcd.setCursor(0,2);
        Serial.println("LEFT");
        lcd.print("L_Indicate ON   ");
        
        while(apds.readGesture()!=DIR_LEFT ){
          lcd.setCursor(13,2);
          lcd.print("<<<");
          lcd.setCursor(13,2);
          lcd.print("   ");
          }
          lcd.setCursor(0,2);
         lcd.print("L_Indicate OFF  ");
         delay(3000);
         a=1;
        break;
      case DIR_RIGHT:
        lcd.setCursor(0,2);
        Serial.println("RIGHT");
        lcd.print("R_Indicate ON    ");
        lcd.setCursor(13,2);
        while(apds.readGesture()!=DIR_RIGHT){
          lcd.setCursor(13,2);
          lcd.print(">>>");
          lcd.setCursor(13,2);
          lcd.print("   ");
          }
          lcd.setCursor(0,2);
         lcd.print("R_Indicate OFF  ");
         delay(3000);
       
         a=1;
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        lcd.setCursor(0,2);
         switch(RL){
          case 0:
            lcd.print("Roof Light ON   ");
            RL=1;
          break;

          case 1:
            lcd.print("Roof Light OFF  ");
            RL=0;
          break;

          default:
          lcd.print("Try Again!!         ");
          
          }
        
       
         
         delay(3000);
         a=1;
        break;
      case DIR_FAR:
        lcd.setCursor(0,2);
        Serial.println("FAR");
        
         switch(WP){
          case 0:
           lcd.print("Wipper ON .........");
           WP=1;
          break;

          case 1:
           lcd.print("Wipper OFF........");
           WP=0;
          break;

          default:
          lcd.print("Try Again!!         ");
          
          }
        
         
         delay(3000);
         a=1;
        break;
      default:
        a=0;
        //Serial.println("NONE");
    }
  }
}
