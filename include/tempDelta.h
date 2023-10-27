#ifndef TEMPDELTA_H   
#define TEMPDELTA_H
#include <iostream>
#include "../external/include/lazycsv.hpp"
#include "../external/include/argumentum/argparse.h"
#include "../external/include/date.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>

void PlotData(std::string file);

void PlotDelta(std::string firstFile, std::string secondFile);

void PlotDeltaByMonth(std::string firstFile, std::string secondFile);

#endif