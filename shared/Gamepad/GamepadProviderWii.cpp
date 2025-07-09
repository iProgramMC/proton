#include "PlatformPrecomp.h"
#include "GamepadProviderWii.h"
#include "GamepadWii.h"
#include "GamepadManager.h"

GamepadProviderWii::GamepadProviderWii()
{
    
}

GamepadProviderWii::~GamepadProviderWii()
{

}

bool GamepadProviderWii::Init()
{
    LogMsg("Initting Vita gamepad provider");

    GamepadWii* pPad = new GamepadWii;
	pPad->SetProvider(this);
	GetGamepadManager()->AddGamepad(pPad);    

    return true;
}

void GamepadProviderWii::Kill()
{

}

void GamepadProviderWii::Update()
{

}