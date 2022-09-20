const TString dirname = "2022-09-19_shower-data";
const TString tag = "";

const int nbin = 201;
const double minbin = -0.05;
const double maxbin = 20.05;

// Bins for energy summed in all layers
const int nbinE = 1001;
const double minbinE = -0.05;
const double maxbinE = 100.05;

const TString pedFile = "Pedestals_after-MD_box.txt";
const TString mipFile = "MIP-pos.txt";

const double threshold = 0.04;

double eMip = 0.08;
double mip[nasic][nch] = {0};

int nsize = 0;
double eseed = 0;
Float_t data_pad[nch*(nasic+2)] = {0};
Float_t edep[nasic][nch] = {0};
Float_t edep_noisy[nasic][nch] = {0};

TFile *finelectron;
TTree *trelectron;
TH2D *hHitMap;
TH2D *hADC[nasic];
TH1D *hShowerDist[nasic];
TH1D *hShowerDistNoisy[nasic];
TH1D *hEnergyAllLayers;
TH2D *hShowerMap[nasic];
TH1D *hSeedEnergy[nasic];
TH1D *hSeedEnergyIdeal[nasic];
TH1D *hShowerSize[nasic];

TF1 *fPedestal[nasic][nch];

int LoadSimData(TString electronfile, int iset);
void LoadMIPCalibration();
void InitOutput(TString energy);
double ConvertADCtoEnergy(int iasic, int ich, double adc);
int ConvertEnergyToADC(int iasic, int ich, double energy);
void FillSignalHisto(int iasic);
double Cluster(TH2D* h, double threshold = 0.04); //in MeV
void CreatePedestalFunctions();
void AddPedestalNoise(int iasic, int channelID);
