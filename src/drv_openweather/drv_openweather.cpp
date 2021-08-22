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
/* Location e.g. "Munich,DE" - make sure no spaces or whitespaces are in the Location string
 */
bool C_OpenWeatherMap::queryWeather(String Location)
{
	bool success = queryCurrentWeather(Location); // Query current location first to get lat/lon of Location
	if (success)
	{
		delay(100);
		success = queryDailyWeather();
	}

	return success;
}
/***************************************************************************************************************/
bool C_OpenWeatherMap::queryDailyWeather()
{
	// https://api.openweathermap.org/data/2.5/onecall?lat=48&lon=11&exclude=current,minutely,hourly,daily,alerts&appid=your_app_id
	// ONE CALL - exclude everything except daily

	Syslog.Add("queryDailyWeather()");

	bool				ret = false;
	String				RequestUrl;
	DynamicJsonDocument doc(16384);

	// OneCall API - daily
	RequestUrl = "http://api.openweathermap.org/data/2.5/onecall?";
	RequestUrl += "lat=" + String(m_sWeather.Location.Latitude__deg, 6);
	RequestUrl += "&lon=" + String(m_sWeather.Location.Longitude__deg, 6);
	RequestUrl += "&exclude=current,minutely,hourly,alerts";
	RequestUrl += "&appid=" + m_ApiKey;
	Syslog.Add("RequestUrl: " + RequestUrl);

	HTTPClient http;															   // Declare an object of class HTTPClient
	http.begin(RequestUrl);														   // Begin http transfer
	int httpCode = http.GET();													   // Sending the request
	if (httpCode > 0)															   // Checking the returning code
	{																			   //
		String payload = http.getString();										   // Getting the request response payload
		Syslog.Add("Received http response. Length: " + String(payload.length())); //
		Syslog.Add(payload);
		Syslog.Add("Decoding JSON ...");

		DeserializationError error = deserializeJson(doc, payload);
		if (error)
			Syslog.Add("JSON Deserialization failed ...");
		else
		{
			ret = true;
			Syslog.Add("JSON deserialization sucess:");

			for (int day = 0; day <= 7; day++)
			{
				m_sWeather.DayWeather[day].SunriseTime__unixUTC				   = doc["daily"][day]["sunrise"];
				m_sWeather.DayWeather[day].SunsetTime__unixUTC				   = doc["daily"][day]["sunset"];
				m_sWeather.DayWeather[day].TemperatureDay__dC				   = (float)(doc["daily"][day]["temp"]["day"]) - 273.15;
				m_sWeather.DayWeather[day].TemperatureMin__dC				   = (float)(doc["daily"][day]["temp"]["min"]) - 273.15;
				m_sWeather.DayWeather[day].TemperatureMax__dC				   = (float)(doc["daily"][day]["temp"]["max"]) - 273.15;
				m_sWeather.DayWeather[day].TemperatureNight__dC				   = (float)(doc["daily"][day]["temp"]["night"]) - 273.15;
				m_sWeather.DayWeather[day].TemperatureEvening__dC			   = (float)(doc["daily"][day]["temp"]["eve"]) - 273.15;
				m_sWeather.DayWeather[day].TemperatureMorning__dC			   = (float)(doc["daily"][day]["temp"]["morn"]) - 273.15;
				m_sWeather.DayWeather[day].Pressure__hPa					   = doc["daily"][day]["pressure"];
				m_sWeather.DayWeather[day].Humidity__pct					   = doc["daily"][day]["humidity"];
				m_sWeather.DayWeather[day].DewPoint__K						   = doc["daily"][day]["dew_point"];
				m_sWeather.DayWeather[day].Wind_speed__m_d_s				   = doc["daily"][day]["wind_speed"];
				m_sWeather.DayWeather[day].Wind_deg							   = doc["daily"][day]["wind_deg"];
				m_sWeather.DayWeather[day].Wind_gust__m_d_s					   = doc["daily"][day]["wind_gust"];
				m_sWeather.DayWeather[day].FieldWeather.WeatherConditionID	   = doc["daily"][day]["weather"][0]["id"];
				m_sWeather.DayWeather[day].FieldWeather.WeatherMainDescription = String((const char *)(doc["daily"][day]["weather"][0]["main"]));
				m_sWeather.DayWeather[day].FieldWeather.WeatherDescription	   = String((const char *)(doc["daily"][day]["weather"][0]["description"]));
				m_sWeather.DayWeather[day].FieldWeather.WeatherIcon			   = String((const char *)(doc["daily"][day]["weather"][0]["icon"]));
				m_sWeather.DayWeather[day].Cloudiness__pct					   = doc["daily"][day]["clouds"];
				m_sWeather.DayWeather[day].RainAmount						   = doc["daily"][day]["rain"];
				m_sWeather.DayWeather[day].UV_Index							   = doc["daily"][day]["uvi"];
				m_sWeather.DayWeather[day].PropabilityOfPrecipitation		   = doc["daily"][day]["pop"];
				m_sWeather.DayWeather[day].SunriseTime__unixUTC				   = m_sWeather.DayWeather[day].SunriseTime__unixUTC + m_sWeather.ShiftSecondsToUTC;
				m_sWeather.DayWeather[day].SunsetTime__unixUTC				   = m_sWeather.DayWeather[day].SunsetTime__unixUTC + m_sWeather.ShiftSecondsToUTC;

				Syslog.Add("Temps: " +
						   String(m_sWeather.DayWeather[day].TemperatureMorning__dC, 0) + " " +
						   String(m_sWeather.DayWeather[day].TemperatureDay__dC, 0) + " " +
						   String(m_sWeather.DayWeather[day].TemperatureEvening__dC, 0) + " " +
						   String(m_sWeather.DayWeather[day].TemperatureNight__dC, 0));
			}
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
bool C_OpenWeatherMap::queryCurrentWeather(String Location)
{
	bool	   ret = false; // return value
	HTTPClient http;		// Declare an object of class HTTPClient
	String	   RequestUrl;

	// Current weather
	RequestUrl = "http://api.openweathermap.org/data/2.5/weather?";
	RequestUrl += "q=" + Location;
	RequestUrl += "&appid=" + m_ApiKey;

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
			ret = true;
			Syslog.Add("JSON deserialization sucess:");
			// Coord
			m_sWeather.Location.Longitude__deg		  = doc["coord"]["lon"];
			m_sWeather.Location.Latitude__deg		  = doc["coord"]["lat"];
			// Weather
			m_sWeather.Weather.WeatherConditionID	  = doc["weather"][0]["id"];
			m_sWeather.Weather.WeatherMainDescription = String((const char *)(doc["weather"][0]["main"]));
			m_sWeather.Weather.WeatherDescription	  = String((const char *)(doc["weather"][0]["description"]));
			m_sWeather.Weather.WeatherIcon			  = String((const char *)(doc["weather"][0]["icon"]));
			// Main
			m_sWeather.Main.Temperature__dC			  = (float)(doc["main"]["temp"]) - 273.15f;
			m_sWeather.Main.TemperatureFeelsLike__dC  = (float)(doc["main"]["feels_like"]) - 273.15f;
			m_sWeather.Main.TemperatureMin__dC		  = (float)(doc["main"]["temp_min"]) - 273.15f;
			m_sWeather.Main.TemperatureMax__dC		  = (float)(doc["main"]["temp_max"]) - 273.15f;
			m_sWeather.Main.Pressure__hPa			  = doc["main"]["pressure"];
			m_sWeather.Main.Humidity__pct			  = doc["main"]["humidity"];
			// Wind
			m_sWeather.Wind.Speed__m_d_s			  = doc["wind"]["speed"];
			m_sWeather.Wind.Direction__deg			  = doc["wind"]["deg"];
			// Sys
			m_sWeather.Visibility__m				  = doc["visibility"];
			// Clouds
			m_sWeather.Cloudiness__pct				  = doc["clouds"]["all"];
			// Timezone
			m_sWeather.ShiftSecondsToUTC			  = doc["timezone"];
			// Sys
			m_sWeather.Sys.SunriseTime__unixUTC		  = doc["sys"]["sunrise"];
			m_sWeather.Sys.SunsetTime__unixUTC		  = doc["sys"]["sunset"];
			m_sWeather.Sys.Country					  = String((const char *)(doc["sys"]["country"]));
			m_sWeather.Sys.SunriseTime__unixUTC		  = m_sWeather.Sys.SunriseTime__unixUTC + m_sWeather.ShiftSecondsToUTC;
			m_sWeather.Sys.SunsetTime__unixUTC		  = m_sWeather.Sys.SunsetTime__unixUTC + m_sWeather.ShiftSecondsToUTC;

			m_sWeather.Weather.WeatherMainDescription.toUpperCase();
			m_sWeather.Weather.WeatherDescription.toUpperCase();
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