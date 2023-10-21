#include <iostream>
#include "read_example_csv.hpp"
#include <lazycsv.hpp>
#include <argumentum/argparse.h>
#include <date.h>

using namespace date;
using namespace std::chrono;

int main(int argc, char *argv[]) {
  auto parser{argumentum::argument_parser{}};
  auto parameters{parser.params()};
  parser.config().program(argv[0]).description(
    "Add a description of your program here!");
  std::string csv_file{};
  parameters.add_parameter(csv_file, "-f", "--file")
    .minargs(1)
    .metavar("WriteTheExpectedTypeHere")
    .required(true)
    .help("Write a description of the parameter here for the help text!");
  if (!parser.parse_args(argc, argv)) {
    return 0;
  }

  std::cout << "I am just a code template, you need to implement the "
               "functionality you want to use yourself!"
            << std::endl;

  std::cout << "We were passed " << argc
            << " command line arguments, the first of which was " << argv[0]
            << std::endl;
  std::cout << "With a good CLI library, we could use the command line "
               "arguments to make a useful program."
            << std::endl;
  //ExampleCsvReader csvReader = ExampleCsvReader(csv_file); //use the relative path here as example "datasets/csv_example.csv"
  //csvReader.ReadCsv();
  
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
  return 0;
}
