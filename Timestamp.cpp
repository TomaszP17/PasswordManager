#include "Timestamp.h"
#include <ctime>

Timestamp::Timestamp() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    hh = timeinfo->tm_hour;
    mm = timeinfo->tm_min;
    ss = timeinfo->tm_sec;
}

int Timestamp::getHours() const {
    return hh;
}

int Timestamp::getMinutes() const {
    return mm;
}

int Timestamp::getSeconds() const {
    return ss;
}
