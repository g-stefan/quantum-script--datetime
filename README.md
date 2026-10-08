# Quantum Script Extension DateTime

Quantum Script extension
- A `DateTime` type for Quantum Script, which has no `Date` in its core:
read the current local date and time, build and store dates, convert them
to and from Unix time, measure elapsed time in milliseconds.
- Local time only, by design: Unix time values count the local wall clock
as if it were UTC, the same on Windows and Linux (see the documentation).
- Preloaded in fabricare build scripts, available in `quantum-script`,
magnet and any host that registers it; used by the `Task` and `SSHRemote`
extensions.

```javascript
Script.requireExtension("DateTime");

DateTime;
DateTime.prototype.getYear();
DateTime.prototype.getMonth();
DateTime.prototype.getDay();
DateTime.prototype.getDayOfWeek();
DateTime.prototype.getHour();
DateTime.prototype.getMinute();
DateTime.prototype.getSecond();
DateTime.prototype.getMilliseconds();
DateTime.prototype.setYear(x);
DateTime.prototype.setMonth(x);
DateTime.prototype.setDay(x);
DateTime.prototype.setDayOfWeek(x);
DateTime.prototype.setHour(x);
DateTime.prototype.setMinute(x);
DateTime.prototype.setSecond(x);
DateTime.prototype.setMilliseconds(x);
DateTime.prototype.toUnixTime();
DateTime.prototype.fromUnixTime(x);
DateTime.timestampInMilliseconds();
```

Built on `quantum-script` and `xyo-system`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose, local time instead of UTC
- [Getting started](docs/getting-started.md) - build, load from a script, hosts, register in a C++ host
- [Script API](docs/script-api.md) - every method: conversion, invalid dates, recipes
- [C++ API](docs/cpp-api.md) - registration, `VariableDateTime`, DLL entry point, notes for maintainers
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--datetime](.claude/skills/quantum-script--datetime/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
