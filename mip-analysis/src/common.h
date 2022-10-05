const double isocut = 10.;
const double isobackthres = 10.;
const bool usefulliso = false;
const double calibScale = 0.3;

const int nasic = 18;
const int nch = 72;
const int nchx = 9;
const int nchy = 8;
const int ncorrch = 16;
const int nslope = 6;

Int_t event = 0;
Int_t asic[nasic] = {0};
Int_t ADC[nasic][nch] = {0};
Int_t CALIB0[nasic] = {0};
Int_t CALIB1[nasic] = {0};
uint32_t hdr0[nasic] = {0};
uint32_t hdr1[nasic] = {0};

Double_t ADCwoPed[nasic][nch] = {0};
Double_t CALIB0woPed[nasic] = {0};
Double_t CALIB1woPed[nasic] = {0};
Double_t Pedestal[nasic][nch] = {0};
Double_t PedestalSigma[nasic][nch] = {0};
Double_t PedCal0[nasic] = {0};
Double_t PedCal1[nasic] = {0};

Int_t Mapping[nchy][nchx] = {//map of channels in XY
    {43,  41,  53,  49,  58,  54,  66,  68,  69},
    {39,  37,  47,  51,  56,  62,  60,  64,  70},
    {38,  42,  45,  46,  50,  59,  55,  65,  71},
    {44,  40,  36,  48,  52,  61,  57,  67,  63},
    { 8,   4,   0,  14,  16,  19,  23,  31,  27},
    { 2,   6,  11,  10,  17,  21,  25,  33,  35},
    { 1,   3,   9,  15,  22,  24,  26,  28,  34},
    { 7,   5,  12,  13,  18,  20,  29,  30,  32}
};

double cmn = 0;
vector<double> cmnHGROC;

TFile *fin, *fout;
TTree *trpad;

void RemovePedestal();
void RemoveCommonNoise();
void RemoveCommonNoiseHGROC();
void RemoveCommonNoiseQuadrant();
double GetCommonNoise(int iasic);
vector<double> GetCommonNoiseHGROC();
vector<double> GetCommonNoiseQuadrant();
int GetMaximum();
int GetX(int ich);
int GetY(int ich);
bool KeepEvent();
bool IsIsolated(int ich);
bool IsFullyIsolated(int ich);
int GetQuadrant(int ich);
bool IsInList(int *list, int ch, int size);

int LoadData(TString padfile, TString pedFile)
{
	if (gSystem->AccessPathName(padfile.Data())) {
		std::cout << "Input file not found!" << std::endl;
		return 0;
	}

	fin = TFile::Open(padfile, "READ");
	trpad = (TTree*) fin->Get("PadData");
    trpad->SetBranchStatus("*", 0);
    trpad->SetBranchStatus("Event", 1);
    trpad->SetBranchStatus("ASIC", 1);
    trpad->SetBranchStatus("ADC", 1);
    trpad->SetBranchStatus("HEADER0", 1);
    trpad->SetBranchStatus("HEADER1", 1);
    trpad->SetBranchStatus("CALIB0", 1);
    trpad->SetBranchStatus("CALIB1", 1);
    trpad->SetBranchAddress("Event", &event);
    trpad->SetBranchAddress("ASIC", &asic);
    trpad->SetBranchAddress("ADC", &ADC);
    trpad->SetBranchAddress("HEADER0", &hdr0);
    trpad->SetBranchAddress("HEADER1", &hdr1);
    trpad->SetBranchAddress("CALIB0", &CALIB0);
    trpad->SetBranchAddress("CALIB1", &CALIB1);

	// Load Pedestal data
    cout << "Load pedestals from file " << pedFile.Data() << endl;
    ifstream fPedestal(pedFile.Data());
    int ich, iasic;
    double ped, pedsigma;
    while (fPedestal>>iasic>>ich>>ped>>pedsigma) {
        //while (fPedestal>>iasic>>ich>>ped) {
            Pedestal[iasic][ich] = ped;
            PedestalSigma[iasic][ich] = pedsigma;
        }

        // Load Pedestal data for calibration channels
        //ifstream fPedestalCal(pedFile.Data());
        //double ped0, ped1;
        //while (fPedestalCal>>iasic>>ped0>>ped1) {
            //    PedCal0[iasic] = ped0;
            //    PedCal1[iasic] = ped1;
            //}
	return 1;
}

void RemovePedestal()
{
    for (int iasic=0; iasic<nasic; iasic++) {
        for (int ich=0; ich<nch; ich++)
            ADCwoPed[iasic][ich] = (Double_t)ADC[iasic][ich] - Pedestal[iasic][ich];

        // Remove from calibration channels
        CALIB0woPed[iasic] = (Double_t)CALIB0[iasic] - PedCal0[iasic];
        CALIB1woPed[iasic] = (Double_t)CALIB1[iasic] - PedCal1[iasic];
    }

}

