//
//  Data_List.cpp
//  Confidence_Calculator
//
//  Created by Connor Puritz on 10/3/18.
//  Copyright © 2018 umich. All rights reserved.
//

#include "Data_List.hpp"

Data_List::Data_List(const std::string& file) {
    read_csv(file);
}

/**************************************************************/

void Data_List::read_csv(const std::string& in_file) {
    std::ifstream ifs(in_file);
    //handle_fatal_error("Error opening " + in_file + ".", !ifs.is_open());
    entry_list.clear();
    
    std::string entry;
    if (std::getline(ifs, entry)) {
        column_names = Entry(entry);
    }
    while (std::getline(ifs, entry)) {
        entry_list.push_back(Entry(entry));
    }
}

/**************************************************************/

void Data_List::write_to_csv(const std::string& out_file) const {
    std::ofstream ofs(out_file);
    //handle_fatal_error("Error opening " + out_file + ".", !ofs.is_open());
    ofs << column_names.print() << "\n";
    for (Entry e:entry_list) {
        ofs << e.print() << "\n";
    }
}

/**************************************************************/
