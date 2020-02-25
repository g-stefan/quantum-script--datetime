//
// Quantum Script Extension DateTime
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_VARIABLEDATETIME_HPP
#define QUANTUM_SCRIPT_EXTENSION_DATETIME_VARIABLEDATETIME_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
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
		template<>
		class TMemory<Quantum::Script::Extension::DateTime::VariableDateTime>:
			public TMemoryPoolActive<Quantum::Script::Extension::DateTime::VariableDateTime> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace DateTime {

				using namespace XYO;
				using namespace Quantum::Script;

				class VariableDateTime :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableDateTime);
					protected:
						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT static const char *strTypeDateTime;
						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT static const char *typeDateTimeKey;
						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT static const void *typeDateTime;
					public:

						XYO::DateTime value;

						inline VariableDateTime() {
							variableType = registerType(typeDateTime, typeDateTimeKey);
						};

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT Variable &operatorReference(Symbol symbolId);

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT Variable *clone(SymbolList &inSymbolList);

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT String toString();

						//
						inline static bool isVariableDateTime(const Variable *value) {
							if(typeDateTime == nullptr) {
								typeDateTime = registerType(typeDateTime, typeDateTimeKey);
							};
							return (value->variableType == typeDateTime);
						};

				};

			};
		};
	};
};


#endif
