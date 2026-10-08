# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...) and `quantum-script--console`
must be installed to the SDK first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`fabricare.json` declares three projects:

| Project | Kind | Purpose |
|---------|------|---------|
| `quantum-script--datetime` | `dll-or-lib`: shared library in a dynamic build, static library in a static build | the extension |
| `quantum-script--datetime.static` | static library, static CRT | self-contained hosts built with `quantum-script.static` (fabricare itself) |
| `test.01` | executable, category `test` | runs `test/test.01.js` with the extension registered as internal |

After `fabricare install`, `quantum-script--datetime.dll` (Windows) /
`libquantum-script--datetime.so` (Linux) sits in the SDK `bin` folder next
to `quantum-script.exe`, which is where `Script.requireExtension("DateTime")`
finds it.

## 2. Use it from a script

```javascript
Script.requireExtension("Console");
Script.requireExtension("DateTime");

var now = new DateTime();
Console.writeLn(now.getYear() + "-" + now.getMonth() + "-" + now.getDay() + " " +
	now.getHour() + ":" + now.getMinute() + ":" + now.getSecond());
// 2026-10-5 0:8:10

var names = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
Console.writeLn(names[now.getDayOfWeek()]);         // Monday

var start = DateTime.timestampInMilliseconds();
// ... work ...
Console.writeLn("took " + (DateTime.timestampInMilliseconds() - start) + " ms");
```

Run it with:

```bash
quantum-script hello-datetime.js
```

`Script.requireExtension("DateTime")` looks for an external
`quantum-script--datetime` library first (the file as named, then every
include path folder: next to the interpreter, next to the script), then for
an internal extension registered by the host. Loading twice does nothing. A
missing extension throws `Unable to open "DateTime"`.

Until the extension is loaded `DateTime` does not exist: `typeof(DateTime)`
is `"undefined"` and `new DateTime()` throws. After loading,
`typeof(DateTime)` is `"Function"` and `typeof(new DateTime())` is
`"DateTime"`.

Numbers print without zero padding (`2026-10-5 0:8:10`); see the
[formatting recipe](script-api.md#format-a-date) for `2026-10-05 00:08:10`.

## 3. fabricare and magnet scripts

`fabricare` registers `DateTime` as an internal extension and its startup
script already calls `Script.requireExtension("DateTime")`, so build scripts
(`fabricare/*.js`) can use `DateTime` directly:

```javascript
// fabricare/make.js
var tempFileBase = Shell.getenv("TEMP") + "\\my-tool." + (new DateTime()).toUnixTime() + ".";
```

`magnet` (through `quantum-script--magnet`) registers `DateTime` as an
internal extension too; its scripts call
`Script.requireExtension("DateTime")` and need no DLL. The `Task` and
`SSHRemote` extensions require `DateTime` themselves, so loading them loads
`DateTime`.

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `DateTime` available as an internal
extension by registering it in the init callback (this is what
`test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/DateTime.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::DateTime::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeString(
		        "Script.requireExtension(\"Console\");"
		        "Script.requireExtension(\"DateTime\");"
		        "Console.writeLn((new DateTime()).getYear());")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("DateTime")`. With the DLL build of the engine an
external `quantum-script--datetime.dll` found on the include path wins over
the internal one for `requireExtension`; use
`Script.requireInternalExtension("DateTime")` to force the internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--datetime"
	]
}
```

`quantum-script--datetime` depends on `quantum-script` and
`quantum-script--console`; fabricare resolves them transitively.

## 5. Static builds

For a self-contained executable depend on the static variants and the static
CRT:

```json
{
	"name": "my-host.static",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script.static",
		"quantum-script--console.static",
		"quantum-script--datetime.static"
	],
	"crt": "static"
}
```

`quantum-script--datetime.static` exports the define
`XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_LIBRARY` to its consumers, which turns
`XYO_QUANTUMSCRIPT_EXTENSION_DATETIME_EXPORT` into nothing and leaves out the
`quantumScriptExtension` DLL entry point. The `dll-or-lib` project
`quantum-script--datetime` does the same on a static platform (for example
`win64-msvc-2026.static`). A static host must register the extension with
`registerInternalExtension` (section 4): external DLLs cannot be loaded into
a host that does not use the engine DLL.

## 6. Threads

Each thread that runs scripts has its own engine, so every thread loads the
extension itself with `Script.requireExtension("DateTime")`. Each thread
gets its own `DateTime` prototype (the context is a per-thread singleton)
and the methods only touch the object they are called on, so separate
threads never share state. Do not pass one `DateTime` object between threads;
pass `toUnixTime()` (a number) and rebuild it with `fromUnixTime` on the
other side.
