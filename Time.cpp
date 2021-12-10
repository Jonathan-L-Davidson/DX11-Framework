#include "Time.h"
#include <Windows.h>
#include <debugapi.h>

Time::Time() {
	_timeFPS = std::chrono::steady_clock::now();
	_fpsCounter = 0;
	_deltaTime = _timeOld - _timeNow;
}

Time::~Time() {
}

void Time::UpdateFPS() {
	double elapsedTime = std::chrono::duration<double, std::milli>(_timeNow - _timeFPS).count();
	_fpsCounter += 1;
	if (elapsedTime > 1000.0f) { // If 1 second passed, reset the FPS timer and reset the counter.
		framerate = _fpsCounter;
		_fpsCounter = 0;
		_timeFPS = std::chrono::steady_clock::now();
	}
}

void Time::StartFrame() {
	_timeOld = _timeNow;
}

void Time::EndFrame() {
	_timeNow = std::chrono::steady_clock::now();
	_deltaTime = _timeNow - _timeOld;
}

double Time::GetDelta() {
	return _deltaTime.count();
}