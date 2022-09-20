#include "common.h"
#include "SimulatedShowerDist.h"

void SimulatedShowerDist(TString electronfile, TString energy, int iset)
{
    if (!LoadSimData(electronfile, iset)) return;
    LoadMIPCalibration();
    InitOutput(energy);
    CreatePedestalFunctions();

    int nevelectron = trelectron->GetEntriesFast();
    for (int iev=0; iev<nevelectron; iev++) {
        trelectron->GetEntry(iev);

        // Get ADC values from simulation (simulation has 20 layers so easier
        // to first save only the 18 layers to an array)
        int iasic = -1;
        int iasictrue = -1;
        while (iasic<nasic+1) {
            for (int iy = 0; iy < nchy; iy++) {
                for (int ix = 0; ix < nchx; ix++) {
                    int channelID = iasic*nch + iy*nchx + ix;
                    if (channelID==-72) channelID = 0;
                    if (channelID%nch==0) iasic++;
                    if (iasic==4 || iasic==9) {
                    } else {
                        if (channelID%nch==0) iasictrue++;
                        edep[iasictrue][channelID%nch] = data_pad[channelID];
                    }
                }
            }
        }

        // Add pedestal noise to the channels
        for (int iasic=0; iasic<nasic; iasic++) {
            for (int ich=0; ich<nch; ich++) {
                AddPedestalNoise(iasic, ich);

                // Take into account saturation
                if (ADC[iasic][ich]>1023) ADC[iasic][ich] = 1023;
            }
        }

        RemovePedestal();
        double sumAllAsics = 0.;
        for (int iasic=0; iasic<nasic; iasic++) {

            // Fill true energy deposit
            double sum = 0.;
            double eseedideal = 0.;
            for (int iy = 0; iy < nchy; iy++) {
                for (int ix = 0; ix < nchx; ix++) {
                    int channelID = iy*nchx + ix;
                    //if (edep[iasic][channelID]<=threshold) continue;
                    if (edep[iasic][channelID]>eseedideal) eseedideal = edep[iasic][channelID];
                    sum += edep[iasic][channelID];
                    hShowerMap[iasic]->Fill(ix, iy, edep[iasic][channelID]);
                }
            }
            if (sum>0)
                hShowerDist[iasic]->Fill(sum);
            sumAllAsics += sum;
            hSeedEnergyIdeal[iasic]->Fill(eseedideal);

            // Do clustering to noise added data
            FillSignalHisto(iasic);
            double integral = Cluster(hADC[iasic]);
            if (integral>0) hShowerDistNoisy[iasic]->Fill(integral);
            hShowerSize[iasic]->Fill(nsize);
            hSeedEnergy[iasic]->Fill(eseed);
            eseed=0;
            cmnHGROC.clear();
        }
        hEnergyAllLayers->Fill(sumAllAsics);
    }
    fout->Write("", TObject::kOverwrite);
}

//_____________________________________________
int LoadSimData(TString electronfile, int iset)
{
    if (gSystem->AccessPathName(electronfile.Data())) {
        cout << "Input file not found!" << endl;
        return 0;
    }

    finelectron = TFile::Open(electronfile, "READ");
    trelectron = (TTree*)finelectron->Get("DataTree");
    trelectron->SetBranchStatus("*", 0);
    trelectron->SetBranchStatus("data_pad", 1);
    trelectron->SetBranchAddress("data_pad", &data_pad);

    // Load Pedestal data
    cout << "Load pedestals from file " << pedFile.Data() << endl;
    ifstream fPedestal(pedFile.Data());
    int ich, iasic;
    double ped, pedsigma;
    while (fPedestal>>iasic>>ich>>ped>>pedsigma) {
        Pedestal[iasic][ich] = ped;
        PedestalSigma[iasic][ich] = pedsigma;
    }

    return 1;
}

void LoadMIPCalibration()
{
    // Load Pedestal data
    cout << "Load MIP peak positions for calibration from file " << mipFile.Data() << endl;
    ifstream fMip(mipFile.Data());
    int ich, iasic;
    double mipPos;
    while (fMip>>iasic>>ich>>mipPos) {
        mip[iasic][ich] = mipPos;
    }
}

