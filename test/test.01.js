// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("DateTime");

var date=new DateTime();
Console.writeLn(date.getYear()+"-"+date.getMonth()+"-"+date.getDay()+" "+
	date.getHour()+":"+date.getMinute()+":"+date.getSecond());

// Regression: new DateTime() must be the current time,
// also when the object is recycled by the memory pool
function oldDateTime() {
	var x = new DateTime();
	x.fromUnixTime(0);
	return x.getYear();
};

var k;
var diff;
for (k = 0; k < 16; ++k) {
	oldDateTime();
	date = new DateTime();
	if (date.getYear() == 1970) {
		throw(new Error("new DateTime() returned a recycled value"));
	};
	diff = date.toUnixTime() * 1000 - DateTime.timestampInMilliseconds();
	if ((diff < -2000) || (diff > 2000)) {
		throw(new Error("new DateTime() is not the current time"));
	};
};

Console.writeLn("new DateTime() - ok");
