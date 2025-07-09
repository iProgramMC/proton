#ifndef GamepadWii_h__
#define GamepadWii_h__

#include <wiiuse/wpad.h>

#include "Gamepad.h"
#include "GamepadProviderWii.h"

class GamepadWii : public Gamepad
{
    public:

        GamepadWii();
        virtual ~GamepadWii();

        virtual bool Init();
        virtual void Kill();
        virtual void Update();
    
    protected:

        void PressButton(int mask, int id);
};

#endif