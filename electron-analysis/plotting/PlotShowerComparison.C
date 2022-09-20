#include "PlotShowerComparison.h"

void PlotShowerComparison()
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
            if (input[iset][ie]=="no-data") {
                cout << "No data for energy " << ie << " GeV in set " << setLabel[iset] << endl;
                for (int iasic=0; iasic<nasic; iasic++) hShowerDist[iset][ie][iasic] = new TH1D(Form("hEmpty_%d", iasic), "", 201, -0.05, 20.05);
                continue;
            }
            if (gSystem->AccessPathName(input[iset][ie].Data())) {
                cout << "File " << input[iset][ie].Data() << " not found, return" << endl;
                return 0;
            }
            fin[iset][ie] = TFile::Open(input[iset][ie], "READ");
            for (int iasic=0; iasic<nasic; iasic++) {
                hShowerDist[iset][ie][iasic] = (TH1D*)fin[iset][ie]->Get(Form("%s%d", histname[iset].Data(), iasic));
            }
        }
    }
    return 1;
}

void ConfigPlots()
{
    cout << "Bin widths: sim=" << hShowerDist[0][0][0]->GetBinWidth(0) << " data=" << hShowerDist[1][0][0]->GetBinWidth(0) << endl;
    for (int iset=0; iset<nset; iset++) {
        for (int ie=0; ie<nenergy; ie++) {
            for (int iasic=0; iasic<nasic; iasic++) {
                hShowerDist[iset][ie][iasic]->SetLineColor(mColor[iset]);
                hShowerDist[iset][ie][iasic]->SetMarkerColor(mColor[iset]);
                hShowerDist[iset][ie][iasic]->SetMarkerStyle(kFullCircle);
                if (setLabel[iset]=="sim.")
                    hShowerDist[iset][ie][iasic]->SetMarkerStyle(kOpenCircle);
                hShowerDist[iset][ie][iasic]->SetMarkerSize(0.5);
                hShowerDist[iset][ie][iasic]->SetTitle(";E_{clust} (MeV); 1/N dN/dE_{clust}");
                if (iset==0) { // simulated data
                    ScaleXaxis(hShowerDist[iset][ie][iasic], ScaleX);
                    hShowerDist[iset][ie][iasic]->ResetStats();
                    //hShowerDist[iset][ie][iasic]->Rebin(2);
                } else {
                    //hShowerDist[iset][ie][iasic]->Rebin(4);
                }
                hShowerDist[iset][ie][iasic]->Scale(1./hShowerDist[iset][ie][iasic]->GetEntries());
                //hShowerDist[iset][ie][iasic]->Scale(1./hShowerDist[iset][ie][iasic]->Integral(2, 201));
                hShowerDist[iset][ie][iasic]->GetXaxis()->SetRangeUser(-0.5, 10.);
                if (bUseLogScale)
                    hShowerDist[iset][ie][iasic]->GetYaxis()->SetRangeUser(0.000001, 1.);
                else
                    hShowerDist[iset][ie][iasic]->GetYaxis()->SetRangeUser(0., 0.12);
                hShowerDist[iset][ie][iasic]->GetXaxis()->SetLabelSize(0.042);
                hShowerDist[iset][ie][iasic]->GetYaxis()->SetLabelSize(0.042);
                hShowerDist[iset][ie][iasic]->GetXaxis()->SetTitleSize(0.042);
                hShowerDist[iset][ie][iasic]->GetYaxis()->SetTitleSize(0.042);
                hShowerDist[iset][ie][iasic]->GetYaxis()->SetTitleOffset(1.4);
            }
        }
    }

    for (int ie=0; ie<nenergy; ie++) {
        for (int iasic=0; iasic<nasic; iasic++) {
            leg[ie][iasic] = new TLegend(0.35, 0.7, 0.85, 0.9);
            leg[ie][iasic]->SetFillStyle(0); leg[ie][iasic]->SetBorderSize(0); leg[ie][iasic]->SetTextSize(0.062);
            leg[ie][iasic]->SetHeader(Form("E=%d GeV, ASIC %d", energy[ie], iasic));
            for (int iset=0; iset<nset; iset++)
                leg[ie][iasic]->AddEntry(hShowerDist[iset][ie][iasic], setLabel[iset], "pe");
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
        canvas[ie] = new TCanvas(Form("canvas_%d", ie), "", 1200, 600);
        canvas[ie]->Divide(6, 3, 0, 0);
        for (int iasic=0; iasic<nasic; iasic++) {
            canvas[ie]->cd(iasic+1);
            if (bUseLogScale) gPad->SetLogy();
            //canvas[ie]->SetBorderSize(2);

            for (int iset=0; iset<nset; iset++) {
                if (iset==0) {
                    //hShowerDist[iset][ie][isim]->GetYaxis()->SetRangeUser(5e-6, 5.);
                    hShowerDist[iset][ie][iasic]->Draw("P");
                } else {
                    hShowerDist[iset][ie][iasic]->Draw("P SAME");
                }
            }
            leg[ie][iasic]->Draw("SAME");
            //t1->Draw("SAME");
            //t2->Draw("SAME");
            gPad->RedrawAxis();
        }
        //canvas[ie]->SaveAs(Form("energy-per-layer_%dGeV.pdf", energy[ie]));
    }
}

Double_t ScaleX(Double_t x)
{
  Double_t v;
  v = axisScale * x; // "linear scaling" function example
  return v;
}

void ScaleAxis(TAxis *a, Double_t (*Scale)(Double_t))
{
  if (!a) return; // just a precaution
  if (a->GetXbins()->GetSize())
    {
      // an axis with variable bins
      // note: bins must remain in increasing order, hence the "Scale"
      // function must be strictly (monotonically) increasing
      TArrayD X(*(a->GetXbins()));
      for(Int_t i = 0; i < X.GetSize(); i++) X[i] = Scale(X[i]);
      a->Set((X.GetSize() - 1), X.GetArray()); // new Xbins
    }
  else
    {
      // an axis with fix bins
      // note: we modify Xmin and Xmax only, hence the "Scale" function
      // must be linear (and Xmax must remain greater than Xmin)
      a->Set( a->GetNbins(),
              Scale(a->GetXmin()), // new Xmin
              Scale(a->GetXmax()) ); // new Xmax
    }
  return;
}

void ScaleXaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetXaxis(), Scale);
  return;
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
