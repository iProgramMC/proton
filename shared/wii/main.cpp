#include "BaseApp.h"

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/system.h>
#include <ogc/video.h>
#include <ogc/gx.h>
#include <fat.h>
#include <network.h>

#include "util/PassThroughPointerEventHandler.h"

#define DEFAULT_FIFO_SIZE 256 * 1024

#define HASPECT 			320
#define VASPECT 			240

int g_winVideoScreenX = 640;
int g_winVideoScreenY = 480;

extern RenderBatcher g_globalBatcher;

PointerEventHandler *g_pPointerEventHandler;

bool g_bRemoteValid = false;
int g_remoteX = 0;
int g_remoteY = 0;

int GetPrimaryGLX()
{
	return g_winVideoScreenX;
}
int GetPrimaryGLY()
{
	return g_winVideoScreenY;
}

static void *xfb = nullptr;
static GXRModeObj *vmode = nullptr;
static char gp_fifo[DEFAULT_FIFO_SIZE] __attribute__((aligned(32)));

void SetupGX()
{
	Mtx44 p;
	int df = 1; // deflicker on/off

	GX_SetCurrentGXThread();
	GX_SetViewport (0, 0, vmode->fbWidth, vmode->efbHeight, 0, 1);
	GX_SetDispCopyYScale ((f32) vmode->xfbHeight / (f32) vmode->efbHeight);
	GX_SetScissor (0, 0, vmode->fbWidth, vmode->efbHeight);

	GX_SetDispCopySrc(0, 0, vmode->fbWidth, vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth, vmode->xfbHeight);
	GX_SetCopyFilter (vmode->aa, vmode->sample_pattern, (df == 1) ? GX_TRUE : GX_FALSE, vmode->vfilter);

	GX_SetFieldMode (vmode->field_rendering, ((vmode->viHeight == 2 * vmode->xfbHeight) ? GX_ENABLE : GX_DISABLE));
	GX_SetPixelFmt (GX_PF_RGB8_Z24, GX_ZC_LINEAR);
	GX_SetDispCopyGamma (GX_GM_1_0);
	GX_SetCullMode (GX_CULL_NONE);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	GX_SetZMode (GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate (GX_TRUE);
	GX_SetAlphaUpdate(GX_FALSE);

	guOrtho(p, VASPECT, -VASPECT, -HASPECT, HASPECT, 100, 1000); // matrix, t, b, l, r, n, f
	GX_LoadProjectionMtx (p, GX_ORTHOGRAPHIC);
	GX_Flush();
}

extern "C" void InitializeGLdata();

void InitializeVideo()
{
	// thanks https://github.com/mardy/SDL/blob/ogc-sdl-1.2/src/video/ogc/SDL_ogc_video.c#L1155
    VIDEO_Init();

    vmode = VIDEO_GetPreferredMode(nullptr);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmode));
	
    VIDEO_Configure(vmode);
	
	VIDEO_ClearFrameBuffer(vmode, xfb, COLOR_BLACK);
    VIDEO_SetNextFramebuffer(xfb);
	
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();
	
	memset(&gp_fifo, 0, DEFAULT_FIFO_SIZE);
	GX_Init(&gp_fifo, DEFAULT_FIFO_SIZE);
	SetupGX();
	
	InitializeGLdata();
}

void CheckMessages()
{
    while (!GetBaseApp()->GetOSMessages()->empty())
	{
		OSMessage m = GetBaseApp()->GetOSMessages()->front();
		GetBaseApp()->GetOSMessages()->pop_front();

		switch (m.m_type)
		{
			case OSMessage::MESSAGE_CHECK_CONNECTION:
				//GetMessageManager()->SendGUI(MESSAGE_TYPE_OS_CONNECTION_CHECKED, (float)RT_kCFStreamEventOpenCompleted, 0.0f);	
				break;
		}
	}
}

bool g_bWasClicking = false;

void WiiRemoteUpdate(int channel)
{
	ir_t ir {};
	WPAD_IR(channel, &ir);
	
	if (!ir.valid) {
		g_bRemoteValid = false;
		return;
	}
	
	g_bRemoteValid = true;
	g_remoteX = ir.x;
	g_remoteY = ir.y;
	
	u32 buttons = WPAD_ButtonsHeld(channel);
	
	if (buttons & WPAD_BUTTON_A)
	{
		if (g_bWasClicking) {
			g_pPointerEventHandler->handlePointerMoveEvent(g_remoteX, g_remoteY, channel);
		}
		else {
			g_pPointerEventHandler->handlePointerDownEvent(g_remoteX, g_remoteY, channel);
			g_bWasClicking = true;
		}
	}
	else
	{
		g_pPointerEventHandler->handlePointerUpEvent(g_remoteX, g_remoteY, channel);
		g_bWasClicking = false;
	}
	
}

Surface g_cursorSurf;

void WiiRemoteDraw()
{
	if (!g_bRemoteValid)
		return;
	
	if (!g_cursorSurf.IsLoaded())
		return;
	
	// TODO: The cursor texture is corrupted and I don't know why.
	// Seems to always happen with RTSimpleApp for some reason
	PrepareForGL();
	g_cursorSurf.Bind();
	g_cursorSurf.Blit(g_remoteX, g_remoteY, 0xFFFFFFFF, 0.0f, CL_Vec2f());
	
	g_globalBatcher.Flush();
}

int main()
{
	fatInitDefault();
	InitializeVideo();
    WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	net_init();
	
    glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	
	g_pPointerEventHandler = new PassThroughPointerEventHandler();

	if (!GetBaseApp()->Init()) exit(0);
	
	g_cursorSurf.LoadFile("interface/checkbox.rttex");
	
	while (true)
	{
		WPAD_ScanPads();
		
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_HOME)
			break;
		
		if (SYS_ResetButtonDown())
			break;
		
        GetBaseApp()->Update();
		GetBaseApp()->Draw();
		
		// Update Remote
		WiiRemoteUpdate(0);
		WiiRemoteDraw();
		
		glFinish();
		
		CheckMessages();

		GX_CopyDisp(xfb, GX_TRUE);
		VIDEO_SetNextFramebuffer(xfb);
		
		VIDEO_Flush();
		VIDEO_WaitVSync();
		
	}
	
	delete g_pPointerEventHandler;
	return 0;
}

