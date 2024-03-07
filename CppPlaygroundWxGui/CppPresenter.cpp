#include "CppPresenter.h"
namespace Gui {

	CppPresenter::CppPresenter() {
		joysticks_ = Joystick::enumerate_joysticks();
	}
	CppPresenter::~CppPresenter() {
	}

	std::vector<JoystickInfo> CppPresenter::GetJoystickList() {
		return joysticks_;
	}
	void CppPresenter::ConnectJoystick(int index) {
		js_ = std::make_shared<Joystick>();
		js_->CreateJoystick(joysticks_[index]);
	}

	JoystickState CppPresenter::GetJoystickResponse() {

	}
}