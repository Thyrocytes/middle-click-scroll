#include <Geode/modify/AppDelegate.hpp>
#include "Mouse.h"

static Mouse& mouse = Mouse::get();

struct MCSAppDelegate final : public geode::Modify<MCSAppDelegate, AppDelegate> {
	void applicationDidEnterBackground() noexcept {
		mouse.m_bWindowFocused = false;
		AppDelegate::applicationDidEnterBackground();
	}

	void applicationWillResignActive() noexcept {
		mouse.m_bWindowFocused = false;
		AppDelegate::applicationWillResignActive();
	}

	void applicationWillEnterForeground() noexcept {
		mouse.m_bWindowFocused = true;
		AppDelegate::applicationWillEnterForeground();
	}

	void applicationWillBecomeActive() noexcept {
		mouse.m_bWindowFocused = true;
		AppDelegate::applicationWillBecomeActive();
	}
};	