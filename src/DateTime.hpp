#pragma once
#include <string>
#include <sstream>

typedef unsigned long long int64;

class DateTime
{
    /* Хранит дату и время в секундах */
private:
    int64 seconds;

public:
    DateTime(int64 sec = 0) : seconds(sec) {};
    DateTime(
        u_int day, u_int month, u_int year,
        u_int hours = 0, u_int minutes = 0, u_int seconds = 0) : seconds((year - 1970) * 31536000 + (month - 1) * 2592000 + (day - 1) * 86400 + hours * 3600 + minutes * 60 + seconds) {};

    std::string toString()
    {
        /* Возвращет дату в виде строки */
        int64 tmp = seconds;
        u_int year = tmp / 31536000 + 1970;
        tmp %= 31536000;
        u_int month = tmp / 2592000 + 1;
        tmp %= 2592000;
        u_int day = tmp / 86400 + 1;
        tmp %= 86400;
        u_int hours = tmp / 3600;
        tmp %= 3600;
        u_int minutes = tmp / 60;

        char result[20];
        snprintf(result, 20, "%04d-%02d-%02d %02d:%02d", year, month, day, hours, minutes);
        return std::string(result);
    }

    DateTime(std::string str)
    {
        /*
            Принимает дату в виде строки формата "DD-MM-YYYY H:M:S"
            и переводит в секунды
        */
        std::stringstream ss(str);

        u_int year;
        u_int month;
        u_int day;
        u_int hours = 0;
        u_int minutes = 0;
        u_int seconds = 0;

        char delim;

        ss >> year >> delim;
        ss >> month >> delim;
        ss >> day >> delim;
        ss >> hours >> delim;
        ss >> minutes >> delim;
        ss >> seconds >> delim;

        this->seconds = (year - 1970) * 31536000 + (month - 1) * 2592000 + (day - 1) * 86400 + hours * 3600 + minutes * 60 + seconds;
    }

    int64 getSeconds() const { return seconds; };
};

class DateTimeRange
{
    /* Хранит временной промежуток */
private:
    DateTime start;
    DateTime end;

public:
    DateTimeRange(DateTime start, DateTime end) : start(start), end(end) {};

    bool overlaps(const DateTimeRange &other) const
    {
        return !(end.getSeconds() < other.start.getSeconds() || start.getSeconds() > other.end.getSeconds());
    }
    DateTime getStart()
    {
        return start;
    }
    DateTime getEnd()
    {
        return end;
    }
};