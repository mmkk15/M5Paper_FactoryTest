/***************************************************************************************************************/
#include "frame_weather.h"
#include "../../resources/MyWeatherIcons.h"
#include "Free_Fonts.h"
#include "drv_openweather/drv_openweather.h"
#include "syslog/syslog.h"

/***************************************************************************************************************/
static M5EPD_Canvas		canvas(&M5.EPD);
static const S_Weather *w = Openweather.getWeather();

/***************************************************************************************************************/
Frame_Weather::Frame_Weather()
	: Frame_Base()
{
	_frame_name = "Frame_Weather";

	exitbtn("Home");
	_canvas_title->drawString("Weather", 270, 34);

	_key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

/***************************************************************************************************************/
Frame_Weather::~Frame_Weather()
{
}

/***************************************************************************************************************/
int Frame_Weather::init(epdgui_args_vector_t &args)
{
	Serial.println("Frame_Weather::init()");

	Openweather.setAPIKey("de1d0d744c2fb068edb714ce62b12957");
	Openweather.queryWeather("Moorenweis,DE");

	_is_run = 1;

	canvas.createCanvas(540, 888);
	canvas.fillCanvas(0);

	M5.EPD.Clear();
	_canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
	EPDGUI_AddObject(_key_exit);

	return 5;
}

/***************************************************************************************************************/
void Frame_Weather::exit()
{
	canvas.deleteCanvas();
}

/***************************************************************************************************************/
int Frame_Weather::run(void)
{
	static uint16_t lastx = 0, lasty = 0;
	static uint8_t	update = 1;
	tp_finger_t		f1	   = M5.TP.readFinger(0);

	if ((f1.x != lastx) || (f1.y != lasty))
	{
		lastx = f1.x;
		lasty = f1.y;
		Serial.printf("x/y %d/%d", f1.x, f1.y);

		update = 1;
	}

	M5.update();
	if (M5.BtnP.wasReleased())
	{
	}

	if (update)
	{
		canvas.setFreeFont(FF20); // FF18
		canvas.setTextDatum(TL_DATUM);
		canvas.setTextColor(15);
		canvas.setTextSize(26);

		// Current weather -------------------------------------------------------------
		canvas.setTextDatum(TC_DATUM);
		canvas.drawString(w->Weather.WeatherDescription, 270, 12);
		canvas.drawFastHLine(0, 62, 540, 15);

		canvas.drawString("Temp", 90, 72);
		canvas.drawString("Clouds", 270, 72);
		canvas.drawString("UVIndex", 450, 72);
		canvas.drawString(String(w->Main.Temperature__dC + 0.5, 0) + " C", 90, 128);
		canvas.drawString(String(w->Cloudiness__pct) + "%", 270, 128);
		canvas.drawString(String(w->DayWeather[0].UV_Index), 450, 128);
		
		canvas.drawFastHLine(0, 182, 540, 15);

		// Next hour forecast ----------------------------------------------------------

		// Draw Day forecasts ----------------------------------------------------------
		int x_off = 90, y_height = 74;
		int x_width = (540 - x_off - 20) / 4;				// Offset 20 for rain
		Serial.printf("Width: %d", x_width);
		int y_off	= 960 - 8 * y_height; // Don't change

		canvas.setFreeFont(FF19);
		canvas.setTextDatum(TC_DATUM);
		canvas.setTextSize(20);
		canvas.drawString("Temp", x_off + 0.5 * x_width, y_off - 30);
		canvas.drawString("Wind", x_off + 1.5 * x_width, y_off - 30);
		canvas.drawString("UVI ", x_off + 2.5 * x_width, y_off - 30);
		canvas.drawString("PoP ", x_off + 3.5 * x_width, y_off - 30);

		// Debug vertical lines
		for (int i = 0; i < 4; i++)
		{
			int x = x_off + i * x_width;
			canvas.drawFastVLine(x, 182, 960, 15);
		}

		for (int i = 0; i < 7; i++)
		{
			canvas.setFreeFont(FF20);
			canvas.setTextSize(26);

			uint height = y_off + y_height * i;
			canvas.drawFastHLine(0, height, 540, 15);
			// Icon
			canvas.pushImage(15, height + 5, 64, 64, getWeatherIcon(w->DayWeather[i + 1].FieldWeather.WeatherIcon));
			canvas.setTextDatum(TC_DATUM);
			// Temperature
			canvas.drawString(String(w->DayWeather[i + 1].TemperatureDay__dC + 0.5, 0) + "", x_off + 0.5 * x_width, height + 20);
			// UVIndex
			canvas.drawString(String(w->DayWeather[i + 1].UV_Index, 1) + "", x_off + 1.5 * x_width, height + 20);
			// Wind
			canvas.drawString(String(w->DayWeather[i + 1].Wind_speed__m_d_s, 1) + "", x_off + 2.5 * x_width, height + 20);
			// Rain
			canvas.drawString(String(w->DayWeather[i + 1].PropabilityOfPrecipitation * 100.0, 0) + "%", x_off + 3.5 * x_width + 10, height + 20);
		}

		update = 0;
		canvas.pushCanvas(0, 72, UPDATE_MODE_GC16);
	}

	return 1;
}

/***************************************************************************************************************/
const unsigned char *Frame_Weather::getWeatherIcon(String IconName)
{
	const unsigned char *p_icon = NULL;

	if (IconName == "01d") // clear sky
		p_icon = ImageResource_01d_clear_sky_64x64;
	else if (IconName == "02d") // few clouds
		p_icon = ImageResource_02d_few_clouds_64x64;
	else if (IconName == "03d") // scattered clouds
		p_icon = ImageResource_03d_scattered_clouds_64x64;
	else if (IconName == "09d") // shower rain
		p_icon = ImageResource_09n_shower_rain_62x62;
	else if (IconName == "10d") // rain
		p_icon = ImageResource_10d_rain_64x64;
	else if (IconName == "11d") // 11
		p_icon = ImageResource_11d_thunderstorm_64x64;
	else if (IconName == "13d") // 13
		p_icon = ImageResource_13d_snow_55x55;
	else if (IconName == "50d") // 50
		p_icon = ImageResource_13d_snow_55x55;
	else
		Syslog.Add("Icon not found ...");

	return p_icon;
}
