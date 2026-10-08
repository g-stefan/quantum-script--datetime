---
name: quantum-script--datetime
description: >-
  How to use the Quantum Script DateTime extension (quantum-script--datetime),
  the date and time type loaded with Script.requireExtension("DateTime") (the
  Quantum Script core has no Date): new DateTime() (current LOCAL date and
  time, arguments ignored), getYear, getMonth (1..12), getDay, getDayOfWeek
  (0 Sunday, computed from the date), getHour, getMinute, getSecond,
  getMilliseconds, the setters setYear / setMonth / setDay / setDayOfWeek /
  setHour / setMinute / setSecond / setMilliseconds (return true / false, no
  range check, no normalization, 16-bit), toUnixTime / fromUnixTime and the
  static DateTime.timestampInMilliseconds() - all counting the local wall
  clock as if it were UTC (not real Unix time, by design in xyo-system). Covers
  the pitfalls: before 5.9.1 a recycled DateTime from the memory pool could
  hold an old value instead of the current time (fixed with activeConstructor),
  invalid dates give a meaningless toUnixTime on Windows, fromUnixTime rejects
  negative values and zeroes milliseconds, objects are shared by reference,
  no toString / format / parse / UTC / time zones; recipes (zero padded
  formatting, build / validate / parse a date, add days and months,
  difference, compare, copy, elapsed time, seeding Random); which hosts have
  it (quantum-script, fabricare build scripts where it is preloaded, magnet,
  Task, SSHRemote); the C++ side (registerInternalExtension, initExecutive,
  VariableDateTime with its XYO::System::DateTime value, DateTimeContext,
  quantumScriptExtension, quantum-script--datetime.static). Use when writing
  or reviewing Quantum Script code or fabricare scripts that use DateTime,
  porting JavaScript Date code to Quantum Script, C++ code that includes
  <XYO/QuantumScript.Extension/DateTime.hpp>, a fabricare.json depending on
  "quantum-script--datetime", or when working inside the
  quantum-script--datetime repository.
---

# quantum-script--datetime

`DateTime` extension of Quantum Script (see the `quantum-script` skill for
the language and its differences from JavaScript; its rules apply).
Purpose: **give scripts the current date and time and a date value type** —
read the clock, build and store dates, measure elapsed time, make time
stamps. It wraps `XYO::System::DateTime` (see the `xyo-system` skill). The
core has no `Date`: `typeof(DateTime)` is `"undefined"` until the extension
is loaded.

Full documentation: `docs/` in the quantum-script--datetime repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--datetime\docs` on this machine):
README (purpose, local time), getting-started (build, load, hosts, C++
registration, static builds, threads), **script-api** (every method,
conversion, invalid dates, recipes), cpp-api, reference. When
in doubt read `source/XYO/QuantumScript.Extension/DateTime/Library.cpp` —
short — and `xyo-system/source/XYO/System/DateTime-OS-*.cpp`.

## Script API

```javascript
Script.requireExtension("DateTime");

