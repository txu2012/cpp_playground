#include "Joystick.h"
#include "atlstr.h"
#define DIRECTINPUT_VERSION 0x0800
#pragma warning(disable:4996)

LPDIRECTINPUT8 di = nullptr;
std::vector<Playground::JoystickInfo> joysticks;

namespace Playground {
	char* Joystick::guid_to_str(const GUID* id, char* out) {
		int i;
		char* ret = out;
		out += sprintf(out, "%.8lX-%.4hX-%.4hX-", id->Data1, id->Data2, id->Data3);
		for (i = 0; i < sizeof(id->Data4); ++i) {
			out += sprintf(out, "%.2hhX", id->Data4[i]);
			if (i == 1) *(out++) = '-';
		}
		return ret;
	}

	Joystick::Joystick() {
	}
	Joystick::~Joystick() { 
		if (jsThread_.joinable()) jsThread_.join(); 
	}

	void Joystick::CreateJoystick(JoystickInfo info) {
		try {
			HRESULT hr;
			// Create Object
			if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, NULL))) {
				throw std::runtime_error("Device Failed to be created");
			};

			// Create device with joystick GUID
			if (FAILED(hr = di->CreateDevice(info.instance_guid, &js_, NULL))) {
				throw std::runtime_error("Joystick Failed to be created with guid.");
			}

			// Set Format
			if (FAILED(hr = js_->SetDataFormat(&c_dfDIJoystick2))) {
				throw std::runtime_error("Failed to set data format.");
			}

			// Set event to joystick
			joystickEvent_ = CreateEventA(NULL, false, false, "JoystickStateChange");
			hr = js_->SetEventNotification(joystickEvent_);

			connected_ = true;

			std::cout << "Joystick created." << std::endl;
		}
		catch (std::exception& ex) {
			std::cout << "Exception in joystick. \n" << ex.what() << std::endl;
		}
	}

	void Joystick::StartJoystick(std::queue<JoystickState> &queue) {
		while (acquiring) {
			auto dwResult = WaitForSingleObject(joystickEvent_, 0);
			if (dwResult == WAIT_OBJECT_0) {
				// Event is set. If the event was created as 
				// autoreset, it has also been reset. 
				JoystickState current_state;
				GetJoystickState(current_state);

				if (current_state.Buttons[11] & 0x80) {
					std::cout << "Exiting joystick thread." << std::endl;
					break;
				}

				queue.push(current_state);
			}
		}

		StopAcquiring();
	}

	HRESULT Joystick::StartAcquiring() {
		HRESULT hr = js_->Acquire();

		while (hr == DIERR_INPUTLOST)
			hr = js_->Acquire();

		std::cout << "Acquiring Joystick Started." << std::endl;

		acquiring = true;
		jsThread_ = std::thread(&Joystick::StartJoystick, this, std::ref(joystickData_));

		return hr;
	}

	HRESULT Joystick::StopAcquiring() {
		HRESULT hr = js_->Unacquire();
		std::cout << "Acquiring Joystick Stopped." << std::endl;
		acquiring = false;

		return hr;
	}

	bool Joystick::IsAcquiring() {
		return acquiring;
	}

	JoystickState Joystick::GetNextState() {
		JoystickState state;
		state = joystickData_.front();
		joystickData_.pop();

		return state;
	}

	void Joystick::GetJoystickState(JoystickState &current_state) {
		if (!js_) {
			std::cout << "No Joystick Found." << std::endl;

			return;
		}

		DIJOYSTATE2 state;

		// Poll the device to get information
		HRESULT hr = js_->Poll();
		if (FAILED(hr)) {
			StartAcquiring();
			return;
		}

		if (FAILED(hr = js_->GetDeviceState(sizeof(DIJOYSTATE2), &state)))
			return;

		current_state.PositionX = state.lX;
		current_state.PositionY = state.lY;
		current_state.PositionZ = state.lRz;

		current_state.ButtonPressed = false;
		for (int i = 0; i < 128; i++) {
			if (state.rgbButtons[i] & 0x80) current_state.ButtonPressed = true;
			current_state.Buttons[i] = state.rgbButtons[i];
		}
	}

	std::vector<JoystickInfo> Joystick::enumerate_joysticks() {
		joysticks = {};

		HRESULT hr;

		hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, NULL);
		hr = di->EnumDevices(DI8DEVCLASS_GAMECTRL,
			[](const DIDEVICEINSTANCE* instance, VOID* context)
			{
				HRESULT hr;
				LPDIRECTINPUTDEVICE8 joystick = nullptr;

				hr = di->CreateDevice(instance->guidInstance, &joystick, NULL);
				if (FAILED(hr)) {
					return DIENUM_CONTINUE;
				}

				DIDEVICEINSTANCE instance2;
				instance2.dwSize = sizeof(DIDEVICEINSTANCE);
				hr = joystick->GetDeviceInfo(&instance2);

				std::wstring name(instance2.tszProductName);

				JoystickInfo info;
				info.name = std::string(name.begin(), name.end());
				info.product_guid = instance2.guidProduct;
				info.instance_guid = instance2.guidInstance;

				joysticks.push_back(info);

				return DIENUM_STOP;
			} , NULL, DIEDFL_ATTACHEDONLY);

		di->Release();		

		return joysticks;
	}
}