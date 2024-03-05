// CppPlayground.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Joystick.h"

int main()
{
    using namespace Playground;
    std::cout << "Hello World!\n";

    auto joysticks = Joystick::enumerate_joysticks();

    std::cout << "Num joysticks found: " << joysticks.size() << std::endl;

    auto joystickEvent = CreateEventA(NULL, false, false, "JoystickStateChange");

    Joystick js(joysticks[0], joystickEvent);
    JoystickState prevState;

    js.StartAcquiring();

    while (TRUE) {
        auto dwResult = WaitForSingleObject(joystickEvent, 0);
        if (dwResult == WAIT_OBJECT_0) {
            // Event is set. If the event was created as 
            // autoreset, it has also been reset. 
            JoystickState current_state;
            js.GetJoystickState(current_state);

            char strText[512] = {};
            for (int i = 0; i < 128; i++)
            {
                if (current_state.Buttons[i] & 0x80)
                {
                    char btnState[256];
                    snprintf(btnState, sizeof(btnState), "%02d ", i);
                    strcat_s(strText, btnState);
                }
            }

            std::cout << "Joystick Moved:"
                << " X: " << current_state.PositionX
                << " Y: " << current_state.PositionY
                << " Z: " << current_state.PositionZ
                << " Buttons: " << strText
                << std::endl;
        }
    }

    /*while (js.connected_) {
        JoystickState current_state;
        js.GetJoystickState(current_state);

        if (prevState.PositionX != current_state.PositionX ||
            prevState.PositionY != current_state.PositionY ||
            prevState.PositionZ != current_state.PositionZ) {
            std::cout << "Joystick Moved:" 
                << " X: " << current_state.PositionX
                << " Y: " << current_state.PositionY
                << " Z: " << current_state.PositionZ
                << std::endl;
        }

        if (current_state.ButtonPressed) {
            char strText[512] = {};
            for (int i = 0; i < 128; i++)
            {
                if (current_state.Buttons[i] & 0x80)
                {
                    char btnState[256];
                    snprintf(btnState, sizeof(btnState), "%02d ", i);
                    strcat_s(strText, btnState);
                }
            }
            std::cout << "Buttons: " << strText << std::endl;
        }

        prevState = current_state;
    }*/
}