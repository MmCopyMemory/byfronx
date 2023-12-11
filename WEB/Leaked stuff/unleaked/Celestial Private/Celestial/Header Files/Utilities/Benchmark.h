#pragma once

#include <Windows.h>
#include <chrono>
#include <iostream>

class benchmark
{
public:
	benchmark() {
		start = std::chrono::high_resolution_clock::now();
	}
	~benchmark() {
		stop();
	}
	void stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto start_ = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
		auto stop_ = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
		auto final = stop_ - start_;
		double in_ms = final * 0.001;
		std::cout << in_ms << " (ms)" << std::endl;
	}
private:
	std::chrono::time_point< std::chrono::high_resolution_clock> start;
};