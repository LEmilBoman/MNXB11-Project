#include <iostream>
#include "external/include/lazycsv.hpp"
#include "external/include/argumentum/argparse.h"
#include "external/include/date.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>

std::map<std::string,int> measurnmentsPerDay(){
    std::string file = "datasets/smhi-opendata_1_53430_20231007_155558_Lund_clean_cut.csv";
    lazycsv::parser parser{file};
    std::map<std::string,int> measurnmentsPerDay;
    for (const auto row:parser){
        const auto [measurnmentDate] = row.cells(0);
        std::string measurnmentDateN {measurnmentDate.raw()};
        if (measurnmentsPerDay.count(measurnmentDateN) == 0){
            measurnmentsPerDay.insert({measurnmentDateN, 1});
        }
        else{
            measurnmentsPerDay[measurnmentDateN]++;
        }
    }
    return(measurnmentsPerDay);
}

std::map<std::string,double> totalTemperaturePerDay(){
    std::string file = "datasets/smhi-opendata_1_53430_20231007_155558_Lund_clean_cut.csv";
    lazycsv::parser parser{file};
    std::map<std::string,double> totalTemperaturePerDay;
    for (const auto row:parser){
        const auto [measurnmantDate, tempearture] = row.cells(0,2);
        std::string measurnmentDateN {measurnmantDate.raw()};
        std::string tempString{tempearture.raw()};
        double temperatureValue {std::stof(tempString)};
        if (totalTemperaturePerDay.count(measurnmentDateN)==0){
            totalTemperaturePerDay.insert({measurnmentDateN,temperatureValue});
        }
        else{
            totalTemperaturePerDay[measurnmentDateN]+=temperatureValue;
        }
    }
    return(totalTemperaturePerDay);
}

std::map<std::string,double> averageTemperaturePerDay(){
    std::map<std::string,int> measurnmentAmount = measurnmentsPerDay();
    std::map<std::string,double> totalTemperature = totalTemperaturePerDay();
    std::map<std::string,double> averageTemp;
    for (const auto day:measurnmentAmount){
        averageTemp.insert({day.first, totalTemperature[day.first]/day.second});
    }
    return(averageTemp);
}

std::map<date::year_month_day, double> averageTemperaturePerDayFormatted(){
    std::map<date::year_month_day,double> averageTemp;
    std::map<std::string,double> toConvert = averageTemperaturePerDay();
    for (const auto cell:toConvert){
        std::string date {cell.first};
        int year = std::stoi(date.substr(0,4));
        int month = std::stoi(date.substr(5,2));
        int day = std::stoi(date.substr(8,2));
        date::year_month_day convertedDate = date::year{year}/month/day;
        averageTemp.insert({convertedDate,cell.second});
    }
    return(averageTemp);
}
//2021-12-23
//0123456789

