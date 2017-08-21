//12/04/14 Cool IT

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT22.h>


const int TEMPERATURE_PRECISION = 9;

//Data wire is plugged into port 7
const int ONE_WIRE_BUS =  7;

OneWire oneWire(ONE_WIRE_BUS);
//Pass our oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);
int numberOfDevices;
DeviceAddress tempDeviceAddress;

const int latchPin = 4;
const int clockPin = 3;
const int dataPin = 2;
const int DHT22_PIN = 6;

// Setup a oneWire Instance
DHT22 myDHT22(DHT22_PIN);
byte dataArray[9];

DeviceAddress LiquidThermometer = {0x28, 0x3D, 0x8D, 0x28, 0x05, 0x00, 0x00, 0xC7};
DeviceAddress FirstDallas = {0x28, 0x2E, 0x5C, 0x8D, 0x05, 0x00, 0x00, 0x7E};
DeviceAddress SecondDallas ={0x28, 0x2B, 0x3C, 0x8D, 0x05, 0x00, 0x00, 0x84};
DeviceAddress ThirdDallas = {0x28, 0xE1, 0xA4, 0x8D, 0x05, 0x00, 0x00, 0x5D};
DeviceAddress ForthDallas = {0x28, 0xF0, 0x0E, 0x8D, 0x05, 0x00, 0x00, 0xBF};
DeviceAddress FifthDallas = {0X28, 0x6E, 0x63, 0x8D, 0x05, 0x00, 0x00, 0xE0};


void setup() {
  sensors.begin();
  Serial.begin(9600);
  Serial.println("SensorNet");
  numberOfDevices = sensors.getDeviceCount();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
    // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
	{
		Serial.print("Found device ");
		Serial.print(i, DEC);
		Serial.print(" with address: ");
		printAddress(tempDeviceAddress);
		Serial.println();
		Serial.print("Setting resolution to ");
		Serial.println(TEMPERATURE_PRECISION, DEC);
		// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
		sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
        	Serial.print("Resolution actually set to: ");
		Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
		Serial.println();
	}else{
		Serial.print("Found ghost device at ");
		Serial.print(i, DEC);
		Serial.print(" but could not detect address. Check power and cabling");
	}
  }

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
   
  dataArray[0] = 3;    // Zero
  dataArray[1] = 159;  // One
  dataArray[2] = 37;   // Two
  dataArray[3] = 13;   // Three
  dataArray[4] = 153;  // Four
  dataArray[5] = 73;   // Five 
  dataArray[6] = 65;   // Six
  dataArray[7] = 31;   // Seven
  dataArray[8] = 1;    // Eight
  dataArray[9] = 9;    // Nine
  
}  // End of setup

void printTemperature(DeviceAddress deviceAddress)
{
  //float tempC = sensors.getTempC(deviceAddress);
  int temp;
  
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.write("Error getting temperature");
  } else {
    Serial.print(" C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
    Serial.println("");
  }
  
  temp = (int) tempC;
      
  displayNum(temp);
  delay(1000);
}

void displayNum(int sensorVal){
  
  int units, tens; 
  int a, b;
  //a = 8; //debug
  //b = 5; 
  
  units = sensorVal % 10;
  a = units;
  //Serial.print("Units: "); // debug
  //Serial.println(units);
  
  // get tens value of temp
  sensorVal = sensorVal/10;  
  tens = sensorVal % 10;
  b = tens;
  //Serial.print("Tens: "); // debug
  //Serial.println(tens);
  
  digitalWrite(latchPin, HIGH);
  shiftOut(dataPin, clockPin, LSBFIRST, dataArray[a]);
  shiftOut(dataPin, clockPin, LSBFIRST, dataArray[b]);
  digitalWrite(latchPin, LOW);

}


// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop() 
{
  //Dallas One Wire
  //delay(3000);
  sensors.requestTemperatures();
  printTemperature(LiquidThermometer);
  printTemperature(FirstDallas);
  printTemperature(SecondDallas);
  printTemperature(ThirdDallas);
  printTemperature(ForthDallas);
  printTemperature(FifthDallas);  

  //DHT Sensor
  DHT22_ERROR_t errorCode;
  delay(3000);
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      //DHT_22_Temp
      int dhtTemp = myDHT22.getTemperatureC();
      displayNum(dhtTemp);
      delay(3000);
      Serial.println("");
      Serial.write("DHTTemp:");
      Serial.write(dhtTemp);
      
      //DHT_22_Humidity
      delay(3000);
      int dhtHumid = myDHT22.getHumidity();
      displayNum(dhtHumid);
      Serial.write("DHTHumidity:");
      Serial.write(dhtHumid);
   
      //delay(3000);
      break;
  }  
  
} // End of Loop  
   
  
