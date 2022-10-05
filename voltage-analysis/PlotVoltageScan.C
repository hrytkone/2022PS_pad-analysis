#include "../mip-analysis/src/common.h"
#include "../mip-analysis/src/langaus.h"

const int nfiles = 14;

TString dirs[nfiles] = {
// VOLTAGE SCAN
    "2022-06-11-07-55-53","2022-06-11-02-54-23","2022-06-11-03-26-05",
    "2022-06-11-03-50-16","2022-06-11-04-14-16","2022-06-11-04-37-50",
    "2022-06-11-05-00-49","2022-06-11-05-24-23","2022-06-11-05-49-01",
    "2022-06-11-06-12-52","2022-06-11-06-37-01","2022-06-11-07-03-26",
    "2022-06-11-07-27-46","2022-06-10-16-27-59"
};

int voltage[nfiles] = { 500, 450, 400, 350, 300, 250, 200, 150, 100, 75, 50, 25, 0, 500 };
Double_t voltGraph[nfiles] = { 500, 450, 400, 350, 300, 250, 200, 150, 100, 75, 50, 25 };
Double_t voltErr[nfiles] = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };
double gausfitmin[nasic][nfiles] = {
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.}, // 0
    {-3.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-5.,-5.,-10.,-4.}, // 1
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-10.,-10.,-10.,-10.,-10.,-4.,-4.}, // 2
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.}, // 3
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.}, // 4
    {-6.,-4.,-4.,-4.,-4.,-4.,-6.,-5.,-4.,-10.,-10.,-4.,-4.,-4.}, // 5
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.}, // 6
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.}, // 7
    {-4.,-4.,-4.,-6.,-4.,-5.,-4.,-10.,-10.,-10.,-5.,-4.,-4.,-4.}, // 8
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.}, // 9
    {-4.,-4.,-4.,-4.,-4.,-4.,-6.,-6.,-10.,-10.,-4.,-10.,-4.,-4.}, // 10
    {-5.,-4.,-5.,-4.,-4.,-4.,-4.,-4.,-4.,-10.,-10.,-4.,-4.,-4.}, // 11
    {-5.,-4.,-4.,-4.,-5.,-4.,-4.,-5.,-4.,-4.,-4.,-4.,-4.,-4.}, // 12
    {-5.,-5.,-5.,-5.,-5.,-4.,-4.,-4.,-5.,-5.,-5.,-5.,-4.,-4.}, // 13
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-10.,-10.,-4.,-4.}, // 14
    {-4.,-5.,-4.,-4.,-4.,-4.,-4.,-4.,-6.,-6.,-10.,-5.,-4.,-4.}, // 15
    {-4.,-5.,-4.,-4.,-4.,-4.,-6.,-6.,-6.,-6.,-6.,-6.,-4.,-4.}, // 16
    {-4.,-4.,-4.,-4.,-4.,-4.,-4.,-4.,-6.,-6.,-6.,-6.,-4.,-4.}  // 17
};
double gausfitmax[nasic][nfiles] = {
    {5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.}, // 0
    {4.,5.,5.,5.,5.,5.,3.,2.,3.,1.,1.,1.,10.,5.}, // 1
    {5.,5.,5.,5.,5.,5.,5.,3.,2.,2.,5.,5.,5.,5.}, // 2
    {5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.}, // 3
    {5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.}, // 4
    {3.,5.,5.,5.,3.,3.,3.,2.,2.,2.,2.,5.,5.,5.}, // 5
    {5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.}, // 6
    {5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.,5.}, // 7
    {5.,5.,3.,3.,5.,2.,5.,3.,1.,2.,1.,5.,5.,5.}, // 8
    {5.,2.,5.,5.,5.,2.,5.,5.,5.,5.,5.,5.,5.,5.}, // 9
    {2.,5.,5.,5.,5.,5.,3.,2.,2.,2.,5.,5.,5.,5.}, // 10
    {3.,5.,5.,5.,5.,5.,5.,3.,3.,2.,3.,5.,5.,5.}, // 11
    {3.,5.,5.,5.,3.,5.,5.,3.,2.,2.,2.,5.,5.,5.}, // 12
    {5.,4.,5.,5.,5.,5.,4.,2.,3.,3.,3.,2.,5.,5.}, // 13
    {2.,2.,2.,2.,2.,2.,2.,2.,2.,2.,2.,2.,2.,2.}, // 14
    {5.,2.,5.,5.,2.,5.,5.,2.,2.,2.,2.,2.,5.,5.}, // 15
    {5.,2.,2.,5.,2.,2.,3.,3.,3.,3.,5.,3.,5.,5.}, // 16
    {5.,2.,5.,5.,5.,2.,5.,5.,3.,3.,3.,5.,5.,5.}  // 17
};

