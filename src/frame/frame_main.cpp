#include "frame_main.h"
#include "WiFi.h"
#include "frame_compare.h"
#include "frame_factorytest.h"
#include "frame_fileindex.h"
#include "frame_home.h"
#include "frame_keyboard.h"
#include "frame_lifegame.h"
#include "frame_setting.h"
#include "frame_testing.h"
#include "frame_wifiscan.h"
#include "weather/frame_weather.h"
#include "syslog/frame_syslog.h"

enum
{
	// First row
	kKeyFactoryTest = 0,
	kKeySetting,
	kKeyKeyboard,
	kKeyWifiScan,

	// Second row
	kKeySDFile,
	kKeyCompare,
	kKeyHome,
	kKeyLifeGame,

	// Third row
	kKeyMQTT,
	kKeyTesting,
	kKeyNC1,
	kKeyShutdown = 11,

	// Forth row
	kKeyWeather,
	kKeyNC5,
	kKeyNC6,
	kKeySyslog = 15,
};

#define KEY_W 92
#define KEY_H 92

/***********************************************************************************************************************/
void key_setting_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Setting");
	if (frame == NULL)
	{
		frame = new Frame_Setting();
		EPDGUI_AddFrame("Frame_Setting", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_keyboard_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Keyboard");
	if (frame == NULL)
	{
		frame = new Frame_Keyboard(0);
		EPDGUI_AddFrame("Frame_Keyboard", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_factorytest_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_FactoryTest");
	if (frame == NULL)
	{
		frame = new Frame_FactoryTest();
		EPDGUI_AddFrame("Frame_FactoryTest", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_wifiscan_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_WifiScan");
	if (frame == NULL)
	{
		frame = new Frame_WifiScan();
		EPDGUI_AddFrame("Frame_WifiScan", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_lifegame_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Lifegame");
	if (frame == NULL)
	{
		frame = new Frame_Lifegame();
		EPDGUI_AddFrame("Frame_Lifegame", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_sdfile_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = new Frame_FileIndex("/");
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_compare_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Compare");
	if (frame == NULL)
	{
		frame = new Frame_Compare();
		EPDGUI_AddFrame("Frame_Compare", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_home_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Home");
	if (frame == NULL)
	{
		frame = new Frame_Home();
		EPDGUI_AddFrame("Frame_Home", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_mqtt_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_MQTT");
	if (frame == NULL)
	{
		Serial.printf("Frame_MQTT not found .... creating ...\n");
		// frame = new Frame_Testing();
		// EPDGUI_AddFrame("Frame_Testing", frame);
	}
	//EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_testing_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Testing");
	if (frame == NULL)
	{
		Serial.printf("Frame_Testing not found .... creating ...\n");
		frame = new Frame_Testing();
		EPDGUI_AddFrame("Frame_Testing", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
void key_syslog_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Syslog");
	if (frame == NULL)
	{
		Serial.printf("Frame_Syslog not found .... creating ...\n");
		frame = new Frame_Syslog();
		EPDGUI_AddFrame("Frame_Syslog", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
static void key_weather_cb(epdgui_args_vector_t &args)
{
	Frame_Base *frame = EPDGUI_GetFrame("Frame_Weather");
	if (frame == NULL)
	{
		Serial.printf("Frame_Weather not found .... creating ...\n");
		frame = new Frame_Weather();
		EPDGUI_AddFrame("Frame_Weather", frame);
	}
	EPDGUI_PushFrame(frame);
	*((int *)(args[0])) = 0;
}

/***********************************************************************************************************************/
extern void key_shutdown_cb(epdgui_args_vector_t &args);

/***********************************************************************************************************************/
Frame_Main::Frame_Main(void)
	: Frame_Base(false)
{
	_frame_name = "Frame_Main";
	_frame_id	= 1;

	_bar = new M5EPD_Canvas(&M5.EPD);
	_bar->createCanvas(540, 44);
	_bar->setTextSize(26);

	_names = new M5EPD_Canvas(&M5.EPD);
	_names->createCanvas(540, 32);
	_names->setTextDatum(CC_DATUM);

	// First button row
	for (int i = 0; i < 4; i++)
	{
		_key[i] = new EPDGUI_Button("测试", 20 + i * 136, 90, KEY_W, KEY_H);
	}
	// Second button row
	for (int i = 0; i < 4; i++)
	{
		_key[i + 4] = new EPDGUI_Button("测试", 20 + i * 136, 240, KEY_W, KEY_H);
	}
	// Third button row
	for (int i = 0; i < 4; i++)
	{
		_key[i + 8] = new EPDGUI_Button("测试", 20 + i * 136, 390, KEY_W, KEY_H);
	}
	// Forth button row
	for (int i = 0; i < 4; i++)
	{
		_key[i + 12] = new EPDGUI_Button("测试", 20 + i * 136, 540, KEY_W, KEY_H);
	}

	// First row
	_key[kKeySetting]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_setting_92x92);
	*(_key[kKeySetting]->CanvasPressed()) = *(_key[kKeySetting]->CanvasNormal());
	_key[kKeySetting]->CanvasPressed()->ReverseColor();
	_key[kKeySetting]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeySetting]->Bind(EPDGUI_Button::EVENT_RELEASED, key_setting_cb);

	_key[kKeyKeyboard]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_keyboard_92x92);
	*(_key[kKeyKeyboard]->CanvasPressed()) = *(_key[kKeyKeyboard]->CanvasNormal());
	_key[kKeyKeyboard]->CanvasPressed()->ReverseColor();
	_key[kKeyKeyboard]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyKeyboard]->Bind(EPDGUI_Button::EVENT_RELEASED, key_keyboard_cb);

	_key[kKeyFactoryTest]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_factorytest_92x92);
	*(_key[kKeyFactoryTest]->CanvasPressed()) = *(_key[kKeyFactoryTest]->CanvasNormal());
	_key[kKeyFactoryTest]->CanvasPressed()->ReverseColor();
	_key[kKeyFactoryTest]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyFactoryTest]->Bind(EPDGUI_Button::EVENT_RELEASED, key_factorytest_cb);

	_key[kKeyWifiScan]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_wifi_92x92);
	*(_key[kKeyWifiScan]->CanvasPressed()) = *(_key[kKeyWifiScan]->CanvasNormal());
	_key[kKeyWifiScan]->CanvasPressed()->ReverseColor();
	_key[kKeyWifiScan]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyWifiScan]->Bind(EPDGUI_Button::EVENT_RELEASED, key_wifiscan_cb);

	_key[kKeyLifeGame]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_lifegame_92x92);
	*(_key[kKeyLifeGame]->CanvasPressed()) = *(_key[kKeyLifeGame]->CanvasNormal());
	_key[kKeyLifeGame]->CanvasPressed()->ReverseColor();
	_key[kKeyLifeGame]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyLifeGame]->Bind(EPDGUI_Button::EVENT_RELEASED, key_lifegame_cb);

	_key[kKeySDFile]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_sdcard_92x92);
	*(_key[kKeySDFile]->CanvasPressed()) = *(_key[kKeySDFile]->CanvasNormal());
	_key[kKeySDFile]->CanvasPressed()->ReverseColor();
	_key[kKeySDFile]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeySDFile]->Bind(EPDGUI_Button::EVENT_RELEASED, key_sdfile_cb);

	_key[kKeyCompare]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_compare_92x92);
	*(_key[kKeyCompare]->CanvasPressed()) = *(_key[kKeyCompare]->CanvasNormal());
	_key[kKeyCompare]->CanvasPressed()->ReverseColor();
	_key[kKeyCompare]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyCompare]->Bind(EPDGUI_Button::EVENT_RELEASED, key_compare_cb);

	_key[kKeyHome]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_home_92x92);
	*(_key[kKeyHome]->CanvasPressed()) = *(_key[kKeyHome]->CanvasNormal());
	_key[kKeyHome]->CanvasPressed()->ReverseColor();
	_key[kKeyHome]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyHome]->Bind(EPDGUI_Button::EVENT_RELEASED, key_home_cb);

	// MQTT
	_key[kKeyMQTT]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_todo_92x92);
	*(_key[kKeyMQTT]->CanvasPressed()) = *(_key[kKeyMQTT]->CanvasNormal());
	_key[kKeyMQTT]->CanvasPressed()->ReverseColor();
	_key[kKeyMQTT]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyMQTT]->Bind(EPDGUI_Button::EVENT_RELEASED, key_mqtt_cb);

	// Testing
	_key[kKeyTesting]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_restart_92x92);
	*(_key[kKeyTesting]->CanvasPressed()) = *(_key[kKeyTesting]->CanvasNormal());
	_key[kKeyTesting]->CanvasPressed()->ReverseColor();
	_key[kKeyTesting]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyTesting]->Bind(EPDGUI_Button::EVENT_RELEASED, key_testing_cb);

	// Shutdown
	_key[kKeyShutdown]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_shutdown_92x92);
	*(_key[kKeyShutdown]->CanvasPressed()) = *(_key[kKeyShutdown]->CanvasNormal());
	_key[kKeyShutdown]->CanvasPressed()->ReverseColor();
	_key[kKeyShutdown]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyShutdown]->Bind(EPDGUI_Button::EVENT_RELEASED, key_shutdown_cb);

	// Weather
	_key[kKeyWeather]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_home_92x92);
	*(_key[kKeyWeather]->CanvasPressed()) = *(_key[kKeyWeather]->CanvasNormal());
	_key[kKeyWeather]->CanvasPressed()->ReverseColor();
	_key[kKeyWeather]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeyWeather]->Bind(EPDGUI_Button::EVENT_RELEASED, key_weather_cb);

	// Syslog
	_key[kKeySyslog]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_setting_92x92);
	*(_key[kKeySyslog]->CanvasPressed()) = *(_key[kKeySyslog]->CanvasNormal());
	_key[kKeySyslog]->CanvasPressed()->ReverseColor();
	_key[kKeySyslog]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[kKeySyslog]->Bind(EPDGUI_Button::EVENT_RELEASED, key_syslog_cb);

	_time			  = 0;
	_next_update_time = 0;
}

