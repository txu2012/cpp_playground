#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <oleauto.h>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <iostream>

namespace Playground {

	struct JoystickInfo {
		int id;
		std::string name;
		GUID product_guid;
		GUID instance_guid;
	};

	struct JoystickState {
		LONG PositionX = 0;
		LONG PositionY = 0;
		LONG PositionZ = 0;

		BYTE Buttons[128] = {};
		bool ButtonPressed = false;
	};

	class Joystick
	{
	public:
		Joystick(JoystickInfo info, HANDLE stateChange);
		~Joystick();

		static std::vector<JoystickInfo> enumerate_joysticks();
		
		HRESULT StartAcquiring();
		void GetJoystickState(JoystickState& current_state);

		bool connected_ = false;

	private:
		GUID guid_;
		LPDIRECTINPUTDEVICE8 js_ = nullptr;

		//HRESULT hr_;
		std::vector<JoystickInfo> joysticks_;
		LPDIRECTINPUT8 di_ = nullptr;
	};

}