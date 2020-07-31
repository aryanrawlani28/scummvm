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

#include "common/algorithm.h"
#include "common/savefile.h"
#include "common/system.h"
#include "common/translation.h"

#include "graphics/thumbnail.h"

#include "engines/advancedDetector.h"

namespace DreamWeb {

struct DreamWebGameDescription {
	ADGameDescription desc;
};

} // End of namespace DreamWeb

class DreamWebMetaEngine : public AdvancedMetaEngine {
public:
	DreamWebMetaEngine();

	const char *getEngineId() const override;

	const char *getName() const override;

	const char *getOriginalCopyright() const override;

#if PLUGIN_ENABLED_STATIC(DREAMWEB)
	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const override;
#endif
	bool hasFeature(MetaEngineFeature f) const override;
	SaveStateList listSaves(const char *target) const override;
	int getMaximumSaveSlot() const override;
	void removeSaveState(const char *target, int slot) const override;
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const override;
};
