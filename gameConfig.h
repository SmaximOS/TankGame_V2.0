#ifndef __GameConfig_h
#define __GameConfig_h

class GameConfig
{
public:
	static constexpr int WIDTH = 80;
	static constexpr int HEIGHT = 25;
	static constexpr int MINX = 0;
	static constexpr int MINY = 0;
	enum class eKeys {
		LEFT_FORWARD_P1 = 'q', LEFT_FORWARDCAP_P1 = 'Q', LEFT_BACKWARDS_P1 = 'a', LEFT_BACKWARDSCAP_P1 = 'A',
		RIGHT_FORWARD_P1 = 'e', RIGHT_FORWARDCAP_P1 = 'E', RIGHT_BACKWARDS_P1 = 'd', RIGHT_BACKWARDSCAP_P1 = 'D', STAY_P1 = 's', STAYCAP_P1 = 'S',
		SHOOT_P1 = 'w', SHOOTCAP_P1 = 'W',
		LEFT_FORWARD_P2 = 'u', LEFT_FORWARDCAP_P2 = 'U', LEFT_BACKWARDS_P2 = 'j', LEFT_BACKWARDSCAP_P2 = 'J',
		RIGHT_FORWARD_P2 = 'o', RIGHT_FORWARDCAP_P2 = 'O', RIGHT_BACKWARDS_P2 = 'l', RIGHT_BACKWARDSCAP_P2 = 'L', STAY_P2 = 'k', STAYCAP_P2 = 'K',
		SHOOT_P2 = 'i', SHOOTCAP_P2 = 'I', ESC = 27, RETURN1 = 'x', RETURN2 = 'X'

	};
	enum class cannonDir { NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST };
};

const int BOARD_ROWS = GameConfig::HEIGHT;
const int BOARD_COLS = GameConfig::WIDTH;

#endif