void RemoveCommonNoise()
{
    for (int iasic=0; iasic<nasic; iasic++)
        for (int ich=0; ich<nch; ich++)
            ADCwoPed[iasic][ich] -= cmn;
}

void RemoveCommonNoiseHGROC(int iasic)
{
    for (int ich=0; ich<nch; ich++) {
        if (ich<36)
            ADCwoPed[iasic][ich] -= cmnHGROC[0];
        else
            ADCwoPed[iasic][ich] -= cmnHGROC[1];
    }

    // Remove from calibration channels
    //CALIB0woPed[iasic] -= calibScale*cmnHGROC[0];
    //CALIB1woPed[iasic] -= calibScale*cmnHGROC[1];
}

double GetCommonNoise(int iasic)
{
	vector<double> cmn;
    for (int ich=0; ich<nch; ich++) {
    	cmn.push_back(ADCwoPed[iasic][ich]);
    }
	std::sort(cmn.begin(), cmn.end());
	int mid = cmn.size()/2.;
    if (mid > 10)
        return (cmn[mid-1] + cmn[mid] + cmn[mid+1])/3.;
	else
		return 0.;
}

vector<double> GetCommonNoiseHGROC(int iasic)
{
    vector<double> cmn1, cmn2;
    vector<double> cmn;
	for (int ich=0; ich<nch; ich++) {
        if (ich<36)
			cmn1.push_back(ADCwoPed[iasic][ich]);
        else
    		cmn2.push_back(ADCwoPed[iasic][ich]);
	}
    std::sort(cmn1.begin(), cmn1.end());
	std::sort(cmn2.begin(), cmn2.end());

	int mid = cmn1.size()/2.;
    cmn.push_back((cmn1[mid-1] + cmn1[mid] + cmn1[mid+1])/3.);
    //cmn.push_back((cmn1[mid-5] + cmn1[mid-4] + cmn1[mid-3] + cmn1[mid-2] + cmn1[mid-1] + cmn1[mid] + cmn1[mid+1] + cmn1[mid+2] + cmn1[mid+3] + cmn1[mid+4] + cmn1[mid+5])/11.);

    mid = cmn2.size()/2.;
    cmn.push_back((cmn2[mid-1] + cmn2[mid] + cmn2[mid+1])/3.);
    //cmn.push_back((cmn2[mid-5] + cmn2[mid-4] + cmn2[mid-3] + cmn2[mid-2] + cmn2[mid-1] + cmn2[mid] + cmn2[mid+1] + cmn2[mid+2] + cmn2[mid+3] + cmn2[mid+4] + cmn2[mid+5])/11.);

    return cmn;
}

int GetMaximum(int iasic)
{
	double max = 0.;
	int imax = -100;
	for (int ich=0; ich<nch; ich++) {
		if (ADCwoPed[iasic][ich] > max) {
			max = ADCwoPed[iasic][ich];
			imax = ich;
		}
	}
	//cout << "maximum value : " << max << endl;
	return imax;
}

int GetX(int ich)
{
    for (int i=0; i<nchx; i++)
        for (int j=0; j<nchy; j++)
            if (Mapping[i][j]==ich)
                return i;
    return -1;
}

int GetY(int ich)
{
    for (int i=0; i<nchx; i++)
        for (int j=0; j<nchy; j++)
            if (Mapping[i][j]==ich)
                return j;
    return -1;
}

bool IsIsolated(int iasic, int ich)
{
    if (ADCwoPed[iasic][ich]<isocut) return false;
    bool isOk = true;
    int x = GetX(ich);
    int y = GetY(ich);
    for (int i=-1; i<2; i++) {
        for (int j=-1; j<2; j++) {
            if (x+i<0 || x+i>=nchx || y+j<0 || y+j>=nchy || Mapping[x+i][y+j]==ich) continue;
            int neighbour = Mapping[x+i][y+j];
            if (ADCwoPed[iasic][neighbour]>isobackthres) {
                isOk = false;
                break;
            }
        }
    }
    return isOk;
}

bool IsFullyIsolated(int iasic, int ich)
{
    for (int i=0; i<nch; i++) {
        if (i==ich) continue;
        if (ADCwoPed[iasic][i]>isobackthres) {
            return false;
        }
    }
    return true;
}

bool KeepEvent(int iasic)
{
    if (usefulliso) {
        int imax = GetMaximum(iasic);
        if (ADCwoPed[iasic][imax] < isocut) return false;
        if (IsFullyIsolated(iasic, imax))
            return true;
    } else {
        for (int ich=0; ich<nch; ich++)
            if (IsIsolated(iasic, ich))
                return true;
    }
    return false;
}
