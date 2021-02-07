#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) {
    int hour = seconds/3600;
    int second = seconds % 60;
int minute = (seconds - hour * 3600) / 60;

string SS = std::to_string(second);
string MM = std:: to_string(minute);
string HH = std:: to_string(hour);

SS = second < 10 ? "0" + SS: SS;
MM = minute < 10 ? "0" + MM: MM;
HH = hour   < 10 ? "0" + HH: HH; 

return HH +":" + MM + ":" + SS;
 }