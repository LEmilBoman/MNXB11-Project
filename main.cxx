#include <iostream>
#include "read_example_csv.hpp"
#include <argumentum/argparse.h>
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
  ExampleCsvReader csvReader = ExampleCsvReader("datasets/csv_example.csv");
  csvReader.ReadCsv();

  return 0;
}
