const int nset = 3;
const int nenergy = 4;
const int nasic = 18;
const int nch = 72;
const TString input[nset][nenergy] = {
    {"2022-09-19_shower-data/sim-002GeV.root",
     "2022-09-19_shower-data/sim-003GeV.root",
     "2022-09-19_shower-data/sim-004GeV.root",
     "2022-09-19_shower-data/sim-005GeV.root"},
    {"2022-09-19_shower-data/2GeV_no-box.root",
     "2022-09-19_shower-data/3GeV_no-box.root",
     "2022-09-19_shower-data/4GeV_no-box.root",
     "2022-09-19_shower-data/5GeV_no-box.root"},
    {"2022-09-19_shower-data/2GeV_box.root",
     "2022-09-19_shower-data/3GeV_box.root",
     "2022-09-19_shower-data/4GeV_box.root",
     "2022-09-19_shower-data/5GeV_box.root"}
};

const int energy[nenergy] = {2,3,4,5};//,5};
const EColor mColor[nset] = {kBlack, kRed, kBlue};//, kMagenta};//, kOrange, kBlack};

int Map[nch] = {//map of channels in XY
    43,  41,  53,  49,  58,  54,  66,  68,  69,
    39,  37,  47,  51,  56,  62,  60,  64,  70,
    38,  42,  45,  46,  50,  59,  55,  65,  71,
    44,  40,  36,  48,  52,  61,  57,  67,  63,
    8,   4,   0,  14,  16,  19,  23,  31,  27,
    2,   6,  11,  10,  17,  21,  25,  33,  35,
    1,   3,   9,  15,  22,  24,  26,  28,  34,
    7,   5,  12,  13,  18,  20,  29,  30,  32
};

TFile *fin[nset][nenergy];
TH1D *hShowerSize[nset][nenergy][nasic];
TH1D *hSeedEnergy[nset][nenergy][nasic];
TCanvas *cSize[nenergy], *cSeed[nenergy];
TLegend *leg[nenergy][nasic];

int LoadData();
void ConfigPlots();
void Plot();
void SetStyle(Bool_t graypalette);

void PlotClusterObs()
{
    SetStyle(0);
    if (!LoadData()) return;
    ConfigPlots();
    Plot();
}

//******************************************************************************
//******************************************************************************
int LoadData()
{
    for (int iset=0; iset<nset; iset++) {
        for (int ie=0; ie<nenergy; ie++) {
            if (gSystem->AccessPathName(input[iset][ie].Data())) {
                std::cout << "File " << input[iset][ie].Data() << " not found, return" << std::endl;
                return 0;
            }
            fin[iset][ie] = TFile::Open(input[iset][ie], "READ");
            for (int iasic=0; iasic<nasic; iasic++) {
                hShowerSize[iset][ie][iasic] = (TH1D*)fin[iset][ie]->Get(Form("hShowerSize_asic%d", iasic));
                hSeedEnergy[iset][ie][iasic] = (TH1D*)fin[iset][ie]->Get(Form("hSeedEnergy_asic%d", iasic));
            }
        }
    }
    return 1;
}

void ConfigPlots()
{
    for (int iset=0; iset<nset; iset++) {
        for (int ie=0; ie<nenergy; ie++) {
            for (int iasic=0; iasic<nasic; iasic++) {
                hShowerSize[iset][ie][iasic]->SetLineColor(mColor[iset]);
                hShowerSize[iset][ie][iasic]->SetMarkerColor(mColor[iset]);
                hShowerSize[iset][ie][iasic]->SetMarkerStyle(kFullCircle);
                if (iset==0)
                    hShowerSize[iset][ie][iasic]->SetMarkerStyle(kOpenCircle);
                hShowerSize[iset][ie][iasic]->SetMarkerSize(0.5);
                hShowerSize[iset][ie][iasic]->SetTitle(";N_{cluster}; 1/N dN/dN_{clust}");
                hShowerSize[iset][ie][iasic]->Scale(1./hShowerSize[iset][ie][iasic]->GetEntries());
                hShowerSize[iset][ie][iasic]->GetXaxis()->SetRangeUser(-0.5, 30.5);
                //hShowerSize[iset][ie][iasic]->GetYaxis()->SetRangeUser(0., 0.12);
                hShowerSize[iset][ie][iasic]->GetXaxis()->SetLabelSize(0.042);
                hShowerSize[iset][ie][iasic]->GetYaxis()->SetLabelSize(0.042);
                hShowerSize[iset][ie][iasic]->GetXaxis()->SetTitleSize(0.042);
                hShowerSize[iset][ie][iasic]->GetYaxis()->SetTitleSize(0.042);
                hShowerSize[iset][ie][iasic]->GetYaxis()->SetTitleOffset(1.4);

                hSeedEnergy[iset][ie][iasic]->SetLineColor(mColor[iset]);
                hSeedEnergy[iset][ie][iasic]->SetMarkerColor(mColor[iset]);
                hSeedEnergy[iset][ie][iasic]->SetMarkerStyle(kFullCircle);
                if (iset==0)
                hSeedEnergy[iset][ie][iasic]->SetMarkerStyle(kOpenCircle);
                hSeedEnergy[iset][ie][iasic]->SetMarkerSize(0.5);
                hSeedEnergy[iset][ie][iasic]->SetTitle(";E_{seed}; 1/N dN/dE_{seed}");
                hSeedEnergy[iset][ie][iasic]->Scale(1./hSeedEnergy[iset][ie][iasic]->GetEntries());
                //hSeedEnergy[iset][ie][iasic]->Scale(1./hSeedEnergy[iset][ie][iasic]->Integral(12,111));
                hSeedEnergy[iset][ie][iasic]->GetXaxis()->SetRangeUser(-0.5, 10.5);
                hSeedEnergy[iset][ie][iasic]->GetYaxis()->SetRangeUser(0.000001, 1.);
                hSeedEnergy[iset][ie][iasic]->GetXaxis()->SetLabelSize(0.042);
                hSeedEnergy[iset][ie][iasic]->GetYaxis()->SetLabelSize(0.042);
                hSeedEnergy[iset][ie][iasic]->GetXaxis()->SetTitleSize(0.042);
                hSeedEnergy[iset][ie][iasic]->GetYaxis()->SetTitleSize(0.042);
                hSeedEnergy[iset][ie][iasic]->GetYaxis()->SetTitleOffset(1.4);

            }
        }
    }

    for (int ie=0; ie<nenergy; ie++) {
        for (int iasic=0; iasic<nasic; iasic++) {
            leg[ie][iasic] = new TLegend(0.55, 0.7, 0.85, 0.9);
            leg[ie][iasic]->SetFillStyle(0); leg[ie][iasic]->SetBorderSize(0); leg[ie][iasic]->SetTextSize(0.062);
            leg[ie][iasic]->SetHeader(Form("E=%d GeV, ASIC %d", energy[ie], iasic));
            leg[ie][iasic]->AddEntry(hShowerSize[0][ie][iasic], "sim", "pe");
            leg[ie][iasic]->AddEntry(hShowerSize[1][ie][iasic], "no box", "pe");
            leg[ie][iasic]->AddEntry(hShowerSize[2][ie][iasic], "box", "pe");
        }
    }
}

