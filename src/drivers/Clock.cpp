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
  static double dt;
  QueryPerformanceCounter(&inT);
  lastT= sysT;
  sysT= ((double) inT.LowPart)/clock_frequency;
  dt= sysT-lastT;
  if (dt < 0.0) dt+= clock_cycle;
  t+= dt;

  return dt;
}

double Clock::get_RTC_const()
{
    static double mySysT, dt;
    QueryPerformanceCounter(&inT);
    mySysT = ((double) inT.LowPart)/clock_frequency;
    dt = mySysT - lastT;
    if ( dt < 0.0 ) dt += clock_cycle;
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