int mColor[nasic] = {kMagenta+2, kBlue, kRed, kViolet, kMagenta, kCyan, kOrange+2, kBlue+2, kRed+2, kViolet+2, kOrange, kCyan+2,
    kBlack, kBlue-7, kRed-7, kViolet-7, kMagenta-7, kAzure+2
};

TFile *finput[nfiles];
TH1D *hMipPerChannel[nfiles][nasic][nch];
TH1D *hMipPedestalSubtracted[nfiles];
TF1 *fGaussianFit[nasic][nfiles];
TF1 *fLandauFit[nasic][nfiles];
TCanvas *c1[nasic];
TCanvas *c2[nasic];
TCanvas *c3[nasic];
TCanvas *c4;
TCanvas *c5;
TCanvas *c6;

TGraphErrors *gMipPos[nasic];
TGraphErrors *gMipWidth[nasic];
TGraphErrors *gSigmaLandauGaus[nasic];

Double_t posMip[nasic][nfiles-2] = {0};
//Double_t posMip[nasic][nfiles] = {0};
Double_t posMipErr[nasic][nfiles-2] = {0};
//Double_t posMipErr[nasic][nfiles] = {0};
Double_t wMip[nasic][nfiles-2] = {0};
//Double_t wMip[nasic][nfiles] = {0};
Double_t wMipErr[nasic][nfiles-2] = {0};
//Double_t wMipErr[nasic][nfiles] = {0};
Double_t sigmaLandauGaus[nasic][nfiles-2] = {0};
//Double_t sigmaLandauGaus[nasic][nfiles] = {0};

void LoadHistos(int ifile);
void LoadPedestals(TString pedFile);
void RedrawBorder();
void SetStyle(Bool_t graypalette);

