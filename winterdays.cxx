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
    h->GetXaxis()->SetRangeUser(1966,2021);
    h->GetXaxis()->SetRange(1966,2021);
    h->GetYaxis()->SetRangeUser(0,250);
    h->SetStats(0);
    h->Draw("SAMES PFC PMC");
    leg1->AddEntry(h,h->GetName(),"f");
  }
  for (auto a : move_avgs){
    a->SetLineWidth(5);
    a->SetStats(0);
    a->Draw("SAMES C PLC PMC");
  }
  leg1->Draw();
  canv1->SaveAs("test_hist.pdf");
}

/*void winterday_hist_gen(std::map<const char*, std::map<Int_t, Int_t>> headers_maps){
  TCanvas* canv1 = new TCanvas("canv1", "Title", 1500, 1000);
  TLegend* leg1 = new TLegend(0.1,0.7,0.48,0.9);
  leg1->SetHeader("Number of Winterdays per year","C");
  for (auto& entry : headers_maps){
    TH1I* h = new TH1I("",";year;# WD",2023,1,2023);
    TH1D* h_mov = new TH1D("",";year;# WD",2023,1,2023);
    for (int bin = 1964; bin <= 2023; ++bin){
      h->SetBinContent(bin, entry.second[bin]);
    }
    for (int b = 1966; b <= 2021; ++b){
      double mov_average {(entry.second[b-2]+entry.second[b-1]+entry.second[b]+entry.second[b+1]+entry.second[b+2])/(double)5};
      h_mov->SetBinContent(b,mov_average);
    }
    h->GetXaxis()->SetRangeUser(1966,2021);
    h->GetXaxis()->SetRange(1966,2021);
    h->GetYaxis()->SetRangeUser(0,250);
    h->SetStats(0);
    h->Draw("SAMES PFC PMC");
    h_mov->SetLineWidth(5);
    h_mov->Draw("SAMES C PLC PMC");
    leg1->AddEntry(h,entry.first,"f");
    leg1->AddEntry(h,"Moving Avg.","l");
  }
  leg1->Draw();
  canv1->SaveAs("test_hist.jpg");
}*/

void rel_winterday_hist_gen(std::map<Int_t, Int_t> map_numer,std::map<Int_t, Int_t> map_denom){

  TCanvas* canv2 = new TCanvas("canv2", "Title", 1500, 1000);
  TLegend* leg1 = new TLegend(0.1,0.7,0.48,0.9);
  leg1->SetHeader("Rel. numb. of wd Lund/Lulea","C");
  TH1D* h = new TH1D("",";year;# WD",2023,1,2023);

  for (int bin =1964; bin<= 2023; ++bin){
    h->SetBinContent(bin, map_numer[bin]/(double)map_denom[bin]);
  }

  h->GetXaxis()->SetRangeUser(1964,2022);
  h->GetXaxis()->SetRange(1964,2022);
  h->GetYaxis()->SetRangeUser(0,1);
  h->SetStats(0);

  h->Draw("SAMES PFC PMC");
  leg1->AddEntry(h,"","f");
  leg1->Draw();
  canv2->SaveAs("rel_test_hist.pdf");  
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
    .help("csv file containing temperature data");
  if (!parser.parse_args(argc, argv)) {
    return 0;
  }
  
  std::map<int, int> lund_wd_map = winterday_map(csv_files[0],2.0);
  std::map<int, int> uppsala_wd_map = winterday_map(csv_files[1],24.0);
  std::map<int, int> lulea_wd_map = winterday_map(csv_files[2],24.0);

  TH1I* lund_hist = new TH1I("Lund",";year;# WD",2023,1,2023);
  TH1D* lund_move_avg = new TH1D("Lund move avg",";year;# WD",2023,1,2023);

  TH1I* uppsala_hist = new TH1I("Uppsala",";year;# WD",2023,1,2023);
  TH1D* uppsala_move_avg = new TH1D("Uppsala move avg",";year;# WD",2023,1,2023);

  TH1I* lulea_hist = new TH1I("Luleaa",";year;# WD",2023,1,2023);
  TH1D* lulea_move_avg = new TH1D("Luleaa move avg",";year;# WD",2023,1,2023);

  winterday_hist_fill(lund_hist,lund_move_avg,lund_wd_map);
  winterday_hist_fill(uppsala_hist,uppsala_move_avg,uppsala_wd_map);
  winterday_hist_fill(lulea_hist,lulea_move_avg,lulea_wd_map);

  std::vector<TH1I*> histograms = {lulea_hist,uppsala_hist,lund_hist};
  std::vector<TH1D*> move_averages ={lund_move_avg,uppsala_move_avg,lulea_move_avg};
  winterday_hist_draw(histograms,move_averages);

  /*std::map<const char*, std::map<int, int>> winterdays_with_header;
  winterdays_with_header["Luleaa"] = lulea_wd_map;
  winterdays_with_header["Uppsala"] = uppsala_wd_map;
  winterdays_with_header["Lund"] = lund_wd_map;
  
  winterday_hist_gen(winterdays_with_header);*/

  rel_winterday_hist_gen(lund_wd_map,lulea_wd_map);

  return 0;
}

