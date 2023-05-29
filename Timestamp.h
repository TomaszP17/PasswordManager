#ifndef MENADZERHASEL_TIMESTAMP_H
#define MENADZERHASEL_TIMESTAMP_H

#include <string>

class Timestamp {
private:
    int hh;
    int mm;
    int ss;
public:
    Timestamp();
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;
};
#endif //MENADZERHASEL_TIMESTAMP_H