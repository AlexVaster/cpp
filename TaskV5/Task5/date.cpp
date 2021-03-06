#include <iostream>
#include "date.h"
#include <ctime>

DateTime::DateTime() {
    today();
}
DateTime::DateTime(DateTime& date) {
    second = date.second;
    minute = date.minute;
    hour = date.hour;
    day = date.day;
    month = date.month;
    year = date.year;
}
DateTime::DateTime(int dd, int MM, int yyyy, int hh, int mm, int ss) {
    day = dd;
    month = MM;
    year = yyyy;
    hour = hh;
    minute = mm;
    second = ss;
    if (!isValid()) today();
}
DateTime& DateTime::setTimer(int h, int m, int s) {
    int timerHour, timerMin, timerSec;
    if (h > 0) timerHour = h;
    else timerHour = 0;
    if (m > 0) timerMin = m;
    else timerMin = 0;
    if (s > 0) timerSec = s;
    else timerSec = 0;
    
    int temp = 0;
    long allSeconds = (day * 86400) + ((timerHour + hour) * 3600)
        + ((timerMin + minute) * 60) + timerSec + second;
    if ((allSeconds / 86400) > day) {
        day = allSeconds / 86400;
        if (day > 30) {
            temp = day / 30;
            month = month + temp;
            day -= 30 * temp;
            if (month > 12) {
                temp = year / 12;
                year = year + temp;
                month -= 12 * temp;
            }
        }
    }
    allSeconds %= 86400;
    hour = allSeconds / 3600;
    allSeconds %= 3600;
    minute = allSeconds / 60;
    allSeconds %= 60;
    second = allSeconds;
    DateTime result(day, month, year, hour, minute, second);
    return result;
}
void DateTime::today() {
    struct tm localtm;
    time_t now = ::time(0);
    localtime_s(&localtm, &now);
    second = localtm.tm_sec;
    minute = localtm.tm_min;
    hour = localtm.tm_hour;
    day = localtm.tm_mday;
    month = localtm.tm_mon + 1;
    year = localtm.tm_year + 1900;
}
std::string DateTime::toString(std::string format) const {
    int n;
    std::string s;
    n = (int)format.find("dd");
    if (n > -1) {
        s = std::to_string(day);
        if (day < 10) {
            s = s.insert(0, "0");
        }
        format.replace(n, 2, s);
    }
    n = (int)format.find("MM");
    if (n > -1) {
        s = std::to_string(month);
        if (month < 10) {
            s = s.insert(0, "0");
        }
        format.replace(n, 2, s);
    }
    n = (int)format.find("yyyy");
    if (n > -1) {
        s = std::to_string(year);
        format.replace(n, 4, s);
    }
    n = (int)format.find("hh");
    if (n > -1) {
        s = std::to_string(hour);
        if (hour < 10) {
            s = s.insert(0, "0");
        }
        format.replace(n, 2, s);
    }
    n = (int)format.find("mm");
    if (n > -1) {
        s = std::to_string(minute);
        if (minute < 10) {
            s = s.insert(0, "0");
        }
        format.replace(n, 2, s);
    }
    n = (int)format.find("ss");
    if (n > -1) {
        s = std::to_string(second);
        if (second < 10) {
            s = s.insert(0, "0");
        }
        format.replace(n, 2, s);
    }
    return format;
}
bool DateTime::isValid() {
    return ((isValidDate(day, month, year)) && (isValidTime(hour, minute, second)));
}
bool DateTime::isValidDate(int day, int month, int year) {
    if (((month < 1) || (month > 12)) || (year < 0)) {
        return false;
    }
    if (((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12)) && ((day < 1) || (day > 31))) {
        return false;
    }
    if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && ((day < 1) || (day > 30))) {
        return false;
    }
    if ((year % 4) == 0) {
        if ((year % 100) != 0) {
            if ((month == 2) && ((day < 1) || (day > 29))) {
                return false;
            }
        }
        else if ((year % 400) == 0) {
            if ((month == 2) && ((day < 1) || (day > 29))) {
                return false;
            }
        }
        else if ((month == 2) && ((day < 1) || (day > 28))) {
            return false;
        }
    }
    else if ((month == 2) && ((day < 1) || (day > 28))) {
        return false;
    }
    return true;
}
bool DateTime::isValidTime(int hour, int minute, int second) {
    return !(((minute < 0) || (minute > 59)) || ((hour < 0) || (hour > 24)) || ((second < 0) || (second > 60)));
}
std::string DateTime::time() const {
    return toString("hh:mm:ss");
}
std::string DateTime::date() const {
    return toString("dd.MM.yyyy");
}
DateTime DateTime::fromString(std::string s) {
    std::string time = s.substr(0, s.find(" "));
    std::string date = s.substr(s.find(" ")+1, s.length());
    int dd = 0, MM = 0, yyyy = 0, hh = 0, mm = 0, ss = 0;
    if (date.find(':') == 2) {
        std::string temp = time;
        time = date;
        date = temp;
    }
    hh = std::stoi(time.substr(0, 2));
    mm = std::stoi(time.substr(3, 4));
    ss = std::stoi(time.substr(6, 7));
    dd = std::stoi(date.substr(0, 2));
    MM = std::stoi(date.substr(3, 4));
    yyyy = std::stoi(date.substr(6, 9));
    DateTime newDate(dd, MM, yyyy, hh, mm, ss);
    return newDate;
}
std::istream& operator>>(std::istream& in, DateTime& timer) {
    char delim;
    in >> timer.hour >> delim >> timer.minute >> delim >> timer.second 
       >> timer.day >> delim >> timer.month >> delim >> timer.year;
    if (!timer.isValid()) timer.today();
    return in;
}
std::ostream& operator<<(std::ostream& out, const DateTime& timer) {
    out << timer.date() << " " << timer.time() << "\n";
    return out;
}   