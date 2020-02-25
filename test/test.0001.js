// Public domain
// http://unlicense.org/

Script.requireExtension("Console");
Script.requireExtension("DateTime");
Script.requireExtension("Math");

var startTimestamp = DateTime.timestampInMilliseconds();

var tick = 0;
for(var k = 0; k < 1000 * 1000; ++k) {
	++tick;
};

var endTimestamp = DateTime.timestampInMilliseconds();
var deltaTimestamp = endTimestamp - startTimestamp;
var operationsPerMillisecond = Math.floor(tick / deltaTimestamp);

Console.writeLn("Operations per millisecond: " + operationsPerMillisecond);
Console.writeLn("-> test 0001 ok");
