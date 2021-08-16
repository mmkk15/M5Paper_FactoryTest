#ifndef _FRAME_TESTING_H_
#define _FRAME_TESTING_H_

/***************************************************************************************************************/
#include "frame_testing.h"
#include "../epdgui/epdgui.h"

/***************************************************************************************************************/
class Frame_Testing : public Frame_Base
{
public:
    Frame_Testing();
    ~Frame_Testing();
    int run();
    int init(epdgui_args_vector_t &args);

private:
    EPDGUI_Textbox *inputbox;
    EPDGUI_Button *key_textclear;
    EPDGUI_Button *key_action;
};

#endif //_FRAME_TESTING_H_