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

	Joystick::Joystick(JoystickInfo info, HANDLE stateChange) {

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
			hr = js_->SetEventNotification(stateChange);

			connected_ = true;

			std::cout << "Joystick created." << std::endl;
		}
		catch (std::exception &ex) {
			std::cout << "Exception in joystick. \n" << ex.what() << std::endl;
		}
	}
	Joystick::~Joystick() {}

	HRESULT Joystick::StartAcquiring() {
		HRESULT hr = js_->Acquire();

		while (hr == DIERR_INPUTLOST)
			hr = js_->Acquire();

		std::cout << "Acquiring Joystick Started." << std::endl;

		return hr;
	}

	HRESULT Joystick::StopAcquiring() {
		HRESULT hr = js_->Unacquire();
		std::cout << "Acquiring Joystick Stopped." << std::endl;

		return hr;
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

				//OLECHAR* guidString;
				//hr = StringFromCLSID(instance2.guidProduct, &guidString);				

				std::wstring name(instance2.tszProductName);

				JoystickInfo info;
				info.name = std::string(name.begin(), name.end());
				info.product_guid = instance2.guidProduct;
				info.instance_guid = instance2.guidInstance;

				joysticks.push_back(info);

				/*char prod_guid[37];
				char instance_guid[37];
				Joystick::guid_to_str(&info.product_guid, prod_guid);
				Joystick::guid_to_str(&info.instance_guid, instance_guid);
				std::cout << "Name: " << info.name
						  << "\nproduct guid: " << prod_guid
						  << "\ninstance guid: " << instance_guid << std::endl;*/

				//::CoTaskMemFree(guidString);

				return DIENUM_STOP;
			} , NULL, DIEDFL_ATTACHEDONLY);

		di->Release();		

		return joysticks;
	}
}