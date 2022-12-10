// Quantum Script Extension DateTime
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_VARIABLEDATETIME_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_VARIABLEDATETIME_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/DateTime/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::DateTime {

	class VariableDateTime;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::DateTime::VariableDateTime> : public TMemoryPoolActive<XYO::QuantumScript::Extension::DateTime::VariableDateTime> {};
};

namespace XYO::QuantumScript::Extension::DateTime {

	class VariableDateTime : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableDateTime);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT, VariableDateTime);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT static const char *strTypeDateTime;

		public:
			XYO::System::DateTime value;

			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT VariableDateTime();

			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT String toString();
	};

};

#endif
