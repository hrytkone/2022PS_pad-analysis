const TString dirname = "2022-06-28_shower-data";
const int nbin = 100;
const double minbin = -0.025;
const double maxbin = 0.525;

Float_t data_pad[nch*(nasic+2)] = {0};

TFile *finsim;
TTree *trsim;
TH1D *hMIP[nasic+2][nch];

int LoadSimData(TString electronfile);
void InitOutput(TString particle, TString energy);
