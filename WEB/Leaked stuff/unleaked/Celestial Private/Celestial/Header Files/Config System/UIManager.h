
#include "SaveLoad.h"
#include <Roblox/Globals/Globals.h>

#include <stdlib.h>
#include <stdio.h>
#include <shlobj_core.h>
#include <direct.h>

void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    std::size_t startPosition = str.find(from);
    if (startPosition == std::string::npos) return;
    str.replace(startPosition, from.length(), to);
}

void SaveConfig(std::string Name)
{
    char path[MAX_PATH];
    
    PWSTR appdata = NULL;
    char dest[MAX_PATH];

    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &appdata) == S_OK) {
        wcstombs(dest, appdata, MAX_PATH);
    }
    else {
        fprintf(stderr, "error getting appdata path\n");
    }

    std::string AppDataCelestial = std::string(std::string(dest) + "\\Celestial\\Configs");

    cConfig CFG(AppDataCelestial + "\\" + std::string(Name) + ".cfg", CONFIG_READWRITE);
    
    CFG.Write<int>("MenuKey", Celestial::Globals::MenuKey);

    /*ESP*/
    CFG.Write<bool>("RenderMaster", Celestial::Globals::RenderMaster);
    CFG.Write<int>("RenderDist", Celestial::Globals::RenderDist);
    CFG.Write<bool>("Teamcheck", Celestial::Globals::Teamcheck);
    CFG.Write<bool>("Healthcheck", Celestial::Globals::Healthcheck);

    CFG.Write<bool>("OverrideTeamColors", Celestial::Globals::useteam);
    CFG.Write<ImColor>("OverrideTeamColor", Celestial::Globals::teamcolor);

    CFG.Write<bool>("VisualizePrediction", Celestial::Globals::visualize_prediction);

    CFG.Write<bool>("SkeletonESP", Celestial::Globals::SkeletonESP);
    CFG.Write<ImColor>("SkeletonColor", Celestial::Globals::SkeletonColor);

    CFG.Write<bool>("ShowFOV", Celestial::Globals::FOV);
    CFG.Write<ImColor>("FOVColor", Celestial::Globals::FOVColor);
    CFG.Write<int>("FOVValue", Celestial::Globals::FOVValue);

    CFG.Write<bool>("BoxESP", Celestial::Globals::PlayerESP);
    CFG.Write<ImColor>("BoxColor", Celestial::Globals::BoxColor);

    CFG.Write<bool>("Tracers", Celestial::Globals::Tracers);
    CFG.Write<ImColor>("TracersColor", Celestial::Globals::TracersColor);

    CFG.Write<bool>("NameESP", Celestial::Globals::NameESP);
    CFG.Write<ImColor>("NameColor", Celestial::Globals::NameColor);

    CFG.Write<bool>("ShowDistance", Celestial::Globals::ShowDistance);
    CFG.Write<ImColor>("DistanceColor", Celestial::Globals::DistanceColor);

    /*AIMBOT*/
    CFG.Write<int>("Smoothness", Celestial::Globals::Smoothness);
    CFG.Write<bool>("AimMaster", Celestial::Globals::AimMaster);

    CFG.Write<bool>("prediction", Celestial::Globals::prediction);
    CFG.Write<int>("p_strength", Celestial::Globals::p_strength);

    CFG.Write<int>("AimKey", Celestial::Globals::AimKey);
    CFG.Write<int>("AimPart", Celestial::Globals::AimPart);
    CFG.Write<int>("XOffset", Celestial::Globals::XOffset);
    CFG.Write<int>("YOffset", Celestial::Globals::YOffset);
    CFG.Write<int>("LockDist", Celestial::Globals::LockDist);

    /*PF*/
    CFG.Write<bool>("ShowGunNames", Celestial::Globals::ShowGunNames);
    CFG.Write<ImColor>("GunColor", Celestial::Globals::GunColor);

    /*Bad Business*/
    CFG.Write<bool>("ShowGrenades", Celestial::Globals::ShowGrenades);
    CFG.Write<bool>("GrenadeWarning", Celestial::Globals::GrenadeWarning);

    /*Apoc*/
    CFG.Write<bool>("ShowLocations", Celestial::Globals::ShowLocations);
    CFG.Write<ImColor>("LocationColor", Celestial::Globals::LocationColor);

    CFG.Write<int>("LocationDist", Celestial::Globals::LocationDist);
    CFG.Write<bool>("ShowLocationsOutline", Celestial::Globals::ShowLocationsOutline);

    CFG.Write<bool>("ShowVehicles", Celestial::Globals::ShowVehicles);
    CFG.Write<ImColor>("VehiclesColor", Celestial::Globals::VehiclesColor);

    CFG.Write<int>("VehicleDist", Celestial::Globals::VehicleDist);
    CFG.Write<bool>("VehicleDistOutline", Celestial::Globals::VehicleDistOutline);

    CFG.Write<bool>("ShowEquipped", Celestial::Globals::ShowEquipped);

    // proj delta
    CFG.Write<bool>("ShowEquippedDELTA", Celestial::Globals::ShowEquippedDELTA);
    CFG.Write<bool>("ShowContainers", Celestial::Globals::ShowContainers);
    CFG.Write<int>("ShowEquippedDELTA_Dist", Celestial::Globals::ItemDist);
    CFG.Write<int>("ContainerDist", Celestial::Globals::ContainerDist);

    /*Criminality*/
    CFG.Write<bool>("ShowDealer", Celestial::Globals::ShowDealer);
    CFG.Write<ImColor>("DealerColor", Celestial::Globals::DealerColor);

    CFG.Write<int>("DealerDist", Celestial::Globals::DealerDist);
    CFG.Write<bool>("DealerDistOutline", Celestial::Globals::DealerDistOutline);

    CFG.Write<bool>("ShowATM", Celestial::Globals::ShowATM);
    CFG.Write<ImColor>("ATMColor", Celestial::Globals::ATMColor);

    CFG.Write<int>("ATMDist", Celestial::Globals::ATMDist);
    CFG.Write<bool>("ATMDistOutline", Celestial::Globals::ATMDistOutline);

    CFG.closeOutputStream();
}

