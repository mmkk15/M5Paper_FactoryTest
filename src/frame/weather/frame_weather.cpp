/***************************************************************************************************************/
#include "frame_weather.h"
#include "../../resources/myImages.h"
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

		// Current weather
		canvas.setTextDatum(TC_DATUM);
		canvas.drawString(w->Weather.WeatherDescription, 270, 12);
		canvas.drawFastHLine(0, 62, 540, 15);

		canvas.drawString("Temp", 90, 72);
		canvas.drawString("Humidity", 270, 72);
		canvas.drawString("Clouds", 450, 72);
		canvas.drawString(String(w->Main.Temperature__dC + 0.5, 0) + " C", 90, 128);
		canvas.drawString(String(w->Main.Humidity__pct) + "%", 270, 128);
		canvas.drawString(String(w->Cloudiness__pct) + "%", 450, 128);
		canvas.drawFastHLine(0, 182, 540, 15);

		canvas.drawCircle(f1.x, f1.y - 72, 10, 15);

		//canvas.pushImage(240, 100, 92, 92, ImageResource_main_icon_keyboard_92x92);

		update = 0;

		canvas.pushCanvas(0, 72, UPDATE_MODE_GC16);
	}

	return 1;
}
