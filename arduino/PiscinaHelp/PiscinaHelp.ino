#include <OneWire.h>
#include <DallasTemperature.h>

//000000000000000000000000000000000000000000000000000000000000000

//Temperatura
const int pinDatosDQ = 13; 
OneWire oneWireObjeto(pinDatosDQ); 
DallasTemperature sensorDS18B20(&oneWireObjeto);

//PH
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.05            //deviation compensate
//#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

//000000000000000000000000000000000000000000000000000000000000000

void setup(){
  Serial.begin(9600);
  
  //Temperatura
  sensorDS18B20.begin(); 
  
  //PH
  //pinMode(LED,OUTPUT);
}

//000000000000000000000000000000000000000000000000000000000000000

void loop(){

//----------------TEMPERATURA----------------
  float temperatura;
  
  sensorDS18B20.requestTemperatures();
  temperatura=sensorDS18B20.getTempCByIndex(0);
  
  Serial.print("Temperatura: "); 
  Serial.print(temperatura); 
  Serial.print(" °C");
  
  delay(1000);
  
  if (temperatura >= 30){
    digitalWrite (2, HIGH);
    digitalWrite (3, LOW);
    digitalWrite (4, LOW); 
  } 
  else if (temperatura > 25 && temperatura < 30){ 
    digitalWrite (2, LOW); 
    digitalWrite (3, HIGH);     
    digitalWrite (4, LOW); 
  } 
  else{ 
    digitalWrite (2, LOW); 
    digitalWrite (3, LOW); 
    digitalWrite (4, HIGH); 
  }

//---------------------PH---------------------

  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval){
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval){    //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    /*
    Serial.print("Voltage:");
    Serial.print(voltage,2);
    */
    Serial.print("    pH: ");
    Serial.println(pHValue,2);
    //digitalWrite(LED,digitalRead(LED)^1);
    printTime=millis();
  }
}

//000000000000000000000000000000000000000000000000000000000000000

double avergearray(int* arr, int number){
  int i,max,min;
  double avg;
  long amount=0;
  
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++) {amount+=arr[i];}
    avg = amount/number;
    return avg;
  }
  else{
    if(arr[0]<arr[1]) {min = arr[0]; max=arr[1];}
    else {min=arr[1]; max=arr[0];}
    
    for(i=2;i<number;i++){
      if(arr[i]<min) {amount+=min; min=arr[i];}      //arr<min
      else{
        if(arr[i]>max) {amount+=max; max=arr[i];}    //arr>max
        else {amount+=arr[i];}                       //min<=arr<=max
      }
    }
    avg = (double)amount/(number-2);
  }
  return avg;
}
