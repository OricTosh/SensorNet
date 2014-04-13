//12/04/2014

#include <DHT22.h>
#include <stdio.h> // Only used for sprintf
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>


// Data wire is plugged into port 3 on the Arduino
const int DHT22_PIN = 3;
int ret;
long previousMillis = 0;
long interval = 115000; //300000;
float dew_point = 0.0;
// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);


// IP/MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {10,52,0,176};
byte gateway[] = {10,52,0,1};

// Your Xively key to let you upload data

char xivelyKey[] = "Your API Key"; 
// Define the strings for our datastream IDs
char myHumidSensor[] = "Humidity";
char myTempSensor[] = "Temperature";
char myDewPoint[] = "Dew_Point_Temperature";
char buf[16];
//char mySensorNet[] = "Other Sensors";
//byte byteRead;

XivelyDatastream datastreams[] = {
  XivelyDatastream(myTempSensor, strlen(myTempSensor), DATASTREAM_FLOAT),
  XivelyDatastream(myHumidSensor, strlen(myHumidSensor), DATASTREAM_FLOAT),
  XivelyDatastream(myDewPoint, strlen(myDewPoint), DATASTREAM_FLOAT)
};

// Finally, wrap the datastreams into a feed
XivelyFeed feed(1166752800, datastreams, 3 /* number of datastreams */);

EthernetClient client;
XivelyClient xivelyclient(client);

void setup(void)
{
    
  // start serial port
  Serial.begin(9600);
   
   Serial.println("Attempting to get an IP address using DHCP:");
   if (!Ethernet.begin(mac)) {
    Serial.println("failed to get an IP address using DHCP, trying manually");
        
       //Ethernet.begin(mac, ip, gateway);
        delay(10000);   
   }
  
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
  
   
    
}  // End of Setup

//--------------------------------------------------------------------
// calculates dew point
// input: humidity [%RH], temperature [°C]
// output: dew point [°C]
float calc_dewpoint(float h,float t){

float k,dew_point ;
k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t);
dew_point = 243.12*k/(17.62-k);
return dew_point;
}

void loop(void)
{ 
 
  if (Serial.available() > 0) {
    int ch = Serial.read();
    Serial.print("Have Some Data ");
    Serial.println(ch);
  } 
  
  
    
 DHT22_ERROR_t errorCode;
  
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  delay(3000); // set warm up to 3s
  unsigned long currentMillis = millis();
  Serial.println(currentMillis); // debug
  
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      Serial.print("Got Data ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");      
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
            
      dew_point = calc_dewpoint(myDHT22.getHumidity(),myDHT22.getTemperatureC()); //calculate dew point
      Serial.println(dew_point);
        
      
      sprintf(buf, "%hi.%01hiC %i.%01i ", myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
                   myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
                   
     // sprintf(buf, "%hi.%01hiC %i.%01i %%RH",
       //            myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
         //          myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
                  
      if(currentMillis - previousMillis > interval)
      {
         previousMillis = currentMillis;
         datastreams[0].setFloat(myDHT22.getTemperatureC());         
         datastreams[1].setFloat(myDHT22.getHumidity());
         datastreams[2].setFloat(dew_point);
                 
         Serial.println("Uploading it to Xively");
         ret = xivelyclient.put(feed, xivelyKey);
         Serial.print("xivelyclient.put returned ");
         Serial.println(ret);             
        
      }
      break;
    case DHT_ERROR_CHECKSUM:
      Serial.print("check sum error ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      break;
    case DHT_BUS_HUNG:
      Serial.print("BUS HUNG");
      break;
    case DHT_ERROR_NOT_PRESENT:
      Serial.println("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      Serial.println("Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      Serial.println("Data Timeout ");
      break;
    case DHT_ERROR_TOOQUICK:
      Serial.println("Polled to quick ");
      break;
  }
}
