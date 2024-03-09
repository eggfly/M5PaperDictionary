#pragma once

#include <stddef.h>

#define RGB565(r,g,b) (((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3))


int strncmpci(const char * str1, const char * str2, size_t num);

bool startsWithIgnoreCase(const char *pre, const char *str);

bool endsWithIgnoreCase(const char* base, const char* str);

float mapToFloat(float x, float a, float b, float c, float d);

double mapToDouble(double x, double a, double b, double c, double d);
