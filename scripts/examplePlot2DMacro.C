//An example macro for manipulating 2D plots

 #include "TFile.h"

 #include "TCanvas.h"
 #include "TROOT.h"
 #include "TH1F.h"
 #include "TLegend.h"
 #include "TStyle.h"
 #include "TLatex.h"

  void examplePlot2DMacro() {  
  
     TFile inFile("ggH_MS2_ctau10.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
     TH2F* h_VertexPosXY = (TH2I*)inFile.Get("VertexPosRZ"); // Load in histogram
   
     TCanvas* canv2 = new TCanvas ("canv2", "canv2", 50, 50, 800, 600); // Canvas to draw histogram on
  
     gPad->SetLogz(); // set log scale for y-axis on for objects drawn on the canvas from this point on

     h_VertexPosRZ->GetXaxis()->SetTitle("Vertex Position Z"); // set a title for the x-axis
     h_VertexPosRZ->GetXaxis()->SetRangeUser(0.0, 200.); // change the x-axis range to 0-200
     h_VertexPosRZ->GetYaxis()->SetTitle("Vertex Position R"); 
     h_VertexPosRZ->GetYaxis()->SetRangeUser(0.0, 200.); //
 
     h_VertexPosRZ->Draw("colz"); // draw histo on canvas
     canv2->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv2->SaveAs(" h_VertexPosRZ.gif"); // .pdf and other formats work too! 
     }
