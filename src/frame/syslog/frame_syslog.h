#ifndef _FRAME_SYSLOG_H_
#define _FRAME_SYSLOG_H_

/***************************************************************************************************************/
#include "../../epdgui/epdgui.h"

/***************************************************************************************************************/
class Frame_Syslog : public Frame_Base
{
public:
    Frame_Syslog();
    ~Frame_Syslog();
    int run();
    int init(epdgui_args_vector_t &args);

private:
    EPDGUI_Textbox *inputbox;
    EPDGUI_Button *key_textclear;
    EPDGUI_Button *key_action;
};

#endif //_FRAME_TESTING_H_