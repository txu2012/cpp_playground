// CppPlayground.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Joystick.h"
#include <thread>
#include <queue>

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

void thread_joystick_process(Playground::Joystick &js) {
    js.StartAcquiring();

    while (js.IsAcquiring()) {
        if (js.States() > 0) {
            auto state = js.GetNextState();

            char strText[512] = {};
            for (int i = 0; i < 128; i++)
            {
                if (state.Buttons[i] & 0x80)
                {
                    char btnState[256];
                    snprintf(btnState, sizeof(btnState), "%02d ", i);
                    strcat_s(strText, btnState);
                }
            }

            std::cout << "Joystick Moved:"
                << " X: " << state.PositionX
                << " Y: " << state.PositionY
                << " Z: " << state.PositionZ
                << " Buttons: " << strText
                << std::endl;

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

int temp()
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
    
    Playground::Joystick js;
    js.CreateJoystick(joysticks[index]);
    
    std::thread jsThread(thread_joystick_process, std::ref(js));
    std::thread inputThread(thread_input);  

    jsThread.join();
    inputThread.join();

    std::cout << "Threads finished" << std::endl;

    return 0;
}