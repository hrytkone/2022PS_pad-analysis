const TString dirname = "2022-09-19_shower-data";
const TString tag = "_no-box_ADC";

const int nenergy = 5;

const int nset = 27;
//const int nset = 2;

const int Energy[nenergy] = {2,3,4,5};

// BOX
/**const TString padfile[nenergy][nset] = {
    // 1 GeV with box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-00-53-05/Output.root", // 4,9,14,16
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-08-07-36/Output.root"}, // 6,9,14,16
    // 2 GeV with box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-20-09-36/Output.root", // 7,14,16
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-22-42-22/Output.root"}, // 7,14,16
    // 3 GeV with box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-17-32-01/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-18-35-39/Output.root"},
    // 4 GeV with box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-15-39-26/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-16-36-08/Output.root"},
    // 5 GeV with box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-13-13-38/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-18-14-23-36/Output.root"}
};

const int asicStatus[nenergy][nset][nasic] = {
    // 1 GeV with box
    {{1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1}, {1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,0,1}},
    // 2 GeV with box
    {{1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}, {1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}},
    // 3 GeV with box
    {{1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}, {1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}},
    // 4 GeV with box
    {{1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}, {1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}},
    // 5 GeV with box
    {{1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}, {1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1}}
};**/

// NO BOX
const TString padfile[nenergy][nset] = {
    // 2 GeV no box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-21-33-20/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-23-28-05/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-23-58-47/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-00-09-21/Output.root",
     "","","","","","","","","","","","","","","","","","","","","","",""},
    // 3 GeV no box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-19-01-06/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-20-22-19/Output.root",
     "","","","","","","","","","","","","","","","","","","","","","","","",""},
    // 4 GeV no box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-16-14-55/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-17-19-47/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-17-35-14/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-17-44-50/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-17-56-51/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-18-09-27/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-07-01-27/Output.root",
     "","","","","","","","","","","","","","","","","","","",""},
    // 5 GeV no box
    {"/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-14-36-48/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-19-15-25-45/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-00-22-53/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-00-29-34/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-00-40-08/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-00-52-07/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-02-20-56/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-02-29-25/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-02-38-19/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-02-45-52/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-02-53-30/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-03-33-34/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-03-41-21/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-03-50-27/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-03-58-33/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-04-05-59/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-04-13-04/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-04-22-52/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-04-38-57/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-04-46-34/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-04-53-12/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-05-01-51/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-05-09-33/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-05-17-12/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-05-25-37/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-05-32-34/Output.root",
     "/media/heimarry/Expansion/tb2022-data/electron_data/2022-06-20-05-43-46/Output.root"}
};


const int asicStatus[nenergy][nset][nasic] = {
    // 2 GeV with box
    {{1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, {1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0}, {1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0},
    {0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
    {0},{0},{0}},
    // 3 GeV with box
    {{1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, {1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
    {0},{0},{0},{0},{0}},
    // 4 GeV with box
    {{1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0},{1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0},
    {1,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1},{1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,1,1,1,1,0,1,1,0,1,1,1,1,0},{1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},{0},{0},{0},{0},{0},{0},{0},{0},{0},
    {0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}},
    // 5 GeV with box
    {{1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0},{1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0},
    {1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1},{1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1},
    {1,0,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1},
    {1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1},{1,1,1,0,0,1,1,1,1,0,1,0,1,0,0,1,1,1},
    {1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1},
    {1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1,1,1},{1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1},{1,0,1,0,0,1,1,1,1,0,1,1,1,1,0,1,1,1},
    {1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1},{1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1},{1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1},{1,0,1,0,0,1,1,1,1,0,1,1,1,1,0,1,1,1},
    {1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1},{1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1},
    {1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1},{1,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1}}
};

const TString mipFile = "MIP-pos.txt";
const TString pedFile = "Pedestals_after-MD_box.txt";
//const double ethres = 0.04;
const double ethres = 10.;
const double eMip = 0.08;

const int nbin = 201;
const double minbin = -0.05;
const double maxbin = 20.05;

// Bins for energy summed in all layers
const int nbinE = 1001;
//const double minbinE = -0.05;
//const double maxbinE = 100.05;
const double minbinE = -0.5;
const double maxbinE = 1000.5;

double mip[nasic][nch] = {0};

int nsize = 0;
double eseed = 0;
TH1D *hEnergyAllLayers;
TH1D *hClusters[nasic];
TH1D *hSeedEnergy[nasic];
TH1D *hShowerSize[nasic];
TH2D *hADC[nasic];

void InitOutput(int energy);
void FillSignalHistos();
void LoadMIPCalibration();
double ClusterADC(TH2D* h, double threshold = 0.04);
bool IsOnEdge(int x, int y);
double ConvertADCtoEnergy(int iasic, int ich, double adc);
