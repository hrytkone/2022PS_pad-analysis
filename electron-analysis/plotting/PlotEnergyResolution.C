#include "PlotEnergyResolution.h"

void PlotEnergyResolution()
{
    SetStyle(0);
    if (!LoadData()) return;
    ConfigPlots();
    GetResolution();
    Plot();
}

//******************************************************************************
//******************************************************************************
int LoadData()
{
    for (int iset=0; iset<nset; iset++) {
        for (int ie=0; ie<nenergy; ie++) {
            if (input[iset][ie]=="no-data") {
                cout << "No data for energy " << ie << " GeV in set " << setLabel[iset] << endl;
                hEnergyAllLayers[iset][ie] = new TH1D("hEmpty", "", 201, -0.05, 20.05);
                continue;
            }
            if (gSystem->AccessPathName(input[iset][ie].Data())) {
                cout << "File " << input[iset][ie].Data() << " not found, return" << endl;
                return 0;
            }
            fin[iset][ie] = TFile::Open(input[iset][ie], "READ");
            hEnergyAllLayers[iset][ie] = (TH1D*)fin[iset][ie]->Get("hEnergyAllLayers");
        }
    }
    return 1;
}

void ConfigPlots()
{
    for (int iset=0; iset<nset; iset++) {
        for (int ie=0; ie<nenergy; ie++) {
            hEnergyAllLayers[iset][ie]->SetLineColor(mColor[iset]);
            hEnergyAllLayers[iset][ie]->SetMarkerColor(mColor[iset]);
            hEnergyAllLayers[iset][ie]->SetMarkerStyle(kFullCircle);
            if (setLabel[iset]=="sim.")
                hEnergyAllLayers[iset][ie]->SetMarkerStyle(kOpenCircle);
            hEnergyAllLayers[iset][ie]->SetMarkerSize(.5);
            hEnergyAllLayers[iset][ie]->SetTitle(";E_{clust} (MeV); 1/N dN/dE_{clust}");
            hEnergyAllLayers[iset][ie]->Scale(1./hEnergyAllLayers[iset][ie]->GetEntries());
            //hEnergyAllLayers[iset][ie]->Scale(1./hEnergyAllLayers[iset][ie]->Integral(2, 201));
            hEnergyAllLayers[iset][ie]->GetXaxis()->SetRangeUser(-0.05, xmax[ie]);
            if (bUseLogScale)
                hEnergyAllLayers[iset][ie]->GetYaxis()->SetRangeUser(0.000001, 1.);
            else
                hEnergyAllLayers[iset][ie]->GetYaxis()->SetRangeUser(0., 0.025);
            hEnergyAllLayers[iset][ie]->GetYaxis()->SetMaxDigits(3);
            hEnergyAllLayers[iset][ie]->GetXaxis()->SetLabelSize(0.042);
            hEnergyAllLayers[iset][ie]->GetYaxis()->SetLabelSize(0.042);
            hEnergyAllLayers[iset][ie]->GetXaxis()->SetTitleSize(0.042);
            hEnergyAllLayers[iset][ie]->GetYaxis()->SetTitleSize(0.042);
            hEnergyAllLayers[iset][ie]->GetYaxis()->SetTitleOffset(1.4);
        }
    }

    for (int ie=0; ie<nenergy; ie++) {
        leg[ie] = new TLegend(0.65, 0.7, 0.85, 0.9);
        leg[ie]->SetFillStyle(0); leg[ie]->SetBorderSize(0); leg[ie]->SetTextSize(0.042);
        leg[ie]->SetHeader(Form("E=%d GeV", energy[ie]));
        for (int iset=0; iset<nset; iset++)
            leg[ie]->AddEntry(hEnergyAllLayers[iset][ie], setLabel[iset], "pe");
    }
}

void GetResolution()
{
    for (int iset=0; iset<nset; iset++) {
        for (int ie=0; ie<nenergy; ie++) {
            // Fit gaussian
            hEnergyAllLayers[iset][ie]->Fit("gaus", "Q0");
            TF1 *fit = hEnergyAllLayers[iset][ie]->GetFunction("gaus");
            double p1 = fit->GetParameter(1);
            double e1 = fit->GetParError(1);
            double p2 = fit->GetParameter(2);
            double e2 = fit->GetParError(2);
            eres[iset][ie] = p2/p1;
            ereserr[iset][ie] = p2/p1*TMath::Sqrt((e2/p2)*(e2/p2) + (e1/p1)*(e1/p1));
        }
        gRes[iset] = new TGraphErrors(nenergy, e, eres[iset], 0, ereserr[iset]);
        gRes[iset]->SetMarkerColor(mColor[iset]);
        gRes[iset]->SetLineColor(mColor[iset]);
        gRes[iset]->SetMarkerStyle(kFullCircle);
        if (setLabel[iset]=="sim.")
            gRes[iset]->SetMarkerStyle(kOpenCircle);
        gRes[iset]->SetMarkerSize(1);
        gRes[iset]->SetTitle(";E (GeV); #DeltaE/E");
        gRes[iset]->GetYaxis()->SetRangeUser(0., 1.5);
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
        canvas[ie] = new TCanvas(Form("canvas_%d", ie), "", 600, 600);
        if (bUseLogScale) gPad->SetLogy();

        for (int iset=0; iset<nset; iset++) {
            if (iset==0) {
                hEnergyAllLayers[iset][ie]->Draw("P");
                //hEnergyAllLayers[iset][ie]->GetFunction("gaus")->Draw("SAME");
            } else {
                hEnergyAllLayers[iset][ie]->Draw("P SAME");
                //hEnergyAllLayers[iset][ie]->GetFunction("gaus")->Draw("SAME");
            }
        }
        leg[ie]->Draw("SAME");
        //t1->Draw("SAME");
        //t2->Draw("SAME");
        gPad->RedrawAxis();
        canvas[ie]->SaveAs(Form("full-energy_%dGeV.pdf", energy[ie]));
    }

    cRes = new TCanvas("cRes", "", 600, 600);
    for (int iset = 0; iset < nset; iset++) {
        if (iset==0) {
            gRes[iset]->Draw("APL");
        } else {
            gRes[iset]->Draw("PL SAME");
        }
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
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.05);
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
