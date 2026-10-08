# API reference

## Script

Available after `Script.requireExtension("DateTime")`.

### Constructor

| Symbol | Returns | Notes |
|--------|---------|-------|
| `new DateTime()` / `DateTime()` | DateTime | current local date and time, reads the clock each time; arguments ignored |

`typeof(new DateTime())` is `"DateTime"`; as a string it is `"DateTime"`,
as a boolean `true`.

### Getters (`DateTime.prototype`)

| Symbol | Returns | Notes |
|--------|---------|-------|
| `getYear()` | Number | full year, `2026` |
| `getMonth()` | Number | `1 .. 12` |
| `getDay()` | Number | day of the month, `1 .. 31` |
| `getDayOfWeek()` | Number | `0` Sunday .. `6` Saturday, computed from the date |
| `getHour()` | Number | `0 .. 23` |
| `getMinute()` | Number | `0 .. 59` |
| `getSecond()` | Number | `0 .. 59` |
| `getMilliseconds()` | Number | `0 .. 999` |

### Setters (`DateTime.prototype`)

| Symbol | Returns | Notes |
|--------|---------|-------|
| `setYear(x)` | Boolean | |
| `setMonth(x)` | Boolean | `1 .. 12` expected, not checked |
| `setDay(x)` | Boolean | `1 .. 31` expected, not checked |
| `setDayOfWeek(x)` | Boolean | stored but not used by `getDayOfWeek()` |
| `setHour(x)` | Boolean | |
| `setMinute(x)` | Boolean | |
| `setSecond(x)` | Boolean | |
| `setMilliseconds(x)` | Boolean | |

All setters: `x` converted with `toNumber` and truncated; `false` (nothing
stored) for negative, `NaN`, infinite, non-numeric or missing `x`; `true`
otherwise. No range check, no normalization; values are 16-bit
(`65536` and more wrap).

### Unix time

| Symbol | Returns | Notes |
|--------|---------|-------|
| `toUnixTime()` | Number | seconds since 1970-01-01, local date and time counted as UTC; meaningless for invalid dates on Windows |
| `fromUnixTime(x)` | Boolean | sets all fields from `x` seconds (local time counted as UTC), milliseconds `0`; `false` and no change for negative / `NaN` / infinite / non-numeric `x` |
| `DateTime.timestampInMilliseconds()` | Number | static; milliseconds since 1970-01-01, local clock counted as UTC; always reads the clock |

### Edge cases

| Expression | Result |
|------------|--------|
| `new DateTime(2020, 1, 1)` | current date (arguments ignored) |
| `"" + new DateTime()` | `"DateTime"` |
| `d.toString()` | throws `"toString" is not a function` |
| `d.setDay(10.9)` | `true`, day `10` |
| `d.setDay("15")` | `true`, day `15` |
| `d.setDay(-1)`, `d.setDay("abc")`, `d.setDay()` | `false` |
| `d.setMonth(13)` | `true` (invalid date) |
| `d.setYear(70000)` | `true`, year `4464` |
| `d.fromUnixTime(0)` | `true`, `1970-1-1 0:0:0`, day of week `4` |
| `d.fromUnixTime(-1)` | `false` |
| `d.fromUnixTime(1700000000.9)` | `true`, `2023-11-14 22:13:20` |
| `2024-02-30` then `toUnixTime()` | Windows: `18446744062065078272`; Linux: the time of `2024-03-01` |
| `var c = d; c.setYear(2000);` | changes `d` too (same object) |
| `DateTime.prototype.getYear.call({})` | throws `invalid parameter` |

### Not defined

UTC and time zone functions, `toString`, `format`, `parse`, `plus`,
`minus`, `compare`, setting the system clock; JavaScript's `Date` and
`Date.now()`.

### Errors

| Message | Cause |
|---------|-------|
| `Unable to open "DateTime"` | the extension library was not found and no internal one is registered |
| `invalid parameter` | a `DateTime.prototype` method called on something that is not a `DateTime` |

## C++

Namespace `XYO::QuantumScript::Extension::DateTime`, umbrella header
`<XYO/QuantumScript.Extension/DateTime.hpp>`.

### Library (`DateTime/Library.hpp`)

| Symbol | Notes |
|--------|-------|
| `void registerInternalExtension(Executive *executive)` | register `"DateTime"` as an internal extension |
| `void initExecutive(Executive *executive, void *extensionId)` | extension init, run by the engine |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry point (not in static builds) |

### Context (`DateTime/Context.hpp`)

| Symbol | Notes |
|--------|-------|
| `class DateTimeContext` | `symbolFunctionDateTime`, `prototypeDateTime` |
| `DateTimeContext *getContext()` | per-thread singleton |

### VariableDateTime (`DateTime/VariableDateTime.hpp`)

| Symbol | Notes |
|--------|-------|
| `XYO::System::DateTime value` | the stored date and time |
| `void activeConstructor()` | called by the pool on every allocation, sets `value` to the current time |
| `static Variable *newVariable()` | from the `TMemoryPoolActive` pool; `value` is the current local date and time |
| `String getVariableType()` | `"DateTime"` |
| `Variable *instancePrototype()` | the `DateTime` prototype of the current thread |
| `Variable *clone(SymbolList &)` | copy with `value.copy` |
| `bool toBoolean()` | `true` |
| `String toString()` | `"DateTime"` |

### Metadata

| Symbol | Notes |
|--------|-------|
| `Version::version()`, `Version::build()`, `Version::versionWithBuild()`, `Version::datetime()` | from `version.json` |
| `Copyright::copyright()`, `Copyright::publisher()`, `Copyright::company()`, `Copyright::contact()` | |
| `License::license()`, `License::shortLicense()` | MIT text |

`Version`, `Copyright` and `License` exist in every XYO library: qualify them
(`Extension::DateTime::Version::versionWithBuild()`).

### Build configuration

| Name | Meaning |
|------|---------|
| `quantum-script--datetime` | fabricare project, `dll-or-lib` |
| `quantum-script--datetime.static` | fabricare project, static library with static CRT |
| `test.01` | fabricare test project, runs `test/test.01.js` |
| `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT` | export / import macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_INTERNAL` | defined while building the DLL (from `QUANTUM_SCRIPT__DATETIME_INTERNAL`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_LIBRARY` | static build: empty export macro, no DLL entry point |
