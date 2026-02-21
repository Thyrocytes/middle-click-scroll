#ifndef MCS_MOUSE_H
#define MCS_MOUSE_H

enum class MouseDirection : uint8_t {
	DEFAULT = 0, ALL = 1, UP = 2,
	DOWN = 3, LEFT = 4, RIGHT = 5
};

struct Mouse final {
	Mouse() noexcept;

	static Mouse& get() noexcept;
	
	void setCursor(MouseDirection direction) noexcept;
	void resetCursor() noexcept;

	bool isMiddleClicking() noexcept;

	double m_dSpeedMult;
	double m_dMaxSpeed;
	MouseDirection m_eCurrentDir;
private:
#ifdef GEODE_IS_WINDOWS
	std::array<HCURSOR, 6> m_aMouseCache;
#elif defined(GEODE_IS_MACOS)
	void* iDefaultCursor() noexcept;
	void iSetCursor(void* cursor) noexcept;
	void* iLoadCursor(gd::string const& imagePath32, gd::string const& imagePath64) noexcept;
	std::array<void*, 6> m_aMouseCache;
#endif
};

#endif
