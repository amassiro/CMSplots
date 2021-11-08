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



std::vector<std::string> value_name;
std::vector<float> value;
std::vector<float> value_err_up;
std::vector<float> value_err_low;
std::vector<std::string> value_name_HR;


// const UInt_t nChannel = 5;
// 
// const TString lChannel[nChannel] = {
//   "eee",
//   "ee#mu",
//   "#mu#mue",
//   "#mu#mu#mu",
//   "combined"
// };


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
                     Float_t     xoffset = 0.2,
                     Float_t     yoffset = 0.05);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// xs
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "tdrstyle.C"


#include <iostream>
#include <fstream>
#include <sstream>




void plotXS(std::string nameFile = "input/hig-16-XXX.signal.strength.txt", std::string outFile="xs") {
//   gInterpreter->ExecuteMacro("tdrstyle.C");
  
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << " nameFile = " << nameFile << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  
  std::ifstream file (nameFile.c_str()); 
  
  std::string buffer;
  float num;
  std::string name;
  
  
  float inclusive =1;
  float inclusive_err_low = 1;
  float inclusive_err_up = 1;
  
  
  while(!file.eof()) {
    std::cout << "num =";
    getline(file,buffer);
    std::cout << "buffer = " << buffer << std::endl;
    if (buffer != "" && buffer.at(0) != '#' ){ ///---> save from empty line at the end and "comments"
      std::stringstream line( buffer );      
      
      if (buffer.at(0) != '*') { 
        line >> name; 
        value_name.push_back(name);
        
        line >> num; 
        value.push_back(num);
        line >> num; 
        value_err_low.push_back(num);
        line >> num; 
        value_err_up.push_back(num);
        
        
        getline(line, name);
        value_name_HR.push_back(name);
      }
      else {
        //---- inclusive one
        line >> name; 
        line >> num; 
        inclusive = num;
        line >> num; 
        inclusive_err_low = num;
        line >> num; 
        inclusive_err_up = num;               
      }
      
    } 
  }
  
//   value.push_back(1);
//   value.push_back(1);
//   value.push_back(1);
//   
//   value_err_up.push_back(0.1);
//   value_err_up.push_back(0.2);
//   value_err_up.push_back(0.1);
//   
//   value_err_low.push_back(0.1);
//   value_err_low.push_back(0.2);
//   value_err_low.push_back(0.3);
//   
  
  int nChannel = value.size();
  
  
  
  setTDRStyle();
  
  TGraphAsymmErrors* graph_values = new TGraphAsymmErrors();
  
  // Fill the graphs
  //----------------------------------------------------------------------------
  
  
  for (UInt_t i=0; i<nChannel; i++) {
    
    graph_values->SetPointError(i, value_err_low.at(i), value_err_up.at(i), 0.25, 0.25);
    graph_values->SetPoint(i, value.at(i), nChannel - i);
       
  }
  
  // cosmetics
  //----------------------------------------------------------------------------
  graph_values->SetFillColor  (kRed+1);
  graph_values->SetLineColor  (kRed+1);
  graph_values->SetLineWidth  (0);
  graph_values->SetMarkerColor(kWhite);
  graph_values->SetMarkerSize (0.8);
  graph_values->SetMarkerStyle(kFullCircle);
  
  // Draw
  //----------------------------------------------------------------------------
  TString cname = outFile.c_str();
  
  TCanvas* canvas = new TCanvas(cname, cname);
  
  canvas->SetLeftMargin(canvas->GetRightMargin());
  
//   Float_t xmin = 0.10;
//   Float_t xmax = 1.75;

  Float_t xmin = -1;
  Float_t xmax = 1.85;
  Float_t ymin = 0.20;

//   std::cout << *(std::min_element(std::begin(value), std::end(value)));

  
  if ((   (*(std::min_element(std::begin(value), std::end(value))))   - (*(std::max_element(std::begin(value_err_low), std::end(value_err_low))))  ) < xmin) {
    xmin = (   (*(std::min_element(std::begin(value), std::end(value))))   - (*(std::max_element(std::begin(value_err_low), std::end(value_err_low))))  )  - 0.5;
  }
  if ((   (*(std::max_element(std::begin(value), std::end(value))))   + (*(std::max_element(std::begin(value_err_up), std::end(value_err_up))))  ) > xmax) {
    xmax = (   (*(std::max_element(std::begin(value), std::end(value))))   + (*(std::max_element(std::begin(value_err_low), std::end(value_err_low))))  )  + 0.5;
  }
  
  //---- just for big ranges
  if ((xmax - xmin) > 3) {
    xmin = xmin - 3;
  }
  
  
  
  //   Float_t ymax = 3*nChannel + ymin + 0.6;
  Float_t ymax = 1*nChannel + ymin + 0.6;
  
  TH2F* h2 = new TH2F("h2", "", 100, xmin, xmax, 100, ymin, ymax);
  
  h2->Draw();
  
  
  // Inclusive box
  //----------------------------------------------------------------------------
//   TBox* average = new TBox(0.8, 1.0-0.25*2, 1.2, ymax);
  cout <<  inclusive-inclusive_err_low << ""<<  inclusive+inclusive_err_up;
  TBox* average = new TBox( inclusive-inclusive_err_low   , 1.0-0.25*2, inclusive+inclusive_err_up, ymax);
  
  
  average->SetLineColor(0);
//   average->SetFillColor(kGray+1);
  average->SetFillColor(kRed+1);
  average->SetFillStyle(3004);
  
  average ->Draw("e2,same");
  
  
  TLine* l_observed = new TLine(inclusive, 1.0-0.25*2,inclusive,ymax);
  l_observed->SetLineColor(kRed);
  l_observed->SetLineWidth(2);
  l_observed->SetLineStyle(2);
  
  l_observed->Draw();


  TLine* l_expected = new TLine(1.00, 1.0-0.25*2,1.00,ymax);
  l_expected->SetLineColor(kBlack);
  l_expected->SetLineWidth(2);
  l_expected->SetLineStyle(4);
  l_expected->Draw();
  
  
  
  
  // Cross sections
  //----------------------------------------------------------------------------
  graph_values->Draw("p2,same");
  
  // 7 TeV labels
  //----------------------------------------------------------------------------
  for (UInt_t i=0; i<nChannel; i++) {
    
    Float_t x = graph_values->GetX()[i];
    Float_t y = graph_values->GetY()[i];
    
    Float_t value_Error = graph_values->GetErrorX(i);
    
    DrawTLatex(42, xmin+0.05, y, 0.03, 12, Form("%s:  %.1f #pm %.1f", value_name_HR.at(i).c_str(), x, value_Error), 0);
//     DrawTLatex(42, xmin+0.05, y, 0.03, 12, Form("%s %.2f #pm %.2f", value_name_HR.at(i).c_str(), x, value_Error), 0);
    //     DrawTLatex(42, xmin+0.05, y, 0.03, 12, Form("%s %.2f #pm %.2f", value_name.at(i).c_str(), x, value_Error), 0);
    //     DrawTLatex(42, xmin+0.05, y, 0.03, 12, Form("%s %.2f #pm %.2f", lChannel[i].Data(), x, g7lumiError), 0);
  }
  
  
  // CMS titles
  //----------------------------------------------------------------------------
  DrawTLatex(61, 0.160, 0.940, 0.055, 31, "CMS");
  DrawTLatex(52, 0.190, 0.940, 0.030, 11, "Preliminary");
  DrawTLatex(42, 0.925, 0.945, 0.040, 31, "137 fb^{-1} (13 TeV)");
  
  h2->GetXaxis()->CenterTitle();
  h2->GetXaxis()->SetTitleOffset(1.4);
  h2->GetXaxis()->SetTitle("#sigma / #sigma_{SM}");
  h2->GetYaxis()->SetTitle("");
  
  // Remove y-axis labels
  //----------------------------------------------------------------------------
  TAxis* yaxis = h2->GetYaxis();
  
  for (Int_t j=1; j<yaxis->GetNbins(); j++) yaxis->SetBinLabel(j, "");
  
  
  //---- legends
  //----------------------------------------------------------------------------
  float ylegend = 0.35;
  float ydelta  = 0.05;
  
  DrawTLegend(0.45, ylegend, average,  "Combined",  "f");   ylegend -= ydelta;
  DrawTLegend(0.45, ylegend, l_expected,  "SM",  "l");   ylegend -= ydelta;
  
  
  
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