#pragma once
#include "Joystick.h"
using namespace Playground;

namespace Gui {

	class CppPresenter
	{
	public:
		CppPresenter();
		~CppPresenter();

		std::vector<Playground::JoystickInfo> GetJoystickList();
		void ConnectJoystick(int index);
		JoystickState GetJoystickResponse();

	private:
		std::vector<JoystickInfo> joysticks_;
		std::shared_ptr<Joystick> js_;
	};

}