void InitOutput(TString energy)
{
    TString filename = Form("%s/sim-%s%s.root", dirname.Data(), energy.Data(),tag.Data());
    if (!gSystem->AccessPathName(filename.Data())) {
        fout = TFile::Open(filename.Data(), "UPDATE");
        hEnergyAllLayers = (TH1D*) fout->Get("hEnergyAllLayers");
        for (int iasic=0; iasic<nasic; iasic++) {
            hShowerSize[iasic] = (TH1D*) fout->Get(Form("hShowerSize_asic%d", iasic));
            hSeedEnergy[iasic] = (TH1D*) fout->Get(Form("hSeedEnergy_asic%d", iasic));
            hSeedEnergyIdeal[iasic] = (TH1D*) fout->Get(Form("hSeedEnergyIdeal_asic%d", iasic));
            hShowerDist[iasic] = (TH1D*) fout->Get(Form("hShowerDist_asic%d", iasic));
            hShowerDistNoisy[iasic] = (TH1D*) fout->Get(Form("hShowerDistNoisy_asic%d", iasic));
            hShowerMap[iasic] = (TH2D*) fout->Get(Form("hShowerMap_asic%d", iasic));
            hADC[iasic] = (TH2D*) fout->Get(Form("hADC_asic%d", iasic));
        }
    } else {
        fout = TFile::Open(filename.Data(), "RECREATE");
        hEnergyAllLayers = new TH1D("hEnergyAllLayers", "Energy summed in all layers", nbinE, minbinE, maxbinE); hEnergyAllLayers->Sumw2();
        for (int iasic=0; iasic<nasic; iasic++) {
            hShowerSize[iasic] = new TH1D(Form("hShowerSize_asic%d", iasic), Form("Shower size distribution for asic %d", iasic), nch+1, -0.5, 71.5); hShowerSize[iasic]->Sumw2();
            hSeedEnergy[iasic] = new TH1D(Form("hSeedEnergy_asic%d", iasic), Form("Seed energy distribution for asic %d", iasic), 161, -1.05, 15.05); hSeedEnergy[iasic]->Sumw2();
            hSeedEnergyIdeal[iasic] = new TH1D(Form("hSeedEnergyIdeal_asic%d", iasic), Form("Ideal seed energy distribution for asic %d", iasic), 161, -1.05, 15.05); hSeedEnergyIdeal[iasic]->Sumw2();
            hShowerDist[iasic] = new TH1D(Form("hShowerDist_asic%d", iasic), "Shower distribution without noise", nbin, minbin, maxbin); hShowerDist[iasic]->Sumw2();
            hShowerDistNoisy[iasic] = new TH1D(Form("hShowerDistNoisy_asic%d", iasic), "Shower distribution with noise", nbin, minbin, maxbin); hShowerDistNoisy[iasic]->Sumw2();
            hShowerMap[iasic] = new TH2D(Form("hShowerMap_asic%d", iasic), "Shower size 2D", 9, -0.5, 8.5, 8, -0.5, 7.5); hShowerMap[iasic]->Sumw2();
            hADC[iasic] = new TH2D(Form("hADC_asic%d", iasic), "", 9, -0.5, 8.5, 8, -0.5, 7.5);
        }
    }
}

double ConvertADCtoEnergy(int iasic, int ich, double adc)
{
    return (eMip/mip[iasic][ich])*adc;
    //return (eMip/10.)*adc;
}

int ConvertEnergyToADC(int iasic, int ich, double energy)
{
    return (int)(mip[iasic][ich]/eMip)*energy;
    //return (int)(10./eMip)*energy;
}

void FillSignalHisto(int iasic)
{
    for (int iy = 0; iy < nchy; iy++) {
        for (int ix = 0; ix < nchx; ix++) {
            int channelID = iy*nchx + ix;
            hADC[iasic]->SetBinContent(ix+1, iy+1, ConvertADCtoEnergy(iasic, channelID, ADCwoPed[iasic][channelID]));
        }
    }
}


