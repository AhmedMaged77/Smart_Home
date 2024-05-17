/*

  The circuit:
 * LCD RS pin to digital pin A0
 * LCD Enable pin to digital pin A1
 * LCD D4 pin to digital pin A2
 * LCD D5 pin to digital pin A3
 * LCD D6 pin to digital pin A4
 * LCD D7 pin to digital pin A5
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/
#define led1pin 13
#define ir1pin 12
#define servopin 10

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
Servo myservo;  // create servo object to control a servo

// FOR KEYPAD
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,4,3,2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char pass[]="666";
char enteredPass[4];
int cnt=0,k=0;
void setup() {
  myservo.attach(servopin);  // attaches the servo on pin 10 to the servo object
  lcd.begin(16, 2);
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  Serial.begin(9600);
  myservo.write(50);
  
  ////////////for ir
  // put your setup code here, to run once:
    pinMode(led1pin,OUTPUT);
    pinMode(ir1pin,INPUT);
  Serial.begin(9600);
  //end of ir
}

void loop() {
  
  char key = keypad.getKey();
  if (key){
    lcd.print(key);
    if(key!='D'){
      enteredPass[cnt++]=key;
    }else if(key=='D'){
      if(cnt==3){
        for(int i=0;i<cnt;i++)
          if(pass[i]==enteredPass[i])
            k++;
             
        if(k==3){
          validPass();
            while(true)
              irSensors();
        }else{
          
          inValidPass();
          
        }
        cnt=0;
        k=0;
        
      }else{
        inValidPass();
        cnt=0;
        k=0;
      }
      
    }
    
  }

  
  
}

void validPass(){
  lcd.clear();
  lcd.print("Valid Password");
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter From Gate");
  
  myservo.write(150);                  // sets the servo position according to the scaled value
  
  lcd.setCursor(0,1);
  lcd.print("Close After ");
  for(int i=9;i>=0;i--){
    lcd.setCursor(13,1);
    lcd.print(i);
    delay(1000);
  }
  
  myservo.write(50);
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
}

void inValidPass(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Invalid Password");
  delay(500);
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
}
void irSensors(){
  
  int readval1=digitalRead(ir1pin);
  Serial.println(readval1);
  Serial.print(readval1);
  delay(40);
 if(readval1==LOW && digitalRead(led1pin)==LOW){
    digitalWrite(led1pin,HIGH);
    delay(5000);
    digitalWrite(led1pin,LOW);
  }
  
}
