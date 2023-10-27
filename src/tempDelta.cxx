#include <iostream>
#include <lazycsv.hpp>
#include <argumentum/argparse.h>
#include <date.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "../include/tempDelta.h"

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
    for (const auto &day:measurnmentAmount){
        averageTemp.insert({day.first, totalTemperature[day.first]/day.second});
    }
    return(averageTemp);
}

std::map<date::year_month_day, double> averageTemperaturePerDayFormatted(std::string file){
    std::map<date::year_month_day,double> averageTemp;
    std::map<std::string,double> toConvert = averageTemperaturePerDay(file);
    for (const auto &cell:toConvert){
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
    TCanvas* canv1 = new TCanvas("canv1", "Title", 1500, 1000);
    auto graph = new TGraph(entryAmount, monthTemp);
    graph->SetTitle("Graph of month average temperature over the years.");
    graph->GetYaxis()->SetTitle("Average Temperature");
    graph->GetXaxis()->SetTitle("Month Since 1949-01-01.");
    graph->SetFillColor(40);
    graph->Draw("AB");
    std::string save = file.substr(file.find('/') + 1,file.find('.')) + ".pdf";
    canv1->SaveAs(save.data());

}

std::map<date::year_month,double> GetTemperatureDelta(std::string firstFile, std::string secondFile){
    std::string lundData = firstFile;
    std::string luleaData = secondFile;
    std::map<date::year_month,double> lundYmMap = avergeTempearaturePerMonth(lundData);
    std::map<date::year_month,double> luleaYmMap = avergeTempearaturePerMonth(luleaData);
    std::map<date::year_month,double> delta;
    for (const auto cell:lundYmMap){
        float currentDelta = lundYmMap[cell.first] - luleaYmMap[cell.first];
        delta.insert({cell.first,currentDelta});
    }
    return(delta);
}

std::map<date::month,double> GetAverageTotalDeltaByMonth(std::string firstFile, std::string secondFile){
    std::map<date::year_month,double> tempDelta = GetTemperatureDelta(firstFile,secondFile);
    std::map<date::month,double> totalDeltaByMonth;
    std::map<date::month,int> readingsByMonth;
    for (const auto cell:tempDelta){
        date::month currentMonth = cell.first.month();
        if (totalDeltaByMonth.count(currentMonth) == 0){
            totalDeltaByMonth.insert({currentMonth, cell.second});
            readingsByMonth.insert({currentMonth,1});
        }
        else{
            totalDeltaByMonth[currentMonth]+=cell.second;
            readingsByMonth[currentMonth]++;
        }
    }
    std::map<date::month,double> deltaByMonth;
    for (const auto cell:totalDeltaByMonth){
        float averageDelta = totalDeltaByMonth[cell.first]/readingsByMonth[cell.first];
        deltaByMonth.insert({cell.first,averageDelta});
    }
    return(deltaByMonth);
}

void PlotDelta(std::string firstFile, std::string secondFile){
    std::map<date::year_month,double> delta = GetTemperatureDelta(firstFile, secondFile);
    int entryAmount = delta.size();
    double monthTemp[entryAmount];
    int counter = 0;
    for (const auto cell:delta){
        std::cout<<cell.second<<std::endl;
        monthTemp[counter] = cell.second;
        counter++;
    }
    TCanvas* canv1 = new TCanvas("canv1", "Title", 1500, 1000);
    auto graph = new TGraph(entryAmount, monthTemp);
    graph->SetTitle("Average monthly temperature difference.");
    graph->GetYaxis()->SetTitle("Average Temperature Difference");
    graph->GetXaxis()->SetTitle("Month Since 1949-01-01.");
    graph->SetFillColor(40);
    graph->Draw("AB");
    canv1->SaveAs("DeltaOverTheYears.pdf");
}

void PlotDeltaByMonth(std::string firstFile, std::string secondFile){
    std::map<date::month,double> deltaByMonth = GetAverageTotalDeltaByMonth(firstFile, secondFile);
     int entryAmount = deltaByMonth.size();
    double monthTemp[entryAmount];
    int counter = 0;
    for (const auto cell:deltaByMonth){
        std::cout<<cell.second<<std::endl;
        monthTemp[counter] = cell.second;
        counter++;
    }
    double xAxis[12] {1,2,3,4,5,6,7,8,9,10,11,12};
    TCanvas* canv1 = new TCanvas("canv1", "Title", 1500, 1000);
    auto graph = new TGraph(12,xAxis, monthTemp);
    graph->SetTitle("Average temperature difference by month");
    graph->GetYaxis()->SetTitle("Average Temperature Difference, Degrees");
    graph->GetXaxis()->SetTitle("Month");
    graph->SetFillColor(40);
    graph->Draw("AB");
    canv1->SaveAs("DeltaByMonth.pdf");

}

//2021-12-23
//0123456789

