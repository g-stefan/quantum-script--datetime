//
// Quantum Script Extension DateTime
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_VARIABLEDATETIME_HPP
#define QUANTUM_SCRIPT_EXTENSION_DATETIME_VARIABLEDATETIME_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace DateTime {

				class VariableDateTime;

			};
		};
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::Extension::DateTime::VariableDateTime> : public TMemoryPoolActive<Quantum::Script::Extension::DateTime::VariableDateTime> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace DateTime {

				using namespace XYO;
				using namespace Quantum::Script;

				class VariableDateTime : public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableDateTime);
						XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT, VariableDateTime);

					protected:
						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT static const char *strTypeDateTime;

					public:
						XYO::DateTime value;

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT VariableDateTime();

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT String getVariableType();

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT Variable *clone(SymbolList &inSymbolList);

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT String toString();
				};

			};
		};
	};
};

#endif
