/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

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
