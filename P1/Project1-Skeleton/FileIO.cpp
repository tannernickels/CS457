#include "FileIO.h"

int FileIO::read(const string& filename){
    string current = "";
    ifstream istr(filename);
        if (istr.fail()) return -1;
        
        istr >> current;
        //data += current;
        while (!istr.fail()){
            
            data += current + " ";
            istr >> current;
            
            
        }
        return 0;
}

int FileIO::write(const string& filename, string& data){
    return 0;
}
