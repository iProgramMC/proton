#ifndef GamepadProviderWii_h__
#define GamepadProviderWii_h__

#include "GamepadProvider.h"

class GamepadProviderWii : public GamepadProvider
{
    public:

        GamepadProviderWii();
        virtual ~GamepadProviderWii();

        virtual string GetName() { return "Nintendo Wii"; }
        virtual bool Init();
	    virtual void Kill();
	    virtual void Update();
};

#endif