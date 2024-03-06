// CppPlayground.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Joystick.h"
#include <thread>
#include <queue>

void JoystickThread(std::queue<Playground::JoystickState>& dataToProcess, std::mutex& mut, bool& exit) {
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
}

void StartJoystick(Playground::JoystickInfo joysticks) {
    using namespace Playground;
    std::queue<Playground::JoystickState> dataToSend;
    std::mutex mut;
    bool exit = false;
    std::thread t1(JoystickThread, std::ref(dataToSend), std::ref(mut), std::ref(exit));

    auto joystickEvent = CreateEventA(NULL, false, false, "JoystickStateChange");

    Joystick js(joysticks, joystickEvent);
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
        }

        if (exit) {
            std::cout << "Exiting joystick acquiring thread." << std::endl;
            break;
        }
    }
    t1.join();

    js.StopAcquiring();
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

bool tryParse(std::string& input, int& output) {
    try {
        output = std::stoi(input);
    }
    catch (std::invalid_argument) {
        return false;
    }
    return true;
}

int main()
{
    auto joysticks = Playground::Joystick::enumerate_joysticks();
    std::cout << "Num joysticks found: " << joysticks.size() << std::endl;

    for (int i = 0; i < joysticks.size(); i++) {
        char prod_guid[37];
        char instance_guid[37];
        std::cout
            << i << ":"
            << " Name: " << joysticks[i].name
            << "; Instance Guid: " << Playground::Joystick::guid_to_str(&joysticks[i].instance_guid, instance_guid)
            << std::endl;
    }

    std::cout << "\nSelect Joystick:" << std::endl;
    std::string selectedIndex;
    int index = -1;

    while (index <= -1) {
        std::getline(std::cin, selectedIndex);

        if (!tryParse(selectedIndex, index)) {
            std::cout << "Not a valid number. Select Joystick:" << std::endl;
            index = -1;
        }
        else {
            if (index >= joysticks.size() || index < 0) {
                std::cout << "Not a valid index. Select Joystick:" << std::endl;
                index = -1;
            }
        }
    }

    std::thread jsThread(StartJoystick, std::ref(joysticks[index]));
    std::thread inputThread(thread_input);

    jsThread.join();
    inputThread.join();

    std::cout << "Threads finished" << std::endl;
}