// Quantum Script Extension Buffer
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/DateTime/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::DateTime {

	class DateTimeContext : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(DateTimeContext);

		public:
			Symbol symbolFunctionDateTime;
			TPointerX<Prototype> prototypeDateTime;

			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT DateTimeContext();
	};

	XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT DateTimeContext *getContext();

};

#endif
