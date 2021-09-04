/* 
 by Shihchaopin <shihkw@gmail.com>
 Farmbot seeder use ESP32
 without sensor 

 modified 2021/08/18
 by Pin
*/

#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32
 #define RunPin               14      // 執行輸入 pin       // Run Enable 
 #define LoadTestPin          12      // 執行測試LOAD pin   // set hole on load or unload @ stop 
 
 #define LoadDegrees          175     // 載入種子角度      
 #define reLoadDegrees        170     // 載入種子角度
 #define fLoadDegrees         150     // 載入種子慢走到几度再變快走
 
 #define fstep                -1      // 載入種子後剛開始走的速度
 
 #define UnloadDegrees        0       // 釋放種子角度
 #define reUnloadDegrees      10      // 釋放種子角度
 #define fUnloadDegrees       15      // 釋放種子慢走到几度再變快走
 #define rstep                1       // 載入種子角度
 

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 18;
int st = 0 ;
void setup() {
  Serial.begin(115200);
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 20, 3000); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
  st = 0 ;
  pinMode(RunPin, INPUT_PULLUP);
  pinMode(LoadTestPin, INPUT_PULLUP);
  digitalWrite(RunPin, HIGH);
}

int now_time ;
int last_time ;
void loop() {
  //now_time=micros();
  now_time=millis();
   if (now_time > ( last_time + 50)) {      
      last_time = now_time; 
      Serial.print("STATE ID:  ");
      Serial.print(st); 
      Serial.print("  RunPin:  ");
      Serial.println(digitalRead(RunPin)); 
        
  }
  switch(st) {
    case 0: // init 
        st = 1 ;
        break;
    case 1:   //idle   
        if (!(digitalRead(RunPin))) st = 2 ;
        if (digitalRead(LoadTestPin)) 
          myservo.write(LoadDegrees);
        else 
          myservo.write(UnloadDegrees);
          
        break;
    case 2:   //start run  load seed   
        myservo.write(LoadDegrees);    
        delay(200);             
        myservo.write(reLoadDegrees);    
        delay(200);                     
        myservo.write(LoadDegrees);    
        delay(200);                     
        myservo.write(reLoadDegrees);    
        delay(200);                     
        myservo.write(LoadDegrees);    
        delay(200);                   
        myservo.write(reLoadDegrees);   
        delay(200);                     
        myservo.write(LoadDegrees);        

        for (pos = LoadDegrees; pos >= fLoadDegrees  ; pos += fstep) { // goes from 0 degrees to 180 degrees
           // in steps of 1 degree
           myservo.write(pos);    
           delay(40);             
        }
        
        st++ ;              
        break;
    case 3:   //Unload 
        myservo.write(UnloadDegrees);    
        delay(400);            
        myservo.write(reUnloadDegrees);    
        delay(50);            
        myservo.write(UnloadDegrees);    
        delay(50);            
        myservo.write(reUnloadDegrees);    
        delay(50);            
        myservo.write(UnloadDegrees);    
        delay(50);            
        myservo.write(reUnloadDegrees);    
        delay(50);          
        myservo.write(UnloadDegrees);    
        delay(50);              


        for (pos = UnloadDegrees; pos <= fUnloadDegrees  ; pos += rstep) { 
           myservo.write(pos);    
           delay(10);             
        }   
        myservo.write(LoadDegrees);    
        delay(600);        
        
        st = 1 ;
        break;
    
    default:
        st = 0;
        break;
  } 
}