void LoadConfig(std::string Name)
{
    PWSTR appdata = NULL;
    char dest[MAX_PATH];

    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &appdata) == S_OK) {
        wcstombs(dest, appdata, MAX_PATH);
    }
    else {
        fprintf(stderr, "error getting appdata path\n");
    }

    std::string AppDataCelestial = std::string(std::string(dest) + "\\Celestial\\Configs");
    cConfig CFG(AppDataCelestial + "\\" + std::string(Name), CONFIG_READ);

    Celestial::Globals::MenuKey = CFG.Read<int>("MenuKey");

    /*ESP*/
    Celestial::Globals::RenderMaster = CFG.Read<bool>("RenderMaster");
    Celestial::Globals::Teamcheck = CFG.Read<bool>("Teamcheck");
    Celestial::Globals::Healthcheck = CFG.Read<bool>("Healthcheck");
    Celestial::Globals::RenderDist = CFG.Read<int>("RenderDist");

    Celestial::Globals::useteam = CFG.Read<bool>("OverrideTeamColors");
    Celestial::Globals::teamcolor = CFG.Read<ImColor>("OverrideTeamColor");

    Celestial::Globals::visualize_prediction = CFG.Read<bool>("VisualizePrediction");

    Celestial::Globals::SkeletonESP = CFG.Read<bool>("SkeletonESP");
    Celestial::Globals::SkeletonColor = CFG.Read<ImColor>("SkeletonColor");

    Celestial::Globals::FOV = CFG.Read<bool>("ShowFOV");
    Celestial::Globals::FOVColor = CFG.Read<ImColor>("FOVColor");
    Celestial::Globals::FOVValue = CFG.Read<int>("FOVValue");

    Celestial::Globals::PlayerESP = CFG.Read<bool>("BoxESP");
    Celestial::Globals::BoxColor = CFG.Read<ImColor>("BoxColor");

    Celestial::Globals::Tracers = CFG.Read<bool>("Tracers");
    Celestial::Globals::TracersColor = CFG.Read<ImColor>("TracersColor");

    Celestial::Globals::NameESP = CFG.Read<bool>("NameESP");
    Celestial::Globals::NameColor = CFG.Read<ImColor>("NameColor");

    Celestial::Globals::ShowDistance = CFG.Read<bool>("ShowDistance");
    Celestial::Globals::DistanceColor = CFG.Read<ImColor>("DistanceColor");

    /*AIMBOT*/
    Celestial::Globals::Smoothness = CFG.Read<int>("Smoothness");
    Celestial::Globals::AimMaster = CFG.Read<bool>("AimMaster");

    Celestial::Globals::prediction = CFG.Read<bool>("prediction");
    Celestial::Globals::p_strength = CFG.Read<int>("p_strength");

    Celestial::Globals::AimKey = CFG.Read<int>("AimKey");
    Celestial::Globals::AimPart = CFG.Read<int>("AimPart");
    Celestial::Globals::XOffset = CFG.Read<int>("XOffset");
    Celestial::Globals::YOffset = CFG.Read<int>("YOffset");
    Celestial::Globals::LockDist = CFG.Read<int>("LockDist");

    /*PF*/
    Celestial::Globals::ShowGunNames = CFG.Read<bool>("ShowGunNames");
    Celestial::Globals::GunColor = CFG.Read<ImColor>("GunColor");

    /*Bad Business*/
    Celestial::Globals::ShowGrenades = CFG.Read<bool>("ShowGrenades");
    Celestial::Globals::GrenadeWarning = CFG.Read<bool>("GrenadeWarning");

    /*Apoc 2*/
    Celestial::Globals::ShowLocations = CFG.Read<bool>("ShowLocations");
    Celestial::Globals::LocationColor = CFG.Read<ImColor>("LocationColor");
    Celestial::Globals::LocationDist = CFG.Read<int>("LocationDist");
    Celestial::Globals::ShowLocationsOutline = CFG.Read<bool>("ShowLocationsOutline");
    Celestial::Globals::ShowEquipped = CFG.Read<bool>("ShowEquipped");

    Celestial::Globals::ShowVehicles = CFG.Read<bool>("ShowVehicles");
    Celestial::Globals::VehiclesColor = CFG.Read<ImColor>("VehiclesColor");
    Celestial::Globals::VehicleDist = CFG.Read<int>("VehicleDist");
    Celestial::Globals::VehicleDistOutline = CFG.Read<bool>("VehicleDistOutline");

    // proh delta
    Celestial::Globals::ShowEquippedDELTA = CFG.Read<bool>("ShowEquippedDELTA");
    Celestial::Globals::ItemDist = CFG.Read<int>("ShowEquippedDELTA_Dist");

    /*Criminality*/
    Celestial::Globals::ShowDealer = CFG.Read<bool>("ShowDealer");
    Celestial::Globals::DealerColor = CFG.Read<ImColor>("DealerColor");
    Celestial::Globals::DealerDist = CFG.Read<int>("DealerDist");
    Celestial::Globals::DealerDistOutline = CFG.Read<bool>("DealerDistOutline");

    Celestial::Globals::ShowATM = CFG.Read<bool>("ShowATM");
    Celestial::Globals::ATMColor = CFG.Read<ImColor>("ATMColor");
    Celestial::Globals::ATMDist = CFG.Read<int>("ATMDist");
    Celestial::Globals::ATMDistOutline = CFG.Read<bool>("ATMDistOutline");
}
