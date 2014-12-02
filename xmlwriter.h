#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <fstream>

using namespace std;

class XMLWriter
{
public:
    XMLWriter();
    void write(string);
    ofstream file;
};

#endif // XMLWRITER_H