/***********************************************************************************************************************/
Frame_Main::~Frame_Main(void)
{
	for (int i = 0; i < NUMBER_OF_KEYS; i++)
	{
		delete _key[i];
	}
}

/***********************************************************************************************************************/
void Frame_Main::AppName(m5epd_update_mode_t mode)
{
	if (!_names->isRenderExist(20))
	{
		_names->createRender(20, 26);
	}
	_names->setTextSize(20);
	_names->fillCanvas(0);
	// Render button names
	// First row
	_names->drawString("Test", 20 + 46, 16);
	_names->drawString("Setting", 20 + 46 + 136, 16);
	_names->drawString("Keyboard", 20 + 46 + 2 * 136, 16);
	_names->drawString("WLAN", 20 + 46 + 3 * 136, 16);
	_names->pushCanvas(0, 186, mode);
	_names->fillCanvas(0);
	// Second row
	_names->drawString("Storage", 20 + 46, 16);
	_names->drawString("Compare", 20 + 46 + 136, 16);
	_names->drawString("Home", 20 + 46 + 2 * 136, 16);
	_names->drawString("LifeGame", 20 + 46 + 3 * 136, 16);
	_names->pushCanvas(0, 337, mode);
	_names->fillCanvas(0);
	// Third row
	_names->drawString("MQTT", 20 + 46 + 0 * 136, 16);
	_names->drawString("Testing", 20 + 46 + 1 * 136, 16);
	_names->drawString("---", 20 + 46 + 2 * 136, 16);
	_names->drawString("Shutdown", 20 + 46 + 3 * 136, 16);
	_names->pushCanvas(0, 488, mode);
	// Forth row
	_names->drawString("Weather", 20 + 46 + 0 * 136, 16);
	_names->drawString("---", 20 + 46 + 1 * 136, 16);
	_names->drawString("---", 20 + 46 + 2 * 136, 16);
	_names->drawString("Syslog", 20 + 46 + 3 * 136, 16);
	_names->pushCanvas(0, 639, mode);
}

