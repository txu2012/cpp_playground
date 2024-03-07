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

#include <thread>
#include <queue>


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
		Joystick();
		~Joystick();

		static std::vector<JoystickInfo> enumerate_joysticks();
		static char* guid_to_str(const GUID* id, char* out);

		void CreateJoystick(JoystickInfo info);
		HRESULT StartAcquiring();
		HRESULT StopAcquiring();
		bool IsAcquiring();

		void GetJoystickState(JoystickState& current_state);
		JoystickState GetNextState();
		size_t States() { return joystickData_.size(); }

		bool connected_ = false;

	private:
		LPDIRECTINPUTDEVICE8 js_ = nullptr;
		LPDIRECTINPUT8 di_ = nullptr;

		HANDLE joystickEvent_;
		std::queue<Playground::JoystickState> joystickData_;
		std::mutex mut_;
		bool acquiring = false;

		std::thread jsThread_;
		void StartJoystick(std::queue<JoystickState>& queue);
	};

}