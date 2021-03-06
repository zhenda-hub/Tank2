#pragma once
#include <windows.h>
class CGameTimer
{
private:
    // 计时器开始时间
    unsigned int m_timeStart{ ::GetTickCount()};
    // 总的超时时间
    unsigned int m_timeout{ 3000};
    // 上一次更新时间
    unsigned int m_timeLast{m_timeStart};
    // 时间间隔
    unsigned int m_timeval{300};

public:

    explicit CGameTimer(unsigned int timeout = 3000, unsigned int timeval = 300)
        : m_timeout(timeout)
        , m_timeval(timeval)
    {
    }

    virtual ~CGameTimer()
    {
    }

    // 设置时间隔
    void SetTimeval(unsigned int timeval)
    {
        m_timeval = timeval;
    }

    // 设置上一次时间
    void SetLastTime()
    {
        m_timeLast = GetTickCount();
    }

    // 重设 起始时间 : 为当前时间
    void StartTimer()
    {
        m_timeStart =  ::GetTickCount();
    }

    // 设置总的超时时间
    void SetTimeout(unsigned int timeout)
    {
        m_timeout = timeout;
    }

    // 判断是否超时(总时间)
    bool IsTimeout() const
    {
        return GetTickCount() - m_timeStart >= m_timeout;
    }

    // 判断是否到达时间隔:如果到了，是否更新
    bool IsTimeval(bool bUpdate = true)
    {
        if(GetTickCount() - m_timeLast >= m_timeval) {
            if(bUpdate) {
                m_timeLast = GetTickCount();
            }
            return true;
        }
        else {
            return false;
        }
    }

};

// 判断时间隔的
class CGameTimeval
{
private:

    unsigned int m_timeLast{ GetTickCount() };

    unsigned int m_timeval{300};

public:

    explicit CGameTimeval(unsigned int timeval = 300)
        : m_timeval(timeval)
    {
    }

    virtual ~CGameTimeval()
    {
    }

    void SetTimeval(unsigned int timeval)
    {
        m_timeval = timeval;
    }
    // 重设 起始时间 : 为当前时间
    void SetLastTime()
    {
        m_timeLast = GetTickCount();
    }

    // 是否到达时间隔:如果到了，是否更新
    bool IsTimeval(bool bUpdate = true)
    {
        if(GetTickCount() - m_timeLast >= m_timeval) {
            if(bUpdate) {
                m_timeLast = GetTickCount();
            }
            return true;
        }
        else {
            return false;
        }
    }

};


