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
     TH2I* h_VertexPosXY = (TH2I*)inFile.Get("h_VertexPosXY"); // Load in histogram
   
     TCanvas* canv2 = new TCanvas ("canv2", "canv2", 50, 50, 800, 600); // Canvas to draw histogram on
   
     gStyle->SetOptStat(0);
     gPad->SetLogz(); // set log scale for y-axis on for objects drawn on the canvas from this point on

     h_VertexPosXY->GetXaxis()->SetTitle("Vertex position x (mm)"); // set a title for the x-axis
     h_VertexPosXY->GetXaxis()->SetRangeUser(-100., 100.); // change the x-axis range to 0-200
     h_VertexPosXY->GetYaxis()->SetTitle("Vertex Position y (mm)"); 
     h_VertexPosXY->GetYaxis()->SetRangeUser(-100., 100.); //
 
     h_VertexPosXY->Draw("colz"); // draw histo on canvas
     canv2->Draw(); // draw canvas!

     // save canvas with drawn histogram
     canv2->SaveAs(" h_VertexPosXY.gif"); // .pdf and other formats work too! 
     }
