const int nset = 3;
const int nenergy = 5;
const int nasic = 18;
const TString input[nset][nenergy] = {
    {"2022-09-16_shower-data/sim-001GeV.root",
     "2022-09-16_shower-data/sim-002GeV.root",
     "2022-09-16_shower-data/sim-003GeV.root",
     "2022-09-16_shower-data/sim-004GeV.root",
     "2022-09-16_shower-data/sim-005GeV.root"},
    {"2022-09-16_shower-data/1GeV_box.root",
     "2022-09-16_shower-data/2GeV_box.root",
     "2022-09-16_shower-data/3GeV_box.root",
     "2022-09-16_shower-data/4GeV_box.root",
     "2022-09-16_shower-data/5GeV_box.root"},
    {"no-data",
     "2022-09-16_shower-data/2GeV_no-box.root",
     "2022-09-16_shower-data/3GeV_no-box.root",
     "2022-09-16_shower-data/4GeV_no-box.root",
     "2022-09-16_shower-data/5GeV_no-box.root"}
};
const TString setLabel[nset] = {"sim.", "box", "no box"};
const int energy[nenergy] = {1,2,3,4,5};
const double e[nenergy] = {1,2,3,4,5};
const double xmax[nenergy] = {40,40,60,60,80};
const EColor mColor[nset] = {kBlack, kRed, kBlue};
const bool bUseLogScale = false;

TFile *fin[nset][nenergy];
TH1D *hEnergyAllLayers[nset][nenergy];

double eres[nset][nenergy] = {0};
double ereserr[nset][nenergy] = {0};
TGraphErrors *gRes[nset];

TCanvas *canvas[nenergy];
TCanvas *cRes;
TLegend *leg[nenergy];

int LoadData();
void ConfigPlots();
void GetResolution();
void Plot();
void SetStyle(Bool_t graypalette);
