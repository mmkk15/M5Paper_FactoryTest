/**********************************************************************************************************************************/
#include "MyOTA.h"

static uint8_t OTAInitialized = 0;

/**********************************************************************************************************************************/
/*** Local functions ***/
void setupOTA(const char *Hostname);


/**********************************************************************************************************************************/
void handleOTA(const char *Hostname)
{
    if (WiFi.isConnected())
    {
        if (OTAInitialized)
        {
            ArduinoOTA.handle();
        }
        else
        {
            setupOTA(Hostname);
        }
    }
}

/**********************************************************************************************************************************/
void setupOTA(const char *Hostname) //const char *nameprefix)
{
    Serial.println("setupOTA: Setting up OTA ...");

    ArduinoOTA.setHostname(Hostname);

    if (WiFi.isConnected())
    {
        // ArduinoOTA.setPort(3232);                                        // Default port = 3232
        // ArduinoOTA.setPassword("admin");                                 // No authentication by default
        // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");  // Password can be set with it's md5 value

        ArduinoOTA.onStart([]()
                           {
                               Serial.printf("Starting update ...");
                               String type;
                               if (ArduinoOTA.getCommand() == U_FLASH)
                                   type = "sketch";
                               else // U_SPIFFS
                                   type = "filesystem";
                               Serial.println("Start updating " + type);
                           });

        ArduinoOTA.onEnd([]()
                         { Serial.println("\nEnd"); });

        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                              {
                                  static int lastpct = 0;
                                  int pct = (progress / (total / 100));
                                  if (pct != lastpct)
                                  {
                                      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
                                      lastpct = pct;
                                  }
                              });

        ArduinoOTA.onError([](ota_error_t error)
                           {
                               Serial.printf("Error[%u]: ", error);
                               if (error == OTA_AUTH_ERROR)
                                   Serial.println("\nAuth Failed");
                               else if (error == OTA_BEGIN_ERROR)
                                   Serial.println("\nBegin Failed");
                               else if (error == OTA_CONNECT_ERROR)
                                   Serial.println("\nConnect Failed");
                               else if (error == OTA_RECEIVE_ERROR)
                                   Serial.println("\nReceive Failed");
                               else if (error == OTA_END_ERROR)
                                   Serial.println("\nEnd Failed");
                           });

        ArduinoOTA.begin();

        Serial.println("OTA Initialized");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("OTA not initialized ... no WiFi connection ...");
    }

    OTAInitialized = 1; // Set OTA as initialized
}