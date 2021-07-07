 #include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TH1F.h"
 #include "TLegend.h"
 #include "TStyle.h"
 #include "TLatex.h"

  void higgsAssumption() {  
  
     TFile inFile("ggH_MS2_ctau10.root"); 
     TH2I* h_higgsassump = (TH2I*)inFile.Get("h_higgsassump"); // Load in histogram
   
     TCanvas* canv2 = new TCanvas ("canv2", "canv2", 50, 50, 800, 600); // Canvas to draw histogram on
     canv2->SetGrid();
   
     gStyle->SetOptStat(0);
    
   
     h_higgsassump->SetTitle("");
     h_higgsassump->GetXaxis()->SetTitle("m_{Higgs} (GeV/c^{2}) - Pion assumption"); // set a title for the x-axis
     h_higgsassump->GetXaxis()->SetRangeUser(0., 200.); // change the x-axis range to 0-200
     h_higgsassump->GetYaxis()->SetTitle("m_{Higgs} (GeV/c^{2}) - Kaon assumption"); 
     h_higgsassump->GetYaxis()->SetRangeUser(0., 200.); //
 
     h_higgsassump->Draw("colz"); // draw histo on canvas
     canv2->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv2->SaveAs("h_higgsassump.png"); 
     }
