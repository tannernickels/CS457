#include "FileIO.h"

void FileIO::openFile(string& filename, string& option){
    if(option.size()!=1){
        cout << "INVALID FILE OPTION: \"r\" to read, \"w\" to write, or \"a\" to append to file" << endl;
    }
    switch(option[0]){
        case 'r':
            istr.open(filename);
            break;
        case 'w':
            ostr.open(filename);
            break;
        case 'a':
            ostr.open(filename, ofstream::app);
            break;
    }
}

void FileIO::close(){
    if (istr.is_open()) istr.close();
    if (ostr.is_open()) ostr.close();
}

string FileIO::readFull(){
    string buf = "";
    string current = "";
        if (istr.fail()) return buf;
        
        //istr >> current;
        //data += current;
        while (getline(istr, current)){
            
            buf += current + "\n";
            //istr >> current;
            
            
        }
        return buf;
}


string FileIO::nextLine(){
    string line = "";
    if(getline(istr, line)){
        return line;
    }
    return "";
}

string FileIO::nextToken(){
    string token = "";
    if (!istr.fail()){
        istr >> token;
        return token;
    }
    return "";
}

//convert a line into its individual tokens and return them in a vector. Useful for 1st line of client test
vector<string> FileIO::tokenizeLine(){
    string line = nextLine();
    istringstream iss(line);
    vector<string> tokens;
    do{
        string buf;
        iss >> buf;
        if(buf.size()>0){
            tokens.push_back(buf);
        }
    }while(iss);

    return tokens;
}
    
//convert a line into a key/value pair. Useful for users.txt. (username: userData).
pair<string, string> FileIO::packageLine(){
    string key = nextToken();
    string value = nextLine();
    return make_pair(key, value);
}

StringMap FileIO::getConfig(){
    StringMap config;
    string line = "";
    string key, value;
    while(!istr.fail()){
        key = nextToken();
        value = nextToken();
        if(key.size()==0){
            continue;
        }
        config.insert(make_pair(key, value));
    }
    return config;
}

void FileIO::printStringMap(StringMap& config){
    cout << "PRINTING CONFIGURATION" << endl;
    for(StringMapIt it = config.begin(); it!=config.end(); ++it){
            cout << it->first << ": " << it->second << endl;
    }
}

vector<string> FileIO::getList(){
    string token = "";
    vector<string> listItems;
    while (!istr.fail()){
        token = nextToken();
        if(token.size()==0) continue;
        listItems.push_back(token);
    }
    return listItems;
}

vector<string> FileIO::getEveryLine(){
    string line;
    vector<string> list;
    while(!istr.fail()){
        line = nextLine();
        if(line.size() == 0) continue;
        list.push_back(line);
    }
    return list;
}

map<string, string> FileIO::readUsersTXT(){
    map<string, string> data;
    while(!istr.fail()){
        pair<string, string> userinfo = packageLine();
        if(userinfo.first.empty() || userinfo.second.empty()) continue;
        userinfo.second = userinfo.second.substr(1); // strip white space delimeter
        data.insert(userinfo);
    }
    return data;
}

void FileIO::writeString(string& s){
    ostr << s;
}

void FileIO::writeMap(StringMap& string_map){
    for(StringMapIt it = string_map.begin(); it != string_map.end(); ++it){
        ostr << it->first << " " << it->second << "\n";
    }
}

void FileIO::writeList(vector<string>& list){
    for(unsigned int i = 0; i < list.size(); i++){
        ostr << list[i] << "\n";
    }
}

void FileIO::writeConfig(StringMap& config){
    for(StringMapIt it = config.begin(); it != config.end(); ++it){
        ostr << it->first << "\t" << it->second << "\n";
    }
}

int main(){
    
    FileIO write("test.txt", "w");
    vector<string> list;
    list.push_back("Apple");
    list.push_back("Banana");
    list.push_back("Racecar");
    StringMap s_map;
    s_map.insert(make_pair("Apple", "Very yummy"));
    s_map.insert(make_pair("Banana", "Pretty good"));
    s_map.insert(make_pair("Racecar", "VROOOOOM VROOOOOOOOM"));

    write.writeConfig(s_map);

}