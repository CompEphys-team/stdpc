#ifndef CLOCK_H
#define CLOCK_H

#include <windows.h>
#include <QVector>

// Forward
class DAQ;

class Clock
{
public:
    Clock();

    void reset_RTC();
    double get_RTC();
    double get_RTC_const();

    double wait_till_elapsed(double target_dt);

    double t;

private:
  LARGE_INTEGER intClock_frequency;
  double sysT;
  double clock_frequency;
  double clock_cycle;


  LARGE_INTEGER inT;
  double lastT;
};

#endif // CLOCK_H
