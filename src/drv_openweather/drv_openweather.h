#ifndef __DRV_OPENWEATHER_H__
#define __DRV_OPENWEATHER_H__

/************************************************************************************/
// Documentation: https://openweathermap.org/weather-conditions

/************************************************************************************/
/*** Includes ***/
#include "drv_openweather_types.h"

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