var d = new DateTime();          // current local date and time; DateTime() works too
d.getYear();   d.getMonth();     // 2026, 1..12 (1 based!)
d.getDay();    d.getDayOfWeek(); // 1..31, 0 Sunday .. 6 Saturday
d.getHour();   d.getMinute();   d.getSecond();   d.getMilliseconds();
d.setYear(x);  d.setMonth(x);   d.setDay(x);     d.setDayOfWeek(x);   // -> true / false
d.setHour(x);  d.setMinute(x);  d.setSecond(x);  d.setMilliseconds(x);
d.toUnixTime();                  // seconds since 1970, LOCAL clock counted as UTC
d.fromUnixTime(x);               // -> true / false, milliseconds become 0
DateTime.timestampInMilliseconds();   // static, local clock in ms, always fresh
```

## Hard rules

1. **Load it first.** `Script.requireExtension("DateTime");` in every script
   and every thread (one engine per thread). Exceptions: fabricare build
   scripts (preloaded); loading `Task` or `SSHRemote` loads it. Missing →
   `Unable to open "DateTime"`.
2. **Local time, never UTC.** `toUnixTime`, `fromUnixTime` and
   `timestampInMilliseconds` count the local wall clock as if it were UTC
   (by design in xyo-system, do not "fix"). At UTC+3, 00:08 local gives the
   Unix time of 00:08 UTC. Round trips are exact and `+ 86400` is always the
   next calendar day, but the numbers must not be exchanged with systems
   expecting real Unix time (HTTP, databases, JS `Date.now()`). There is no
   way to get UTC or the time zone from this extension.
3. **`new DateTime()` reads the clock each time since 5.9.1.** Objects come
   from a `TMemoryPoolActive` pool; `VariableDateTime::activeConstructor()`
   refreshes `value` on every allocation. Before 5.9.1 a recycled
   `DateTime` kept its old value (`1970` after a freed object was set with
   `fromUnixTime(0)`, the same `toUnixTime()` seconds apart). Only for
   scripts that must run with an older installed extension, build "now"
   from `DateTime.timestampInMilliseconds()` (`fromUnixTime` of the seconds,
   then `setMilliseconds`). When building a date, set **every** field (or use
   `fromUnixTime`): unset fields keep the current time.
4. **Constructor arguments are ignored.** `new DateTime(2020, 1, 1)` is now.
   Build dates with the setters in order year, month, day, hour, minute,
   second, milliseconds.
5. **Setters store, they do not validate.** `x` goes through `toNumber` and
   is truncated (`setDay(10.9)` → 10, `setDay("15")` → 15). They return
   `false` and store nothing only for negative / `NaN` / infinite /
   non-numeric / missing `x`. `setMonth(13)`, `setDay(30)` in February,
   `setHour(25)` return `true` — `true` means stored, not valid. No carry
   (`setMinute(75)` stays 75). Fields are 16-bit: `setYear(70000)` → 4464.
   Validate with `isValidDate` (below) before setting.
6. **Invalid dates break `toUnixTime`.** Windows returns a meaningless huge
   number (`18446744062065078272`); Linux normalizes (`02-30` → `03-01`).
   Never rely on either.
7. **`getDayOfWeek()` is computed** from year / month / day each call, so it
   is right after setters. `setDayOfWeek(x)` has no useful effect.
8. **`fromUnixTime`** truncates `x`, sets milliseconds to `0`, returns
   `false` (object unchanged) for negative `x`: dates before 1970 only via
   setters. Huge values overflow silently (`1000000000000000` → year ~7879).
9. **Reference semantics.** `var c = d;` shares one object; `==` compares
   identity, not dates. Copy with `fromUnixTime(d.toUnixTime())` +
   `setMilliseconds(d.getMilliseconds())`; compare
   `toUnixTime() * 1000 + getMilliseconds()`.
10. **No formatting.** `"" + d` and `Console.writeLn(d)` print `"DateTime"`;
    `d.toString()` throws (not a function). Numbers print without padding
    (`2026-10-5 0:8:10`): use the `pad` recipe. No `parse`, `toISOString`,
    `getFullYear`, `Date`, `Date.now()`.
11. Methods called on a non-`DateTime` (`var f = d.getYear; f();`) throw
    `invalid parameter`.
12. `timestampInMilliseconds()` is the local wall clock: a clock change or
    the end of daylight saving makes elapsed times jump. Fine for timeouts
    and progress, not a monotonic clock.

## Recipes

```javascript
function pad(value, size) {                         // pad(7, 2) -> "07"
	var text = "" + value;
	while (text.length < size) {
		text = "0" + text;
	};
	return text;
};
function formatDateTime(d) {                        // "2026-10-05 00:08:10"
	return pad(d.getYear(), 4) + "-" + pad(d.getMonth(), 2) + "-" + pad(d.getDay(), 2) + " " +
	       pad(d.getHour(), 2) + ":" + pad(d.getMinute(), 2) + ":" + pad(d.getSecond(), 2);
};
function makeDateTime(year, month, day, hour, minute, second) {
	var d = new DateTime();
	d.setYear(year); d.setMonth(month); d.setDay(day);
	d.setHour(hour); d.setMinute(minute); d.setSecond(second); d.setMilliseconds(0);
	return d;
};
function isLeapYear(year) {
	return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
};
function daysInMonth(year, month) {
	if (month == 2) { return isLeapYear(year) ? 29 : 28; };
	if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) { return 30; };
	return 31;
};
function isValidDate(year, month, day) {            // 1601..30827: Windows range
	if ((year < 1601) || (year > 30827) || (month < 1) || (month > 12)) { return false; };
	return (day >= 1) && (day <= daysInMonth(year, month));
};
function addSeconds(d, seconds) {                   // days: 86400 * n, no DST jumps
	var out = new DateTime();
	out.fromUnixTime(d.toUnixTime() + seconds);
	return out;
};
var days = (end.toUnixTime() - start.toUnixTime()) / 86400;
var dayName = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"][d.getDayOfWeek()];
var start = DateTime.timestampInMilliseconds(); /* work */ var ms = DateTime.timestampInMilliseconds() - start;
var part = "2026-08-09".split("-");                 // parse, then isValidDate + makeDateTime
makeDateTime(Convert.toNumber(part[0]), Convert.toNumber(part[1]), Convert.toNumber(part[2]), 0, 0, 0);
```

Add months on the fields (clamp the day with `daysInMonth`), see
`docs/script-api.md`. Seed `Random` with `timestampInMilliseconds()`, not
`toUnixTime()` (same second → same seed); never for secrets. Remember
Quantum Script: `&&` / `||` return booleans (no `x || default`), no
exponent literals, declare loop variables with `var`.

## C++

```cpp
#include <XYO/QuantumScript.Extension/DateTime.hpp>
using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {                     // host init callback
	Extension::DateTime::registerInternalExtension(executive);    // scripts still requireExtension("DateTime")
};
```

- fabricare.json dependency: `"quantum-script--datetime"` (`dll-or-lib`:
  DLL on dynamic platforms, static lib on `*.static` platforms; pulls in
  `quantum-script` and `quantum-script--console`) or
  `"quantum-script--datetime.static"` (static lib, static CRT, exports
  `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_LIBRARY`; used by fabricare) together
  with `quantum-script.static` and `"crt": "static"`.
- DLL entry point `extern "C" quantumScriptExtension(Executive *, void *)`
  only with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` and without
  `XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_LIBRARY`. Static hosts must register
  the extension as internal.
