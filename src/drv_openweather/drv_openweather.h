#ifndef __DRV_OPENWEATHER_H__
#define __DRV_OPENWEATHER_H__

/************************************************************************************/
// Documentation: https://openweathermap.org/weather-conditions

/************************************************************************************/
/*** Includes ***/
#include "drv_openweather_types.h"

/*
Day icon	Night icon	Description
01d.png 	01n.png 	clear sky
02d.png 	02n.png 	few clouds
03d.png 	03n.png 	scattered clouds
04d.png 	04n.png 	broken clouds
09d.png 	09n.png 	shower rain
10d.png 	10n.png 	rain
11d.png 	11n.png 	thunderstorm
13d.png 	13n.png 	snow
50d.png 	50n.png 	mist
*/

/************************************************************************************/
class C_OpenWeatherMap
{
public:
	C_OpenWeatherMap();
	C_OpenWeatherMap(String ApiKey);
	~C_OpenWeatherMap();

	void			 setAPIKey(String ApiKey);
	bool			 queryWeather(String Location);
	const S_Weather *getWeather();
	String			 getWeatherString();

private:
	String	  m_ApiKey;
	S_Weather m_sWeather;
	String	  m_WeatherString;

	String ConvertUnixTime(uint32_t unix_time);
};

/************************************************************************************/
extern C_OpenWeatherMap Openweather;

#endif