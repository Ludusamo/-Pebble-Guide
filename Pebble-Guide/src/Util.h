#ifndef UTIL_H
#define UTIL_H

#include <pebble.h>

static char intToChar(int i) {
	return (char) i + 48;	
}

static char *twoDigitToString(int i) {
	char *output = "00";
  int firstDigit = i / 10;
  int secondDigit = i % 10;
	output[0] = intToChar(firstDigit);
  output[1] = intToChar(secondDigit);
	return output;
}

static int twoDigitStringToInt(char *str) {
  return (int) (((str[0] - 48) * 10) + (str[1] - 48));
}

static char *minuteToTime(int min) {
	int hour = min / 60;
	int minute = min % 60;
	char *timeString = "00:00";
	timeString[0] = intToChar(hour / 10);
	timeString[1] = intToChar(hour % 10);
	timeString[3] = intToChar(minute / 10);
	timeString[4] = intToChar(minute % 10);
	return timeString;
}

#endif //UTIL_H
