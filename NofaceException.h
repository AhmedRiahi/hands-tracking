#ifndef __PFA__NofaceException__
#define __PFA__NofaceException__

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class NofaceException : public exception{
    public :
    string getMessage();
};

#endif /* defined(__PFA__NofaceException__) */
