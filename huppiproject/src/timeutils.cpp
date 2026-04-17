#include "timeutils.h"
#include <time.h>

void timeInit() {
  configTime(0, 0, "pool.ntp.org");

  setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 1);
  tzset();

  while (time(nullptr) < 100000) {
    delay(500);
  }
}

int getCurrentHour() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);
  return t->tm_hour;
}

int getCurrentMinute() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);
  return t->tm_min;
}

int getCurrentDay() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);
  return t->tm_mday;
}

String getCurrentTime() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  char buf[6];
  sprintf(buf, "%02d:%02d", t->tm_hour, t->tm_min);

  return String(buf);
}

String getDisplayDate() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  char buf[20];
  sprintf(buf, "%02d.%02d.%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

  return String(buf);
}

String getApiDate() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  char buf[20];
  sprintf(buf, "%04d/%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

  return String(buf);
}
