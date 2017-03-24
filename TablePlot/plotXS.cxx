//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// xs ratios
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "TAxis.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TSystem.h"


// Constants
//------------------------------------------------------------------------------
const UInt_t nChannel = 5;

const TString lChannel[nChannel] = {
  "eee",
  "ee#mu",
  "#mu#mue",
  "#mu#mu#mu",
  "combined"
};

const Float_t xs7eval[] = {22.46, 19.04, 19.13, 20.36, 20.14};
const Float_t xs7stat[] = { 3.12,  2.75,  2.60,  2.31,  1.32};
const Float_t xs7syst[] = { 1.40,  1.54,  1.61,  1.53,  1.13};
const Float_t xs7lumi[] = { 0.49,  0.42,  0.42,  0.45,  0.44};

const Float_t xs8eval[] = {24.80, 22.38, 23.94, 24.93, 24.09};
const Float_t xs8stat[] = { 1.92,  1.62,  1.52,  1.29,  0.87};
const Float_t xs8syst[] = { 1.74,  1.92,  1.85,  2.29,  1.62};
const Float_t xs8lumi[] = { 0.64,  0.58,  0.62,  0.65,  0.63};


const Float_t xs13nlo_mcfm = 42.6;  // From SMP-16-002 (v11)


// From Table 16 of AN-16-028 (v4)
// These are the observed signal strengths.
// To extract the observed xs we multiply by xs13nlo_mcfm.
//------------------------------------------------------------------------------
const Float_t xs13eval[] = {0.889, 1.096, 0.977, 0.915, 0.959};
const Float_t xs13stat[] = {0.202, 0.181, 0.169, 0.127, 0.080};
const Float_t xs13syst[] = {0.108, 0.111, 0.111, 0.100, 0.080};
const Float_t xs13lumi[] = {0.034, 0.038, 0.034, 0.031, 0.032};


// From Table 4 of http://arxiv.org/pdf/1604.08576.pdf
//------------------------------------------------------------------------------
const Float_t xs_nlo       [] = {17.72, 21.80, 45.09};   // 7 TeV   8 TeV   13 TeV
const Float_t err_nlo_left [] = {0.041, 0.039, 0.039};   // -4.1%   -3.9%   -3.9%
const Float_t err_nlo_right[] = {0.053, 0.051, 0.049};   // +5.3%   +5.1%   +4.9%

const Float_t xs_nnlo       [] = {19.18, 23.68, 49.98};  // 7 TeV   8 TeV   13 TeV
const Float_t err_nnlo_left [] = {0.018, 0.018, 0.020};  // -1.8%   -1.8%   -2.0%
const Float_t err_nnlo_right[] = {0.017, 0.018, 0.022};  // +1.7%   +1.8%   +2.2%


// Functions
//------------------------------------------------------------------------------
void     DrawTLatex (Font_t      tfont,
                     Float_t     x,
                     Float_t     y,
                     Float_t     tsize,
                     Short_t     align,
                     const char* text,
                     Bool_t      setndc = true);

TLegend* DrawTLegend(Float_t     x1,
                     Float_t     y1,
                     TObject*    hist,
                     TString     label,
                     TString     option,
                     Float_t     tsize   = 0.03,
                     Float_t     xoffset = 0.3,
                     Float_t     yoffset = 0.05);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// xs
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "tdrstyle.C"