void Plot()
{
    TText *t1 = new TText(.44, .84, "2022 PS testbeam");
    t1->SetTextSize(0.062);
    t1->SetNDC();
    TText *t2 = new TText(.44, .79, "FoCal-E pad layers");
    t2->SetTextSize(0.062);
    t2->SetNDC();

    for (int ie=0; ie<nenergy; ie++) {
        cSize[ie] = new TCanvas(Form("cSize_%d", ie), "", 1200, 600);
        cSize[ie]->Divide(6, 3, 0, 0);
        for (int iasic=0; iasic<nasic; iasic++) {
            cSize[ie]->cd(iasic+1);
            //gPad->SetLogy();
            //canvas[ie]->SetLogy();
            //canvas[ie]->SetBorderSize(2);

            for (int iset=0; iset<nset; iset++) {
                if (iset==0) {
                    hShowerSize[iset][ie][iasic]->Draw("HIST");
                } else {
                    hShowerSize[iset][ie][iasic]->Draw("HIST SAME");
                }
            }
            leg[ie][iasic]->Draw("SAME");
            //t1->Draw("SAME");
            //t2->Draw("SAME");
            gPad->RedrawAxis();
        }
        cSize[ie]->SaveAs(Form("cluster-size_%dGeV.eps", energy[ie]));
    }

    for (int ie=0; ie<nenergy; ie++) {
        cSeed[ie] = new TCanvas(Form("cSeed_%d", ie), "", 1200, 600);
        cSeed[ie]->Divide(6, 3, 0, 0);
        for (int iasic=0; iasic<nasic; iasic++) {
            cSeed[ie]->cd(iasic+1);
            gPad->SetLogy();
            for (int iset=0; iset<nset; iset++) {
                if (iset==0) {
                    hSeedEnergy[iset][ie][iasic]->Draw("HIST");
                } else {
                    hSeedEnergy[iset][ie][iasic]->Draw("HIST SAME");
                }
            }
            leg[ie][iasic]->Draw("SAME");
            //t1->Draw("SAME");
            //t2->Draw("SAME");
            gPad->RedrawAxis();
        }
        cSeed[ie]->SaveAs(Form("seed-energy_%dGeV.eps", energy[ie]));
    }
}


void SetStyle(Bool_t graypalette)
{
    cout << "Setting style!" << endl;

    //gStyle->Reset("Plain");
    //gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    //gStyle->SetLineScalePS(1);
    //if(graypalette) gStyle->SetPalette(8,0);
    //else gStyle->SetPalette(1);
    gStyle->SetCanvasColor(10);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetFrameLineWidth(2);
    gStyle->SetFrameFillColor(kWhite);
    gStyle->SetPadColor(10);
    gStyle->SetPadTickX(0);
    gStyle->SetPadTickY(0);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetHistLineWidth(1);
    //gStyle->SetHistLineColor(kRed);
    gStyle->SetFuncWidth(2);
    //gStyle->SetFuncColor(kGreen);
    gStyle->SetLineWidth(1);
    gStyle->SetLabelSize(0.035,"xyz");
    gStyle->SetLabelOffset(0.01,"y");
    gStyle->SetLabelOffset(0.01,"x");
    gStyle->SetLabelColor(kBlack,"xyz");
    //gStyle->SetTitleSize(0.035,"xyz");
    //gStyle->SetTitleOffset(1.25,"y");
    //gStyle->SetTitleOffset(1.2,"x");
    //gStyle->SetTitleFillColor(kWhite);
    gStyle->SetTextSizePixels(26);
    gStyle->SetTextFont(42);
    //  gStyle->SetTickLength(0.04,"X");  gStyle->SetTickLength(0.04,"Y");

    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendFillColor(kWhite);
    //  gStyle->SetFillColor(kWhite);
    gStyle->SetLegendFont(42);
}
