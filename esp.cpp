#include <Wire.h>
#include "max32664.h"


int trigPin = 12;    // Trigger
int echoPin = 14;    // Echo


#define POWER_PIN_WATER_LEVEL 15
#define SIGNAL_PIN_WATER_LEVEL 35
#define RESET_PIN 05
#define MFIO_PIN 04
#define tonePin 13
#define RAWDATA_BUFFLEN 250

#include "PubSubClient.h"
#include "ThingSpeak.h"

// Ensure correct credentials to connect to your WiFi Network.
char ssid[] = "";
char pass[] = "";
// Ensure that the credentials here allow you to publish and subscribe to the ThingSpeak channel.
#define channelID xxxxxx
const char mqttUserName[] = ""; 
const char clientID[] = "";
const char mqttPass[] = "";
// It is strongly recommended to use secure connections. However, certain hardware does not work with the WiFiClientSecure library.
// Comment out the following #define to use non-secure MQTT connections to ThingSpeak server. 
// #define USESECUREMQTT
// Comment the following line if not using an ESP8266.
// #define ESP8266BOARD

#ifdef ESP8266BOARD
  #include "ESP8266WiFi.h"
  const char* PROGMEM thingspeak_cert_thumbprint = "9780c25078532fc0fd03dae01bfd8c923fff9878";
#else
  #include <WiFi.h>
  const char * PROGMEM thingspeak_ca_cert = "";
#endif

#ifdef USESECUREMQTT
  #include <WiFiClientSecure.h>
  #define mqttPort 8883
  WiFiClientSecure client; 
#else
  #define mqttPort 1883
  WiFiClient client;
#endif

const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS; 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient(client);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to handle messages from MQTT subscription.
void mqttSubscriptionCallback(char* topic, byte* payload, unsigned int length) {
  // Print the details of the message that was received to the serial monitor.
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Subscribe to ThingSpeak channel for updates.
void mqttSubscribe(long subChannelID){
  String myTopic = "channels/" + String(subChannelID) + "/subscribe";
  mqttClient.subscribe(myTopic.c_str());
}

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString = "channels/" + String(pubChannelID) + "/publish";
  mqttClient.publish(topicString.c_str(), message.c_str());
}

// Connect to WiFi.
void connectWifi() {
  Serial.print("Connecting to Wi-Fi...");
  // Loop until WiFi connection is successful
  #ifdef ESP8266BOARD
    while(WiFi.waitForConnectResult() != WL_CONNECTED) {
  #else
    while(WiFi.status() != WL_CONNECTED) {
  #endif
    WiFi.begin(ssid, pass);
    delay(connectionDelay * 1000);
    Serial.print(WiFi.status()); 
  }
  Serial.println("Connected to Wi-Fi.");
}

// Connect to MQTT server.
void mqttConnect() {
  // Loop until connected.
  while (!mqttClient.connected()) {
    // Connect to the MQTT broker.
    if (mqttClient.connect( clientID, mqttUserName, mqttPass)) {
      Serial.print("MQTT to ");
      Serial.print(server);
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println(" successful.");
    } 
    else {
      Serial.print( "MQTT connection failed, rc = " );
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println( " Will try again in a few seconds" );
      delay(connectionDelay * 1000);
    }
  }
}

int value_water_level = 0; // variable to store the sensor value

max32664 MAX32664(RESET_PIN, MFIO_PIN, RAWDATA_BUFFLEN);


void mfioInterruptHndlr(){
  //Serial.println("i");
}

void enableInterruptPin(){

  //pinMode(mfioPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MAX32664.mfioPin), mfioInterruptHndlr, FALLING);

}

void loadAlgomodeParameters(){

  algomodeInitialiser algoParameters;
  /*  Replace the predefined values with the calibration values taken with a reference spo2 device in a controlled environt.
      Please have a look here for more information, https://pdfserv.maximintegrated.com/en/an/an6921-measuring-blood-pressure-MAX32664D.pdf
      https://github.com/Protocentral/protocentral-pulse-express/blob/master/docs/SpO2-Measurement-Maxim-MAX32664-Sensor-Hub.pdf
  */

  algoParameters.calibValSys[0] = 120;
  algoParameters.calibValSys[1] = 122;
  algoParameters.calibValSys[2] = 125;

  algoParameters.calibValDia[0] = 80;
  algoParameters.calibValDia[1] = 81;
  algoParameters.calibValDia[2] = 82;

  algoParameters.spo2CalibCoefA = 1.5958422;
  algoParameters.spo2CalibCoefB = -34.659664;
  algoParameters.spo2CalibCoefC = 112.68987;

  MAX32664.loadAlgorithmParameters(&algoParameters);
}



void setup(){

  Serial.begin(57600);
  delay(500);

  // Connect to Wi-Fi network.
  connectWifi();
  // Configure the MQTT client
  mqttClient.setServer(server, mqttPort); 
  // Set the MQTT message handler function.
  mqttClient.setCallback(mqttSubscriptionCallback);
  // Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize(2048);
  // Use secure MQTT connections if defined.
  #ifdef USESECUREMQTT
    // Handle functionality differences of WiFiClientSecure library for different boards.
    #ifdef ESP8266BOARD
      client.setFingerprint(thingspeak_cert_thumbprint);
    #else
      client.setCACert(thingspeak_ca_cert);
    #endif
  #endif

  // Serial.begin(9600);
  pinMode(POWER_PIN_WATER_LEVEL, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN_WATER_LEVEL, LOW); // turn the sensor OFF
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  Wire.begin();

  loadAlgomodeParameters();

  int result = MAX32664.hubBegin();
  if (result == CMD_SUCCESS){
    Serial.println("Sensorhub begin!");
  }else{
    //stay here.
    while(1){
      Serial.println("Could not communicate with the sensor! please make proper connections");
      delay(5000);
    }
  }

  bool ret = MAX32664.startBPTcalibration();
  while(!ret){

    delay(10000);
    Serial.println("failed calib, please retsart");
    //ret = MAX32664.startBPTcalibration();
  }

  delay(1000);

  //Serial.println("start in estimation mode");
  ret = MAX32664.configAlgoInEstimationMode();
  while(!ret){

    //Serial.println("failed est mode");
    ret = MAX32664.configAlgoInEstimationMode();
    delay(10000);
  }

  //MAX32664.enableInterruptPin();
  Serial.println("Getting the device ready..");
  delay(1000);
}



int flag = 0;
long cm;



long dur;
void loop(){
  flag = 0;

  // Reconnect to WiFi if it gets disconnected.
  if (WiFi.status() != WL_CONNECTED) {
      connectWifi();
  }

  // Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
     mqttConnect(); 
     mqttSubscribe(channelID);
  }
  
  // Call the loop to maintain connection to the server.
  mqttClient.loop(); 

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //  Serial.println("**************************************************************************************");
   Serial.println("********************************PURPLE BOTTLE INFO************************************");
  //  Serial.println("***************************************************************************************");
  pinMode(echoPin, INPUT);
  dur = pulseIn(echoPin, HIGH);
  cm = (dur/2) / 29.1; 
  Serial.println(cm);
  long dis = 24.0 - cm;
  Serial.print("The water level is:");
  Serial.println(dis);
  if (dis < 10)
  {
     tone(tonePin,5000,200);
     Serial.println("################################################");
     Serial.println("The value of water level in purple bottle is low.");
     Serial.print(dis);
     Serial.println("cm");
     Serial.println("################################################");
  }



      



  //  Serial.println("**************************************************************************************");
   Serial.println("********************************SMALL BOTTLE INFO************************************");
  //  Serial.println("***************************************************************************************");

  digitalWrite(POWER_PIN_WATER_LEVEL, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value_water_level = analogRead(SIGNAL_PIN_WATER_LEVEL); // read the analog value from sensor
  digitalWrite(POWER_PIN_WATER_LEVEL, LOW);   // turn the sensor OFF
    double x = (value_water_level-200);
  x = x*0.0027778;
  Serial.print("Water level value:::");
  Serial.println(x);
  if (x<=1.5)    // Upon calibration we found that sensor outputs 700 when there is no water
  {
     tone(tonePin,1000,200);
     Serial.println("################################################");
     Serial.println("The value of water level in the first plastic bottle is low.");
     Serial.print(x);
     Serial.println("cm");
     Serial.println("################################################");

  }



   

   
   Serial.println("********************************MEDICAL INFO************************************");

  uint8_t num_samples = MAX32664.readSamples();

  if(num_samples){

    Serial.print("sys = ");
    Serial.print(MAX32664.max32664Output.sys);
    Serial.print(", dia = ");
    Serial.print(MAX32664.max32664Output.dia);
    Serial.print(", hr = ");
    Serial.print(MAX32664.max32664Output.hr);
    Serial.print(" spo2 = ");
    Serial.println(MAX32664.max32664Output.spo2);
    
    if (MAX32664.max32664Output.hr==0 && MAX32664.max32664Output.spo2==0 && MAX32664.max32664Output.dia==0 && MAX32664.max32664Output.sys==0 )
    {
      ;
    }
    else if (MAX32664.max32664Output.hr > 100 || MAX32664.max32664Output.hr < 60 )
    {
        tone(tonePin, 5000, 200);
        Serial.println("################################################");
        Serial.println("Heart rate problem.Take care!");
        Serial.println(MAX32664.max32664Output.hr);
        Serial.println("################################################");
    }
    else if (MAX32664.max32664Output.spo2 < 95)
    {
        tone(tonePin, 4000, 200);
        Serial.println("################################################");
        Serial.println("Low Spo2.Take care!");
        Serial.println(MAX32664.max32664Output.spo2);
        Serial.println("################################################");
    }
    else if (MAX32664.max32664Output.dia < 60 || MAX32664.max32664Output.dia >  100   || MAX32664.max32664Output.sys < 100 || MAX32664.max32664Output.sys > 140  )
    {
        tone(tonePin,3000,200);
        Serial.println("################################################");
        Serial.println("Blood pressure problem.Take care!");
        Serial.println(MAX32664.max32664Output.sys);
        Serial.println(MAX32664.max32664Output.dia);
        Serial.println("################################################");
    }

  }


  // Update ThingSpeak channel periodically. The update results in the message to the subscriber.
  if (abs(long(millis()) - lastPublishMillis) > updateInterval * 1000) {
    mqttPublish(channelID, (
      String("field1=") + 
      String(MAX32664.max32664Output.spo2) + 
      String(",field2=") + 
      String(MAX32664.max32664Output.hr) + 
      String(",field3=") + 
      String(MAX32664.max32664Output.sys) + 
      String(",field4=") + 
      String(MAX32664.max32664Output.dia) + 
      String(",field5=") + 
      String(value_water_level))
    );
    lastPublishMillis = millis();
  }
  

  delay(600);
  noTone(tonePin);
}