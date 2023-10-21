#include "read_example_csv.hpp"
#include "lazycsv.hpp"
#include <stdio.h>
#include <iostream>
#include <string>

ExampleCsvReader::ExampleCsvReader(std::string path){
    SetPath(path);
}

void ExampleCsvReader::SetPath(std::string path){
    csv_path = path;
}

void ExampleCsvReader::PrintPath(){
    std::cout << csv_path << std::endl;
}

std::string ExampleCsvReader::ReturnPath(){
    return(csv_path);
}

void ExampleCsvReader::ReadCsv(){
    lazycsv::parser parser{csv_path};
    for (const auto row:parser){
        const auto [day, year, month, measurnment] = row.cells(0,1,2,4);
        std::cout<<day.raw()<<" "<<year.raw()<<" "<<month.raw()<<" "<<measurnment.raw()<<std::endl;
    }

}

