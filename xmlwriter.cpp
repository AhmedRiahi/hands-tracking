#include "xmlwriter.h"

XMLWriter::XMLWriter()
{
    this->file.open("out.xml");
}

void XMLWriter::write(string out){
    this->file << out;
}
