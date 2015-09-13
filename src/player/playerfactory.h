#include "player_export.h"

LRCX_BEGIN_NS

class Player;

class PLAYER_EXPORT PlayerFactory
{
public:
	static std::vector<std::string> keys();
	static std::unique_ptr<Player> create(const std::string &key);
};

LRCX_END_NS