/***********************************************************************************************************************/
void Frame_Main::StatusBar(m5epd_update_mode_t mode)
{
	if ((millis() - _time) < _next_update_time)
	{
		return;
	}
	Serial.printf("Updating status bar ...\n");

	char buf[20];
	_bar->fillCanvas(0);
	_bar->drawFastHLine(0, 43, 540, 15);
	_bar->setTextDatum(CL_DATUM);
	//_bar->drawString("Mike", 10, 27);
	if (WiFi.isConnected())
	{
		_bar->drawString(WiFi.SSID().c_str(), 10, 27);
	}
	else
	{
		_bar->drawString("Offline", 10, 27);
	}

	// Battery
	_bar->setTextDatum(CR_DATUM);
	_bar->pushImage(498, 8, 32, 32, ImageResource_status_bar_battery_32x32);
	uint32_t vol = M5.getBatteryVoltage();
	if (vol < 3300)
	{
		vol = 3300;
	}
	else if (vol > 4350)
	{
		vol = 4350;
	}
	float battery = (float)(vol - 3300) / (float)(4350 - 3300);
	Serial.printf("Battery voltage: %dmV\n", vol);
	if (battery <= 0.01)
	{
		battery = 0.01;
	}
	if (battery > 1)
	{
		battery = 1;
	}
	uint8_t px = battery * 25;
	sprintf(buf, "%d%%", (int)(battery * 100));
	_bar->drawString(buf, 498 - 10, 27);
	_bar->fillRect(498 + 3, 8 + 10, px, 13, 15);
	// _bar->pushImage(498, 8, 32, 32, 2, ImageResource_status_bar_battery_charging_32x32);

	// Time
	rtc_time_t time_struct;
	rtc_date_t date_struct;
	M5.RTC.getTime(&time_struct);
	M5.RTC.getDate(&date_struct);
	sprintf(buf, "%2d:%02d", time_struct.hour, time_struct.min);
	_bar->setTextDatum(CC_DATUM);
	_bar->drawString(buf, 270, 27);
	_bar->pushCanvas(0, 0, mode);

	_time			  = millis();
	_next_update_time = (60 - time_struct.sec) * 1000;
}

/******************************************************************************************************************/
int Frame_Main::init(epdgui_args_vector_t &args)
{
	_is_run = 1;
	M5.EPD.WriteFullGram4bpp(GetWallpaper());
	for (int i = 0; i < NUMBER_OF_KEYS; i++)
	{
		EPDGUI_AddObject(_key[i]);
	}
	_time			  = 0;
	_next_update_time = 0;
	StatusBar(UPDATE_MODE_NONE);
	AppName(UPDATE_MODE_NONE);
	return 9;
}

/******************************************************************************************************************/
int Frame_Main::run()
{
	StatusBar(UPDATE_MODE_GL16);
	return 1;
}