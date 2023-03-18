#ifndef __GlobalFunctions__
#define __GlobalFunctions__
#include "Riostream.h"
#include "TMath.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TNtuple.h"
#include "TSystem.h"
#include "TEnv.h"
#include "TFile.h"
#include "TMatrixD.h"
#include "TFitResult.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <exception>

using namespace std;

//Global variables
extern Bool_t Debug;

extern TString globalRootFileName;
extern std::vector<TString> globalHistogramName;
#endif
