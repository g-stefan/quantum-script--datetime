//
// Quantum Script Extension DateTime
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-datetime-license.hpp"
#include "quantum-script-extension-datetime.hpp"
#include "quantum-script-extension-datetime-variabledatetime.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_NO_VERSION
#include "quantum-script-extension-datetime-version.hpp"
#endif

#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"

#include "quantum-script-variablestring.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace DateTime {

				using namespace XYO;
				using namespace Quantum::Script;

				DateTimeContext::DateTimeContext() {
					symbolFunctionDateTime = 0;
					prototypeDateTime.pointerLink(this);
				};

				DateTimeContext *getContext() {
					return TSingleton<DateTimeContext>::getValue();
				};

				static TPointer<Variable> functionDateTime(VariableFunction *function, Variable *this_, VariableArray *arguments) {
					return VariableDateTime::newVariable();
				};

				static void deleteContext() {
					DateTimeContext *dateTimeContext = getContext();
					dateTimeContext->prototypeDateTime.deleteMemory();
					dateTimeContext->symbolFunctionDateTime = 0;
				};

				static void newContext(Executive *executive, void *extensionId) {
					VariableFunction *defaultPrototypeFunction;

					DateTimeContext *dateTimeContext = getContext();
					executive->setExtensionDeleteContext(extensionId, deleteContext);

					dateTimeContext->symbolFunctionDateTime = Context::getSymbol("DateTime");
					dateTimeContext->prototypeDateTime.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionDateTime, nullptr, nullptr);
					((Context::getGlobalObject())->operatorReferenceOwnProperty(dateTimeContext->symbolFunctionDateTime))=defaultPrototypeFunction;
					dateTimeContext->prototypeDateTime = defaultPrototypeFunction->prototype;
				};


				static TPointer<Variable> getYear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-year\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getYear());
				};

				static TPointer<Variable> getMonth(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-month\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getMonth());
				};

				static TPointer<Variable> getDay(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-day\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getDay());
				};

				static TPointer<Variable> getDayOfWeek(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-day-of-week\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getDayOfWeek());
				};

				static TPointer<Variable> getHour(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-hour\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getHour());
				};

				static TPointer<Variable> getMinute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-minute\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getMinute());
				};

				static TPointer<Variable> getSecond(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-second\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getSecond());
				};

				static TPointer<Variable> getMilliseconds(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-get-miliseconds\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getMilliseconds());
				};

				static TPointer<Variable> setYear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-year\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setYear((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> setMonth(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-month\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setMonth((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> setDay(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-day\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setDay((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> setDayOfWeek(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-day-of-week\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setDayOfWeek((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> setHour(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-hour\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setHour((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> setMinute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-minute\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setMinute((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> setSecond(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-second\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setSecond((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> setMilliseconds(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-set-miliseconds\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.setMilliseconds((uint16_t)value);

					return VariableBoolean::newVariable(true);
				};


				static TPointer<Variable> toUnixTime(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-to-unix-time\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableDateTime *)this_)->value.toUnixTime());
				};

				static TPointer<Variable> fromUnixTime(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-from-unix-time\n");
#endif
					if(!VariableDateTime::isVariableDateTime(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};

					((VariableDateTime *)this_)->value.fromUnixTime((uint64_t)value);

					return VariableBoolean::newVariable(true);
				};

				static TPointer<Variable> timestampInMilliseconds(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- datetime-timestamp-in-milliseconds\n");
#endif

					return VariableNumber::newVariable(XYO::System::DateTime::timestampInMilliseconds());
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("DateTime", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "DateTime\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "DateTime");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_DATETIME_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::DateTime::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);

					newContext(executive, extensionId);

					executive->setFunction2("DateTime.prototype.getYear()", getYear);
					executive->setFunction2("DateTime.prototype.getMonth()", getMonth);
					executive->setFunction2("DateTime.prototype.getDay()", getDay);
					executive->setFunction2("DateTime.prototype.getDayOfWeek()", getDayOfWeek);
					executive->setFunction2("DateTime.prototype.getHour()", getHour);
					executive->setFunction2("DateTime.prototype.getMinute()", getMinute);
					executive->setFunction2("DateTime.prototype.getSecond()", getSecond);
					executive->setFunction2("DateTime.prototype.getMilliseconds()", getMilliseconds);
					executive->setFunction2("DateTime.prototype.setYear(x)", setYear);
					executive->setFunction2("DateTime.prototype.setMonth(x)", setMonth);
					executive->setFunction2("DateTime.prototype.setDay(x)", setDay);
					executive->setFunction2("DateTime.prototype.setDayOfWeek(x)", setDayOfWeek);
					executive->setFunction2("DateTime.prototype.setHour(x)", setHour);
					executive->setFunction2("DateTime.prototype.setMinute(x)", setMinute);
					executive->setFunction2("DateTime.prototype.setSecond(x)", setSecond);
					executive->setFunction2("DateTime.prototype.setMilliseconds(x)", setMilliseconds);
					executive->setFunction2("DateTime.prototype.toUnixTime()", toUnixTime);
					executive->setFunction2("DateTime.prototype.fromUnixTime(x)", fromUnixTime);
					executive->setFunction2("DateTime.timestampInMilliseconds(x)", timestampInMilliseconds);

				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_DATETIME_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::DateTime::initExecutive(executive, extensionId);
};
#endif

