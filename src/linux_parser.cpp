#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//  An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >>  version >> kernel;
  }
  return kernel;
}

// use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//  Read and return the system memory utilization 
float LinuxParser::MemoryUtilization() { 
string key, line, value;
float MemTotal, MemFree, Buffers, Cached;
std::ifstream filestream(kProcDirectory + kMeminfoFilename);
if (filestream.is_open()){
  while(std::getline(filestream, line)){
std::istringstream linestream(line);

while (linestream >> key >> value){
if (key == "MemTotal:"){
  MemTotal = stof(value);
}
else if (key == "MemFree:"){
  MemFree = stof(value);
}
else if (key == "Buffers:"){
  Buffers = stof(value);
}
else if (key == "Cached:"){
  Cached = stof(value);
}
}
  }
}
return (MemTotal - MemFree - Buffers - Cached)/MemTotal;

 }

// Read and return the system uptime
long int LinuxParser::UpTime() {
string uptime, line, idletime;
std::ifstream filestream(kProcDirectory + kUptimeFilename);
if (filestream.is_open()){
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> uptime >> idletime; 
}
return stol(uptime);
 }

//  Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long total_jiff, value;
  string cpu, line;
  std::ifstream filestream (kProcDirectory + kStatFilename);
if (filestream.is_open()){
  std::istringstream linestream(line);
  std::getline(linestream, line);
  linestream >> cpu; 
  while (linestream >> value){
    total_jiff += value;
  }
}
  return total_jiff;
}

// Read and return the number of active jiffies for a PID

long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  vector<string> stats;
std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
if (filestream.is_open()){
  std::getline(filestream, line);
  std::istringstream linestream(line);
  while(linestream >> value){
    stats.emplace_back(value);
  }
  long utime = std::stol(stats[13]);
  long stime = std::stol(stats[14]);
  long cutime = std::stol(stats[15]);
  long cstime = std::stol(stats[16]);

return utime + stime + cutime + cstime;
}
return 0;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> jiffies;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream (line);
    while (linestream >> key){
      if (key != "cpu"){
        jiffies.emplace_back(std::stol(key));
      }
    }
  }
  return jiffies[0] + jiffies[1] + jiffies[2];
}

//  Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector <long> jiffies;
  string line, key;
std::ifstream filestream(kProcDirectory + kStatFilename);
if (filestream.is_open()){
std::getline(filestream, line);
std::istringstream  linestream(line);
while (linestream >> key){
  if (key != "cpu"){
    jiffies.emplace_back(std::stol(key));
  }
}
}
  return jiffies[3] + jiffies[4];
 }

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, key;
  vector<string>jiffies; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key){
      if (key != "cpu") {
        jiffies.emplace_back(key);
      }
    }

  }
  return jiffies;
 }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
if (filestream.is_open()){
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while (linestream >> key >> value){
      if (key == "processes"){
        return std::stoi(value);
      }
    }
  }
  
}
return 0;
 }

//   Read and return the number of running processes
int LinuxParser::RunningProcesses() {
   string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
if (filestream.is_open()){
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    while (linestream >> key >> value){
      if (key == "procs_running"){
        return std::stoi(value);
      }
    }
  }
  
}
return 0;
  
}

//   Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
string line;
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
  }
  return line;
}

//   Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 
string line, value, key;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key == "VmSize:"){
          return std::to_string(std::stoi(value) / 1024);
        }
      }
    }
  }
  return value;
 }

//   Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "Uid:"){
          return value;
        }
      }
    }
  } 
  return value;
}
 
//   Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
string line,name, pidNumber;
  std::ifstream filestream (kPasswordPath);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(),'x', ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> pidNumber){
        if (pidNumber == uid){
          return name;
        }
      }
    }
  }
  return name;
 }

//   Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
string line, value;
vector<string> stats;
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      stats.emplace_back(value);
    }
    return std::stol(stats[21]);
  }
  return 0;
}