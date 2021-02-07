#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//  Return this process's ID
int Process::Pid() { return this->_pid;}

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    int tick = sysconf(_SC_CLK_TCK);
    float totalTime = LinuxParser::ActiveJiffies(this->_pid) / tick;
    auto seconds = LinuxParser::UpTime() - (LinuxParser::UpTime(this->_pid) / tick);
    return totalTime/seconds;
 }

//    Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(this->_pid);
}

//    Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(this->_pid);
 }

//    Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(this->_pid);
 }

//    Return the age of this process (in seconds)
long int Process::UpTime() { 
    int tick = sysconf(_SC_CLK_TCK);
    long seconds = LinuxParser::UpTime() - (LinuxParser::UpTime(this->_pid) / tick);
    return seconds;
}

//    Overload the "less than" comparison operator for Process objects

bool Process::operator<(Process const& a) const {
     return _cpuUtilization > a._cpuUtilization; 
     }