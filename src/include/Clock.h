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

    void add(DAQ*);
    void remove(DAQ*);
    void clear();

    void start();
    double get_RTC();
    void stop();

    double wait_till_elapsed(double target_dt);

    double t;

    QVector<DAQ*> board;

private:
  LARGE_INTEGER intClock_frequency;
  double sysT;
  double clock_frequency;
  double clock_cycle;
};

#endif // CLOCK_H
