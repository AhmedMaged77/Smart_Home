const int analogPin = A0; // Analog pin connected to the junction of thermistor and resistor
  const float seriesResistor = 10000.0; // Value of the fixed resistor in ohms
  const float nominalResistance = 10000.0; // Resistance of the thermistor at 25 degrees C
  const float nominalTemperature = 25.0; // Temperature for nominal resistance (usually 25 C)
  const float bCoefficient = 3950.0; // Beta coefficient of the thermistor
  const int thermistorNominalTemperature = 25; // The temperature at which the nominal resistance is specified (usually 25 C)

    // Variables
    float analogValue;
    float resistance;
    float temperature;  
   #define fan 4
   #define Buzzer 13  //for alarm
   #define alarmLed 12  //for alarm
  
   #define trigPin  9   //ultrasonic for television
   #define echoPin 8
   #define TeleLed 7   // led for televisoin
  
   #define gardenLed 11
   #define ldr A1  
   #include "DHT.h"

  #define DHTPIN 2     // Digital pin connected to the DHT sensor
  #define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
  DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin (9600);
  pinMode(fan,OUTPUT);
  pinMode(gardenLed,OUTPUT);
  pinMode(ldr,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(alarmLed, OUTPUT);
  pinMode(TeleLed,OUTPUT);
   dht.begin();
}




void  loop() {

  // code for the garden ldr
  int ldrvalue = analogRead(ldr);
  if(ldrvalue<40){
    digitalWrite(gardenLed,HIGH);
  }
  else{
    digitalWrite(gardenLed,LOW);
  }
  Serial.print("ldr value");
  Serial.println(ldrvalue);
  // code for tempsensor
  analogValue = analogRead(analogPin); // Read the analog value from the voltage divider
  resistance = seriesResistor / ((1023.0 / analogValue) - 1); // Calculate the thermistor resistance

  // Use the Steinhart-Hart equation to calculate temperature
  float steinhart;
  steinhart = resistance / nominalResistance; // (R/Ro)
  steinhart = log(steinhart); // ln(R/Ro)
  steinhart /= bCoefficient; // 1/B * ln(R/Ro)
  steinhart += 1.0 / (nominalTemperature + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart; // Invert
  steinhart -= 273.15; // Convert to Celsius

  temperature = steinhart;

  // Print the temperature to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  

  delay(100);  
  if(temperature>40){
    digitalWrite(fan,HIGH);
  }
  else{
    digitalWrite(fan,LOW);
  }
  if(temperature>50){
    digitalWrite(Buzzer, HIGH);
    digitalWrite(alarmLed, !digitalRead(alarmLed));
  }
  else{
    digitalWrite(Buzzer, LOW);
    digitalWrite(alarmLed, LOW);
  }

  //  code for ultrasonic
  int duration, distance;
  digitalWrite(trigPin,  LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin,  LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0344 / 2;
  
  if (distance<20) {
    digitalWrite(TeleLed, HIGH);
  }
  else{
    digitalWrite(TeleLed, LOW);
  }
  
    Serial.print("Distance from the television: ");
    Serial.print(
        distance); // Print the output in serial monitor
    Serial.println(" cm");
    delay(500);


    // Humidity
     float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%  Temperature: ");
    Serial.println(t);
}
