/*****************************************************************************************************************************/
#ifndef __OPENWEATHER_TYPES_H__
#define __OPENWEATHER_TYPES_H__

/*****************************************************************************************************************************/
#include <WString.h>
#include <inttypes.h>

/*********************************************/
typedef struct
{
	double Latitude__deg;
	double Longitude__deg;
} S_FieldLocation;

/*********************************************/
typedef struct
{
	uint16_t WeatherConditionID;
	String	 WeatherMainDescription;
	String	 WeatherDescription;
	String	 WeatherIcon;
} S_FieldWeather;

/*********************************************/
typedef struct
{
	float	 Temperature__dC;
	float	 TemperatureFeelsLike__dC;
	uint16_t Pressure__hPa;
	float	 TemperatureMin__dC;
	float	 TemperatureMax__dC;
	uint16_t Humidity__pct;
} S_FieldMain;

/*********************************************/
typedef struct
{
	float	 Speed__m_d_s;
	uint16_t Direction__deg;
} S_FieldWind;

/*********************************************/
typedef struct
{
	uint32_t SunriseTime__unixUTC;
	uint32_t SunsetTime__unixUTC;
	String	 Country;
} S_FieldSys;

/*********************************************/
typedef struct
{
	S_FieldLocation Location;
	S_FieldWeather	Weather;
	S_FieldMain		Main;
	S_FieldWind		Wind;
	uint32_t		Visibility__m;
	uint8_t			Cloudiness__pct;
	S_FieldSys		Sys;
	uint32_t		ShiftSecondsToUTC;
} S_Weather;

#endif
