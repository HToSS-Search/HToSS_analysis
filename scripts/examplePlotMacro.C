//An example macro for manipulating plots

 #include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TH1F.h"
 #include "TLegend.h"
 #include "TStyle.h"
 #include "TLatex.h"

 void examplePlotMacro(std::string histogram) {

     TFile inFile(std::string histogram); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_genParPt = (TH1F*)inFile.Get(std::string histogram); // Load in histogram

     TCanvas* canvy = new TCanvas ("canvy", "canvy", 50, 50, 800, 600); // Canvas to draw histogram on
     gPad->SetLogy(); // set log scale for y-axis on for objects drawn on the canvas from this point on

     h_genParPt->GetXaxis()->SetTitle("p_{T} GeV"); // set a title for the x-axis
     h_genParPt->GetXaxis()->SetRangeUser(0.0, 200.); // change the x-axis range to 0-200

     h_genParPt->Draw("colz"); // draw histo on canvas
     canvy->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canvy->SaveAs(std::string histogram.gif); // .pdf and other formats work too!

     delete canvy;
 }
