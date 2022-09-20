#include "common.h"
#include "CalibrationMIP.h"

void CalibrationMIP(TString ifile, TString particle, TString energy)
{
    if (!LoadSimData(ifile)) return;
    InitOutput(particle, energy);

    int nev = trsim->GetEntriesFast();
    for (int iev=0; iev<nev; iev++) {
        trsim->GetEntry(iev);

        int iasic = -1;
        while (iasic<nasic+1) {
            for (int iy = 0; iy < nchy; iy++) {
                for (int ix = 0; ix < nchx; ix++) {
                    int channelID = iasic*nch + iy*nchx + ix;
                    if (channelID%nch==0) iasic++;
                    if (data_pad[channelID]<=0) continue;
                    hMIP[iasic][channelID%nch]->Fill(data_pad[channelID]);
                    //cout << data_pad[channelID] << endl;
                }
            }
        }
    }
    fout->Write("", TObject::kOverwrite);
}

//******************************************************************************
int LoadSimData(TString simfile)
{
    if (gSystem->AccessPathName(simfile.Data())) {
        cout << "Input file not found!" << endl;
        return 0;
    }

    finsim = TFile::Open(simfile, "READ");
    trsim = (TTree*)finsim->Get("DataTree");
    trsim->SetBranchStatus("*", 0);
    trsim->SetBranchStatus("data_pad", 1);
    trsim->SetBranchAddress("data_pad", &data_pad);

    return 1;
}

void InitOutput(TString particle, TString energy)
{
    if (!gSystem->AccessPathName(Form("%s/calib-%s-%s.root", dirname.Data(), particle.Data(), energy.Data()))) {
        fout = TFile::Open(Form("%s/calib-%s-%s.root", dirname.Data(), particle.Data(), energy.Data()), "UPDATE");
        for (int iasic=0; iasic<nasic+2; iasic++) {
            for (int ich=0; ich<nch; ich++) {
                hMIP[iasic][ich] = (TH1D*)fout->Get(Form("hMIP_asic%d_ch%d", iasic, ich));
            }
        }
    } else {
        fout = TFile::Open(Form("%s/calib-%s-%s.root", dirname.Data(), particle.Data(), energy.Data()), "RECREATE");
        for (int iasic=0; iasic<nasic+2; iasic++) {
            for (int ich=0; ich<nch; ich++) {
                hMIP[iasic][ich] = new TH1D(Form("hMIP_asic%d_ch%d", iasic, ich), Form("Simulated MIP for ASIC %d, ch %d", iasic, ich), nbin, minbin, maxbin); hMIP[iasic][ich]->Sumw2();
            }
        }
    }
}
