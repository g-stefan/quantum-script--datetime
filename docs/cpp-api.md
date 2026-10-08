# C++ API

For hosts that embed Quantum Script and for maintainers of the extension.
Read the `quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first: native functions, `Variable`,
`Prototype` and `TPointer` work the same way here.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/DateTime.hpp>   // Library.hpp

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::DateTime`. Note that
`XYO::System::DateTime` (the C++ date class) has the same short name: inside
code that uses both, qualify them.

Export macro: `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT`:

| Define | Effect |
|--------|--------|
| `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_INTERNAL` (or `QUANTUM_SCRIPT__DATETIME_INTERNAL`, set by fabricare while building the DLL) | export macro = `XYO_PLATFORM_LIBRARY_EXPORT` |
| none | export macro = `XYO_PLATFORM_LIBRARY_IMPORT` (consumers of the DLL) |
| `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_LIBRARY` (exported by `quantum-script--datetime.static`) | export macro empty, no `quantumScriptExtension` entry point (static library) |
| `XYO_PLATFORM_COMPILE_STATIC` (static platforms) | `XYO_PLATFORM_LIBRARY_EXPORT` / `IMPORT` are empty |

The `quantumScriptExtension` entry point is compiled only when
`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` is defined and
`XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_LIBRARY` is not.

## Registering the extension

```cpp
void Extension::DateTime::registerInternalExtension(Executive *executive);
void Extension::DateTime::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"DateTime"` as an internal
  extension; call it from the host's init callback (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `DateTime` in a thread. It:

  1. sets the extension name, info (`"DateTime"` and the short license text),
     version and public flag;
  2. creates the context (`newContext`): gets the symbol `DateTime`, creates
     the global function `DateTime` (native `functionDateTime`, which
     returns `VariableDateTime::newVariable()`), keeps its prototype in
     `DateTimeContext::prototypeDateTime` and registers `deleteContext` with
     `setExtensionDeleteContext`;
  3. registers the 19 native functions with
     `executive->setFunction2("DateTime.prototype.getYear()", getYear)` and
     so on.

  Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--datetime.dll`.

## `DateTimeContext` (`DateTime/Context.hpp`)

```cpp
class DateTimeContext : public Object {
	public:
		Symbol symbolFunctionDateTime;          // the global name "DateTime"
		TPointerX<Prototype> prototypeDateTime; // prototype of DateTime objects
};

DateTimeContext *getContext();                  // TSingleton<DateTimeContext>
```

`TSingleton` is per thread in multithreaded builds, so each thread that
loads the extension has its own prototype.

## `VariableDateTime` (`DateTime/VariableDateTime.hpp`)

The script value type. A C++ host or another extension can create and read
`DateTime` values directly:

```cpp
class VariableDateTime : public Variable {
	public:
		XYO::System::DateTime value;            // the date and time

		void activeConstructor();               // run by the pool on every allocation: value = now
		static Variable *newVariable();         // from the TMemoryPoolActive pool, value = now
		String getVariableType();               // "DateTime", what typeof() returns
		Variable *instancePrototype();          // getContext()->prototypeDateTime->prototype
		Variable *clone(SymbolList &);          // new object, value.copy(value)
		bool toBoolean();                       // true
		String toString();                      // "DateTime"
};
```

```cpp
// return the current date and time to a script
static TPointer<Variable> myNow(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	return Extension::DateTime::VariableDateTime::newVariable();    // value is the current time
};

