//###########################################################################
// FILE:   clock-arch.h
// TITLE:  uIP Project Specific Clock-Architecture header file
//###########################################################################
// $TI Release: F28M35x Support Library v130 $
// $Release Date: Mon Apr 30 15:31:28 CDT 2012 $
//###########################################################################

#ifndef __CLOCK_ARCH_H__
#define __CLOCK_ARCH_H__

// Define how many clock ticks in one second.
// Note:  This should match the value of SYSTICKHZ in the main program.
#define CLOCK_CONF_SECOND       100

// Define the clock type used for returning system ticks.
typedef unsigned long clock_time_t;

#endif // __CLOCK_ARCH_H__


