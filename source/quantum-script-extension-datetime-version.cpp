//
// Quantum Script Extension DateTime
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "quantum-script-extension-datetime-version.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace DateTime {
				namespace Version {

					static const char *version_ = "2.2.0";
					static const char *build_ = "20";
					static const char *versionWithBuild_ = "2.2.0.20";
					static const char *datetime_ = "2021-08-09 17:38:05";

					const char *version() {
						return version_;
					};
					const char *build() {
						return build_;
					};
					const char *versionWithBuild() {
						return versionWithBuild_;
					};
					const char *datetime() {
						return datetime_;
					};

				};
			};
		};
	};
};



