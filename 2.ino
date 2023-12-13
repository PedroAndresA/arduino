/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBUS-Ih5HamdZe3jjVEO4Nx8qPoHOrLS07z";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "AndresG";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "1234567890";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "esp32_prueba_3";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "potenciometro"; // Put here your Variable label to which data  will be published

const char *VARIABLE_LABEL2 = "led";
const int LED = 2;

const int PUBLISH_FREQUENCY = 1000; // Update rate in milliseconds

unsigned long timer;
uint8_t analogPin = 34; // Pin used to read data from GPIO34 ADC_CH6.

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    if((char)payload[0] == '1')
    {
      digitalWrite(LED, HIGH);

    }
    else
    {
      digitalWrite(LED, LOW);
    }
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL2);

  timer = millis();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  int dato = millis() - timer;
  if (abs(dato) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    float value = analogRead(analogPin);
    ubidots.add(VARIABLE_LABEL, value); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL2);
    timer = millis();
  }
  ubidots.loop();
}