using namespace std;

#include "Timer.h"
#include <cstddef>

Timer::Timer(){
	gettimeofday(&_start_t,NULL);	
}

Timer::~Timer(){
}

double Timer::getElapsedTime(){
	timeval t;
	gettimeofday(&t,NULL);
	
	double	elapsedtime = (t.tv_sec - _start_t.tv_sec) * 1000.0;
	elapsedtime += (t.tv_usec - _start_t.tv_usec) / 1000.0;
	
	elapsedtime = elapsedtime*0.001;
	
	/*double	elapsedtime = (t.tv_sec - _start_t.tv_sec);
	elapsedtime += (t.tv_usec - _start_t.tv_usec) / 1000000.0;*/
	
	return elapsedtime;
}