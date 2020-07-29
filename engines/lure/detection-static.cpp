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

#include "lure/detection-static.h"

LureMetaEngine::LureMetaEngine() : AdvancedMetaEngine(Lure::gameDescriptions, sizeof(Lure::LureGameDescription), lureGames
#ifdef USE_TTS
        , optionsList
#endif
        ) {
    _md5Bytes = 1024;

    // Use kADFlagUseExtraAsHint to distinguish between EGA and VGA versions
    // of italian Lure when their datafiles sit in the same directory.
    _flags = kADFlagUseExtraAsHint;
    _guiOptions = GUIO1(GUIO_NOSPEECH);
}

const char *LureMetaEngine::getEngineId() const {
    return "lure";
}

const char *LureMetaEngine::getName() const {
    return "Lure of the Temptress";
}

const char *LureMetaEngine::getOriginalCopyright() const {
    return "Lure of the Temptress (C) Revolution";
}

bool LureMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave);
}

int LureMetaEngine::getMaximumSaveSlot() const { return 999; }

void LureMetaEngine::removeSaveState(const char *target, int slot) const {
	Common::String filename = target;
	filename += Common::String::format(".%03d", slot);

	g_system->getSavefileManager()->removeSavefile(filename);
}

SaveStateList LureMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String saveDesc;
	Common::String pattern = "lure.###";

	filenames = saveFileMan->listSavefiles(pattern);

	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		// Obtain the last 3 digits of the filename, since they correspond to the save slot
		int slotNum = atoi(file->c_str() + file->size() - 3);

		if (slotNum >= 0 && slotNum <= 999) {
			Common::InSaveFile *in = saveFileMan->openForLoading(*file);
			if (in) {
				saveDesc = Lure::getSaveName(in);
				saveList.push_back(SaveStateDescriptor(slotNum, saveDesc));
				delete in;
			}
		}
	}

	// Sort saves based on slot number.
	Common::sort(saveList.begin(), saveList.end(), SaveStateDescriptorSlotComparator());
	return saveList;
}
