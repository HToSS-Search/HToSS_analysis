//An example macro for manipulating plots

 #include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TH1F.h"
 #include "TLegend.h"
 #include "TStyle.h"
 #include "TLatex.h"

  void examplePlotMacro() {  
  
     TFile inFile("ggH_MS2_ctau10.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH1F* h_genParPt = (TH1F*)inFile.Get("h_genParPt"); // Load in histogram
     TH1F* h_genParEta = (TH1F*)inFile.Get("h_genParEta");
   
     TCanvas* canv1 = new TCanvas ("canv1", "canv1", 50, 50, 800, 600); // Canvas to draw histogram on
     TCanvas* canv2 = new TCanvas ("canv2", "canv2", 50, 50, 800, 600);
   
     gPad->SetLogy(); // set log scale for y-axis on for objects drawn on the canvas from this point on

     h_genParPt->GetXaxis()->SetTitle("p_{T} GeV"); // set a title for the x-axis
     h_genParPt->GetXaxis()->SetRangeUser(0.0, 200.); // change the x-axis range to 0-200
     h_genParEta->GetXaxis()->SetTitle("#eta"); 
     h_genParEta->GetXaxis()->SetRangeUser(0.0, 200.);

     h_genParPt->Draw("colz"); // draw histo on canvas
     canv1->Draw(); // draw canvas!
     h_genParEta->Draw("colz"); 
     canv2->Draw(); 

     // save canvas with drawn histogram
     canv1->SaveAs("h_genParPt.gif"); // .pdf and other formats work too!
     canv2->SaveAs("h_genParEta.gif"); 
     }

