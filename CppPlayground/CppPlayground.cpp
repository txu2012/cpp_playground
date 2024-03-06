// CppPlayground.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Joystick.h"
#include <thread>
#include <queue>

void StartJoystick(std::queue<Playground::JoystickState>& dataToSend, std::mutex& mut, bool &exit) {
    using namespace Playground;
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

            dataToSend.push(current_state);

            if (exit) {
                std::cout << "Exiting joystick acquiring thread." << std::endl;
                break;
            }
        }
    }

    js.StopAcquiring();
}

void JoystickThread() {
    std::queue<Playground::JoystickState> dataToProcess;
    std::mutex mut;
    bool exit = false;
    std::thread t1(StartJoystick, std::ref(dataToProcess), std::ref(mut), std::ref(exit));

    while (TRUE) {
        if (!dataToProcess.empty()) {
            const auto& current_state = dataToProcess.front();
            dataToProcess.pop();

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

            std::cout << "Processed." << std::endl;

            if (current_state.Buttons[11] & 0x80) {
                std::cout << "Exiting joystick thread." << std::endl;
                exit = true;
                break;
            }
        }
    }
    t1.join();
}

void thread_input() {
    while (TRUE) {
        std::cout << "Enter Text: " << std::endl;

        std::string line;
        std::getline(std::cin, line);

        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) { return std::tolower(c); });
        if (line == "exit") {
            std::cout << "Exiting input thread." << std::endl;
            break;
        }
    }
}

int main()
{
    std::cout << "Hello World!\n";

    std::thread jsThread(JoystickThread);
    std::thread inputThread(thread_input);

    jsThread.join();
    inputThread.join();

    std::cout << "Threads finished" << std::endl;
}