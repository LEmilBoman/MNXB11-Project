#include <stdio.h>
#include <string>
class ExampleCsvReader{
    private:
        std::string csv_path;
    public:
        ExampleCsvReader(std::string path);
        void PrintPath();
        std::string ReturnPath();
        void ReadCsv();
        void SetPath(std::string path);
};
