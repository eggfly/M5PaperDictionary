#include "utils.h"
#include <string.h>
#include <limits.h>
#include <Arduino.h>

int strncmpci(const char * str1, const char * str2, size_t num) {
  int ret_code = 0;
  size_t chars_compared = 0;

  if (!str1 || !str2) {
    ret_code = INT_MIN;
    return ret_code;
  }

  while ((chars_compared < num) && (*str1 || *str2))  {
    ret_code = tolower((int)(*str1)) - tolower((int)(*str2));
    if (ret_code != 0)
    {
      break;
    }
    chars_compared++;
    str1++;
    str2++;
  }

  return ret_code;
}

bool startsWithIgnoreCase(const char *pre, const char *str) {
  return strncmpci(pre, str, strlen(pre)) == 0;
}

bool endsWithIgnoreCase(const char* base, const char* str) {
  int blen = strlen(base);
  int slen = strlen(str);
  return (blen >= slen) && (0 == strncmpci(base + blen - slen, str, strlen(str)));
}


float mapToFloat(float x, float a, float b, float c, float d) {
  float f = (x - a) / (b - a) * (d - c) + c;
  return f;
}


double mapToDouble(double x, double a, double b, double c, double d) {
  double f = (x - a) / (b - a) * (d - c) + c;
  return f;
}
