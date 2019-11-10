//
//  Entry.cpp
//
//  Created by Connor Puritz on 10/4/18.
//  Copyright © 2018 umich. All rights reserved.
//

#include <vector>
#include <sstream>
#include <iostream>

class Entry {
private:
    std::vector<float> fields;
    
    // Parses a CSV entry @arg into individual fields and adds them
    // to 'fields'. This function overwrites 'fields'.
    void parse_csv_entry(const std::string& s) {
        fields.clear();
        std::stringstream ss(s);
        std::string s;
        while (std::getline(ss, s, ',')) {
            // Last field for an entry often has a newline or carriage return
            // character, which will break several functions. So delete it.
            if (s.back() == '\r' || s.back() == '\n') {
                s.pop_back();
            }
            fields.push_back(std::stof(f));
        }
    }
    
public:
    Entry() {}
    Entry(const std::string& s) { parse_csv_entry(s); }
    
    std::string at(size_t j) const { return fields[j]; }
    std::vector<std::string> get_fields() const { return fields; }
    
    void push_back(const std::string& data) {
        fields.push_back(data);
    }
    
    std::string print() const {
        std::string output;
        for (std::string s:fields) {
            output += s + ",";
        }
        return output.substr(0, output.length() - 1);
    }
};
