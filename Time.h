#pragma once
#include <chrono>

class Time {
private:
	std::chrono::time_point<std::chrono::steady_clock> _timeFPS;

	std::chrono::time_point<std::chrono::steady_clock> _timeNow;
	std::chrono::time_point<std::chrono::steady_clock> _timeOld;

	std::chrono::duration<double> _time;
	std::chrono::duration<double> _deltaTime;

	int _fpsCounter;
public:
	Time();
	~Time();

	int framerate = 0;

	void UpdateFPS();

	void StartFrame();
	void EndFrame();
	
	double GetTime();
	double GetDelta();
};

