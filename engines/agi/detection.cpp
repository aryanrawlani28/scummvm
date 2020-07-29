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

#include "agi/detection-static.h"

#include "agi/agi.h"
#include "agi/preagi.h"
#include "agi/preagi_mickey.h"
#include "agi/preagi_troll.h"
#include "agi/preagi_winnie.h"


namespace Agi {

uint32 AgiBase::getGameID() const {
	return _gameDescription->gameID;
}

uint32 AgiBase::getFeatures() const {
	return _gameFeatures;
}

Common::Platform AgiBase::getPlatform() const {
	return _gameDescription->desc.platform;
}

Common::Language AgiBase::getLanguage() const {
	return _gameDescription->desc.language;
}

uint16 AgiBase::getVersion() const {
	return _gameVersion;
}

uint16 AgiBase::getGameType() const {
	return _gameDescription->gameType;
}

const char *AgiBase::getGameMD5() const {
	return _gameDescription->desc.filesDescriptions[0].md5;
}

void AgiBase::initFeatures() {
	_gameFeatures = _gameDescription->features;
}

void AgiBase::setFeature(uint32 feature) {
	_gameFeatures |= feature;
}

void AgiBase::setVersion(uint16 version) {
	_gameVersion = version;
}

void AgiBase::initVersion() {
	_gameVersion = _gameDescription->version;
}

const char *AgiBase::getDiskName(uint16 id) {
	for (int i = 0; _gameDescription->desc.filesDescriptions[i].fileName != NULL; i++)
		if (_gameDescription->desc.filesDescriptions[i].fileType == id)
			return _gameDescription->desc.filesDescriptions[i].fileName;

	return "";
}

} // End of namespace Agi

bool Agi::AgiBase::hasFeature(EngineFeature f) const {
	return
	    (f == kSupportsReturnToLauncher) ||
	    (f == kSupportsLoadingDuringRuntime) ||
	    (f == kSupportsSavingDuringRuntime);
}


#if PLUGIN_ENABLED_DYNAMIC(AGI)
extern "C" PLUGIN_EXPORT bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) {
#else
bool Agi::AgiMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
#endif
	const Agi::AGIGameDescription *gd = (const Agi::AGIGameDescription *)desc;
	bool res = true;

	switch (gd->gameType) {
	case Agi::GType_PreAGI:
		switch (gd->gameID) {
		case GID_MICKEY:
			*engine = new Agi::MickeyEngine(syst, gd);
			break;
		case GID_TROLL:
			*engine = new Agi::TrollEngine(syst, gd);
			break;
		case GID_WINNIE:
			*engine = new Agi::WinnieEngine(syst, gd);
			break;
		default:
			res = false;
			error("PreAGI engine: unknown gameID");
			break;
		}
		break;
	case Agi::GType_V1:
	case Agi::GType_V2:
	case Agi::GType_V3:
		*engine = new Agi::AgiEngine(syst, gd);
		break;
	default:
		res = false;
		error("AGI engine: unknown gameType");
	}

	return res;
}

#if PLUGIN_ENABLED_DYNAMIC(AGI)
	REGISTER_PLUGIN_ENGINE_DYNAMIC(AGI, PLUGIN_TYPE_ENGINE);
#endif

namespace Agi {

bool AgiBase::canLoadGameStateCurrently() {
	if (!(getGameType() == GType_PreAGI)) {
		if (getFlag(VM_FLAG_MENUS_ACCESSIBLE)) {
			if (!_noSaveLoadAllowed) {
				if (!cycleInnerLoopIsActive()) {
					// We can't allow to restore a game, while inner loop is active
					// For example Mixed Up Mother Goose has an endless loop for user name input
					// Which means even if we abort the inner loop, the game would keep on calling
					// GetString() until something is entered. And this would of course also happen
					// right after restoring a saved game.
					return true;
				}
			}
		}
	}
	return false;
}

bool AgiBase::canSaveGameStateCurrently() {
	if (getGameID() == GID_BC) // Technically in Black Cauldron we may save anytime
		return true;

	if (!(getGameType() == GType_PreAGI)) {
		if (getFlag(VM_FLAG_MENUS_ACCESSIBLE)) {
			if (!_noSaveLoadAllowed) {
				if (!cycleInnerLoopIsActive()) {
					if (promptIsEnabled()) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

int AgiEngine::agiDetectGame() {
	int ec = errOK;

	assert(_gameDescription != NULL);

	if (getVersion() <= 0x2001) {
		_loader = new AgiLoader_v1(this);
	} else if (getVersion() <= 0x2999) {
		_loader = new AgiLoader_v2(this);
	} else {
		_loader = new AgiLoader_v3(this);
	}
	ec = _loader->detectGame();

	return ec;
}

} // End of namespace Agi
