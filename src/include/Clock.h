/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef CLOCK_H
#define CLOCK_H

#include <Windows.h>
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