- Script value: `Extension::DateTime::VariableDateTime` (public member
  `XYO::System::DateTime value`, `newVariable()`, `getVariableType()` →
  `"DateTime"`). Test with `TIsType<VariableDateTime>(x)`.
  `newVariable()` returns an object whose `value` is the current local time
  (set by `activeConstructor()`, also for recycled objects; rule 3).
- The namespace `Extension::DateTime` and the class `XYO::System::DateTime`
  share a short name: qualify both.
- `DateTimeContext` (`getContext()`, per-thread `TSingleton`) holds the
  `DateTime` symbol and prototype; `deleteContext` is registered with
  `setExtensionDeleteContext`.
- Hosts that register it: fabricare (preloaded), magnet; `Task` (timers via
  `timestampInMilliseconds`) and `SSHRemote` (Random seed) require it.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console and DateTime as internal extensions and runs
  `test/test.01.js`; run `make` first), `fabricare install` (see the
  `fabricare` skill). `quantum-script` and `quantum-script--console` must be
  installed first. With the SDK installed a script runs directly:
  `quantum-script file.js` (uses the installed `quantum-script--datetime.dll`).
- Native methods live in `DateTime/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`:
  check `TIsType<VariableDateTime>(this_)` (else
  `throw(Error("invalid parameter"))`), reject `isnan` / `isinf` /
  `signbit` arguments by returning `false`, register in `initExecutive` with
  `executive->setFunction2("DateTime.prototype.name(x)", name)`.
- Local-time-as-UTC lives in `xyo-system` and is relied on by fabricare,
  Task and SSHRemote; do not change it here.
- New or changed methods: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `test/test.01.js` and this skill. `test/test.01.js`
  checks that `new DateTime()` after a freed `fromUnixTime(0)` object is the
  current time (regression for rule 3); keep `activeConstructor()` when
  changing `VariableDateTime`.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
