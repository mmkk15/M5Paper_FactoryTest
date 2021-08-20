/***************************************************************************************************************/
#include "frame_testing.h"
#include "syslog/syslog.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

/***************************************************************************************************************/
static uint16_t textsize = 26;

int			 temp;
int			 humidity;
uint_fast8_t visibility	 = 0;
uint_fast8_t pressure	 = 0;
float		 wind_speed	 = 0.0;
float		 wind_degree = 0.0;

/***************************************************************************************************************/
// CALLBACK
void key_testing_textclear_cb(epdgui_args_vector_t &args)
{
	Syslog.Add("Testing: Clear textbox");
	((EPDGUI_Textbox *)(args[0]))->SetText("");
}

/***************************************************************************************************************/
void key_action_cb(epdgui_args_vector_t &args)
{
	Syslog.Add("Testing: Action");

	Syslog.Add("************************************");
	Syslog.Add("Querying weather:");

	HTTPClient http; // Declare an object of class HTTPClient
	String	   Location = "Moorenweis, DE";
	String	   API_Key	= "de1d0d744c2fb068edb714ce62b12957";

	String RequestUrl;
	// ONE CALL - very long answer
	RequestUrl = "http://api.openweathermap.org/data/2.5/onecall?";
	RequestUrl += "lat=48.1546";
	RequestUrl += "&lon=11.0821";
	RequestUrl += "&exclude=minutely,hourly";

	// Current weather
	RequestUrl = "http://api.openweathermap.org/data/2.5/weather?";
	RequestUrl += "q=Moorenweis,DE";
	RequestUrl += "&appid=" + API_Key;

	Syslog.Add("RequestUrl: " + RequestUrl);
	http.begin(RequestUrl);
	int httpCode = http.GET();			   // Sending the request
	if (httpCode > 0)					   // Checking the returning code
	{									   //
		String payload = http.getString(); // Getting the request response payload
        Syslog.Add("Received http response. Length: " + String(payload.length()));
		Syslog.Add(payload);
		((EPDGUI_Textbox *)(args[0]))->SetText(payload);

		//DynamicJsonDocument doc(512);
        StaticJsonDocument<1024> doc;

		// Parse JSON object
		// Deserialize the JSON document
		DeserializationError error = deserializeJson(doc, payload);

		if (error)
			Syslog.Add("JSON Deserialization failed ...");
		else
		{
			temp		= (int)(doc["main"]["temp"]) - 273.15;	   // Get temperature in °C
			humidity	= doc["main"]["humidity"];				   // Get humidity in %
			visibility	= (uint_fast8_t)doc["visibility"];		   // Get visibility in m
			pressure	= (uint_fast8_t)(doc["main"]["pressure"]); // Get pressure in bar
			wind_speed	= (float)(doc["wind"]["speed"]);		   // Get wind speed in m/s
			wind_degree = (float)(doc["wind"]["deg"]);			   // Get wind degree in °

			// upweatherId	  = (const char *)(root["weather"][0]["id"]);
			// upweatherMain = (const char *)(root["weather"][0]["main"]);
			// upweatherDesc = (const char *)(root["weather"][0]["description"]);
			// upweatherIcon = (const char *)(root["weather"][0]["icon"]);

			// upsunrise = (root["sys"]["sunrise"]);
			// upsunset  = (root["sys"]["sunset"]);
			Syslog.Add("Temperature: " + String(temp) + "°C");
			Syslog.Add("Humidity:    " + String(humidity) + "%");
			Syslog.Add("Wind speed:  " + String(wind_speed) + "m/s");
			Syslog.Add("Wind speed:  " + String(wind_degree) + "°");
		}
	}
	else
	{
		Syslog.Add("  Error getting weather ...");
	}
	http.end(); // Close connection
}

/***************************************************************************************************************/
Frame_Testing::Frame_Testing()
	: Frame_Base()
{
	_frame_name = "Frame_Testing";

	inputbox = new EPDGUI_Textbox(4, 100, 532, 512);
	inputbox->SetTextSize(textsize);
	inputbox->SetState(EPDGUI_Textbox::EVENT_PRESSED);

	key_textclear = new EPDGUI_Button("CLEAR", 10, 900, 250, 52);
	key_action	  = new EPDGUI_Button("ACTION", 300, 900, 250, 52);

	key_textclear->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)inputbox);
	key_textclear->Bind(EPDGUI_Button::EVENT_RELEASED, key_testing_textclear_cb);

	key_action->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)inputbox);
	key_action->Bind(EPDGUI_Button::EVENT_RELEASED, key_action_cb);

	exitbtn("Home");
	_canvas_title->drawString("Testing", 270, 34);

	_key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

/***************************************************************************************************************/
Frame_Testing::~Frame_Testing()
{
	delete inputbox;
	delete key_textclear;
	delete key_action;
}

/***************************************************************************************************************/
int Frame_Testing::init(epdgui_args_vector_t &args)
{
	Serial.println("Testing: Init");

	_is_run = 1;
	M5.EPD.Clear();
	_canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
	EPDGUI_AddObject(inputbox);
	EPDGUI_AddObject(key_textclear);
	EPDGUI_AddObject(key_action);
	EPDGUI_AddObject(_key_exit);
	return 4;
}

/***************************************************************************************************************/
int Frame_Testing::run(void)
{
	return 1;
}
