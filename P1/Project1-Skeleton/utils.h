#include <assert.h>
#include <string> 
using std::string;


namespace util
{

    //https://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number
    //int roundUpMultipleOfTwo(int numToRound, int multiple) 
    //{
        //positive only 
      //    assert(multiple && ((multiple & (multiple - 1)) == 0));
       //   return (numToRound + multiple - 1) & -multiple;
   // }

    string getPwd(string userData){
        return userData.substr(0, userData.find(' '));
    }

    // DONT GRIEF ME TANNER, IT DOES WHAT IT SHOULD DO...
    string getChannelDescription(string attributes){
        return attributes.substr(0, attributes.find(' '));
	
    }
    string getChannelPass(string attributes){
        return attributes.substr(attributes.find(' ') + 1, attributes.size());
    }

}