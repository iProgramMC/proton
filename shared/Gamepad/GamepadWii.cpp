#include "GamepadWii.h"

#include "../wii/WiiRemote.h"

GamepadWii::GamepadWii()
{

}

GamepadWii::~GamepadWii()
{

}

bool GamepadWii::Init()
{
    m_name = "Nintendo Wii";
    m_buttonsUsedCount = 11;
    m_axisUsedCount = 2;

    m_buttons[0].m_virtualKey = VIRTUAL_KEY_DIR_LEFT;    // Remote D-Pad
    m_buttons[1].m_virtualKey = VIRTUAL_KEY_DIR_RIGHT;   // Remote D-Pad
    m_buttons[2].m_virtualKey = VIRTUAL_KEY_DIR_UP;      // Remote D-Pad
    m_buttons[3].m_virtualKey = VIRTUAL_KEY_DIR_DOWN;    // Remote D-Pad
    m_buttons[4].m_virtualKey = VIRTUAL_DPAD_SELECT;     // Minus Button
    m_buttons[5].m_virtualKey = VIRTUAL_DPAD_START;      // Plus Button
	
	// A button handled separately for clicks
	// TODO: Figure out a solution?!
	
    m_buttons[6].m_virtualKey = VIRTUAL_KEY_GAME_JUMP;   // B Button
    m_buttons[7].m_virtualKey = VIRTUAL_KEY_BACK;        // 1 Button
    m_buttons[8].m_virtualKey = VIRTUAL_KEY_GAME_FIRE;   // 2 Button
	
    m_buttons[9].m_virtualKey = VIRTUAL_KEY_GAME_JUMP;   // Z Button
    m_buttons[10].m_virtualKey = VIRTUAL_KEY_GAME_FIRE;  // C Button

    return true;
}

void GamepadWii::Kill()
{

}

void GamepadWii::Update()
{
    PressButton(WPAD_BUTTON_LEFT,  0);
    PressButton(WPAD_BUTTON_RIGHT, 1);
    PressButton(WPAD_BUTTON_UP,    2);
    PressButton(WPAD_BUTTON_DOWN,  3);
    PressButton(WPAD_BUTTON_MINUS, 4);
    PressButton(WPAD_BUTTON_PLUS,  5);
    PressButton(WPAD_BUTTON_B,     6);
    PressButton(WPAD_BUTTON_1,     7);
    PressButton(WPAD_BUTTON_2,     8);
    PressButton(WPAD_NUNCHUK_BUTTON_Z, 9);
    PressButton(WPAD_NUNCHUK_BUTTON_C, 10);

    SendArcadeDirectionByKey(VIRTUAL_KEY_DIR_DOWN,  GetWiiRemote()->IsButtonHeld(WPAD_BUTTON_DOWN));
	SendArcadeDirectionByKey(VIRTUAL_KEY_DIR_UP,    GetWiiRemote()->IsButtonHeld(WPAD_BUTTON_UP));
	SendArcadeDirectionByKey(VIRTUAL_KEY_DIR_LEFT,  GetWiiRemote()->IsButtonHeld(WPAD_BUTTON_LEFT));
	SendArcadeDirectionByKey(VIRTUAL_KEY_DIR_RIGHT, GetWiiRemote()->IsButtonHeld(WPAD_BUTTON_RIGHT));

	CL_Vec2f pos = GetWiiRemote()->GetNunchuckStickPos();

	SetAxis(0, pos.x);
	SetAxis(1, pos.y);

    Gamepad::Update();
}

void GamepadWii::PressButton(int mask, int id)
{
	bool isHeld = GetWiiRemote()->IsButtonHeld(mask);
	
    if (m_buttons[id].m_bDown != isHeld)
		OnButton(isHeld, id);
}