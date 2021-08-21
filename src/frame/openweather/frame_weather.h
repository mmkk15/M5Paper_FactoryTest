#ifndef _FRAME_OPENWEATHER_H_
#define _FRAME_OPENWEATHER_H_

/***************************************************************************************************************/
#include "../../epdgui/epdgui.h"

/***************************************************************************************************************/
class Frame_Weather : public Frame_Base
{
public:
	Frame_Weather();
	~Frame_Weather();
	int	 run();
	int	 init(epdgui_args_vector_t &args);
	void exit();

private:
};

#endif //_FRAME_OPENWEATHER_H_