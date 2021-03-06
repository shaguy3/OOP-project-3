#include "date.h"

Date::Date() :
    day(0), month(0), year(0) 
{}

Date::Date(int _day, int _month, int _year) :
    day(0), month(0), year(0)
{
    if (_day > 31 || _day < 1) { throw invalid_argument("Date: Invalid day num"); }
    if (_month > 12 || _month < 1) { throw invalid_argument("Date: Invalid month num"); }
    if (_year <= 0) { throw invalid_argument("Date: Invalid year num"); }

    day = _day;
    month = _month;
    year = _year;
}

int Date::getDay() {
    return day;
}

int Date::getMonth() {
    return month;
}

int Date::getYear() {
    return year;
}

bool Date::setDay(int _day) {
    day = _day;
    return true;
}

bool Date::setMonth(int _month) {
    month = _month;
    return true;
}

bool Date::setYear(int _year) {
    year = _year;
    return true;
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.day << '.' << date.month << '.' << date.year;
    return os;
}

Date::~Date() {
}

void Date::save(ostream& out) const
{
    out.write(rcastcc(&day), sizeof(day));
    out.write(rcastcc(&month), sizeof(month));
    out.write(rcastcc(&year), sizeof(year));
}
void Date::load(istream& in)
{
    in.read(rcastc(&day), sizeof(day));
    in.read(rcastc(&month), sizeof(day));
    in.read(rcastc(&year), sizeof(day));
}