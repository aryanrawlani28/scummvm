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

#include "base/plugins.h"

#include "engines/advancedDetector.h"
#include "common/file.h"

#include "graphics/surface.h"

#include "fullpipe/fullpipe.h"
#include "fullpipe/gameloader.h"

namespace Fullpipe {

uint32 FullpipeEngine::getFeatures() const {
	return _gameDescription->flags;
}

bool FullpipeEngine::isDemo() {
	return _gameDescription->flags & ADGF_DEMO;
}

Common::Language FullpipeEngine::getLanguage() const {
	return _gameDescription->language;
}

} // End of namspace Fullpipe

class FullpipeMetaEngine : public AdvancedMetaEngine {
public:
	const char *getName() const override {
		return "fullpipe";
	}

	bool hasFeature(MetaEngineFeature f) const override;

	int getMaximumSaveSlot() const override { return 99; }
	SaveStateList listSaves(const char *target) const override;
	void removeSaveState(const char *target, int slot) const override;
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const override;

	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const override;
};

bool FullpipeMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves) ||
		(f == kSupportsDeleteSave) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportThumbnail) ||
		(f == kSavesSupportCreationDate) ||
		(f == kSavesSupportPlayTime) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSimpleSavesNames);
}

bool Fullpipe::FullpipeEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsReturnToLauncher) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

SaveStateList FullpipeMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String pattern("fullpipe.s##");

	filenames = saveFileMan->listSavefiles(pattern);

	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		// Obtain the last 2 digits of the filename, since they correspond to the save slot
		int slotNum = atoi(file->c_str() + file->size() - 2);

		if (slotNum >= 0 && slotNum <= getMaximumSaveSlot()) {
			Common::ScopedPtr<Common::InSaveFile> in(saveFileMan->openForLoading(*file));
			if (in) {
				Fullpipe::FullpipeSavegameHeader header;
				if (!Fullpipe::readSavegameHeader(in.get(), header)) {
					continue;
				}

				SaveStateDescriptor desc;

				Fullpipe::parseSavegameHeader(header, desc);

				desc.setSaveSlot(slotNum);

				saveList.push_back(desc);
			}
		}
	}

	// Sort saves based on slot number.
	Common::sort(saveList.begin(), saveList.end(), SaveStateDescriptorSlotComparator());
	return saveList;
}

void FullpipeMetaEngine::removeSaveState(const char *target, int slot) const {
	g_system->getSavefileManager()->removeSavefile(Fullpipe::getSavegameFile(slot));
}

SaveStateDescriptor FullpipeMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	Common::ScopedPtr<Common::InSaveFile> f(g_system->getSavefileManager()->openForLoading(
		Fullpipe::getSavegameFile(slot)));

	if (f) {
		Fullpipe::FullpipeSavegameHeader header;
		if (!Fullpipe::readSavegameHeader(f.get(), header, false)) {
			return SaveStateDescriptor();
		}

		// Create the return descriptor
		SaveStateDescriptor desc;

		Fullpipe::parseSavegameHeader(header, desc);

		desc.setSaveSlot(slot);
		desc.setThumbnail(header.thumbnail);

		return desc;
	}

	return SaveStateDescriptor();
}

bool FullpipeMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	if (desc) {
		*engine = new Fullpipe::FullpipeEngine(syst, desc);
	}
	return desc != 0;
}

#if PLUGIN_ENABLED_DYNAMIC(FULLPIPE)
	REGISTER_PLUGIN_DYNAMIC(FULLPIPE, PLUGIN_TYPE_ENGINE, FullpipeMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(FULLPIPE, PLUGIN_TYPE_ENGINE, FullpipeMetaEngine);
#endif
