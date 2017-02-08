#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

template<typename Out>
void split(const std::string &s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

std::map<string, string> load_conversion(const char* path) {
  std::map<string, string> table;
  {
    std::ifstream file(path);
    string line;
    if (file.is_open()) {
      while (std::getline(file, line)) {
        vector<string> tokens = split(line, '\t');
        table.insert(std::make_pair(tokens[1], tokens[0]));
        table.insert(std::make_pair(tokens[2], tokens[0]));
      }
    }
    file.close();
  }
  return table;
}

int main(int argc, char** argv) {
  if (argc <= 1) {
    cerr << "Usage: unescape [escape_table] < stdin" << endl;
    return 1;
  }
  std::map<string, string> conversion_table = load_conversion(argv[1]);
  
  string line;
  while (std::getline(std::cin, line)) {
    int offset = -1;
    for (size_t i=0; i < line.size(); ++i) {
      char c = line[i];
      if (c == '&') {
        offset = i;
      } else if (c == ';') {
        string tag = line.substr(offset, i+1);
        auto ptr = conversion_table.find(tag);
        string replace;
        if (ptr != conversion_table.end()) {
          tag = ptr->second;
        }
        cout << tag;
        offset = -1;
      } else if (offset == -1) {
        cout << c;
      }
    }
    if (offset != -1) {
      cout << line.substr(offset, line.size());
    }
    cout << endl;
  }
}
