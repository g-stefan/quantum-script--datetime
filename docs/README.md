# Quantum Script Extension DateTime — Documentation

`quantum-script--datetime` is the **date and time extension of Quantum
Script**. The Quantum Script core has no `Date` object; loading this
extension with `Script.requireExtension("DateTime")` adds a `DateTime` type
that holds one calendar date and time of day, down to the millisecond:

```javascript
Script.requireExtension("DateTime");

var now = new DateTime();               // current local date and time
now.getYear();                          // 2026
now.getMonth();                         // 1 .. 12
now.getDay();                           // 1 .. 31
now.getDayOfWeek();                     // 0 (Sunday) .. 6 (Saturday)
now.getHour();                          // 0 .. 23
now.toUnixTime();                       // seconds since 1970-01-01 (local clock)
DateTime.timestampInMilliseconds();     // milliseconds, for timing
```

It is a thin wrapper over `XYO::System::DateTime` from `xyo-system`
(`SYSTEMTIME` on Windows, `struct tm` on Linux).

- **Fields, not a timestamp.** A `DateTime` stores year, month, day, hour,
  minute, second and milliseconds separately. Getters read a field, setters
  write one field and do nothing else: no range checks, no normalization.
- **Local time only.** `new DateTime()` is the local wall clock. There is no
  UTC, no time zone and no daylight saving information.
- **Unix time of the local clock.** `toUnixTime()`, `fromUnixTime(x)` and
  `DateTime.timestampInMilliseconds()` count the **local** date and time as
  if it were UTC. `fromUnixTime(toUnixTime())` gives back the same local date
  and time, and adding `86400` always moves to the same time on the next
  calendar day, but the numbers are **not** the real Unix time unless the
  machine runs on UTC (see [Local time](#local-time-not-utc)).
- **Same results on Windows and Linux.** Both platforms use the same counting,
  so values stored on one read back the same on the other.

```
scripts: quantum-script .js, fabricare build scripts, magnet, Task, SSHRemote, embedding hosts, ...
quantum-script--datetime  <-- this extension: the DateTime type
quantum-script            (Executive, Variable, Context)
xyo-system                XYO::System::DateTime (GetLocalTime / gettimeofday + localtime_r)
xyo-multithreading, xyo-encoding, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

| Need | What `DateTime` gives |
|------|-----------------------|
| Show or log the current date and time | `new DateTime()` and the `get*` methods |
| Time stamps for file names, temporary names, build stamps | `(new DateTime()).toUnixTime()` or formatted fields |
| Measure how long something takes, wait, time out | `DateTime.timestampInMilliseconds()` |
| Store a date as one number and read it back | `toUnixTime()` / `fromUnixTime(x)` |
| Build a given date, add days or hours, compare dates | the `set*` methods plus unix time arithmetic |
| Day of the week of any date | `getDayOfWeek()`, computed from year / month / day |
| Seed a random generator | `rnd.seed((new DateTime()).toUnixTime())` |

Other parts of the stack depend on it: `fabricare` preloads it for build
scripts, `magnet` registers it, the `Task` extension uses
`DateTime.timestampInMilliseconds()` for its timers and `SSHRemote` seeds
its random generator with `toUnixTime()`.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("DateTime");` | loading twice does nothing |
| Current date and time | `new DateTime()` | arguments are ignored; reads the clock each time (before 5.9.1 see [old versions](script-api.md#versions-before-591-new-datetime-could-return-an-old-value)) |
| Read a field | `getYear()`, `getMonth()`, `getDay()`, `getHour()`, `getMinute()`, `getSecond()`, `getMilliseconds()` | month and day start at **1** |
| Day of the week | `getDayOfWeek()` | `0` Sunday .. `6` Saturday |
| Change a field | `setYear(x)`, `setMonth(x)`, ... | `true` if stored, `false` for negative / `NaN` / non-numeric input; **no range check** |
| Seconds since 1970-01-01 | `toUnixTime()` | local clock counted as UTC |
| Date from seconds | `fromUnixTime(x)` | milliseconds become `0`; negative → `false` |
| Milliseconds now | `DateTime.timestampInMilliseconds()` | static, no object needed |
| Add or subtract time | `out.fromUnixTime(d.toUnixTime() + seconds)` | no daylight saving jumps |
| Compare two dates | compare `toUnixTime()` (and `getMilliseconds()`) | `==` compares objects, not dates |
| Copy | `c.fromUnixTime(d.toUnixTime()); c.setMilliseconds(d.getMilliseconds());` | `var c = d;` shares the object |
| Format as text | your own zero padding, see [recipes](script-api.md#format-a-date) | there is no `toString` / `format` |

## Local time, not UTC

This is by design in `xyo-system` and will not change. `toUnixTime()` takes
the local date and time fields and counts them as if they were UTC:

```javascript
// The machine is in Romania (UTC+3 in summer), local time 2026-10-05 00:08:10
(new DateTime()).toUnixTime();      // 1791158890 = 2026-10-05 00:08:10 "UTC"
                                    // the real Unix time is 1791148090
```

Consequences:

- Values round-trip exactly: `fromUnixTime(toUnixTime())` is the same local
  date and time on Windows and Linux, in any time zone.
- Date arithmetic is calendar arithmetic: `+ 86400` is always the next day at
  the same hour, also across daylight saving changes.
- The numbers are **not** comparable with Unix time from other systems
  (HTTP dates, databases, JavaScript `Date.now()`), and the extension has no
  way to convert to UTC.
- When the clock moves back at the end of daylight saving, one local hour
  happens twice and `timestampInMilliseconds()` goes back by an hour; elapsed
  times measured across that moment are wrong.

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, fabricare and magnet scripts, register it in a C++ host, static builds, threads |
| [Script API](script-api.md) | The `DateTime` type, every method, argument conversion, invalid dates, recipes |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, `VariableDateTime`, `DateTimeContext`, the DLL entry point, notes for maintainers |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Convert`, `Script.requireExtension`,
embedding, writing extensions) is documented in the `quantum-script`
repository, `docs/`. The underlying C++ class is documented by the
`xyo-system` repository.

## Source map

```
source/XYO/QuantumScript.Extension/DateTime.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/DateTime.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/DateTime/
    Dependency.hpp                                         <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                     initExecutive, registerInternalExtension,
                                                           the DateTime constructor and the 19 native methods
    Context.hpp                                            DateTimeContext: the DateTime symbol and prototype
    VariableDateTime[.hpp/.cpp]                            the script value type, holds an XYO::System::DateTime
    Copyright / License / Version                          library metadata
    Library.rc, *.rh                                       Windows version resource
test/test.01.cpp                                           C++ host registering Console and DateTime as internal
test/test.01.js                                            prints the current date and time
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--datetime/`](../.claude/skills/quantum-script--datetime/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use
`DateTime` (Quantum Script tools, fabricare and magnet scripts, other
extensions).
