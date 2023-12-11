#pragma once 

#include "Triggerbot.h"

Celestial::Scheduler Roblox_Scheduler;

void Celestial::Triggerbot::Init()
{
	/*Initalize the job we'll be hooking for trigger bot*/

	Roblox_Scheduler.HookJob
	(
		"Heartbeat",
		Stepped
	);
}

void Celestial::Triggerbot::Stepped()
{
	printf("hi\n");
	Sleep(0.01);
}
