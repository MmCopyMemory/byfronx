#pragma once

#include "SDK.h"
#include <Task Scheduler/Task Scheduler.h>
#include <Roblox/Globals/Globals.h>
#include <Roblox/World/World.h>
#include <Utilities/Benchmark.h>
#include <thread>

Celestial::World WGame;

const auto scheduler_c = std::make_unique<Celestial::Scheduler>();
const auto scheduler = scheduler_c.get();


std::uintptr_t get_players(const std::uintptr_t datamodel)
{
	std::uintptr_t Services = *reinterpret_cast<std::uintptr_t*>(datamodel + 0x6C);
	if (!Services)
		return 0;

	std::uintptr_t Players = *reinterpret_cast<std::uintptr_t*>(Services + 0x68);
	if (!Players)
		return 0;

	return Players;
}

void teleport_support()
{
	auto Jobs = *reinterpret_cast<std::uintptr_t*>(scheduler->GetScheduler + 0x134);
	printf("Jobs: %x\n", Jobs);

	static auto stub = *reinterpret_cast<std::uintptr_t*>(Jobs + 0x40);

	while (true)
	{
		//if (!FindWindow(0, L"Roblox"))
		//	exit(0);

		*reinterpret_cast<double*>(scheduler->GetScheduler + 280) = 1.0 / Celestial::Globals::FPSLimit;

		//printf("zz\n");

		if (stub != *reinterpret_cast<std::uintptr_t*>(Jobs + 0x40))
		{
			RenderSettings.Enabled = false;
			Celestial::Globals::RenderMaster = false;

			printf("disabling render for teleport\n");

			Sleep(1000);

			Jobs = *reinterpret_cast<std::uintptr_t*>(scheduler->GetScheduler + 0x134);
			stub = *reinterpret_cast<std::uintptr_t*>(Jobs + 0x40);

			Sleep(1000);

			while (stub == 0)
				Sleep(1000);

			Celestial::Globals::DataModel = scheduler->datamodel();

			do
			{
				Sleep(1);
			} while (!WGame.IsLoaded(Celestial::Globals::DataModel - 12));

			switch (WGame.GetCreatorId(Celestial::Globals::DataModel - 12))
			{
			case 15161263: {
				Celestial::Globals::Game = "CBRO";
				break;
			}//10475715

			case 10475715: {
				Celestial::Globals::Game = "CBRO";
				break;
			}

			case 4121917: {
				Celestial::Globals::Game = "Recoil";
				break;
			}

			case 3765739: {
				Celestial::Globals::Game = "ProjDelta";
				printf("hbm\n");
				break;
			}

			case 1103278: {
				Celestial::Globals::Game = "Phantom Forces";
				break;
			}

			case 5158503: {
				Celestial::Globals::Game = "Rush Point";
				break;
			}

			case 3532271: {
				Celestial::Globals::Game = "Bad Business";
				break;
			}

			case 29761878:
			{
				Celestial::Globals::Game = "Criminality";
				break;
			}

			case 1066925:
			{
				Celestial::Globals::Game = "Apoc 2";
				break;
			}

			case 2615635:
			{
				printf("idek\n");
				Celestial::Globals::Game = "Frontlines";
				break;
			}

			case 7077761:
			{
				Celestial::Globals::Game = "Riotfall";
				break;
			}

			//case 1315961587:
			//{
			//	Celestial::Globals::Game = "Deadline";
			//	break;
			//}

			case 4818157:
			{
				printf("al3 groundzz\n");
				Celestial::Globals::Game = "th3ltgrounds";
				break;
			}

			case 5295712:
			{
				Celestial::Globals::Game = "future";
				printf("futuretopzzz\n");
				break;
			}

			default: {
				Celestial::Globals::Game = "Universal";
				break;
			}
			}


			Celestial::Globals::Workspace = WGame.GetService(Celestial::Globals::DataModel, "Workspace");
			Celestial::Globals::Players = WGame.GetService(Celestial::Globals::DataModel, "Players");
			Celestial::Globals::LocalPlayer = WGame.GetLocalPlayer(Celestial::Globals::Players);
			Celestial::Globals::Camera = *reinterpret_cast<std::uintptr_t*>(Celestial::Globals::Workspace + 0x330);

			printf("re-enabling render after teleport\n");

			RenderSettings.Enabled = true;
			Celestial::Globals::RenderMaster = true;;

			Sleep(1000);
		}
	}
}


Celestial::SDK::SDK()
{
	scheduler->init();

	Celestial::Globals::DataModel = scheduler->datamodel();

	printf("datamodel: %x\n", Celestial::Globals::DataModel);

	do
	{
		Sleep(1);
	} while (!WGame.IsLoaded(Celestial::Globals::DataModel - 12));

	Celestial::Globals::VisualEngine = scheduler->visualengine();


	Celestial::Globals::Workspace = WGame.GetService(Celestial::Globals::DataModel, "Workspace");
	printf("workspace: %x\n", Celestial::Globals::Workspace);
	Celestial::Globals::Players = WGame.GetService(Celestial::Globals::DataModel, "Players");
	printf("players: %x\n", Celestial::Globals::Players);
	Celestial::Globals::LocalPlayer = WGame.GetLocalPlayer(Celestial::Globals::Players);
	printf("localplayer: %x\n", Celestial::Globals::LocalPlayer);
	Celestial::Globals::Camera = *reinterpret_cast<std::uintptr_t*>(Celestial::Globals::Workspace + 0x390);
	switch (WGame.GetCreatorId(Celestial::Globals::DataModel - 12))
	{
		case 15161263: {
			Celestial::Globals::Game = "CBRO";
			break;
		}//10475715

		case 10475715: {
			Celestial::Globals::Game = "CBRO";
			break;
		}

		case 4121917: {
			Celestial::Globals::Game = "Recoil";
			break;
		}

		case 3765739: {
			Celestial::Globals::Game = "ProjDelta";
			printf("hbm\n");
			break;
		}

		case 1103278: {
			Celestial::Globals::Game = "Phantom Forces";
			printf("hbm\n");
			break;
		}

		case 5158503: {
			Celestial::Globals::Game = "Rush Point";
			break;
		}

		case 3532271: {
			Celestial::Globals::Game = "Bad Business";
			break;
		}

		case 29761878:
		{
			Celestial::Globals::Game = "Criminality";
			break;
		}
		
		case 1066925:
		{
			Celestial::Globals::Game = "Apoc 2";
			break;
		}

		case 2615635:
		{
			Celestial::Globals::Game = "Frontlines";
			break;
		}

		case 7077761:
		{
			Celestial::Globals::Game = "Riotfall";
			break;
		}

		//case 1315961587:
		//{
		//	Celestial::Globals::Game = "Deadline";
		//	break;
		//}

		case 4818157:
		{
			printf("al3 groundzz\n");
			Celestial::Globals::Game = "th3ltgrounds";
			break;
		}

		case 5295712:
		{
			Celestial::Globals::Game = "future";
			printf("futuretopzzz\n");
			break;
		}

		default: {
			Celestial::Globals::Game = "Universal";
			break;
		}
	}

	//std::thread(teleport_support).detach();
}
