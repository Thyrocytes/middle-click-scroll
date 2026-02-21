#include <Geode/Geode.hpp>
#include <Geode/modify/CCScrollLayerExt.hpp>
#include <Geode/modify/TableView.hpp>

#include "Mouse.h"

static Mouse& mouse = Mouse::get();

#define settingUpdates(flag, name, type) flag = geode::Mod::get()->getSettingValue<type>(name); \
	geode::listenForSettingChanges<type>(name, [](type newer) { \
		flag = newer; \
	});

$on_mod(Loaded) {
	settingUpdates(mouse.m_dSpeedMult, "scroll-speed-mult", double);
	settingUpdates(mouse.m_dMaxSpeed, "max-scroll-speed", double);
}

// implementation is mostly the same as previous version. could be improved

template <class Derived, class Base>
struct MouseScrollClass : geode::Modify<Derived, Base> {
	struct Fields {
		bool m_bScrolling;
		cocos2d::CCPoint m_pStartPoint;
	};

	void mscUpdate(float) {
		if constexpr (requires { this->m_disableMovement; }) {
			if (this->m_disableMovement) return;
		}

		mouse.updateInputs();

		if (mouse.m_bLeftClick || mouse.m_bRightClick) {
			this->m_fields->m_bScrolling = false;
			mouse.resetCursor();
		}

		auto startPoint = geode::cocos::getMousePos();
		if (mouse.m_bMiddleClick) {
			if (!this->m_fields->m_bScrolling) {
				if (!overlapping(startPoint)) return;
				this->m_fields->m_bScrolling = true;
				this->m_fields->m_pStartPoint = startPoint;
			}
		}
		else {
			this->m_fields->m_bScrolling = false;
			mouse.resetCursor();
		}

		if (this->m_fields->m_bScrolling) {
			auto velocity = (this->m_fields->m_pStartPoint - startPoint) * mouse.m_dSpeedMult;
			velocity.x = velocity.x < -mouse.m_dMaxSpeed ? -mouse.m_dMaxSpeed : velocity.x > mouse.m_dMaxSpeed ? mouse.m_dMaxSpeed : velocity.x;
			velocity.y = velocity.y < -mouse.m_dMaxSpeed ? -mouse.m_dMaxSpeed : velocity.y > mouse.m_dMaxSpeed ? mouse.m_dMaxSpeed : velocity.y;

			if (velocity.y != 0) {
				if constexpr (requires { this->m_disableVertical; }) {
					if (this->m_disableVertical) {
						mouse.setCursor(MouseDirection::ALL);
					}
					else {
						mouse.setCursor(velocity.y > 0 ? MouseDirection::DOWN : velocity.y < 0 ? MouseDirection::UP : MouseDirection::ALL);
					}
				}
				else {
					mouse.setCursor(velocity.y > 0 ? MouseDirection::DOWN : velocity.y < 0 ? MouseDirection::UP : MouseDirection::ALL);
				}
			}
			else {
				mouse.setCursor(MouseDirection::ALL);
			}

			if constexpr (std::is_base_of_v<TableView, Base>) {
				this->scrollWheel(velocity.y, velocity.x);
			}
			else if constexpr (std::is_base_of_v<CCScrollLayerExt, Base>) {
				this->scrollLayer(velocity.y);
			}
		}
	}

	void registerWheel() noexcept {
		this->schedule(schedule_selector(Derived::mscUpdate));
	}

	bool overlapping(cocos2d::CCPoint const& point) noexcept {
		auto target = this->getParent();
		if (!target) target = this;
		return this->boundingBox().containsPoint(target->convertToNodeSpace(point));
	}
};

struct ScrollLayerMouseScroll final : MouseScrollClass<ScrollLayerMouseScroll, CCScrollLayerExt> {
    using MouseScrollClass::MouseScrollClass;
	void registerWithTouchDispatcher() noexcept {
		registerWheel();
		CCScrollLayerExt::registerWithTouchDispatcher();
	}
};

struct TableViewMouseScroll final : MouseScrollClass<TableViewMouseScroll, TableView> {
	using MouseScrollClass::MouseScrollClass;
	void registerWithTouchDispatcher() noexcept {
		if (m_unused2) {
			registerWheel();
		}
		TableView::registerWithTouchDispatcher(); 
	}
};