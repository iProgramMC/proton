#ifndef WIIREMOTE_H
#define WIIREMOTE_H

#include <wiiuse/wpad.h>

class WiiRemote
{
public:
	WiiRemote(int channel);
	
	void LoadResource();
	
	bool IsValid() { return m_bIsValid; }
	CL_Vec2f GetPosition() { return m_position; }
	float GetAngle() { return m_angle; }
	
	void Update();
	void RenderCursor();
	
	bool IsButtonHeld(uint32 button) { return m_buttonsHeld & button; }
	bool IsButtonPressed(uint32 button) { return m_buttonsDown & button; }
	bool IsButtonReleased(uint32 button) { return m_buttonsUp & button; }
	
	CL_Vec2f GetNunchuckStickPos() { return m_nunchuckStickPos; }
	
private:
	int m_channel = 0;
	
	// cursor
	bool m_bIsValid = false;
	CL_Vec2f m_position;
	float m_angle = 0.0f;
	bool m_bWasClicking = false;
	
	// button bit mask
	uint32 m_buttonsHeld = 0;
	uint32 m_buttonsDown = 0;
	uint32 m_buttonsUp = 0;
	
	// nunchuck
	CL_Vec2f m_nunchuckStickPos;
};

void LoadCursorResource();
WiiRemote* GetWiiRemote();

#endif//WIIREMOTE_H