double Cluster(TH2D* h, double threshold)
{
    int x,y,z;
	int NX = h->GetNbinsX();
	int NY = h->GetNbinsY();
    h->GetBinXYZ(h->GetMaximumBin(), x, y, z);
    //if (IsOnEdge(x, y)) return -1.;

    eseed = h->GetBinContent(x, y);
    if (eseed<=threshold) return -1;
    TH2D* hclusrec = new TH2D("hclusrec", "", 9, -0.5, 8.5, 8, -0.5, 7.5);
    TH2D* hclustag = new TH2D("hclustag", "", 9, -0.5, 8.5, 8, -0.5, 7.5);
	hclusrec->SetBinContent(x, y, eseed);
	hclustag->SetBinContent(x, y, 1);

    nsize = 0;
	for(int ix = 0; ix < NX; ix++){
		for(int iy = 0; iy < NY; iy++){
			if (hclustag->GetBinContent(ix+1, iy+1) == 1){
				bool found = 0;
				if( hclusrec->GetBinContent(ix+1, iy+2) == 0 && h->GetBinContent(ix+1, iy+2) > threshold && hclustag->GetBinContent(ix+1, iy+2) == 0 ){
					hclustag->SetBinContent(ix+1, iy+2, 1);
					hclusrec->SetBinContent(ix+1, iy+2, h->GetBinContent(ix+1, iy+2) );
					found = 1;
                    nsize++;
				}
				if( hclusrec->GetBinContent(ix+1, iy) == 0 && h->GetBinContent(ix+1, iy) > threshold && hclustag->GetBinContent(ix+1, iy) == 0 ){
					hclustag->SetBinContent(ix+1, iy, 1);
					hclusrec->SetBinContent(ix+1, iy, h->GetBinContent(ix+1, iy) );
					found = 1;
                    nsize++;
				}
				if( hclusrec->GetBinContent(ix, iy+1) == 0 && h->GetBinContent(ix, iy+1) > threshold && hclustag->GetBinContent(ix, iy+1) == 0 ){
					hclustag->SetBinContent(ix, iy+1, 1);
					hclusrec->SetBinContent(ix, iy+1, h->GetBinContent(ix, iy+1) );
                    found = 1;
                    nsize++;
				}
				if( hclusrec->GetBinContent(ix+2, iy+1) == 0 && h->GetBinContent(ix+2, iy+1) > threshold && hclustag->GetBinContent(ix+2, iy+1) == 0){
					hclustag->SetBinContent(ix+2, iy+1, 1);
					hclusrec->SetBinContent(ix+2, iy+1, h->GetBinContent(ix+2, iy+1) );
                    found = 1;
                    nsize++;
				}
				hclustag->SetBinContent(ix+1, iy+1, 0);
				if( found == 1 ){
					ix = 0;
					iy = 0;
				}
			}
		}
	}

	double integral = 0;
	for(int ix = 0; ix < NX; ix++) {
		for (int iy = 0; iy < NY; iy++) {
            int channelID = iy*nchx + ix;
            integral += hclusrec->GetBinContent(ix+1, iy+1);
            //cout << hclusrec->GetBinContent(ix+1, iy+1) << " ";
		}
        //cout << endl;
	}
    //cout << endl;

    //if (nsize==0) integral = -1.;

	delete hclusrec;
	delete hclustag;
	return integral;
}

void CreatePedestalFunctions()
{
    for (int iasic=0; iasic<nasic; iasic++) {
        for (int ich=0; ich<nch; ich++) {
            fPedestal[iasic][ich] = new TF1(Form("fPedestal_%d_%d", iasic, ich), "gaus", 0., 300.);
            fPedestal[iasic][ich]->SetParameters(1., Pedestal[iasic][ich], PedestalSigma[iasic][ich]);
        }
    }
}

void AddPedestalNoise(int iasic, int ich)
{
    double noise = fPedestal[iasic][ich]->GetRandom(0., 300.);
    if (iasic==0 && ich==32) noise = 0;
    ADC[iasic][ich] = ConvertEnergyToADC(iasic, ich, edep[iasic][ich]) + (int)noise;
    //cout << "ASIC" << iasic << " ch" << ich << " : " << ADC[iasic][ich] << endl;
}

// Check if channels is one of the edge channels
bool IsOnEdge(int x, int y)
{
    if (x==1 || x==9) return true;
    if (y==1 || y==8) return true;
    return false;
}
