// Quantum Script Extension DateTime
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/DateTime/Library.hpp>
#include <XYO/QuantumScript.Extension/DateTime/Copyright.hpp>
#include <XYO/QuantumScript.Extension/DateTime/License.hpp>
#include <XYO/QuantumScript.Extension/DateTime/Version.hpp>
#include <XYO/QuantumScript.Extension/DateTime/Context.hpp>
#include <XYO/QuantumScript.Extension/DateTime/VariableDateTime.hpp>

namespace XYO::QuantumScript::Extension::DateTime {

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
		(Context::getGlobalObject())->setPropertyBySymbol(dateTimeContext->symbolFunctionDateTime, defaultPrototypeFunction);
		dateTimeContext->prototypeDateTime = defaultPrototypeFunction->prototype;
	};

	static TPointer<Variable> getYear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-year\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getYear());
	};

	static TPointer<Variable> getMonth(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-month\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getMonth());
	};

	static TPointer<Variable> getDay(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-day\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getDay());
	};

	static TPointer<Variable> getDayOfWeek(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-day-of-week\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getDayOfWeek());
	};

	static TPointer<Variable> getHour(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-hour\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getHour());
	};

	static TPointer<Variable> getMinute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-minute\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getMinute());
	};

	static TPointer<Variable> getSecond(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-second\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getSecond());
	};

	static TPointer<Variable> getMilliseconds(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-get-miliseconds\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.getMilliseconds());
	};

	static TPointer<Variable> setYear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-year\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setYear((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> setMonth(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-month\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setMonth((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> setDay(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-day\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setDay((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> setDayOfWeek(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-day-of-week\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setDayOfWeek((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> setHour(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-hour\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setHour((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> setMinute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-minute\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setMinute((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> setSecond(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-second\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setSecond((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> setMilliseconds(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-set-miliseconds\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.setMilliseconds((uint16_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> toUnixTime(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-to-unix-time\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableDateTime *)this_)->value.toUnixTime());
	};

	static TPointer<Variable> fromUnixTime(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-from-unix-time\n");
#endif
		if (!TIsType<VariableDateTime>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableDateTime *)this_)->value.fromUnixTime((uint64_t)value);

		return VariableBoolean::newVariable(true);
	};

	static TPointer<Variable> timestampInMilliseconds(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- datetime-timestamp-in-milliseconds\n");
#endif

		return VariableNumber::newVariable(XYO::System::DateTime::timestampInMilliseconds());
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("DateTime", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "DateTime\r\n";
		info << License::shortLicense();

		executive->setExtensionName(extensionId, "DateTime");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::DateTime::Version::versionWithBuild());
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
		executive->setFunction2("DateTime.timestampInMilliseconds()", timestampInMilliseconds);
	};

};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::DateTime::initExecutive(executive, extensionId);
};
#endif
