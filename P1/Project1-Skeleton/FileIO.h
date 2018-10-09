#ifndef FILE_IO_INCLUDE
#define FILE_IO_INCLUDE

#include <iostream>
using std::istream;
using std::ostream;
using std::ios;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
using std::to_string;


class FileIO{
    
public:
    
    FileIO() : data(""){};
    
    int read(const string& filename);
    int write(const string& filename, string& data);
    
    string get(){return data;};
    
private: 
    
    string data;
    
};

#endif
