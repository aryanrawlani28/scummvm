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

#if !defined(INCLUDED_FROM_BASE_PLUGINS_H) && !defined(RC_INVOKED)
#error This file may only be included by base/plugins.h or dists/scummvm.rc
#endif

// plugin macros are defined in this simple internal header so that scummvm.rc
//  can include them without causing problems for Windows resource compilers.

#define STATIC_PLUGIN 1
#define DYNAMIC_PLUGIN 2

#define PLUGIN_ENABLED_STATIC(ID) \
	(ENABLE_##ID && !PLUGIN_ENABLED_DYNAMIC(ID))

#ifdef DYNAMIC_MODULES
	#define PLUGIN_ENABLED_DYNAMIC(ID) \
		(ENABLE_##ID && (ENABLE_##ID == DYNAMIC_PLUGIN))
#else
	#define PLUGIN_ENABLED_DYNAMIC(ID) 0
#endif

#define PLUGIN_ENABLED(ID) (ENABLE_##ID)
