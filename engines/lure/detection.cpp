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

namespace Lure {

uint32 LureEngine::getFeatures() const { return _gameDescription->features; }
Common::Language LureEngine::getLanguage() const { return _gameDescription->desc.language; }
Common::Platform LureEngine::getPlatform() const { return _gameDescription->desc.platform; }

LureLanguage LureEngine::getLureLanguage() const {
	switch (_gameDescription->desc.language) {
	case Common::IT_ITA: return LANG_IT_ITA;
	case Common::FR_FRA: return LANG_FR_FRA;
	case Common::DE_DEU: return LANG_DE_DEU;
	case Common::ES_ESP: return LANG_ES_ESP;
	case Common::RU_RUS: return LANG_RU_RUS;
	case Common::EN_ANY: return LANG_EN_ANY;
	case Common::UNK_LANG: return LANG_UNKNOWN;
	default:
		error("Unknown game language");
	}
}

} // End of namespace Lure

bool Lure::LureEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsReturnToLauncher) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

#if PLUGIN_ENABLED_DYNAMIC(LURE)
extern "C" PLUGIN_EXPORT bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) {
#else
bool LureMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
#endif
	const Lure::LureGameDescription *gd = (const Lure::LureGameDescription *)desc;
	if (gd) {
		*engine = new Lure::LureEngine(syst, gd);
	}
	return gd != 0;
}

#if PLUGIN_ENABLED_DYNAMIC(LURE)
	REGISTER_PLUGIN_ENGINE_DYNAMIC(LURE, PLUGIN_TYPE_ENGINE);
#endif
