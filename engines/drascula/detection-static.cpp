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

#include "drascula/detection-static.h"

namespace Drascula {

DrasculaMetaEngine::DrasculaMetaEngine() : AdvancedMetaEngine(Drascula::gameDescriptions, sizeof(Drascula::DrasculaGameDescription), drasculaGames) {
    _guiOptions = GUIO1(GUIO_NOMIDI);
}

const char *DrasculaMetaEngine::getEngineId() const {
    return "drascula";
}

const char *DrasculaMetaEngine::getName() const {
    return "Drascula: The Vampire Strikes Back";
}

const char *DrasculaMetaEngine::getOriginalCopyright() const {
    return "Drascula: The Vampire Strikes Back (C) 2000 Alcachofa Soft, (C) 1996 Digital Dreams Multimedia, (C) 1994 Emilio de Paz";
}

bool DrasculaMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportThumbnail) ||
		(f == kSavesSupportCreationDate) ||
		(f == kSavesSupportPlayTime) ||
		(f == kSimpleSavesNames);
}

const ExtraGuiOptions DrasculaMetaEngine::getExtraGuiOptions(const Common::String &target) const {
	ExtraGuiOptions options;
	options.push_back(drasculaExtraGuiOption);
	return options;
}

#include "drascula/saveload.h" // For using "loadMetaData" in the below function(s).

SaveStateList DrasculaMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String pattern = target;
	pattern += ".###";

	filenames = saveFileMan->listSavefiles(pattern);

	SaveStateList saveList;
	int slotNum = 0;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		// Obtain the last 3 digits of the filename, since they correspond to the save slot
		slotNum = atoi(file->c_str() + file->size() - 3);

		if (slotNum >= 0 && slotNum <= getMaximumSaveSlot()) {
			Common::InSaveFile *in = saveFileMan->openForLoading(*file);
			if (in) {
				SaveStateDescriptor desc = Drascula::loadMetaData(in, slotNum, false);
				if (desc.getSaveSlot() != slotNum) {
					// invalid
					delete in;
					continue;
				}
				saveList.push_back(desc);
				delete in;
			}
		}
	}

	// Sort saves based on slot number.
	Common::sort(saveList.begin(), saveList.end(), SaveStateDescriptorSlotComparator());
	return saveList;
}

SaveStateDescriptor DrasculaMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	Common::String fileName = Common::String::format("%s.%03d", target, slot);

	Common::InSaveFile *in = g_system->getSavefileManager()->openForLoading(fileName);

	SaveStateDescriptor desc;
	// Do not allow save slot 0 (used for auto-saving) to be deleted or
	// overwritten.
	desc.setDeletableFlag(slot != 0);
	desc.setWriteProtectedFlag(slot == 0);

	if (in) {
		desc = Drascula::loadMetaData(in, slot, false);
		if (desc.getSaveSlot() != slot) {
			delete in;
			return SaveStateDescriptor();
		}

		Graphics::Surface *thumbnail;
		if (!Graphics::loadThumbnail(*in, thumbnail)) {
			delete in;
			return SaveStateDescriptor();
		}
		desc.setThumbnail(thumbnail);

		delete in;
	}

	return desc;
}

int DrasculaMetaEngine::getMaximumSaveSlot() const { return 999; }

void DrasculaMetaEngine::removeSaveState(const char *target, int slot) const {
	Common::String fileName = Common::String::format("%s.%03d", target, slot);
	g_system->getSavefileManager()->removeSavefile(fileName);
}

} // End of namespace Drascula

