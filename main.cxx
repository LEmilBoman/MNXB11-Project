#include <iostream>
#include <optional>
#include <vector>
#include "external/include/lazycsv.hpp"
#include "external/include/argumentum/argparse.h"
#include "external/include/date.h"
#include "include/tempDelta.h"

using namespace date;
using namespace std::chrono;

void ParseExample(std::string csv_file){
  //Parser over the csv file and index for all variables of interest.
  lazycsv::parser lazyParser{csv_file};
  auto d_index = lazyParser.index_of("day");
  auto y_index = lazyParser.index_of("year");
  auto m_index = lazyParser.index_of("month");
  auto v_index = lazyParser.index_of("measurement");
  
  //loop over each line in the csv file.
  for (const auto line : lazyParser){
    //extract the day, year, month, and measurement as strings.
    const auto [d,y,m,v] = line.cells(d_index,y_index,m_index,v_index);
    std::string tempd{d.trimed()};
    std::string tempy{y.trimed()};
    std::string tempm{m.trimed()};
    std::string tempmea{v.trimed()};
    
    //the date based on strings from csv file.
    auto date = year{stoi(tempy)}/stoi(tempm)/stoi(tempd);

    //check if valid date
    if (!date.ok()){
      std::cout << date << std::endl;
    } else{
      //convert to date with weekday and print
      date::year_month_weekday date_wd{date};
      std::cout << date_wd << " " << tempmea << std::endl;
    }
}
}


int main(int argc, char *argv[]) {
  auto parser{argumentum::argument_parser{}};
  auto parameters{parser.params()};
  parser.config().program(argv[0]).description(
    "Add a description of your program here!");
  std::optional<std::string> csv_file{};
  parameters.add_parameter(csv_file, "-e", "--example")
    .minargs(1)
    .metavar("Relative file path")
    .required(false)
    .help("Processes the provided example file");
  std::optional<std::vector<std::string>> deltaFiles;
  parameters.add_parameter(deltaFiles, "-d", "--delta")
    .minargs(2)
    .metavar("Relative file path")
    .required(false)
    .help("Finds the temperature delta for two files.");
  if (!parser.parse_args(argc, argv)) {
    return 0;
  }

  if (deltaFiles){
    PlotData((*deltaFiles)[0]);
    PlotData((*deltaFiles)[1]);
    PlotDelta((*deltaFiles)[0], (*deltaFiles)[1]);
    PlotDelta((*deltaFiles)[0], (*deltaFiles)[1]);
  }

  if (csv_file){
    ParseExample(*csv_file);
  }
  return 0;
}
