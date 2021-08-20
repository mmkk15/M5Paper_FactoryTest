/**************************************************************************************************************************/
#ifndef __SYSLOG_H__
#define __SYSLOG_H__

#pragma once

/**************************************************************************************************************************/
#include <WString.h>
#include <inttypes.h>
#include <vector>

/**************************************************************************************************************************/
#define SYSLOG_MAX_LENGTH 100

/**************************************************************************************************************************/
class C_SYSLOG
{
public: // -----------------------------------------------------------------------
    C_SYSLOG();
    ~C_SYSLOG();

    void Add(String entry);
    void clearSyslog();
    String getSyslogString(int StartElementNumber = 0, int NumberOfElements = 25);
    bool hasChanged();

private: // -----------------------------------------------------------------------
    std::vector<String> m_Syslog;
    bool m_hasChanged;
};

extern C_SYSLOG Syslog;

#endif