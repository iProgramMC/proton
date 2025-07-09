#include "PlatformPrecomp.h"
#include "WiiRemote.h"
#include "util/PointerEventHandler.h"

// ----- EXTERNS -----
extern RenderBatcher g_globalBatcher;
extern PointerEventHandler *g_pPointerEventHandler; // main.cpp
extern void LoadFileFromMemoryCompressed(Surface* surf, const uint8_t* data, size_t size); // main.cpp

// Cursor Resource
#include "cursor.h"

Surface g_cursorSurf;

void LoadCursorResource()
{
	LoadFileFromMemoryCompressed(&g_cursorSurf, (uint8*)bin2c_cursor_rttex, sizeof(bin2c_cursor_rttex));
}

// ----- WiiRemote Class Singleton -----

// TODO: Support multiple Wii Remotes
WiiRemote g_wiiRemote(WPAD_CHAN_0);

WiiRemote* GetWiiRemote() {
	return &g_wiiRemote;
}

// ----- WiiRemote Class -----

WiiRemote::WiiRemote(int channel) : m_channel(channel)
{
}

void WiiRemote::LoadResource()
{
	
}

void WiiRemote::Update()
{
	// Update Button
	WPAD_ScanPads();
	
	// Update Cursor Position & Click Status
	ir_t ir {};
	WPAD_IR(m_channel, &ir);
	
	m_bIsValid = ir.valid;
	if (!m_bIsValid)
	{
		if (m_bWasClicking)
		{
			g_pPointerEventHandler->handlePointerUpEvent(int(m_position.x), int(m_position.y), m_channel);
			m_bWasClicking = false;
		}
		
		return;
	}
	
	float x = std::min(std::max(ir.x * 720.0f / 640.0f, 0.0f), float(GetPrimaryGLX()));
	float y = std::min(std::max(ir.y * 720.0f / 640.0f, 0.0f), float(GetPrimaryGLY()));
	ConvertCoordinatesIfRequired(x, y);
	
	m_position = CL_Vec2f(x, y);
	
	m_buttonsHeld = WPAD_ButtonsHeld(m_channel);
	m_buttonsDown = WPAD_ButtonsDown(m_channel);
	m_buttonsUp   = WPAD_ButtonsUp(m_channel);
	
	const uint32 clickButton = WPAD_BUTTON_A;
	
	if (IsButtonHeld(clickButton))
	{
		if (!m_bWasClicking)
			g_pPointerEventHandler->handlePointerDownEvent(int(x), int(y), m_channel);
		
		g_pPointerEventHandler->handlePointerMoveEvent(int(x), int(y), m_channel);
		m_bWasClicking = true;
	}
	
	if (IsButtonReleased(clickButton) || (!IsButtonHeld(clickButton) && m_bWasClicking))
	{
		g_pPointerEventHandler->handlePointerUpEvent(int(x), int(y), m_channel);
		m_bWasClicking = false;
	}
	
	// Reset accessories' data.
	//
	// This is because they may have been unplugged.  They will be updated
	// later if they're still plugged in.
	m_nunchuckStickPos = CL_Vec2f();
	
	// Update Accessories
	expansion_t exp;
	WPAD_Expansion(m_channel, &exp);
	
	switch (exp.type)
	{
		case EXP_NUNCHUK:
		{
			// Update Nunchuk Stick
			float ang = exp.nunchuk.js.ang * M_PI / 180, mag = exp.nunchuk.js.mag;
			
			// note: swapped sin/cos because ang==0 means UP, not RIGHT.
			m_nunchuckStickPos = CL_Vec2f(
				sin(ang) * mag,
				cos(ang) * mag
			);
			break;
		}
	}
}

void WiiRemote::RenderCursor()
{
	if (!m_bIsValid)
		return;
	
	if (!g_cursorSurf.IsLoaded())
		return;
	
	const int offsetX = 23;
	const int offsetY = 8;
	const float scale = 0.6f;
	
	PrepareForGL();
	g_cursorSurf.Bind();
	g_cursorSurf.BlitRotated(
		m_position.x - offsetX * scale,
		m_position.y - offsetY * scale,
		CL_Vec2f(scale, scale),
		ALIGNMENT_UPPER_LEFT,
		0xFFFFFFFF,
		m_angle,
		CL_Vec2f(offsetX, offsetY) * scale
	);
	
	g_globalBatcher.Flush();
}
