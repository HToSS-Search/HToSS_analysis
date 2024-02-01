# #now merge things
mkdir total
hadd -f /scratch/sdansana/output_QCD_MuEnrichedPt5.root QCD/*.root
mv /scratch/sdansana/output_QCD_MuEnrichedPt5.root total/output_QCD_MuEnrichedPt5.root

hadd -f /scratch/sdansana/output_wPlusJets.root wPlusJets/*.root
mv /scratch/sdansana/output_wPlusJets.root total/output_wPlusJets.root

hadd -f /scratch/sdansana/output_singleTop.root singleTop/*.root
mv /scratch/sdansana/output_singleTop.root total/output_singleTop.root 

hadd -f /scratch/sdansana/output_ttbar.root ttbar/*.root
mv /scratch/sdansana/output_ttbar.root total/output_ttbar.root 

hadd -f /scratch/sdansana/output_ttV.root ttV/*.root
mv /scratch/sdansana/output_ttV.root total/output_ttV.root 

hadd -f /scratch/sdansana/output_VV.root VV/*.root
mv /scratch/sdansana/output_VV.root total/output_VV.root 

hadd -f /scratch/sdansana/output_VVV.root VVV/*.root
mv /scratch/sdansana/output_VVV.root total/output_VVV.root 

hadd -f /scratch/sdansana/output_DYJetsToLL.root DYJetsToLL_Pt/*.root
mv /scratch/sdansana/output_DYJetsToLL.root total/output_DYJetsToLL.root

hadd -f /scratch/sdansana/output_Run2017.root Data/*.root
mv /scratch/sdansana/output_Run2017.root total/output_Run2017.root

cp ggH/*.root total/.
