#ifndef TIMER_H
#define TIMER_H

using namespace std;
#include <sys/time.h>

class Timer{
public:
	timeval	_start_t;
	
	Timer();
	~Timer();
	
	double getElapsedTime();	
};

#endif