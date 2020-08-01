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

#include "sky/control.h"
#include "sky/sky.h"

#include "base/plugins.h"

#include "common/textconsole.h"

#include "sky/detection-static.h"


bool Sky::SkyEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsReturnToLauncher) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

#if !PLUGIN_ENABLED_DYNAMIC(SKY)
CREATE_INSTANCE_PLUGIN_ME {
#else
Common::Error SkyMetaEngine::createInstance(OSystem *syst, Engine **engine) const {
#endif
	assert(engine);
	*engine = new Sky::SkyEngine(syst);
	return Common::kNoError;
}


#if PLUGIN_ENABLED_DYNAMIC(SKY)
	REGISTER_PLUGIN_ENGINE_DYNAMIC(SKY, PLUGIN_TYPE_ENGINE);
#endif

namespace Sky {
Common::Error SkyEngine::loadGameState(int slot) {
	// We don't need to offset "slot" here. Both loadGameState and quickXRestore
	// are called with the ScummVM Save File Manager's "slot" as argument
	uint16 result = _skyControl->quickXRestore(slot);
	return (result == GAME_RESTORED) ? Common::kNoError : Common::kUnknownError;
}

/**
* Manually saving a game should save it into ScummVM Save File Managers slots 1 or greater.
* ScummVM Save file manager's slot 0 is reserved for the autosave.
* However, natively, the index 0 (_selectedGame) is the first manually saved game.
* @param slot is the save slot on the ScummVM file manager's list
*
*/
Common::Error SkyEngine::saveGameState(int slot, const Common::String &desc, bool isAutosave) {
	// prevent writing to autosave slot when user selects it manually
	// ie. from the ScummVM in-game menu Save feature
	// This also secures _selectedGame which is unsigned integer (uint16)
	// from overflowing in the subtraction below
	if (slot < 0 || (!isAutosave && slot == 0)) {
		return Common::kWritePermissionDenied;
	}
	// Set the save slot and save the game
	// _selectedGame value is one unit lower than the ScummVM's Save File Manager's slot value
	// Note that *_selectedGame* value 0 corresponds to a manually saved game (the first in order)
	//   whereas *slot* value 0 corresponds to the autosave
	if (slot > 0) {
		// We don't care for updating the _selectedGame when slot == 0
		// (in the case of autosave) but we do include the check for slot > 0
		// to guard from overflow, which would be bad practice to allow.
		_skyControl->_selectedGame = slot - 1;
	}
	if (_skyControl->saveGameToFile(false, nullptr, isAutosave) != GAME_SAVED)
		return Common::kWritePermissionDenied;

	// Load current save game descriptions
	Common::StringArray saveGameTexts;
	saveGameTexts.resize(MAX_SAVE_GAMES+1);
	_skyControl->loadDescriptions(saveGameTexts);

	// Update the save game description at the given slot
	if (!isAutosave) {
		saveGameTexts[_skyControl->_selectedGame] = desc;
	}

	// Save the updated descriptions
	_skyControl->saveDescriptions(saveGameTexts);

	return Common::kNoError;
}

bool SkyEngine::canLoadGameStateCurrently() {
	return _systemVars->pastIntro && _skyControl->loadSaveAllowed();
}

bool SkyEngine::canSaveGameStateCurrently() {
	return _systemVars->pastIntro && _skyControl->loadSaveAllowed();
}

} // End of namespace Sky