// read a DateTime argument
Variable *x = arguments->index(0);
if (TIsType<Extension::DateTime::VariableDateTime>(x)) {
	uint64_t t = ((Extension::DateTime::VariableDateTime *)x)->value.toUnixTime();
};
```

`XYO_DYNAMIC_TYPE_DEFINE` gives it runtime type information, so
`TIsType<VariableDateTime>(variable)` works across DLL boundaries.

`TMemory<VariableDateTime>` is specialized to `TMemoryPoolActive`: objects
are recycled, and the C++ constructor runs only when the pool allocates a
brand new one. The pool calls `activeConstructor()` on every
`newVariable()`, also for a recycled object, and it copies the current
local time (`XYO::System::DateTime now; value.copy(now);`) into `value`, so
a new `DateTime` never keeps the value of a freed one. Before version
5.9.1 there was no `activeConstructor()` and a recycled object kept its old
`value` (see
[old versions](script-api.md#versions-before-591-new-datetime-could-return-an-old-value)).
`clone()` overwrites the fresh value with `value.copy`.

## The native functions

All are `static` in `DateTime/Library.cpp`, with the signature

```cpp
static TPointer<Variable> name(VariableFunction *function, Variable *this_, VariableArray *arguments);
```

| Script function | C++ |
|-----------------|-----|
| `DateTime()` / `new DateTime()` | `functionDateTime`: `VariableDateTime::newVariable()`, arguments ignored |
| `getYear()`, `getMonth()`, `getDay()`, `getDayOfWeek()`, `getHour()`, `getMinute()`, `getSecond()`, `getMilliseconds()` | `value.getX()` as a `VariableNumber` |
| `setYear(x)`, `setMonth(x)`, `setDay(x)`, `setDayOfWeek(x)`, `setHour(x)`, `setMinute(x)`, `setSecond(x)`, `setMilliseconds(x)` | `x->toNumber()`; `false` if `isnan`, `isinf` or `signbit`; else `value.setX((uint16_t)x)` and `true` |
| `toUnixTime()` | `value.toUnixTime()` |
| `fromUnixTime(x)` | same checks as the setters, then `value.fromUnixTime((uint64_t)x)` and `true` |
| `DateTime.timestampInMilliseconds()` | `XYO::System::DateTime::timestampInMilliseconds()` |

Every prototype method first checks `TIsType<VariableDateTime>(this_)` and
throws `Error("invalid parameter")` otherwise.

With `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME` defined each function prints a trace
line (`- datetime-get-year`, `- datetime-to-unix-time`, ...).

## `XYO::System::DateTime` behavior that shows through

From `xyo-system` (`DateTime-OS-Windows.cpp`, `DateTime-OS-Linux.cpp`):

| Topic | Windows (`SYSTEMTIME`) | Linux (`struct tm` + milliseconds) |
|-------|------------------------|-------------------------------------|
| Constructor | `GetLocalTime` | `gettimeofday` + `localtime_r` |
| `toUnixTime` | `SystemTimeToFileTime` of the local fields, as UTC | `timegm` of the local fields |
| `fromUnixTime` | `FileTimeToSystemTime`, milliseconds `0` | `gmtime_r`, milliseconds `0` |
| `timestampInMilliseconds` | `GetLocalTime` → FILETIME / 10000 − epoch | `localtime_r` + `timegm`, plus `tv_usec / 1000` |
| `getDayOfWeek` | round trip through FILETIME; the stored field if the date is invalid | `timegm` on a copy (normalizes invalid dates) |
| Invalid fields in `toUnixTime` | `SystemTimeToFileTime` fails, result undefined | normalized by `timegm` |

Local time counted as UTC is deliberate and documented in `xyo-system`
(`DateTime.hpp`: "LOCAL TIME, NOT UTC - by design").

`plus`, `minus`, `compare`, `copy` and `setLocalTime` exist on the C++ class
but are not exposed to scripts.

## Notes for maintainers

- New method: write `static TPointer<Variable> name(...)` in
  `Library.cpp` (check `TIsType<VariableDateTime>(this_)`, convert
  arguments with `toNumber()` and reject `NaN` / infinite / negative values
  as the setters do), register it in `initExecutive` with
  `executive->setFunction2("DateTime.prototype.name(x)", name)` (or
  `"DateTime.name()"` for a static function). Then update `README.md`,
  `docs/script-api.md`, `docs/reference.md`, the skill in
  `.claude/skills/quantum-script--datetime/` and `test/test.01.js`.
- The local-time-as-UTC counting belongs to `xyo-system` and is relied on by
  fabricare, `Task` and `SSHRemote`; do not "fix" it here.
- `test/test.01.js` loads the extension and prints the current date and
  time; run it with `fabricare test`.
