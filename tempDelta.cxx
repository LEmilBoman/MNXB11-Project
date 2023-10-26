#include <iostream>
#include "external/include/lazycsv.hpp"
#include "external/include/argumentum/argparse.h"
#include "external/include/date.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>

std::map<std::string,int> measurnmentsPerDay(std::string file){
    //std::string file = "datasets/smhi-opendata_1_53430_20231007_155558_Lund_clean_cut.csv";
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

std::map<std::string,double> totalTemperaturePerDay(std::string file){
    //std::string file = "datasets/smhi-opendata_1_53430_20231007_155558_Lund_clean_cut.csv";
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

std::map<std::string,double> averageTemperaturePerDay(std::string file){
    std::map<std::string,int> measurnmentAmount = measurnmentsPerDay(file);
    std::map<std::string,double> totalTemperature = totalTemperaturePerDay(file);
    std::map<std::string,double> averageTemp;
    for (const auto day:measurnmentAmount){
        averageTemp.insert({day.first, totalTemperature[day.first]/day.second});
    }
    return(averageTemp);
}

std::map<date::year_month_day, double> averageTemperaturePerDayFormatted(std::string file){
    std::map<date::year_month_day,double> averageTemp;
    std::map<std::string,double> toConvert = averageTemperaturePerDay(file);
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

std::map<date::year_month, double> avergeTempearaturePerMonth(std::string file){
    std::map<date::year_month, double> totalTemp;
    std::map<date::year_month, int> daysPerMonth;
    std::map<date::year_month, double> averageTemp;
    std::map<date::year_month_day,double> ymd = averageTemperaturePerDayFormatted(file);
    
    for (const auto cell:ymd){
        date::year_month ymDate = cell.first.year()/cell.first.month();
        if (totalTemp.count(ymDate)==0){
            totalTemp.insert({ymDate,cell.second});
            daysPerMonth.insert({ymDate,1});
        }
        else{
            totalTemp[ymDate]+=cell.second;
            daysPerMonth[ymDate]++;
        }
    }

    for (const auto cell:totalTemp){
        date::year_month ymDate = cell.first;
        int dayCount = daysPerMonth[ymDate];
        double tempTotal = cell.second;
        double averageTempValue = tempTotal/dayCount;
        averageTemp.insert({ymDate,averageTempValue});
    }
    return(averageTemp);
}

void PlotData(std::string file){
    std::map<date::year_month, double> ymMap = avergeTempearaturePerMonth(file);
    int entryAmount = ymMap.size();
    std::cout<<entryAmount<<std::endl;
    double monthTemp[entryAmount];
    int counter = 0;
    for (const auto cell:ymMap){
        std::cout<<cell.second<<std::endl;
        monthTemp[counter] = cell.second;
        counter++;
    }
    TCanvas* canvas = new TCanvas("canvas", "Graph");
    auto graph = new TGraph(entryAmount, monthTemp);
    graph->SetTitle("Graph of month average temperature over the years.");
    graph->GetYaxis()->SetTitle("Average Temperature");
    graph->GetXaxis()->SetTitle("Month Since 1949-01-01.");
    graph->SetFillColor(40);
    graph->Draw("AB");
}

std::map<date::year_month,double> GetTemperatureDelta(){
    std::string lundData = "datasets/smhi-opendata_1_53430_20231007_155558_Lund_clean_cut.csv";
    std::string luleaData = "datasets/smhi-opendata_1_162860_20231007_155220_Lulea_clean_cut.csv";
    std::map<date::year_month,double> lundYmMap = avergeTempearaturePerMonth(lundData);
    std::map<date::year_month,double> luleaYmMap = avergeTempearaturePerMonth(luleaData);
    std::map<date::year_month,double> delta;
    for (const auto cell:lundYmMap){
        float currentDelta = lundYmMap[cell.first] - luleaYmMap[cell.first];
        delta.insert({cell.first,currentDelta});
    }
    return(delta);
}



void PlotDelta(){
    std::map<date::year_month,double> delta = GetTemperatureDelta();
    int entryAmount = delta.size();
    double monthTemp[entryAmount];
    int counter = 0;
    for (const auto cell:delta){
        std::cout<<cell.second<<std::endl;
        monthTemp[counter] = cell.second;
        counter++;
    }
    TCanvas* canvas = new TCanvas("canvas", "Graph");
    auto graph = new TGraph(entryAmount, monthTemp);
    graph->SetTitle("Average monthly temperature difference.");
    graph->GetYaxis()->SetTitle("Average Temperature Difference");
    graph->GetXaxis()->SetTitle("Month Since 1949-01-01.");
    graph->SetFillColor(40);
    graph->Draw("AB");
}
//2021-12-23
//0123456789

