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


#include "dreamweb/dreamweb.h"
#include "dreamweb/structs.h"

#include "detection-static.h"

bool DreamWeb::DreamWebEngine::hasFeature(EngineFeature f) const {
	switch(f) {
	case kSupportsReturnToLauncher:
		return true;
	case kSupportsSubtitleOptions:
		return _gameDescription->desc.flags & ADGF_CD;
	default:
		return false;
	}
	return false;
}

#if PLUGIN_ENABLED_DYNAMIC(DREAMWEB)
extern "C" PLUGIN_EXPORT bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) {
#else
bool DreamWebMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
#endif
	const DreamWeb::DreamWebGameDescription *gd = (const DreamWeb::DreamWebGameDescription *)desc;
	if (gd) {
		*engine = new DreamWeb::DreamWebEngine(syst, gd);
	}
	return gd != 0;
}

#if PLUGIN_ENABLED_DYNAMIC(DREAMWEB)
	REGISTER_PLUGIN_ENGINE_DYNAMIC(DREAMWEB, PLUGIN_TYPE_ENGINE);
#endif

namespace DreamWeb {

Common::Error DreamWebEngine::loadGameState(int slot) {
	return Common::kNoError;
}

Common::Error DreamWebEngine::saveGameState(int slot, const Common::String &desc, bool isAutosave) {
	return Common::kNoError;
}

bool DreamWebEngine::canLoadGameStateCurrently() {
	return false;
}

bool DreamWebEngine::canSaveGameStateCurrently() {
	return false;
}

Common::Language DreamWebEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

bool DreamWebEngine::isCD() {
	return _gameDescription->desc.flags & ADGF_CD;
}

} // End of namespace DreamWeb
