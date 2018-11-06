#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

typedef map<string, string> StringMap;
typedef map<string, string>::iterator StringMapIt;

/* Wrapper class for ifstream file reading. Provides useful methods for extracting various data formats from different files
 *  -Constructor: accepts a filename to read from. This way, a new instance of FileIO must be created for each file to be read
 */
class FileIO{

public:
    
    FileIO(string filename, string option){openFile(filename, option);}
    ~FileIO(){close();}
    
    //METHODS FOR READING FROM FILE
    //reads an entire file into a string as is, including whitespace and newlines. useful for banner.txt
    string readFull();
    //reads the next line in the stream and returns it
    string nextLine();
    //reads the next token in the stream (split on whitespace) and returns it
    string nextToken();
    
    //convert a line into its individual tokens and return them in a vector. Useful for 1st line of client test
    vector<string> tokenizeLine();
    
    //convert a line into a key/value pair. Useful for users.txt. (username: userData).
    pair<string, string> packageLine();
    
    //returns a map containing key/value pairs read from a '.conf' file. Assumes one key/value per line
    StringMap getConfig();
    
    void printStringMap(StringMap& config);
    
    //returns a vector of strings from a file that lists items. i.e. bannedusers.txt, channels.txt
    vector<string> getList();
    
    //returns a vector of strings where every element is an entire line from the provided file
    vector<string> getEveryLine();

    map<string, string> readUsersTXT();

    //METHODS FOR WRITING TO FILE
    //Writes a string as is to the file (i.e. does not append anything to string)
    void writeString(string& s);

    //Writes a StringMap object to file, appending a newline after each entry
    void writeMap(StringMap& string_map);

    //Writes a vector to file, appending a newline after each item
    void writeList(vector<string>& list);

    //Nearly identical to writeMap(). Separates entries by tab instead of space
    void writeConfig(StringMap& config);

    void writePair(pair<string, string> string_pair);
    
    void close();
private:

    ifstream istr;
    ofstream ostr;
        
    // Opens the file depending on "r" or "w" to read or write to file, and "a" to append to a file.
    // Accessed by the constructor
    void openFile(string& filename, string& option);
    
    
};

#endif
