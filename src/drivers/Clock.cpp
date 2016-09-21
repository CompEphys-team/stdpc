#include "Clock.h"
#include "Daq.h"

Clock::Clock()
{
    // setup of the system clock
    QueryPerformanceFrequency(&intClock_frequency);
    clock_frequency= (double) intClock_frequency.LowPart;
    clock_cycle= ((double) UINT_MAX + 1.0)/clock_frequency;
}


//---------------------------------------------------------------------------
void Clock::reset_RTC()
{
  static LARGE_INTEGER inT;
  QueryPerformanceCounter(&inT);
  sysT= ((double) inT.LowPart)/clock_frequency;
  t= 0.0;
}


//---------------------------------------------------------------------------
double Clock::get_RTC()
{
  static LARGE_INTEGER inT;
  static double dt, lastT;

  QueryPerformanceCounter(&inT);
  lastT= sysT;
  sysT= ((double) inT.LowPart)/clock_frequency;
  dt= sysT-lastT;
  if (dt < 0.0) dt+= clock_cycle;
  t+= dt;

  return dt;
}


//---------------------------------------------------------------------------
double Clock::wait_till_elapsed(double target_dt)
{
    static LARGE_INTEGER inT;
    static double dt, newT;

    double tolerance= 4.8e-3*target_dt;
    newT= sysT;
    while (newT-sysT-target_dt < -tolerance)
    {
        QueryPerformanceCounter(&inT);
        newT= ((double) inT.LowPart)/clock_frequency;
        if (newT < sysT) sysT-= clock_cycle;
    }
    dt= newT-sysT;
    sysT= newT;
    t+= dt;
    return dt;
}
