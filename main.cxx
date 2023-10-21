
#include <iostream>
#include <stdio.h>
#include "read_example_csv.hpp"
int main(int argc, char *argv[]) {
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
