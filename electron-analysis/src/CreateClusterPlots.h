const int nevents = 20;

const int nset = 2;
const TString padfile[nset] = {
    // 1 GeV with box
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-00-53-05/Output.root", // 4,9,14,16
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-08-07-36/Output.root" // 6,9,14,16
    // 2 GeV with box
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-20-09-36/Output.root", // 7,14,16
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-22-42-22/Output.root" // 7,14,16
    // 3 GeV with box
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-17-32-01/Output.root",
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-18-35-39/Output.root"
    // 4 GeV with box
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-15-39-26/Output.root",
    //"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-16-36-08/Output.root"
    // 5 GeV with box
    "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-13-13-38/Output.root",
    "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-14-23-36/Output.root"
};

const TString mipFile = "MIP-pos.txt";
const TString pedFile = "Pedestals_after-MD_box.txt";
const double eMip = 0.08;

double mip[nasic][nch] = {0};

TH2D *hADC[nasic][nevents];
TH2D *hADCwoPed[nasic][nevents];
TH2D *hADCwoPedAndCMN[nasic][nevents];
TH2D *hEnergy[nasic][nevents];
TH2D *hCluster[nasic][nevents];

void InitOutput(int energy, int evstart, int evend);
void FillSignalHistos();
void LoadMIPCalibration();
double ClusterADC(TH2D* h, int iasic, int iev, double threshold = 0.04); // 20 ADC = 150 MeV
bool IsOnEdge(int x, int y);
double ConvertADCtoEnergy(int iasic, int ich, double adc);
