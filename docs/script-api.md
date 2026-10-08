# Script API

Everything the extension defines after `Script.requireExtension("DateTime")`:
the global function `DateTime`, 18 methods on `DateTime.prototype` and one
static function. All results shown below were produced by the interpreter on
Windows, in a UTC+3 time zone.

## The `DateTime` type

```javascript
var d = new DateTime();      // current local date and time
var e = DateTime();          // the same, `new` is optional
```

- The constructor **ignores its arguments**: `new DateTime(2020, 1, 1)` is
  still the current date. Build a given date with the setters or
  `fromUnixTime` (see [recipes](#build-a-given-date)).
- `typeof(d)` is `"DateTime"`, `d instanceof DateTime` is `true`,
  `Script.isObject(d)` is `false`.
- As a string (`"" + d`, `Console.writeLn(d)`) it is the text `"DateTime"`.
  There is no `toString()` method and no formatting function.
- As a boolean it is always `true`.
- It is a reference: `var c = d;` makes `c` and `d` the same object, and a
  setter called on one changes both. `==` compares objects, not dates. See
  [Copy](#copy-a-datetime) and [Compare](#compare-two-dates).
- It has no properties, only methods. Calling a method on anything that is
  not a `DateTime` (`DateTime.prototype.getYear.call({})`, or a method taken
  out of the object: `var f = d.getYear; f();`) throws
  `Error: invalid parameter`.

## Getters

All return numbers; none take arguments.

| Method | Range | Notes |
|--------|-------|-------|
| `getYear()` | `1601 .. 30827` on Windows | full year: `2026` |
| `getMonth()` | `1 .. 12` | **1 based** (JavaScript `Date` is 0 based) |
| `getDay()` | `1 .. 31` | day of the month |
| `getDayOfWeek()` | `0 .. 6` | `0` Sunday, `1` Monday, ... `6` Saturday; computed from year, month and day |
| `getHour()` | `0 .. 23` | |
| `getMinute()` | `0 .. 59` | |
| `getSecond()` | `0 .. 59` | |
| `getMilliseconds()` | `0 .. 999` | `0` after `fromUnixTime` |

The ranges hold for dates read from the clock or produced by
`fromUnixTime`. A getter returns whatever a setter stored, valid or not.

`getDayOfWeek()` is calculated from the stored date each time it is called,
so it is right after the setters change the date:

```javascript
var d = new DateTime();
d.setYear(2024);
d.setMonth(2);
d.setDay(29);
d.getDayOfWeek();     // 4 (Thursday)
```

## Setters

```javascript
d.setYear(x);  d.setMonth(x);  d.setDay(x);  d.setDayOfWeek(x);
d.setHour(x);  d.setMinute(x);  d.setSecond(x);  d.setMilliseconds(x);
```

Each one converts `x` with `toNumber`, then:

| Argument | Result | Example |
|----------|--------|---------|
| number `>= 0` | stored after truncation, returns `true` | `setDay(10.9)` stores `10` |
| numeric string | converted, returns `true` | `setDay("15")` stores `15` |
| negative, `NaN`, `±Infinity` | nothing stored, returns `false` | `setDay(-1)`, `setDay(0/0)` |
| non-numeric string, missing argument | `NaN`: nothing stored, returns `false` | `setDay("abc")`, `setDay()` |
| `65536` or more | stored **modulo 65536** (16-bit field), returns `true` | `setYear(70000)` stores `4464` |

What the setters do **not** do:

- **No range check.** `setMonth(13)`, `setDay(31)` in February,
  `setHour(25)` all return `true` and store the value. The result is an
  invalid date (see [Invalid dates](#invalid-dates)). `true` means "stored",
  not "valid".
- **No normalization or carry.** `setMinute(75)` does not move to the next
  hour.
- **No effect on other fields.** Setting the day of a date does not update
  the day of the week stored inside; `getDayOfWeek()` computes it anyway.
- `setDayOfWeek(x)` stores a value that `getDayOfWeek()` does not use for a
  valid date. It exists for symmetry with `XYO::System::DateTime`; it has no
  useful effect from scripts.

Set fields in the order year, month, day when building a date, and check the
values yourself before setting them (see the
[validation recipe](#validate-a-date)).

## Unix time

```javascript
d.toUnixTime();          // number of seconds
d.fromUnixTime(x);       // true / false
DateTime.timestampInMilliseconds();
```

| Function | Returns | Notes |
|----------|---------|-------|
| `toUnixTime()` | seconds since 1970-01-01 00:00:00 | the **local** date and time counted as if it were UTC; milliseconds are dropped |
| `fromUnixTime(x)` | `true`, or `false` if `x` is negative, `NaN`, `±Infinity` or not numeric (the object is then unchanged) | sets every field from `x` (truncated to an integer); milliseconds become `0` |
| `DateTime.timestampInMilliseconds()` | milliseconds since 1970-01-01, local clock counted as UTC | static: call it on `DateTime`, not on an object |

```javascript
var d = new DateTime();
d.fromUnixTime(0);              // 1970-1-1 0:0:0, day of week 4 (Thursday)
d.fromUnixTime(1700000000);     // 2023-11-14 22:13:20
d.fromUnixTime(4102444800);     // 2100-1-1 0:0:0
d.toUnixTime();                 // 4102444800
d.fromUnixTime(-1);             // false, d is unchanged
```

- **Local time, not UTC.** These numbers are the local wall clock written as
  Unix time: on a machine at UTC+3, 00:08:10 local time gives the Unix time
  of 00:08:10 UTC, three hours ahead of the real one. This is deliberate
  (see [Local time](README.md#local-time-not-utc)): values round-trip
  exactly and date arithmetic ignores daylight saving. Do not exchange them
  with systems that expect real Unix time.
- **Dates before 1970 cannot be set with `fromUnixTime`** (negative values
  return `false`). Use the setters for them; `toUnixTime()` of such a date
  is not meaningful.
- Seconds are whole numbers, but there is no exponent syntax for large
  constants: write `4102444800`, not `4.1e9`.
- `toUnixTime() * 1000 + getMilliseconds()` gives the same scale as
  `timestampInMilliseconds()`.
- Keep values in a sensible range. Very large inputs overflow inside the
  conversion and give a wrong date without an error
  (`fromUnixTime(1000000000000000)` returns `true` and a year near `7879`).

## Invalid dates

The setters accept impossible dates (`2024-02-30`, month `13`, hour `25`).
What happens next depends on the platform:

| Operation on an invalid date | Windows | Linux |
|------------------------------|---------|-------|
| getters | return the stored fields | return the stored fields |
| `getDayOfWeek()` | the stored day of week (usually the one from the clock) | computed after normalizing (`02-30` is read as `03-01`) |
| `toUnixTime()` | a meaningless huge number (`18446744062065078272`) | the normalized date (`02-30` counts as `03-01`) |

Never rely on either behavior: validate dates before storing them in a
`DateTime`.

## Versions before 5.9.1: `new DateTime()` could return an old value

`DateTime` objects are recycled by a memory pool. Before version 5.9.1 a
recycled object was not refreshed, so `new DateTime()` could return the
date of a freed `DateTime` (for example `1970` after another object was set
with `fromUnixTime(0)`), and `(new DateTime()).toUnixTime()` could return
the same number seconds apart. Since 5.9.1 every `new DateTime()` reads the
clock. Scripts that must also run with an older extension can read the
current time from `DateTime.timestampInMilliseconds()`, which always reads
the clock:

```javascript
function now() {
	var d = new DateTime();
	var ms = DateTime.timestampInMilliseconds();
	d.fromUnixTime((ms - ms % 1000) / 1000);
	d.setMilliseconds(ms % 1000);
	return d;
};
```

## Recipes

The recipes below use only the core and this extension (no `Math`), and
were run with the interpreter.

### Format a date

```javascript
function pad(value, size) {
	var text = "" + value;
	while (text.length < size) {
		text = "0" + text;
	};
	return text;
};

function formatDateTime(d) {
	return pad(d.getYear(), 4) + "-" + pad(d.getMonth(), 2) + "-" + pad(d.getDay(), 2) + " " +
	       pad(d.getHour(), 2) + ":" + pad(d.getMinute(), 2) + ":" + pad(d.getSecond(), 2);
};

formatDateTime(new DateTime());              // "2026-10-05 00:08:10"
pad(d.getMilliseconds(), 3);                 // "007"

var dayName = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
dayName[d.getDayOfWeek()];                   // "Monday"

// compact stamp for file names: 20261005-000810
"" + d.getYear() + pad(d.getMonth(), 2) + pad(d.getDay(), 2) + "-" +
	pad(d.getHour(), 2) + pad(d.getMinute(), 2) + pad(d.getSecond(), 2);
```

### Build a given date

```javascript
function makeDateTime(year, month, day, hour, minute, second) {
	var d = new DateTime();
	d.setYear(year);
	d.setMonth(month);
	d.setDay(day);
	d.setHour(hour);
	d.setMinute(minute);
	d.setSecond(second);
	d.setMilliseconds(0);
	return d;
};

var d = makeDateTime(2024, 2, 28, 22, 30, 0);
d.getDayOfWeek();                            // 3 (Wednesday)
```

Set every field: an unset field keeps the current date and time.

### Validate a date

```javascript
function isLeapYear(year) {
	return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
};

function daysInMonth(year, month) {
	if (month == 2) {
		return isLeapYear(year) ? 29 : 28;
	};
	if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
		return 30;
	};
	return 31;
};

function isValidDate(year, month, day) {
	if ((year < 1601) || (year > 30827)) {
		return false;
	};
	if ((month < 1) || (month > 12)) {
		return false;
	};
	return (day >= 1) && (day <= daysInMonth(year, month));
};

isValidDate(2024, 2, 29);     // true
isValidDate(2023, 2, 29);     // false
isValidDate(1900, 2, 29);     // false
```

`1601 .. 30827` is the range Windows can convert; Linux accepts more, but
keep scripts portable.

### Parse `YYYY-MM-DD`

```javascript
function parseDate(text) {
	var part = text.split("-");
	return makeDateTime(Convert.toNumber(part[0]), Convert.toNumber(part[1]), Convert.toNumber(part[2]), 0, 0, 0);
};

formatDateTime(parseDate("2026-08-09"));     // "2026-08-09 00:00:00"
```

Check the parts with `isValidDate` before building the date when the text
comes from outside.

### Add or subtract time

Work in seconds through Unix time. Because the values are local time
counted as UTC, `86400` seconds is always exactly one calendar day, also
across daylight saving changes.

```javascript
function addSeconds(d, seconds) {
	var out = new DateTime();
	out.fromUnixTime(d.toUnixTime() + seconds);
	return out;
};

var d = makeDateTime(2024, 2, 28, 22, 30, 0);
formatDateTime(addSeconds(d, 2 * 3600));     // "2024-02-29 00:30:00"
formatDateTime(addSeconds(d, 2 * 86400));    // "2024-03-01 22:30:00"
formatDateTime(addSeconds(d, -86400));       // "2024-02-27 22:30:00"
```

`fromUnixTime` sets the milliseconds to `0`; copy them back with
`setMilliseconds` if they matter. Subtracting below 1970 fails
(`fromUnixTime` returns `false`).

Months have different lengths, so add months on the fields instead:

```javascript
function addMonths(d, months) {
	var total = d.getYear() * 12 + (d.getMonth() - 1) + months;
	var month = total % 12 + 1;
	var year = (total - (month - 1)) / 12;
	var day = d.getDay();
	var last = daysInMonth(year, month);
	var out = makeDateTime(year, month, (day > last) ? last : day, d.getHour(), d.getMinute(), d.getSecond());
	return out;
};

formatDateTime(addMonths(makeDateTime(2026, 1, 31, 0, 0, 0), 1));    // "2026-02-28 00:00:00"
```

### Difference between two dates

```javascript
var start = makeDateTime(2026, 1, 1, 0, 0, 0);
var end = makeDateTime(2026, 12, 25, 0, 0, 0);
(end.toUnixTime() - start.toUnixTime()) / 86400;    // 358 days
```

### Compare two dates

```javascript
function compareDateTime(a, b) {
	var x = a.toUnixTime() * 1000 + a.getMilliseconds();
	var y = b.toUnixTime() * 1000 + b.getMilliseconds();
	if (x < y) {
		return -1;
	};
	if (x > y) {
		return 1;
	};
	return 0;
};
```

`a == b` is `true` only when `a` and `b` are the same object.

### Copy a `DateTime`

```javascript
function copyDateTime(d) {
	var out = new DateTime();
	out.fromUnixTime(d.toUnixTime());
	out.setMilliseconds(d.getMilliseconds());
	return out;
};

var k = copyDateTime(d);
k.setYear(2000);           // d is unchanged
```

### Start of the day, end of the month

```javascript
var today = new DateTime();
today.setHour(0);
today.setMinute(0);
today.setSecond(0);
today.setMilliseconds(0);            // today at 00:00:00.000

var eom = makeDateTime(2026, 2, 1, 0, 0, 0);
eom.setDay(daysInMonth(eom.getYear(), eom.getMonth()));     // 2026-02-28
```

### Measure elapsed time, time out

```javascript
var start = DateTime.timestampInMilliseconds();
// ... work ...
var elapsed = DateTime.timestampInMilliseconds() - start;   // milliseconds

var deadline = DateTime.timestampInMilliseconds() + 5000;
while (DateTime.timestampInMilliseconds() < deadline) {
	// poll something, then give the CPU back (for example with the Task extension)
};
```

The clock is the local wall clock: if the system time changes (manual
change, time synchronization, end of daylight saving) the difference jumps.
This is how the `Task` extension times its jobs.

### Seed a random generator

```javascript
Script.requireExtension("Random");
var rnd = new Random();
rnd.seed(DateTime.timestampInMilliseconds());
```

Seconds (`toUnixTime()`) repeat for every run started in the same second;
prefer milliseconds. Neither is a secret: do not use it for keys or tokens.

## Not available

- UTC, time zones, daylight saving information, the real Unix time.
- Parsing and formatting functions (`toString`, `toISOString`, `format`,
  `parse`): use the recipes above.
- Arithmetic and comparison methods (`plus`, `minus`, `compare` exist in the
  C++ class `XYO::System::DateTime` but are not exposed to scripts): use
  Unix time.
- Setting the system clock (`setLocalTime` in C++ is not exposed).
- A JavaScript `Date` object: `Date`, `Date.now()`, `getFullYear()` do not
  exist. The equivalents are `DateTime`, `DateTime.timestampInMilliseconds()`
  (local clock) and `getYear()`; months are 1 based here.