void PlotVoltageScan()
{
    SetStyle(0);

    for (int iasic=0; iasic<nasic; iasic++) {
        c1[iasic] = new TCanvas(Form("c1_%d", iasic), "c1", 800, 680);
        gPad->SetTopMargin(0.21);
        gPad->SetBottomMargin(0.21);
        gPad->SetLeftMargin(0.18);
        TLatex ltitle;
        ltitle.SetTextSize(0.042);
        ltitle.DrawLatexNDC(0.06, 0.955, Form("FoCal-E pad layers, 2022 PS testbeam, Voltage scan (ASIC %d)", iasic));
        c1[iasic]->Divide(4, 3, 0, 0);
        TLatex laxis;
        laxis.SetTextSize(0.035);
        laxis.DrawLatexNDC(0.93, 0.01, "ADC");
        laxis.SetTextAngle(90);
        laxis.DrawLatexNDC(0.022, 0.84, "counts");
        for (int ifile=0; ifile<nfiles-2; ifile++) {
            c1[iasic]->cd(ifile+1);
            //gPad->SetLogy();

            finput[ifile] = TFile::Open(Form("voltage_data/%s/nopedcmn_v2.root", dirs[ifile].Data()));

            LoadHistos(ifile);

            int ich = 17;
            //int ich = 29;
            if (ifile==nfiles-1) ich = 16;

            fGaussianFit[iasic][ifile] = new TF1(Form("fGaussianFit_%d_%d", iasic, ifile), "[0]*TMath::Gaus(x,[1],[2])", -20., 100.);
            fGaussianFit[iasic][ifile]->SetNpx(1000);
            fGaussianFit[iasic][ifile]->SetLineColor(kBlack);
            fGaussianFit[iasic][ifile]->SetParameters(hMipPerChannel[ifile][iasic][ich]->GetMaximum(), hMipPerChannel[ifile][iasic][ich]->GetMean(), hMipPerChannel[ifile][iasic][ich]->GetRMS());
            //fGaussianFit[ifile]->SetParLimits(1, -2., 2.);
            //fGaussianFit[ifile]->SetParLimits(2, 0., 20.);

            int binmax = hMipPerChannel[ifile][iasic][ich]->GetMaximumBin();
            double x = hMipPerChannel[ifile][iasic][ich]->GetXaxis()->GetBinCenter(binmax);
            hMipPerChannel[ifile][iasic][ich]->GetXaxis()->SetRangeUser(x+gausfitmin[iasic][ifile], x+gausfitmax[iasic][ifile]);
            cout << "\nGaussian range : [ " << x+gausfitmin[iasic][ifile] << " " << x+gausfitmax[iasic][ifile] << " ]" << endl;
            //hMipPerChannel[ifile][iasic][ich]->GetXaxis()->SetRangeUser(gausfitmin[ifile][iasic], gausfitmax[ifile][iasic]);
            //hMipPerChannel[ifile][iasic][ich]->Fit("gaus", "Q");
            hMipPerChannel[ifile][iasic][ich]->Fit(Form("fGaussianFit_%d_%d", iasic, ifile), "RLQE");
            hMipPerChannel[ifile][iasic][ich]->GetXaxis()->SetRangeUser(-11., 45.);
            if (iasic>13)
                hMipPerChannel[ifile][iasic][ich]->GetYaxis()->SetRangeUser(0.01, 25000.);
            else
                hMipPerChannel[ifile][iasic][ich]->GetYaxis()->SetRangeUser(0.01, 19500.);
            hMipPerChannel[ifile][iasic][ich]->SetTitle(";;");
            hMipPerChannel[ifile][iasic][ich]->GetYaxis()->SetMaxDigits(3);
            hMipPerChannel[ifile][iasic][ich]->GetXaxis()->SetLabelSize(0.1);
            hMipPerChannel[ifile][iasic][ich]->GetYaxis()->SetLabelSize(0.1);
            if (ifile==0 || ifile==4)
                hMipPerChannel[ifile][iasic][ich]->GetYaxis()->SetLabelSize(0.12);
            if (ifile==9 || ifile==10 || ifile==11)
                hMipPerChannel[ifile][iasic][ich]->GetXaxis()->SetLabelSize(0.12);
            hMipPerChannel[ifile][iasic][ich]->GetXaxis()->SetNdivisions(505);
            hMipPerChannel[ifile][iasic][ich]->GetYaxis()->SetNdivisions(505);
            hMipPerChannel[ifile][iasic][ich]->SetMarkerStyle(kFullCircle);
            hMipPerChannel[ifile][iasic][ich]->SetMarkerColor(kBlack);
            hMipPerChannel[ifile][iasic][ich]->SetMarkerSize(0.85);

            cout << "Voltage " << voltage[ifile] << "\tpedestal pos=" << fGaussianFit[iasic][ifile]->GetParameter(1) << endl;

            hMipPedestalSubtracted[ifile] = (TH1D*) hMipPerChannel[ifile][iasic][ich]->Clone(Form("hMipPedestalSubtracted_%d", ifile));
            hMipPedestalSubtracted[ifile]->SetLineColor(kRed);
            hMipPedestalSubtracted[ifile]->Add(fGaussianFit[iasic][ifile], -1, "I");

            // Setting fit range and start values
            Double_t fr[2];
            Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];
            fr[0]=0.3*hMipPedestalSubtracted[ifile]->GetMean();
            fr[1]=1.4*hMipPedestalSubtracted[ifile]->GetMean();
            if (iasic==13 && ifile<8) {
                fr[0]=0.6*hMipPedestalSubtracted[ifile]->GetMean();
                fr[1]=1.6*hMipPedestalSubtracted[ifile]->GetMean();
            }
            if (ifile>8) {
                fr[0]=0.05*hMipPedestalSubtracted[ifile]->GetMean();
                //fr[1]=1.5*hMipPedestalSubtracted[ifile]->GetMean();
            }

            pllo[0]=0.5; pllo[1]=5.0; pllo[2]=1.0; pllo[3]=0.4;
            plhi[0]=5.0; plhi[1]=50.0; plhi[2]=1000000.0; plhi[3]=5.0;
            sv[0]=1.8; sv[1]=20.0; sv[2]=50000.0; sv[3]=3.0;

            Double_t chisqr;
            Int_t    ndf;
            fLandauFit[iasic][ifile] = langaufit(hMipPedestalSubtracted[ifile],fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);

            Double_t SNRPeak, SNRFWHM;
            langaupro(fp,SNRPeak,SNRFWHM);

            double mip = fLandauFit[iasic][ifile]->GetParameter(1) - fGaussianFit[iasic][ifile]->GetParameter(1);

            cout << "\tMIP pos = " << fLandauFit[iasic][ifile]->GetParameter(1) - fGaussianFit[iasic][ifile]->GetParameter(1) << endl;
            cout << "\tMIP w = " << fLandauFit[iasic][ifile]->GetParameter(0) << endl;
            cout << "\tGaus under landau sigma = " << fLandauFit[iasic][ifile]->GetParameter(3) << endl;
            if (ifile < nfiles-2) {
                //posMip[iasic][ifile] = fLandauFit[iasic][ifile]->GetParameter(1);
                posMip[iasic][ifile] = mip;
                posMipErr[iasic][ifile] = TMath::Sqrt(fLandauFit[iasic][ifile]->GetParError(1)*fLandauFit[iasic][ifile]->GetParError(1)
                + fGaussianFit[iasic][ifile]->GetParError(1)*fGaussianFit[iasic][ifile]->GetParError(1));
                wMip[iasic][ifile] = fLandauFit[iasic][ifile]->GetParameter(0);
                wMipErr[iasic][ifile] = fLandauFit[iasic][ifile]->GetParError(0);
                sigmaLandauGaus[iasic][ifile] = fLandauFit[iasic][ifile]->GetParameter(3);
            }


            hMipPerChannel[ifile][iasic][ich]->Draw("P");
            TLatex l;
            l.SetTextAlign(12);
            if (ifile==8) {
                l.SetTextSize(0.14);
            } else {
                l.SetTextSize(0.16);
            }
            l.DrawLatexNDC(0.55, 0.78, Form("%d V", voltage[ifile]));
            //l.DrawLatexNDC(0.55, 0.8, Form("ch %d", ich));
            //hMipPedestalSubtracted[ifile]->Draw("HIST SAME");
            if (ifile<nfiles-1)
                fGaussianFit[iasic][ifile]->Draw("SAME");
            if (ifile<nfiles-2) {
                fLandauFit[iasic][ifile]->Draw("SAME");
            }
        }
        c1[iasic]->cd();
        c1[iasic]->SaveAs(Form("voltage_data/figs/mips-asic%d.pdf", iasic));

        // Plot MIP pos & width as function of voltage
        gMipPos[iasic] = new TGraphErrors(nfiles-2, voltGraph, posMip[iasic], voltErr, voltErr);
        gMipPos[iasic]->GetXaxis()->SetTitleSize(0.05);
        gMipPos[iasic]->GetYaxis()->SetTitleSize(0.05);
        gMipPos[iasic]->GetYaxis()->SetTitleOffset(0.95);
        gMipPos[iasic]->SetMarkerSize(1.2);
        gMipPos[iasic]->SetMarkerStyle(kFullCircle);
        gMipPos[iasic]->SetMarkerColor(kBlack);
        gMipPos[iasic]->RemovePoint(nfiles-3);
        gMipPos[iasic]->RemovePoint(nfiles-4);

        gMipWidth[iasic] = new TGraphErrors(nfiles-2, voltGraph, wMip[iasic], voltErr, voltErr);
        gMipWidth[iasic]->SetMarkerSize(1.2);
        gMipWidth[iasic]->SetMarkerStyle(kFullCircle);
        gMipWidth[iasic]->SetMarkerColor(kRed);
        //gMipWidth[iasic]->RemovePoint(nfiles-3);
        //gMipWidth[iasic]->RemovePoint(nfiles-4);


        TLegend *leg = new TLegend(0.445, 0.3, 0.8, 0.49);
        leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.046);
        leg->SetHeader(Form("ASIC %d", iasic));
        leg->AddEntry(gMipPos[iasic], "MIP peak position", "pl");
        leg->AddEntry(gMipWidth[iasic], "MIP peak width", "pl");

        c2[iasic] = new TCanvas(Form("c2_%d", iasic), "c2", 600, 600);
        gMipPos[iasic]->GetYaxis()->SetRangeUser(0., 14.9);
        gMipPos[iasic]->GetXaxis()->SetRangeUser(0., 505);
        gMipPos[iasic]->SetTitle(";Voltage (V);ADC");
        gMipPos[iasic]->Draw("APL");
        gMipWidth[iasic]->Draw("PL SAME");
        leg->Draw("SAME");

        TLatex lvolt;
        lvolt.SetTextSize(0.046);
        lvolt.DrawLatexNDC(0.45, 0.57, "FoCal-E pad layers");
        lvolt.DrawLatexNDC(0.45, 0.51, "2022 PS testbeam");

        c2[iasic]->SaveAs(Form("voltage_data/figs/vscan-asic%d.pdf", iasic));

        // Plot sigma of the gaussian under landau
        gSigmaLandauGaus[iasic] = new TGraphErrors(nfiles-2, voltGraph, sigmaLandauGaus[iasic], voltErr, voltErr);
        gSigmaLandauGaus[iasic]->SetMarkerSize(1.2);
        gSigmaLandauGaus[iasic]->GetXaxis()->SetTitleSize(0.05);
        gSigmaLandauGaus[iasic]->GetYaxis()->SetTitleSize(0.05);
        gSigmaLandauGaus[iasic]->SetMarkerStyle(kFullCircle);

        c3[iasic] = new TCanvas(Form("c3_%d", iasic), "c3", 900, 900);
        gSigmaLandauGaus[iasic]->GetYaxis()->SetRangeUser(0., 5.9);
        gSigmaLandauGaus[iasic]->GetXaxis()->SetRangeUser(0., 505);
        gSigmaLandauGaus[iasic]->SetTitle(";Voltage (V);#sigma");
        gSigmaLandauGaus[iasic]->Draw("APL");
        c3[iasic]->SaveAs(Form("voltage_data/figs/landau-noise-asic%d.pdf", iasic));

    }

    TLegend *legAllAsics = new TLegend(0.15, 0.55, 0.35, 0.9);
    legAllAsics->SetFillStyle(0); legAllAsics->SetBorderSize(0); legAllAsics->SetTextSize(0.026);

    c4 = new TCanvas("c4", "c4", 900, 900);
    for (int iasic=0; iasic<nasic; iasic++) {
        gSigmaLandauGaus[iasic]->GetYaxis()->SetRangeUser(0., 5.9);
        gSigmaLandauGaus[iasic]->SetTitle(";Voltage (V);Gaussian #sigma (ADC)");
        gSigmaLandauGaus[iasic]->SetMarkerColor(mColor[iasic]);
        gSigmaLandauGaus[iasic]->SetLineColor(mColor[iasic]);
        gSigmaLandauGaus[iasic]->SetFillColor(mColor[iasic]);
        if (iasic==0 || iasic==3 || iasic==4 || iasic==6 || iasic==7 || iasic==9) continue;
        legAllAsics->AddEntry(gSigmaLandauGaus[iasic], Form("ASIC %d", iasic), "f");
        if (iasic==1)
            gSigmaLandauGaus[iasic]->Draw("APL");
        else
            gSigmaLandauGaus[iasic]->Draw("SAME PL");
    }
    legAllAsics->Draw("SAME");
    c4->SaveAs("voltage_data/figs/landau-noise-all.pdf");

    c5 = new TCanvas("c5", "c5", 900, 900);
    for (int iasic=0; iasic<nasic; iasic++) {
        gMipPos[iasic]->GetYaxis()->SetRangeUser(0., 15.9);
        gMipPos[iasic]->SetTitle(";Voltage (V);ADC");
        gMipPos[iasic]->SetMarkerColor(mColor[iasic]);
        gMipPos[iasic]->SetLineColor(mColor[iasic]);
        gMipWidth[iasic]->SetMarkerColor(mColor[iasic]);
        gMipWidth[iasic]->SetLineColor(mColor[iasic]);
        gMipWidth[iasic]->SetMarkerStyle(kFullSquare);
        if (iasic==0 || iasic==3 || iasic==4 || iasic==6 || iasic==7 || iasic==9) continue;
        if (iasic==1)
            gMipPos[iasic]->Draw("APL");
        else
            gMipPos[iasic]->Draw("SAME PL");
        gMipWidth[iasic]->Draw("SAME PL");
    }
    legAllAsics->Draw("SAME");
    c5->SaveAs("voltage_data/figs/landau-all.pdf");
}

