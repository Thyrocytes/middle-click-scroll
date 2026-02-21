#include "Mouse.h"

#ifdef GEODE_IS_WINDOWS
	#include <windows.h>
#elif defined(GEODE_IS_MACOS)
	#include <CoreGraphics/CGEventSource.h>
#endif

Mouse::Mouse() {
#ifdef GEODE_IS_WINDOWS
	m_aMouseCache[0] = LoadCursor(NULL, IDC_ARROW);
	m_aMouseCache[1] = LoadCursor(NULL, MAKEINTRESOURCE(32654));
	m_aMouseCache[2] = LoadCursor(NULL, MAKEINTRESOURCE(32655));
	m_aMouseCache[3] = LoadCursor(NULL, MAKEINTRESOURCE(32656));
	m_aMouseCache[4] = LoadCursor(NULL, MAKEINTRESOURCE(32658));
	m_aMouseCache[5] = LoadCursor(NULL, MAKEINTRESOURCE(32657));
#elif defined(GEODE_IS_MACOS)
	auto res = geode::Mod::get()->getResourcesDir();
	m_aMouseCache[0] = iDefaultCursor();
	m_aMouseCache[1] = iLoadCursor(res / "all.png", res / "all_64.png");
	m_aMouseCache[2] = iLoadCursor(res / "up.png", res / "up_64.png");
	m_aMouseCache[3] = iLoadCursor(res / "down.png", res / "down_64.png");
	m_aMouseCache[4] = iLoadCursor(res / "left.png", res / "left_64.png");
	m_aMouseCache[5] = iLoadCursor(res / "right.png", res / "right_64.png");
#endif
}

Mouse& Mouse::get() {
	static Mouse mouse;
	return mouse;
}

void Mouse::setCursor(MouseDirection direction) {
#ifdef GEODE_IS_WINDOWS
	SetCursor(m_aMouseCache[static_cast<int>(direction)]);
#elif defined(GEODE_IS_MACOS)
	iSetCursor(m_aMouseCache[static_cast<int>(direction)]);
#endif
}

void Mouse::resetCursor() {
#ifdef GEODE_IS_WINDOWS
	SetCursor(m_aMouseCache[0]);
#elif defined(GEODE_IS_MACOS)
	iSetCursor(m_aMouseCache[0]);
#endif
}

void Mouse::updateInputs() {
#ifdef GEODE_IS_WINDOWS
	m_bMiddleClick = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	m_bLeftClick = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	m_bRightClick = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
#elif defined(GEODE_IS_MACOS)
	m_bMiddleClick = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter);
	m_bLeftClick = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonLeft);
	m_bRightClick = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonRight);
#endif
}