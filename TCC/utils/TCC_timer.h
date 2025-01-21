#ifndef TCC__TIMER_H
#define TCC__TIMER_H

#define SDL_MAIN_HANDLED
#include <SDL.h>

// Uint64 выбран, поскольку такой тип возвращает SDL_GetPerformanceCounter в SDL2
typedef Uint64 TCC_TIMER_Time;

#define _TCC_getPerfomanceCounter SDL_GetPerformanceCounter
#define _TCC_getPerformanceFrequency SDL_GetPerformanceFrequency

#define TCC_TIMER_getTime _TCC_getPerfomanceCounter

#define TCC_TIMER_getDiffUS(start, end) ( ((end) - (start)) * 1000000LLU  / _TCC_getPerformanceFrequency() ) 


#endif