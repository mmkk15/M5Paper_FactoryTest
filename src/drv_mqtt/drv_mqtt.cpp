/*************************************************************************************************************/
/*** Includes ***/
#include "drv_mqtt.h"
#include "WiFi.h"
#include "syslog/syslog.h"
#include <PubSubClient.h>

/*************************************************************************************************************/
void callback(char *topic, byte *message, unsigned int length);

/*************************************************************************************************************/
void MQTT_connect()
{
	// Add your MQTT Broker IP address, example:
	//const char* mqtt_server = "192.168.1.144";
	uint8_t		test;
	const char *mqtt_server = "10.0.10.1";

	if (WiFi.isConnected())
	{
		WiFiClient	 wifiClient;
		PubSubClient client(wifiClient);

		client.setServer(mqtt_server, 1883);
		client.setCallback(callback);
	}
}

/*************************************************************************************************************/
void callback(char *topic, byte *message, unsigned int length)
{
	Serial.print("Message arrived on topic: ");
	Serial.print(topic);
	Serial.print(". Message: ");
	String messageTemp;

	for (int i = 0; i < length; i++)
	{
		Serial.print((char)message[i]);
		messageTemp += (char)message[i];
	}
	Serial.println();
}
