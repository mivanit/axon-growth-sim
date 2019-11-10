//
//  Data_List.hpp
//  Confidence_Calculator
//
//  Created by Connor Puritz on 10/3/18.
//  Copyright © 2018 umich. All rights reserved.
//

#ifndef Data_List_hpp
#define Data_List_hpp

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include "Entry.cpp"
#include "Util.hpp"

class Data_List {
private:
    std::vector<Entry> entry_list;
    Entry column_names;
    
public:
    // Create empty list
    Data_List() {}
    
    // Creates list based on the CSV file @arg1. Calls read_csv().
    Data_List(const std::string&);
    
    // Reads in and parses a CSV file named @arg1. Overwrites all private members.
    void read_csv(const std::string&);
    
    // Writes 'entry_list' back into a CSV file named @arg1.
    void write_to_csv(const std::string&) const;
    
    // Add a new column to the list
    void add_column(const std::string& name) { column_names.push_back(name); }
    
    // Add an entry to the list
    void add_entry(const Entry& e) { entry_list.push_back(e); }
    
    // Returns the list of entries
    std::vector<Entry>& get_entry_list() { return entry_list; }
    
    // Returns the names of the columns
    Entry get_column_names() const { return column_names; }
};

#endif
