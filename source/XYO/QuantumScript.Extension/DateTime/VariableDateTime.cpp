//
// Quantum Script Extension DateTime
//
// Copyright (c) 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript.Extension/DateTime/Context.hpp>
#include <XYO/QuantumScript.Extension/DateTime/VariableDateTime.hpp>

namespace XYO::QuantumScript::Extension::DateTime {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableDateTime, "{18EC322A-3BE8-4107-8D89-65C24C7E1561}");
	const char *VariableDateTime::strTypeDateTime = "DateTime";

	VariableDateTime::VariableDateTime() {
		XYO_DYNAMIC_TYPE_PUSH(VariableDateTime);
	};

	String VariableDateTime::getVariableType() {
		return strTypeDateTime;
	};

	Variable *VariableDateTime::newVariable() {
		return (Variable *)TMemory<VariableDateTime>::newMemory();
	};

	Variable *VariableDateTime::instancePrototype() {
		return (Extension::DateTime::getContext())->prototypeDateTime->prototype;
	};

	Variable *VariableDateTime::clone(SymbolList &inSymbolList) {
		VariableDateTime *out = (VariableDateTime *)newVariable();
		out->value.copy(value);
		return out;
	};

	bool VariableDateTime::toBoolean() {
		return true;
	};

	String VariableDateTime::toString() {
		return strTypeDateTime;
	};

};
