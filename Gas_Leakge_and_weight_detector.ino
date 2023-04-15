

/*GAS detector security system.
 * https://srituhobby.com
 */
#define BLYNK_TEMPLATE_ID "TMPLwWij7pXw"
#define BLYNK_TEMPLATE_NAME "Gas Sensor"
#define BLYNK_AUTH_TOKEN "O-GQohADZbvuqCMb0INRjhmwkKnxXRZG"
#include <Wire.h>
#include <Servo.h>
#include <HX711_ADC.h>
#include <LiquidCrystal_I2C.h>
#define Yellow D5
#define White D6
#define Green D7
#define Buzzer D8

#define BLYNK_PRINT Serial
//#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//Wire.begin(2,0);
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711_ADC LoadCell(D3,D4);
char auth[] = BLYNK_AUTH_TOKEN;// Enter your Auth token
char ssid[] = "Yash";//Enter your WIFI SSIS
char pass[] = "12345678";//Enter your WIFI password
//char ssid[] = "P";//Enter your WIFI SSIS
//char pass[] = "Prince@123";//Enter your WIFI password
BlynkTimer timer;
int pinValue = 0;
#define Sensor A0
Servo Myservo;
void setup() 
{
  pinMode(Yellow,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  Myservo.attach(D0);
  Myservo.write(0);
  pinMode(White,OUTPUT);
  pinMode(Green,OUTPUT);
  Wire.begin(D2,D1);
  Serial.begin(115200);
  lcd.backlight();
  lcd.init();
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(385.0); // calibration factor for load cell => strongly dependent on your individual setup
  lcd.begin(16, 2); // begins connection to the LCD module
  lcd.backlight(); // turns on the backlight
  lcd.setCursor(2,0);
  lcd.print("IOT based ");
  lcd.setCursor(1,1);
  lcd.print("Weighing Scale");
  delay(2000);
  lcd.clear();
  pinMode(2, OUTPUT);
  pinMode(Sensor, INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L,gasWeight);
  timer.setInterval(100L,notification);
}



void notification() 
{
  int sensor = analogRead(Sensor);
  sensor = map(sensor, 0, 1024, 0, 100);
  Serial.println(sensor);
 
    Blynk.virtualWrite(V0, sensor);
    if(sensor>=30)    
    {
      Blynk.logEvent("gas","Gas Detected!");
      digitalWrite(Buzzer,1);delay(400);
      digitalWrite(Buzzer,0);delay(400);
      digitalWrite(Buzzer,1);delay(400);
      digitalWrite(Buzzer,0);delay(400);
      digitalWrite(Buzzer,1);delay(400);
      digitalWrite(Buzzer,0);delay(400);
      Myservo.write(180);
      //digitalWrite(2,1);
//      Myservo.write(90);
    }
     
  } 
  void gasWeight()
  {
    
  LoadCell.update(); // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
  lcd.setCursor(0, 0); // set cursor to first row
  lcd.print("Gas Weight :"); // print out to LCD
  lcd.setCursor(12, 0); // set cursor to secon row
  lcd.print(i);
  Blynk.virtualWrite(V1,i);
  if(i>=-1 && i<=3)
  {
    lcd.setCursor(0,0);
    lcd.print("No  Cylinder  on");
    lcd.setCursor(0,1);
    lcd.print("   trolley"); 
    delay(2000);
    lcd.clear();
  }
  else if(i>3 && i<400)
  {
  Blynk.logEvent("weight","Refill Your Cylinder!");
  lcd.setCursor(0,1);
  lcd.print("Refill your Gas");
  digitalWrite(Yellow,1);
  digitalWrite(White,0);
  digitalWrite(Green,0);
  
  
  }
  else if(i>400 && i<1000)
  {
  
  lcd.setCursor(0,1);
  lcd.print("  Gas level 50%");
  digitalWrite(Yellow,0);
  digitalWrite(White,1);
  digitalWrite(Green,0);
  
  }
  else if(i>1000 && i<2000)
  {
  
  lcd.setCursor(0,1);
  lcd.print("  Ready to use");
  digitalWrite(Yellow,0);
  digitalWrite(White,0);
  digitalWrite(Green,1);
  
  }
  }
 BLYNK_WRITE(V2)
 {
  digitalWrite(Buzzer,1);delay(200);
  digitalWrite(Buzzer,0);delay(200);
  digitalWrite(Buzzer,1);delay(200);
  digitalWrite(Buzzer,0);delay(200);
  digitalWrite(Buzzer,1);delay(200);
  digitalWrite(Buzzer,0);delay(200);
  Myservo.write(param.asInt());
 }
 BLYNK_WRITE(V3)
 {
  digitalWrite(Buzzer,1);delay(200);
  digitalWrite(Buzzer,0);delay(200);
  digitalWrite(Buzzer,1);delay(200);
  digitalWrite(Buzzer,0);delay(200);
  digitalWrite(Buzzer,1);delay(200);
  digitalWrite(Buzzer,0);delay(200);
  int val = param.asInt();
  if(val == 1)
  {  
  Myservo.write(180);
  }
  if(val == 0)
  {  
  Myservo.write(0);
  }
 }
 
void loop() 
{
  gasWeight();
  Blynk.run();
  timer.run();
}
