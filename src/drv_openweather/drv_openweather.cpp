/***************************************************************************************************************/
#include "drv_openweather.h"

/***************************************************************************************************************/
#include "syslog/syslog.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

/***************************************************************************************************************/
C_OpenWeatherMap::C_OpenWeatherMap()
{
	Syslog.Add("Creating Openweather instance ...");
	m_ApiKey		= "";
	m_WeatherString = "";
}

/***************************************************************************************************************/
C_OpenWeatherMap::C_OpenWeatherMap(String ApiKey)
{
	C_OpenWeatherMap();
	setAPIKey(ApiKey);
}

/***************************************************************************************************************/
C_OpenWeatherMap::~C_OpenWeatherMap()
{
}

/***************************************************************************************************************/
/* Location e.g. "Munich, DE" */
bool C_OpenWeatherMap::queryWeather(String Location)
{
	bool ret = false;

	Syslog.Add("************************************");
	Syslog.Add("DRV_Openweather::getWeather()");

	HTTPClient http; // Declare an object of class HTTPClient
	//String	   Location = "Moorenweis, DE";
	String API_Key = "de1d0d744c2fb068edb714ce62b12957";

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

		Syslog.Add("Decoding JSON ...");
		DynamicJsonDocument	 doc(4096); // Parse JSON object
		DeserializationError error = deserializeJson(doc, payload);

		if (error)
			Syslog.Add("JSON Deserialization failed ...");
		else
		{
			Syslog.Add("JSON deserialization sucess:");
			// Coord
			m_sWeather.Location.Longitude__deg		  = doc["coord"]["lon"];
			m_sWeather.Location.Latitude__deg		  = doc["coord"]["lat"];
			// Weather
			m_sWeather.Weather.WeatherConditionID	  = doc["weather"][0]["id"];
			m_sWeather.Weather.WeatherMainDescription = String((const char *)(doc["weather"][0]["main"]));
			m_sWeather.Weather.WeatherDescription	 = String((const char *)(doc["weather"][0]["description"]));
			m_sWeather.Weather.WeatherIcon			 = String((const char *)(doc["weather"][0]["icon"]));
			// Main
			m_sWeather.Main.Temperature__dC			 = (float)(doc["main"]["temp"]) - 273.15f;
			m_sWeather.Main.TemperatureFeelsLike__dC = (float)(doc["main"]["feels_like"]) - 273.15f;
			m_sWeather.Main.TemperatureMin__dC		 = (float)(doc["main"]["temp_min"]) - 273.15f;
			m_sWeather.Main.TemperatureMax__dC		 = (float)(doc["main"]["temp_max"]) - 273.15f;
			m_sWeather.Main.Pressure__hPa			 = doc["main"]["pressure"];
			m_sWeather.Main.Humidity__pct			 = doc["main"]["humidity"];
			// Wind
			m_sWeather.Wind.Speed__m_d_s			 = doc["wind"]["speed"];
			m_sWeather.Wind.Direction__deg			 = doc["wind"]["deg"];
			// Sys
			m_sWeather.Visibility__m				 = doc["visibility"];
			// Clouds
			m_sWeather.Cloudiness__pct				 = doc["clouds"]["all"];
			// Timezone
			m_sWeather.ShiftSecondsToUTC			 = doc["timezone"];
			// Sys
			m_sWeather.Sys.SunriseTime__unixUTC		 = doc["sys"]["sunrise"];
			m_sWeather.Sys.SunsetTime__unixUTC		 = doc["sys"]["sunset"];
			m_sWeather.Sys.Country					 = String((const char *)(doc["sys"]["country"]));
			m_sWeather.Sys.SunriseTime__unixUTC		 = m_sWeather.Sys.SunriseTime__unixUTC + m_sWeather.ShiftSecondsToUTC;
			m_sWeather.Sys.SunsetTime__unixUTC		 = m_sWeather.Sys.SunsetTime__unixUTC + m_sWeather.ShiftSecondsToUTC;

			m_sWeather.Weather.WeatherMainDescription.toUpperCase();
			m_sWeather.Weather.WeatherDescription.toUpperCase();

			m_WeatherString = "";
			m_WeatherString += "Decoded weather: --------------\n";
			m_WeatherString += "Location Lat: " + String(m_sWeather.Location.Latitude__deg, 6) + "deg" + "\n";
			m_WeatherString += "Location Lon: " + String(m_sWeather.Location.Longitude__deg, 6) + "deg" + "\n";
			m_WeatherString += "Weather cond: " + String(m_sWeather.Weather.WeatherConditionID) + "\n";
			m_WeatherString += "Weather Main: " + String(m_sWeather.Weather.WeatherMainDescription) + "\n";
			m_WeatherString += "Weather Desc: " + String(m_sWeather.Weather.WeatherDescription) + "\n";
			m_WeatherString += "Weather icon: " + String(m_sWeather.Weather.WeatherIcon) + "\n";
			m_WeatherString += "Temperature:  " + String(m_sWeather.Main.Temperature__dC) + "C" + "\n";
			m_WeatherString += "Feels like :  " + String(m_sWeather.Main.TemperatureFeelsLike__dC) + "C" + "\n";
			m_WeatherString += "Temp min:     " + String(m_sWeather.Main.TemperatureMin__dC) + "C" + "\n";
			m_WeatherString += "Temp max:     " + String(m_sWeather.Main.TemperatureMax__dC) + "C" + "\n";
			m_WeatherString += "Pressure:     " + String(m_sWeather.Main.Pressure__hPa) + "hPa" + "\n";
			m_WeatherString += "Humidity:     " + String(m_sWeather.Main.Humidity__pct) + "%" + "\n";
			m_WeatherString += "Wind speed:   " + String(m_sWeather.Wind.Speed__m_d_s) + "m/s" + "\n";
			m_WeatherString += "Wind direc.:  " + String(m_sWeather.Wind.Direction__deg) + "deg" + "\n";
			m_WeatherString += "Visibility:   " + String(m_sWeather.Visibility__m) + "m" + "\n";
			m_WeatherString += "Coudiness:    " + String(m_sWeather.Cloudiness__pct) + "%" + "\n";
			m_WeatherString += "Timezone:     " + String(m_sWeather.ShiftSecondsToUTC) + "s" + "\n";
			m_WeatherString += "Sunrise:      " + String(ConvertUnixTime(m_sWeather.Sys.SunriseTime__unixUTC)) + "\n";
			m_WeatherString += "Sunset:       " + String(ConvertUnixTime(m_sWeather.Sys.SunsetTime__unixUTC)) + "\n";
			m_WeatherString += "Country:      " + String(m_sWeather.Sys.Country) + "\n";
		}
	}
	else
	{
		Syslog.Add("  Error getting weather ...");
	}
	http.end(); // Close connection

	return ret;
}

/***************************************************************************************************************/
const S_Weather *C_OpenWeatherMap::getWeather()
{
	return (const S_Weather *)&m_sWeather;
}

/***************************************************************************************************************/
void C_OpenWeatherMap::setAPIKey(String ApiKey)
{
	Syslog.Add("C_OpenWeatherMap::setAPIKey(): " + m_ApiKey);
	m_ApiKey = ApiKey;
}

/***************************************************************************************************************/
String C_OpenWeatherMap::getWeatherString()
{
	return m_WeatherString;
}

/***************************************************************************************************************/
String C_OpenWeatherMap::ConvertUnixTime(uint32_t unix_time)
{
	char	   output[20];
	time_t	   tm	  = unix_time;
	struct tm *now_tm = localtime(&tm);

	strftime(output, sizeof(output), "%H:%M", now_tm);

	return output;
}

/***************************************************************************************************************/
C_OpenWeatherMap Openweather;