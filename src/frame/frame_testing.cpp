/***************************************************************************************************************/
#include "frame_testing.h"
#include "drv_openweather/drv_openweather.h"
#include "syslog/syslog.h"

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
	Openweather.queryWeather("Moorenweis, DE");

	String weatherString = Openweather.getWeatherString();

	((EPDGUI_Textbox *)(args[0]))->SetText(weatherString);
}

/***************************************************************************************************************/
Frame_Testing::Frame_Testing()
	: Frame_Base()
{
	_frame_name = "Frame_Testing";

	inputbox = new EPDGUI_Textbox(4, 100, 532, 750);
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
