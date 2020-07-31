/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "plumbers/plumbers.h"

#include "plumbers/detection.h"

namespace Plumbers {
const char *PlumbersGame::getGameId() const { return _gameDescription->gameId; }
Common::Platform PlumbersGame::getPlatform() const { return _gameDescription->platform; }
}

#if PLUGIN_ENABLED_DYNAMIC(PLUMBERS)
extern "C" PLUGIN_EXPORT bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) {
#else
bool PlumbersMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
#endif
	if (desc)
		*engine = new Plumbers::PlumbersGame(syst, desc);

	return desc != nullptr;
}

#if PLUGIN_ENABLED_DYNAMIC(PLUMBERS)
REGISTER_PLUGIN_ENGINE_DYNAMIC(PLUMBERS, PLUGIN_TYPE_ENGINE);
#endif
