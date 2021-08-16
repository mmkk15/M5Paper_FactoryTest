/***************************************************************************************************************/
#include "frame_testing.h"
#include <HTTPClient.h>

/***************************************************************************************************************/
static uint16_t textsize = 26;

/***************************************************************************************************************/
// CALLBACK
void key_testing_textclear_cb(epdgui_args_vector_t &args)
{
    Serial.println("Testing: Clear textbox");
    ((EPDGUI_Textbox *)(args[0]))->SetText("");
}

/***************************************************************************************************************/
void key_action_cb(epdgui_args_vector_t &args)
{
    Serial.println("Testing: Action");

    Serial.println("************************************");
    Serial.println("Querying weather:");

    HTTPClient http; // Declare an object of class HTTPClient
    String Location = "Moorenweis, DE";
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

    http.begin(RequestUrl);                                     
    int httpCode = http.GET(); // Sending the request
    if (httpCode > 0) // Checking the returning code
    {
        String payload = http.getString(); // Getting the request response payload
        Serial.println(payload);
        ((EPDGUI_Textbox *)(args[0]))->SetText(payload);
    }
    else
    {
        Serial.println("  Error getting weather ...");
    }
    Serial.println("");
}

/***************************************************************************************************************/
Frame_Testing::Frame_Testing() : Frame_Base()
{
    _frame_name = "Frame_Testing";

    inputbox = new EPDGUI_Textbox(4, 100, 532, 512);
    inputbox->SetTextSize(textsize);
    inputbox->SetState(EPDGUI_Textbox::EVENT_PRESSED);

    key_textclear = new EPDGUI_Button("CLEAR", 10, 900, 250, 52);
    key_action = new EPDGUI_Button("ACTION", 300, 900, 250, 52);

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
