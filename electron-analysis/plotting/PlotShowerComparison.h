const int nset = 3;
const int nenergy = 5;
const int nasic = 18;
const TString input[nset][nenergy] = {
    {"2022-09-19_shower-data/sim-001GeV.root",
     "2022-09-19_shower-data/sim-002GeV.root",
     "2022-09-19_shower-data/sim-003GeV.root",
     "2022-09-19_shower-data/sim-004GeV.root",
     "2022-09-19_shower-data/sim-005GeV.root"},
     {"2022-09-19_shower-data/1GeV_box.root",
      "2022-09-19_shower-data/2GeV_box.root",
      "2022-09-19_shower-data/3GeV_box.root",
      "2022-09-19_shower-data/4GeV_box.root",
      "2022-09-19_shower-data/5GeV_box.root"},
    {"no-data",
     "2022-09-19_shower-data/2GeV_no-box.root",
     "2022-09-19_shower-data/3GeV_no-box.root",
     "2022-09-19_shower-data/4GeV_no-box.root",
     "2022-09-19_shower-data/5GeV_no-box.root"}
};
const TString histname[nset] = {"hShowerDistNoisy_asic", "hClusters_asic", "hClusters_asic"};
const TString setLabel[nset] = {"sim.", "box", "no box"};
const int energy[nenergy] = {1,2,3,4,5};
const EColor mColor[nset] = {kBlack, kRed, kBlue};
const double axisScale = 1.;
const bool bUseLogScale = false;

TFile *fin[nset][nenergy];
TH1D *hShowerDist[nset][nenergy][nasic];

TCanvas *canvas[nenergy];
TLegend *leg[nenergy][nasic];

int LoadData();
void ConfigPlots();
void Plot();
Double_t ScaleX(Double_t x);
void ScaleAxis(TAxis *a, Double_t (*Scale)(Double_t));
void ScaleXaxis(TH1 *h, Double_t (*Scale)(Double_t));
void SetStyle(Bool_t graypalette);
