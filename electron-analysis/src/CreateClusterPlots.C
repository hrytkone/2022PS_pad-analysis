#include "common.h"
#include "CreateClusterPlots.h"

void CreateClusterPlots(int energy, int evstart, int evend)
{
    InitOutput(energy, evstart, evend);
    LoadMIPCalibration();

    //for (int iset=0; iset<nset; iset++) {
    for (int iset=0; iset<1; iset++) {
        if (!LoadData(padfile[iset], pedFile)) return;

        int nev = trpad->GetEntriesFast();
        cout << "Energy : " << energy << "   entries : " << nev << endl;
        //for (int iev=0; iev<nev; iev++) {
        for (int iev=evstart; iev<evend; iev++) {
            trpad->GetEntry(iev);

            RemovePedestal();
            for (int iasic = 0; iasic < nasic; iasic++) {
                for (int iy = 0; iy < nchy; iy++) {
                    for (int ix = 0; ix < nchx; ix++) {
                        int channelID = Mapping[iy][ix];
                        hADC[iasic][iev]->SetBinContent(ix+1, iy+1, ADC[iasic][channelID]);
                        hADCwoPed[iasic][iev]->SetBinContent(ix+1, iy+1, ADCwoPed[iasic][channelID]);
                    }
                }
                cmnHGROC = GetCommonNoiseHGROC(iasic);
                RemoveCommonNoiseHGROC(iasic);
                for (int iy = 0; iy < nchy; iy++) {
                    for (int ix = 0; ix < nchx; ix++) {
                        int channelID = Mapping[iy][ix];
                        hADCwoPedAndCMN[iasic][iev]->SetBinContent(ix+1, iy+1, ADCwoPed[iasic][channelID]);
                        hEnergy[iasic][iev]->SetBinContent(ix+1, iy+1, ConvertADCtoEnergy(iasic, channelID, ADCwoPed[iasic][channelID]));
                    }
                }
                cmnHGROC.clear();
                double integral = ClusterADC(hEnergy[iasic][iev], iasic, iev);
            }
        }
    }
    fout->cd();
    fout->Write("", TObject::kOverwrite);
}

//____________________________________________________
void InitOutput(int energy, int evstart, int evend)
{
    TString outfile = Form("2022-07-11_shower-data/events_%dGeV.root", energy);
    fout = TFile::Open(outfile.Data(), "RECREATE");
    for (int i=evstart; i<evend; i++) {
        for (int iasic=0; iasic<nasic; iasic++) {
            hADC[iasic][i] = new TH2D(Form("hEvent%d_asic%d_ADC", i, iasic), "", 9, -0.5, 8.5, 8, -0.5, 7.5);
            hADCwoPed[iasic][i] = new TH2D(Form("hEvent%d_asic%d_ADCwoPed", i, iasic), "", 9, -0.5, 8.5, 8, -0.5, 7.5);
            hADCwoPedAndCMN[iasic][i] = new TH2D(Form("hEvent%d_asic%d_ADCwoPedAndCMN", i, iasic), "", 9, -0.5, 8.5, 8, -0.5, 7.5);
            hEnergy[iasic][i] = new TH2D(Form("hEvent%d_asic%d_energy", i, iasic), "", 9, -0.5, 8.5, 8, -0.5, 7.5);
            hCluster[iasic][i] = new TH2D(Form("hEvent%d_asic%d_cluster", i, iasic), "", 9, -0.5, 8.5, 8, -0.5, 7.5);
        }
    }
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

double ClusterADC(TH2D* h, int iasic, int iev, double threshold)
{
    int x,y,z;
	int NX = h->GetNbinsX();
	int NY = h->GetNbinsY();
    h->GetBinXYZ(h->GetMaximumBin(), x, y, z);
    if (IsOnEdge(x, y)) return -1.;

    double max = h->GetBinContent(x, y);
    cout << "\n event " << iev << "   iasic=" << iasic << endl;
    cout << "SEED : bin=" << x << " " << y << "   value=" << max << endl;
    TH2D* hclusrec = new TH2D("hclusrec", "", 9, -0.5, 8.5, 8, -0.5, 7.5);
    TH2D* hclustag = new TH2D("hclustag", "", 9, -0.5, 8.5, 8, -0.5, 7.5);
	hclusrec->SetBinContent(x, y, h->GetBinContent(x, y) );
	hclustag->SetBinContent(x, y, 1);

    int nsize = 0;
	for (int ix = 0; ix < NX; ix++){
		for (int iy = 0; iy < NY; iy++){
			if (hclustag->GetBinContent(ix+1, iy+1) == 1) {
				bool found = 0;
				if ( hclusrec->GetBinContent(ix+1, iy+2) == 0 && h->GetBinContent(ix+1, iy+2) > threshold && hclustag->GetBinContent(ix+1, iy+2) == 0 ){
					hclustag->SetBinContent(ix+1, iy+2, 1);
					hclusrec->SetBinContent(ix+1, iy+2, h->GetBinContent(ix+1, iy+2) );
                    found = 1;
                    nsize++;
				}
				if ( hclusrec->GetBinContent(ix+1, iy) == 0 && h->GetBinContent(ix+1, iy) > threshold && hclustag->GetBinContent(ix+1, iy) == 0 ){
					hclustag->SetBinContent(ix+1, iy, 1);
					hclusrec->SetBinContent(ix+1, iy, h->GetBinContent(ix+1, iy) );
                    found = 1;
                    nsize++;
				}
				if ( hclusrec->GetBinContent(ix, iy+1) == 0 && h->GetBinContent(ix, iy+1) > threshold && hclustag->GetBinContent(ix, iy+1) == 0 ){
					hclustag->SetBinContent(ix, iy+1, 1);
					hclusrec->SetBinContent(ix, iy+1, h->GetBinContent(ix, iy+1) );
                    found = 1;
                    nsize++;
				}
				if ( hclusrec->GetBinContent(ix+2, iy+1) == 0 && h->GetBinContent(ix+2, iy+1) > threshold && hclustag->GetBinContent(ix+2, iy+1) == 0){
					hclustag->SetBinContent(ix+2, iy+1, 1);
					hclusrec->SetBinContent(ix+2, iy+1, h->GetBinContent(ix+2, iy+1) );
					found = 1;
                    nsize++;
				}
				hclustag->SetBinContent(ix+1, iy+1, 0);
				if ( found == 1 ) {
					ix = 0;
					iy = 0;
				}
			}
		}
	}
	double integral = 0;
    for (int iy = 0; iy < NY; iy++) {
	       for (int ix = 0; ix < NX; ix++) {
               integral += hclusrec->GetBinContent(ix+1, iy+1);
               hCluster[iasic][iev]->Fill(ix+1, iy+1, hclusrec->GetBinContent(ix+1, iy+1));
		}
	}

    //if (nsize==0) integral = -1.;

	delete hclusrec;
	delete hclustag;
	return integral;
}

// Check if channels is one of the edge channels
bool IsOnEdge(int x, int y)
{
    if (x==1 || x==9) return true;
    if (y==1 || y==8) return true;
    return false;
}

double ConvertADCtoEnergy(int iasic, int ich, double adc)
{
    return (eMip/mip[iasic][ich])*adc;
}
