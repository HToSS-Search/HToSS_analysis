//An example macro for manipulating plots

#include "TFile.h"

#include "TCanvas.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLatex.h"

#include <string>
#include <iostream>

void examplePlotMacro(const std::string& histogram, const histo) {

    TFile inFile("ggH_MS2_ctau10.root"); // read in file. Exercise: can this be done through a command line arguement? That way you don't have to edit code for each new histogram ...
    
    std::cout << "Enter in name of histogram: ";
   
    //TH2I* h_VertexPosXY = (TH2I*)inFile.Get("h_VertexPosXY"); // Load in histogram
    TH1F* histo = (TH1F*)inFile.Get(histogram);
    
    TCanvas* canvy = new TCanvas ("canvy", "canvy", 50, 50, 800, 600); // Canvas to draw histogram on
    //gPad->SetLogy(); // set log scale for y-axis on for objects drawn on the canvas from this point on

    histo->GetXaxis()->SetTitle("p_{T} GeV"); // set a title for the x-axis
    histo->GetXaxis()->SetRangeUser(0.0, 200.); 
    //h_VertexPosXY->GetXaxis()->SetTitle("Vertex position x"); // set a title for the x-axis
    //h_VertexPosXY->GetXaxis()->SetRangeUser(-150., 150.); // change the x-axis range to 0-200
    //h_VertexPosXY->GetXaxis()->SetRangeUser(0.0, 200.); // change the x-axis range to 0-200
    
    //h_VertexPosXY->GetYaxis()->SetTitle("Vertex position y"); // set a title for the x-axis
    //h_VertexPosXY->GetYaxis()->SetRangeUser(-150., 150.); // change the x-axis range to 0-200

    histo->Draw();
    //h_VertexPosXY->Draw(); // draw histo on canvas
    canvy->Draw(); // draw canvas!

    // save canvas with drawn histogram
    canvy->SaveAs(histogram.gif); // .pdf and other formats work too!
    canvy->SaveAs(histogram); 
    delete canvy;
}
main()
{
    string histogram;
    std::cin >> histogram;
    std::cin >> histo
}
