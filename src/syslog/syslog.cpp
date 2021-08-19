
/**************************************************************************************************************************/
#include "syslog.h"
#include "HardwareSerial.h"

/**************************************************************************************************************************/
C_SYSLOG::C_SYSLOG()
{
	clearSyslog();
	m_hasChanged = false;
}

/**************************************************************************************************************************/
C_SYSLOG::~C_SYSLOG()
{
}

/**************************************************************************************************************************/
void C_SYSLOG::Add(String entry)
{
	Serial.println(entry);

	uint16_t secs = (uint16_t)((float)millis() / 1000.0f) % 0x1FFF; // Mask seconds to 0 - 8191 (four digits)

	String s = ""; 
	for (int i = s.length(); i < 5; i++) { s += " "; }
	s += String(secs) + ": " + entry;

	m_Syslog.push_back(s);
	if (m_Syslog.size() > SYSLOG_MAX_LENGTH)
	{
		m_Syslog.erase(m_Syslog.begin());
	}

	m_hasChanged = true;
}

/**************************************************************************************************************************/
bool C_SYSLOG::hasChanged()
{
	return m_hasChanged;
}

/**************************************************************************************************************************/
void C_SYSLOG::clearSyslog()
{
	m_Syslog.clear();
}

/**************************************************************************************************************************/
String C_SYSLOG::getSyslogString(int StartElementNumber, int NumberOfElements)
{
	String s = "";
	if (m_Syslog.size())
	{
		Serial.printf("getSyslogString #Elements:%d %d\n", NumberOfElements, StartElementNumber);

		int SEM = StartElementNumber;
		if(SEM >= m_Syslog.size())
		{
			SEM = 0;
		}
		for(int Index = SEM; Index < SEM + NumberOfElements; Index++)
		{
			Serial.printf("I: %d\n", Index);
			s += m_Syslog[Index] + "\n"; 
			if(Index >= (m_Syslog.size() - 1))
				break;
		}
	}
	else
	{
		s += "Syslog empty ...";
	}

	m_hasChanged = false;

	return s;
}

/**************************************************************************************************************************/
// Instance of syslog
C_SYSLOG Syslog;