void LoadHistos(int ifile)
{
    for (int iasic=0; iasic<nasic; iasic++) {
        for (int ich=0; ich<nch; ich++) {
            hMipPerChannel[ifile][iasic][ich] = (TH1D*)finput[ifile]->Get(Form("hMipPerChannel_%d_%d", iasic, ich));
        }
    }
}

void LoadPedestals(TString pedFile)
{
    // Load Pedestal data
    ifstream fPedestal(pedFile.Data());
    int ich, iasic;
    double ped;
    while (fPedestal>>iasic>>ich>>ped) {
        Pedestal[iasic][ich] = ped;
    }
}

void RedrawBorder()
{
   gPad->Update();
   gPad->RedrawAxis();
   TLine l;
   l.DrawLineNDC(gPad->GetUxmin(), gPad->GetUymax(), gPad->GetUxmax(), gPad->GetUymax());
   l.DrawLineNDC(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax());
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
    gStyle->SetPadBottomMargin(0.11);
    gStyle->SetPadTopMargin(0.06);
    gStyle->SetPadLeftMargin(0.11);
    gStyle->SetPadRightMargin(0.06);
    gStyle->SetHistLineWidth(1);
    //gStyle->SetHistLineColor(kRed);
    gStyle->SetFuncWidth(3);
    //gStyle->SetFuncColor(kGreen);
    gStyle->SetLineWidth(1);
    gStyle->SetLabelSize(0.042,"xyz");
    gStyle->SetLabelOffset(0.01,"y");
    gStyle->SetLabelOffset(0.01,"x");
    gStyle->SetLabelColor(kBlack,"xyz");
    gStyle->SetTitleSize(0.042,"xyz");
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
