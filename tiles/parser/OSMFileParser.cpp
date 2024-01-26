//
// Created by alina-mlynorik on 8/21/23.
//

#include <sstream>
#include <pugixml.hpp>
#include <iostream>
#include "OSMFileParser.h"

using namespace std;

OSMFileParser::OSMFileParser(const string &fileName, double xScale, double yScale) :  xScale(xScale),
                                                                                   yScale(yScale) {
    file.open(fileName);
}

OSMFileParser::~OSMFileParser() {
    if(file.is_open())
        file.close();
}

void OSMFileParser::parse() {
    string s;
    while (!file.eof()){
        getline(file, s);
        stringstream ss(s);
        pugi::xml_document doc;
        pugi::xml_parse_result res = doc.load(ss);
        if(!res)
            cerr<<"cant read xml : "<<ss.str();
        if(s.find("/>")!=string::npos){

        }else{

        }

    }
}
