// Quantum Script Extension DateTime
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_VERSION_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_VERSION_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/DateTime/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::DateTime::Version {

	XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT const char *version();
	XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT const char *build();
	XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT const char *versionWithBuild();
	XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT const char *datetime();

};

#endif
