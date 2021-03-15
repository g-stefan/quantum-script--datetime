//
// Quantum Script Extension DateTime
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_HPP
#define QUANTUM_SCRIPT_EXTENSION_DATETIME_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME__EXPORT_HPP
#include "quantum-script-extension-datetime--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_COPYRIGHT_HPP
#include "quantum-script-extension-datetime-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_LICENSE_HPP
#include "quantum-script-extension-datetime-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_VERSION_HPP
#include "quantum-script-extension-datetime-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace DateTime {

				using namespace XYO;
				using namespace Quantum::Script;

				class DateTimeContext:
					public Object {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(DateTimeContext);
					public:

						Symbol symbolFunctionDateTime;
						TPointerX<Prototype> prototypeDateTime;

						QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT DateTimeContext();
				};

				QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT DateTimeContext *getContext();

				QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif

