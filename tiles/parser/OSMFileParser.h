//
// Created by alina-mlynorik on 8/21/23.
//

#ifndef MAPDOWNLOADER_OSMFILEPARSER_H
#define MAPDOWNLOADER_OSMFILEPARSER_H

#include <fstream>

using namespace std;

class OSMFileParser {
private:
    ifstream file;
    double xScale;
    double yScale;
public:
    OSMFileParser(const string &fileName, double xScale, double yScale);
    void parse();
    virtual ~OSMFileParser();

};


#endif //MAPDOWNLOADER_OSMFILEPARSER_H