void plotXS() {
//   gInterpreter->ExecuteMacro("tdrstyle.C");
  
  setTDRStyle();
  
  TGraphErrors* g7stat = new TGraphErrors(nChannel);
  TGraphErrors* g7syst = new TGraphErrors(nChannel);
  TGraphErrors* g7lumi = new TGraphErrors(nChannel);
  
  TGraphErrors* g8stat = new TGraphErrors(nChannel);
  TGraphErrors* g8syst = new TGraphErrors(nChannel);
  TGraphErrors* g8lumi = new TGraphErrors(nChannel);
  
  TGraphErrors* g13stat = new TGraphErrors(nChannel);
  TGraphErrors* g13syst = new TGraphErrors(nChannel);
  TGraphErrors* g13lumi = new TGraphErrors(nChannel);
  
  
  // Fill the 7 TeV graphs
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++)
  {
    Float_t xs = xs_nnlo[0];
    
    Float_t errorSquared = (xs7stat[i] * xs7stat[i]);
    
    g7stat->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
    
    errorSquared += (xs7syst[i] * xs7syst[i]);
    
    g7syst->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
    
    errorSquared += (xs7lumi[i] * xs7lumi[i]);
    
    g7lumi->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
    
    g7stat->SetPoint(i, xs7eval[i] / xs, nChannel-i);
    g7syst->SetPoint(i, xs7eval[i] / xs, nChannel-i);
    g7lumi->SetPoint(i, xs7eval[i] / xs, nChannel-i);
  }
  
  
  // Fill the 8 TeV graphs
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++)
  {
    Float_t xs = xs_nnlo[1];
    
    Float_t errorSquared = (xs8stat[i] * xs8stat[i]);
    
    g8stat->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
    
    errorSquared += (xs8syst[i] * xs8syst[i]);
    
    g8syst->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
    
    errorSquared += (xs8lumi[i] * xs8lumi[i]);
    
    g8lumi->SetPointError(i, sqrt(errorSquared) / xs, 0.25);
    
    g8stat->SetPoint(i, xs8eval[i] / xs, 2*nChannel-i);
    g8syst->SetPoint(i, xs8eval[i] / xs, 2*nChannel-i);
    g8lumi->SetPoint(i, xs8eval[i] / xs, 2*nChannel-i);
  }
  
  
  // Fill the 13 TeV graphs
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++)
  {
    Float_t xs = xs_nnlo[2];
    
    Float_t errorSquared = (xs13stat[i] * xs13stat[i]);
    
    g13stat->SetPointError(i, sqrt(errorSquared) * xs13nlo_mcfm / xs, 0.25);
    
    errorSquared += (xs13syst[i] * xs13syst[i]);
    
    g13syst->SetPointError(i, sqrt(errorSquared) * xs13nlo_mcfm / xs, 0.25);
    
    errorSquared += (xs13lumi[i] * xs13lumi[i]);
    
    g13lumi->SetPointError(i, sqrt(errorSquared) * xs13nlo_mcfm / xs, 0.25);
    
    g13stat->SetPoint(i, xs13eval[i] * xs13nlo_mcfm / xs, 3*nChannel-i);
    g13syst->SetPoint(i, xs13eval[i] * xs13nlo_mcfm / xs, 3*nChannel-i);
    g13lumi->SetPoint(i, xs13eval[i] * xs13nlo_mcfm / xs, 3*nChannel-i);
  }
  
  
  // 7 TeV cosmetics
  //----------------------------------------------------------------------------
  g7stat->SetFillColor  (kRed+1);
  g7stat->SetLineColor  (kRed+1);
  g7stat->SetLineWidth  (0);
  g7stat->SetMarkerColor(kWhite);
  g7stat->SetMarkerSize (0.8);
  g7stat->SetMarkerStyle(kFullCircle);
  
  g7syst->SetFillColor(kOrange+7);
  g7syst->SetLineColor(kOrange+7);
  g7syst->SetLineWidth(0);
  
  g7lumi->SetFillColor(kOrange+7);
  g7lumi->SetLineColor(kOrange+7);
  g7lumi->SetLineWidth(0);
  
  
  // 8 TeV cosmetics
  //----------------------------------------------------------------------------
  g8stat->SetFillColor  (kBlue);
  g8stat->SetLineColor  (kBlue);
  g8stat->SetLineWidth  (0);
  g8stat->SetMarkerColor(kWhite);
  g8stat->SetMarkerSize (0.8);
  g8stat->SetMarkerStyle(kFullTriangleUp);
  
  g8syst->SetFillColor(kAzure-4);
  g8syst->SetLineColor(kAzure-4);
  g8syst->SetLineWidth(0);
  
  g8lumi->SetFillColor(kAzure-4);
  g8lumi->SetLineColor(kAzure-4);
  g8lumi->SetLineWidth(0);
  
  
  // 13 TeV cosmetics
  //----------------------------------------------------------------------------
  g13stat->SetFillColor  (kGreen+3);
  g13stat->SetLineColor  (kGreen+3);
  g13stat->SetLineWidth  (0);
  g13stat->SetMarkerColor(kWhite);
  g13stat->SetMarkerSize (0.8);
  g13stat->SetMarkerStyle(kFullSquare);
  
  g13syst->SetFillColor(kGreen-5);
  g13syst->SetLineColor(kGreen-5);
  g13syst->SetLineWidth(0);
  
  g13lumi->SetFillColor(kGreen-5);
  g13lumi->SetLineColor(kGreen-5);
  g13lumi->SetLineWidth(0);
  
  
  // Draw
  //----------------------------------------------------------------------------
  TString cname = "xs_ratios";
  
  TCanvas* canvas = new TCanvas(cname, cname);
  
  canvas->SetLeftMargin(canvas->GetRightMargin());
  
  Float_t xmin = 0.10;
  Float_t xmax = 1.75;
  Float_t ymin = 0.20;
  Float_t ymax = 3*nChannel + ymin + 0.6;
  
  TH2F* h2 = new TH2F("h2", "", 100, xmin, xmax, 100, ymin, ymax);
  
  h2->Draw();
  
  
  // NLO WZ cross-section
  //----------------------------------------------------------------------------
  TBox* nlo7tev = new TBox((1 - err_nlo_left [0]) * (xs_nlo[0]/xs_nnlo[0]), (2./3.)*(ymax-ymin),
                           (1 + err_nlo_right[0]) * (xs_nlo[0]/xs_nnlo[0]), ymax);
  
  TBox* nlo8tev = new TBox((1 - err_nlo_left [1]) * (xs_nlo[1]/xs_nnlo[1]), (1./3.)*(ymax-ymin),
                           (1 + err_nlo_right[1]) * (xs_nlo[1]/xs_nnlo[1]), (2./3.)*(ymax-ymin));
  
  TBox* nlo13tev = new TBox((1 - err_nlo_left [2]) * (xs_nlo[2]/xs_nnlo[2]), ymin,
                            (1 + err_nlo_right[2]) * (xs_nlo[2]/xs_nnlo[2]), (1./3.)*(ymax-ymin));
  
  nlo7tev->SetLineColor(0);
  nlo7tev->SetFillColor(kGray+1);
  nlo7tev->SetFillStyle(3004);
  
  nlo8tev->SetLineColor(0);
  nlo8tev->SetFillColor(kGray+1);
  nlo8tev->SetFillStyle(3004);
  
  nlo13tev->SetLineColor(0);
  nlo13tev->SetFillColor(kGray+1);
  nlo13tev->SetFillStyle(3004);
  
  nlo7tev ->Draw("e2,same");
  nlo8tev ->Draw("e2,same");
  nlo13tev->Draw("e2,same");
  
  
  // NNLO WZ cross-section
  //----------------------------------------------------------------------------
  TBox* nnlo7tev  = new TBox(1 - err_nnlo_left[0], (2./3.)*(ymax-ymin), 1 + err_nnlo_right[0],                ymax);
  TBox* nnlo8tev  = new TBox(1 - err_nnlo_left[1], (1./3.)*(ymax-ymin), 1 + err_nnlo_right[1], (2./3.)*(ymax-ymin));
  TBox* nnlo13tev = new TBox(1 - err_nnlo_left[2],                ymin, 1 + err_nnlo_right[2], (1./3.)*(ymax-ymin));
  
  nnlo7tev->SetLineColor(0);
  nnlo7tev->SetFillColor(kGray+1);
  nnlo7tev->SetFillStyle(1001);
  
  nnlo8tev->SetLineColor(0);
  nnlo8tev->SetFillColor(kGray+1);
  nnlo8tev->SetFillStyle(1001);
  
  nnlo13tev->SetLineColor(0);
  nnlo13tev->SetFillColor(kGray+1);
  nnlo13tev->SetFillStyle(1001);
  
  nnlo7tev ->Draw("e2,same");
  nnlo8tev ->Draw("e2,same");
  nnlo13tev->Draw("e2,same");
  
  
  // Cross sections
  //----------------------------------------------------------------------------
  g7lumi->Draw("p2,same");
  g7syst->Draw("p2,same");
  g7stat->Draw("p2,same");
  
  g8lumi->Draw("p2,same");
  g8syst->Draw("p2,same");
  g8stat->Draw("p2,same");
  
  g13lumi->Draw("p2,same");
  g13syst->Draw("p2,same");
  g13stat->Draw("p2,same");
  
  
  // 7 TeV labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {
    
    Float_t x = g7stat->GetX()[i];
    Float_t y = g7stat->GetY()[i];
    
    Float_t g7lumiError = g7lumi->GetErrorX(i);
    
    DrawTLatex(42, xmin+0.05, y, 0.03, 12, Form("%s %.2f #pm %.2f",
                                                lChannel[i].Data(), x, g7lumiError), 0);
  }
  
  
  // 8 TeV labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {
    
    Float_t x = g8stat->GetX()[i];
    Float_t y = g8stat->GetY()[i];
    
    Float_t g8lumiError = g8lumi->GetErrorX(i);
    
    DrawTLatex(42, xmin+0.05, y, 0.03, 12, Form("%s %.2f #pm %.2f",
                                                lChannel[i].Data(), x, g8lumiError), 0);
  }
  
  
  // 13 TeV labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {
    
    Float_t x = g13stat->GetX()[i];
    Float_t y = g13stat->GetY()[i];
    
    Float_t g13lumiError = g13lumi->GetErrorX(i);
    
    DrawTLatex(42, xmin+0.05, y, 0.03, 12, Form("%s %.2f #pm %.2f",
                                                lChannel[i].Data(), x, g13lumiError), 0);
  }
  
  
  // CMS titles
  //----------------------------------------------------------------------------
  DrawTLatex(61, 0.930, 0.850, 0.055, 31, "CMS");
  //DrawTLatex(52, 0.190, 0.940, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.925, 0.945, 0.040, 31, "4.9 fb^{-1} (7 TeV) + 19.6 fb^{-1} (8 TeV) + 2.3 fb^{-1} (13 TeV)");
  
  h2->GetXaxis()->CenterTitle();
  h2->GetXaxis()->SetTitleOffset(1.4);
  h2->GetXaxis()->SetTitle("#sigma_{WZ} / #sigma_{WZ}^{NNLO}");
  h2->GetYaxis()->SetTitle("");
  
  
  // Remove y-axis labels
  //----------------------------------------------------------------------------
  TAxis* yaxis = h2->GetYaxis();
  
  for (Int_t j=1; j<yaxis->GetNbins(); j++) yaxis->SetBinLabel(j, "");
  
  
  // Additional legend
  //----------------------------------------------------------------------------
  float ylegend = 0.76;
  float ydelta  = 0.05;
  
  DrawTLegend(0.76, ylegend, nlo7tev,  "NLO",  "f");   ylegend -= ydelta;
  DrawTLegend(0.76, ylegend, nnlo7tev, "NNLO", "f");   ylegend -= ydelta;
  
  DrawTLatex(42, 0.775, ylegend, 0.03, 11, "13 TeV");  ylegend -= ydelta;
  DrawTLegend(0.76, ylegend, g13stat, "stat", "fp");   ylegend -= ydelta;
  DrawTLegend(0.76, ylegend, g13syst, "syst", "f");    ylegend -= ydelta;
  
  DrawTLatex(42, 0.775, ylegend, 0.03, 11, "8 TeV");   ylegend -= ydelta;
  DrawTLegend(0.76, ylegend, g8stat,   "stat", "fp");  ylegend -= ydelta;
  DrawTLegend(0.76, ylegend, g8syst,   "syst", "f");   ylegend -= ydelta;
  
  DrawTLatex(42, 0.775, ylegend, 0.03, 11, "7 TeV");   ylegend -= ydelta;
  DrawTLegend(0.76, ylegend, g7stat, "stat", "fp");    ylegend -= ydelta;
  DrawTLegend(0.76, ylegend, g7syst, "syst", "f");
  
  
  // Save
  //----------------------------------------------------------------------------
  canvas->Update();
  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();
  
  gSystem->mkdir("pdf", kTRUE);
  gSystem->mkdir("png", kTRUE);
  
  canvas->SaveAs("pdf/" + cname + ".pdf");
  canvas->SaveAs("png/" + cname + ".png");
}


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Font_t      tfont,
                Float_t     x,
                Float_t     y,
                Float_t     tsize,
                Short_t     align,
                const char* text,
                Bool_t      setndc)
{
  TLatex* tl = new TLatex(x, y, text);
  
  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont ( tfont);
  tl->SetTextSize ( tsize);
  
  tl->Draw("same");
}


//------------------------------------------------------------------------------
// DrawTLegend
//------------------------------------------------------------------------------
TLegend* DrawTLegend(Float_t  x1,
                     Float_t  y1,
                     TObject* hist,
                     TString  label,
                     TString  option,
                     Float_t  tsize,
                     Float_t  xoffset,
                     Float_t  yoffset)
{
  TLegend* legend = new TLegend(x1,
                                y1,
                                x1 + xoffset,
                                y1 + yoffset);
  
  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (tsize);
  
  legend->AddEntry(hist, label.Data(), option.Data());
  legend->Draw();
  
  return legend;
}