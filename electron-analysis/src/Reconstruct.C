#include "common.h"
#include "Reconstruct.h"

void Reconstruct(int ienergy)
{
    InitOutput(ienergy);
    LoadMIPCalibration();

    for (int iset=0; iset<nset; iset++) {
        if (padfile[ienergy][iset]=="") continue;
        if (!LoadData(padfile[ienergy][iset], pedFile)) return;

        int nev = trpad->GetEntriesFast();
        cout << "Energy : " << Energy[ienergy] << "   entries : " << nev << endl;
        for (int iev=0; iev<nev; iev++) {
            if (iev%10000==0) cout << iev << "/" << nev << endl;
            trpad->GetEntry(iev);

            RemovePedestal();
            FillSignalHistos();

            double sumAllAsics = 0.;
            for (int iasic=0; iasic<nasic; iasic++) {
                if (!asicStatus[ienergy][iset][iasic]) continue;
                double integral = ClusterADC(hADC[iasic], ethres);
                sumAllAsics += integral;
                if (integral>0) hClusters[iasic]->Fill(integral);
                hShowerSize[iasic]->Fill(nsize);
                hSeedEnergy[iasic]->Fill(eseed);
            }
            hEnergyAllLayers->Fill(sumAllAsics);
        }
    }
    fout->cd();
    fout->Write("", TObject::kOverwrite);
}

//______________________________________________________________________________
void InitOutput(int ienergy)
{
    TString outfile = Form("%s/%dGeV%s.root", dirname.Data(), Energy[ienergy], tag.Data());
    fout = TFile::Open(outfile.Data(), "RECREATE");
    hEnergyAllLayers = new TH1D("hEnergyAllLayers", "Energy summed in all layers", nbinE, minbinE, maxbinE); hEnergyAllLayers->Sumw2();
    for (int iasic=0; iasic<nasic; iasic++) {
        hShowerSize[iasic] = new TH1D(Form("hShowerSize_asic%d", iasic), Form("Shower size distribution for asic %d", iasic), nch+1, -0.5, 71.5); hShowerSize[iasic]->Sumw2();
        hClusters[iasic] = new TH1D(Form("hClusters_asic%d", iasic), Form("Cluster distributions for asic %d", iasic), nbin, minbin, maxbin); hClusters[iasic]->Sumw2();
        hADC[iasic] = new TH2D(Form("hADC_asic%d", iasic), "", 9, -0.5, 8.5, 8, -0.5, 7.5); hADC[iasic]->Sumw2();
        hSeedEnergy[iasic] = new TH1D(Form("hSeedEnergy_asic%d", iasic), Form("Seed energy distribution for asic %d", iasic), 161, -1.05, 15.05); hSeedEnergy[iasic]->Sumw2();
    }
}

void FillSignalHistos()
{
    for (int iasic = 0; iasic < nasic; iasic++) {
        //cmnHGROC = GetCommonNoiseHGROC(iasic);
        //RemoveCommonNoiseHGROC(iasic);
        cmn = GetCommonNoiseEdges(iasic);
        RemoveCommonNoise(iasic);
        //cout << "ASIC" << iasic << "\tCMN:" << cmnHGROC[0] << "  " << cmnHGROC[1] << "\tCMN edge:" << cmn << endl;
        for (int iy = 0; iy < nchy; iy++) {
            for (int ix = 0; ix < nchx; ix++) {
                int channelID = Mapping[iy][ix];
                if (iasic==0 && channelID==32)
                    hADC[iasic]->SetBinContent(ix+1, iy+1, 0.);
                else
                    hADC[iasic]->SetBinContent(ix+1, iy+1, ADCwoPed[iasic][channelID]);
                //hADC[iasic]->SetBinContent(ix+1, iy+1, ConvertADCtoEnergy(iasic, channelID, ADCwoPed[iasic][channelID]));
                //hADC[iasic]->SetBinContent(ix+1, iy+1, ADC[iasic][channelID]);
                //cout << hADC[iasic]->GetBinContent(ix+1, iy+1) << " ";
            }
            //cout << endl;
        }
        cmnHGROC.clear();
        cmn = 0;
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

double ClusterADC(TH2D* h, double threshold)
{
    int x,y,z;
	int NX = h->GetNbinsX();
	int NY = h->GetNbinsY();
    h->GetBinXYZ(h->GetMaximumBin(), x, y, z);
    //x -= 1;
    //y -= 1;
    if (IsOnEdge(x, y)) return -1.;

    eseed = h->GetBinContent(x, y);
    //if (eseed<=threshold) return -1;
    TH2D* hclusrec = new TH2D("hclusrec", "", 9, -0.5, 8.5, 8, -0.5, 7.5);
    TH2D* hclustag = new TH2D("hclustag", "", 9, -0.5, 8.5, 8, -0.5, 7.5);
	hclusrec->SetBinContent(x, y, eseed );
	hclustag->SetBinContent(x, y, 1);

    nsize = 0;
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
//    cout << "size:" << nsize << endl;
    for (int iy = 0; iy < NY; iy++) {
	       for (int ix = 0; ix < NX; ix++) {
//               cout << hclusrec->GetBinContent(ix+1, iy+1) << " ";
               integral += hclusrec->GetBinContent(ix+1, iy+1);
		}
//        cout << endl;
	}
//    cout << endl;

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
    //if (x==0 || x==8) return true;
    //if (y==0 || y==7) return true;
    return false;
}

double ConvertADCtoEnergy(int iasic, int ich, double adc)
{
    return (eMip/(mip[iasic][ich]))*adc;
    //return (eMip/20.)*adc;
}
