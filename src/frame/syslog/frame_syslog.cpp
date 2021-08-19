/***************************************************************************************************************/
#include "frame_syslog.h"
#include "syslog/syslog.h"

/***************************************************************************************************************/
static uint16_t textsize = 26;

/***************************************************************************************************************/
// CALLBACK
static void key_syslog_textclear_cb(epdgui_args_vector_t &args)
{
	Serial.println("Syslog: Clear syslog");
	((EPDGUI_Textbox *)(args[0]))->SetText("");
}

/***************************************************************************************************************/
Frame_Syslog::Frame_Syslog() : Frame_Base()
{
	Syslog.Add("Creating instance of Frame_Syslog ...");

	_frame_name = "Frame_Syslog";

	inputbox = new EPDGUI_Textbox(4, 100, 532, 780);
	inputbox->SetTextSize(textsize);
	inputbox->SetState(EPDGUI_Textbox::EVENT_PRESSED);

	key_textclear = new EPDGUI_Button("CLEAR", 10, 900, 540, 52);

	key_textclear->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)inputbox);
	key_textclear->Bind(EPDGUI_Button::EVENT_RELEASED, key_syslog_textclear_cb);

	exitbtn("Home");
	_canvas_title->drawString("Syslog", 270, 34);

	_key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

/***************************************************************************************************************/
Frame_Syslog::~Frame_Syslog()
{
	Syslog.Add("Frame_Syslog::~Frame_Syslog");

	delete inputbox;
	delete key_textclear;
}

/***************************************************************************************************************/
int Frame_Syslog::init(epdgui_args_vector_t &args)
{
	Syslog.Add("Frame_Syslog::init ...");

	_is_run = 1;
	M5.EPD.Clear();
	_canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
	EPDGUI_AddObject(inputbox);
	EPDGUI_AddObject(key_textclear);
	EPDGUI_AddObject(_key_exit);
	//M5.EPD.UpdateFull(UPDATE_MODE_GC16);

	Syslog.Add("Frame_Syslog::init end");

	return 4;
}

/***************************************************************************************************************/
int Frame_Syslog::run(void)
{
	static int shift = 0;

	if (Syslog.hasChanged())
	{
		inputbox->SetText(Syslog.getSyslogString());
	}

	M5.update();
	if (M5.BtnR.wasReleased())
	{
		if(shift < 100)
			shift += 5;

		inputbox->SetText(Syslog.getSyslogString(shift));
	}

	if (M5.BtnL.wasReleased())
	{
		if(shift)
		{
			shift -= 5;
		}

		inputbox->SetText(Syslog.getSyslogString(shift));
	}

	return 1;
}
