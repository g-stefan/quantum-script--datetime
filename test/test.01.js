// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("DateTime");

var date=new DateTime();
Console.writeLn(date.getYear()+"-"+date.getMonth()+"-"+date.getDay()+" "+
	date.getHour()+":"+date.getMinute()+":"+date.getSecond());
	
