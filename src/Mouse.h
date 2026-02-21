#ifndef MCS_MOUSE_H
#define MCS_MOUSE_H

enum class MouseDirection : uint8_t {
	DEFAULT = 0, ALL = 1, UP = 2,
	DOWN = 3, LEFT = 4, RIGHT = 5
};

struct Mouse final {
	Mouse();
	
	Mouse(Mouse const&) = delete;
	Mouse& operator=(Mouse const&) = delete;
	Mouse(Mouse&&) = delete;
	Mouse& operator=(Mouse&&) = delete;

	static Mouse& get();
	
	void setCursor(MouseDirection direction);
	void resetCursor();

	void updateInputs();

	double m_dSpeedMult;
	double m_dMaxSpeed;

	bool m_bLeftClick;
	bool m_bRightClick;
	bool m_bMiddleClick;
	bool m_bWindowFocused;
	MouseDirection m_eCurrentDir;
private:
#ifdef GEODE_IS_WINDOWS
	std::array<HCURSOR, 6> m_aMouseCache;
#elif defined(GEODE_IS_MACOS)
	std::array<void*, 6> m_aMouseCache;
#endif
};

#endif