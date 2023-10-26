#include <iostream>
#include "read_example_csv.hpp"
#include <lazycsv.hpp>
#include <argumentum/argparse.h>
#include <date.h>
#include <vector>
#include <map>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>

using namespace date;
using namespace std::chrono;

std::map<int, int> winterday_map(std::string csv_file, double numb_measurments){
  //Parser over the csv file and index for all variables of interest.
  lazycsv::parser lazyParser{csv_file};
  auto d_index = lazyParser.index_of("date");
  auto v_index = lazyParser.index_of("temperature");
  
  std::map<std::string, double> date_temperature_map;
  std::map<int, int> year_winterday_map;
  //loop over each line in the csv file.
  for (const auto line : lazyParser){
    //extract the day, year, month, and measurement as strings.
    const auto [d,v] = line.cells(d_index,v_index);
    std::string tempd{d.trimed()};
    std::string tempmea{v.trimed()};
    date_temperature_map[tempd] += stod(tempmea);  
  }

  for (auto& entry : date_temperature_map){
    double temp{entry.second};
    entry.second = temp/numb_measurments;
    if (entry.second < 0.0){
      year_winterday_map[stoi(entry.first.substr(0,4))] += 1;
    }
  }
  return year_winterday_map;
}

void winterday_hist_fill(TH1I* h, TH1D* h_move, std::map<int, int> m){
  for (int bin = 1964; bin <= 2023; ++bin){
    h->SetBinContent(bin, m[bin]);
  }
  for (int b = 1966; b <= 2021; ++b){
    double mov_average {(m[b-2]+m[b-1]+m[b]+m[b+1]+m[b+2])/(double)5};
    h_move->SetBinContent(b,mov_average);
  }
}

void winterday_hist_draw(std::vector<TH1I*> hists, std::vector<TH1D*> move_avgs){
  TCanvas* canv1 = new TCanvas("canv1", "Title", 1500, 1000);
  TLegend* leg1 = new TLegend(0.1,0.7,0.48,0.9);
  leg1->SetHeader("Number of Winterdays per year","C");
  for (auto h : hists){
    h->GetXaxis()->SetRangeUser(1964,2023);
    h->GetXaxis()->SetRange(1964,2023);
    h->GetYaxis()->SetRangeUser(0,250);
    h->SetStats(0);
    h->Draw("SAMES PFC PMC");
    leg1->AddEntry(h,h->GetName(),"f");
  }
  gPad->RedrawAxis();
  for (auto a : move_avgs){
    a->GetXaxis()->SetRangeUser(1966,2021);
    a->GetXaxis()->SetRange(1966,2021);
    a->GetYaxis()->SetRangeUser(0,250);
    a->SetLineWidth(5);
    a->SetStats(0);
    a->Draw("SAMES C PLC PMC");
  }
  leg1->Draw();
  canv1->SaveAs("winterday_hist.pdf");
}

void rel_winterday_hist_gen(std::map<Int_t, Int_t> map_numer,std::map<Int_t, Int_t> map_denom,const char* header_title,const char* utput_file){

  TCanvas* canv2 = new TCanvas("canv2", "Title", 1500, 1000);
  TLegend* leg1 = new TLegend(0.1,0.8,0.48,0.9);
  leg1->SetHeader("","C");
  TH1D* h = new TH1D("",";year;Relative Number of Winterdays",2023,1,2023);

  for (int bin =1964; bin<= 2023; ++bin){
    h->SetBinContent(bin, map_numer[bin]/(double)map_denom[bin]);
  }

  h->GetXaxis()->SetRangeUser(1964,2022);
  h->GetXaxis()->SetRange(1964,2022);
  h->GetYaxis()->SetRangeUser(0,1);
  h->SetStats(0);

  h->Draw("SAMES PFC PMC");
  leg1->AddEntry(h,header_title,"f");
  leg1->Draw();
  canv2->SaveAs(utput_file);  
}    

int main(int argc, char *argv[]) {
  auto parser{argumentum::argument_parser{}};
  auto parameters{parser.params()};
  parser.config().program(argv[0]).description(
    "Add a description of your program here!");
  std::vector<std::string> csv_files;
  parameters.add_parameter(csv_files, "-f", "--file")
    .minargs(3)
    .metavar("csv files containing temperature data")
    .required(true)
    .help("need to provide 3 csv file containing temperature data");
  std::vector<std::string> city_names;
  parameters.add_parameter(city_names, "-n", "--name")
    .minargs(3)
    .metavar("names of the cities where the temperatures in csv files where measured.")
    .required(true)
    .help("need to provide 3 names of the cities where the temperatures in csv files where measured.");
  if (!parser.parse_args(argc, argv)) {
    return 0;
  }
  
  

  std::map<int, int> city1_wd_map = winterday_map(csv_files[0],2.0);
  std::map<int, int> city2_wd_map = winterday_map(csv_files[1],24.0);
  std::map<int, int> city3_wd_map = winterday_map(csv_files[2],24.0);
  
  TH1I* city1_hist = new TH1I(city_names[0].c_str(),";year;Number of Winterdays",2023,1,2023);
  TH1D* city1_move_avg = new TH1D("Lund move avg",";year;Number of Winterdays",2023,1,2023);

  TH1I* city2_hist = new TH1I(city_names[1].c_str(),";year;Number of Winterdays",2023,1,2023);
  TH1D* city2_move_avg = new TH1D("Uppsala move avg",";year;Number of Winterdays",2023,1,2023);

  TH1I* city3_hist = new TH1I(city_names[2].c_str(),";year;Number of Winterdays",2023,1,2023);
  TH1D* city3_move_avg = new TH1D("Luleaa move avg",";year;Number of Winterdays",2023,1,2023);

  winterday_hist_fill(city1_hist,city1_move_avg,city1_wd_map);
  winterday_hist_fill(city2_hist,city2_move_avg,city2_wd_map);
  winterday_hist_fill(city3_hist,city3_move_avg,city3_wd_map);

  std::vector<TH1I*> histograms = {city3_hist,city2_hist,city1_hist};
  std::vector<TH1D*> move_averages ={city1_move_avg,city2_move_avg,city3_move_avg};
  winterday_hist_draw(histograms,move_averages);

  rel_winterday_hist_gen(city1_wd_map,city3_wd_map,("Rel. numb. of wd "+city_names[0]+"/"+city_names[2]).c_str(),("rel_"+city_names[0]+"_hist.pdf").c_str());
  rel_winterday_hist_gen(city2_wd_map,city3_wd_map,("Rel. numb. of wd "+city_names[1]+"/"+city_names[2]).c_str(),("rel_"+city_names[1]+"_hist.pdf").c_str());

  return 0;
}

