import ROOT
import os

# Suppress ROOT GUI
ROOT.gROOT.SetBatch(True)

# Mass and ctau values
mass_values = [0.4, 0.6, 0.8, 0.9, 1, 1.1, 1.2, 1.4, 1.6, 1.8, 2]
ctau_values = [0, 1, 10, 100]

directory="plots/UL2018/PionAssumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_unblinded_250104/total/"
# Histogram name
hist_name = "h_AvgMass_mumu_hh"
hist_name_tot = "h_genHiggsPt"

# Dictionary to store histograms
histograms = {}
histograms_tot = {}

# Load histograms
for mass in mass_values:
    if mass>1:
        directory=directory.replace("Pion","Kaon")
    else:
        directory=directory.replace("Kaon","Pion")
    for ctau in ctau_values:
        filename = "output_HToSS_MH125_MS"+str(mass).replace('.','p')+"_ctauS"+str(ctau).replace('.','p')+".root"
        filename=directory+filename
        if not os.path.exists(filename):
            print(f"Missing file: {filename}")
            continue

        file = ROOT.TFile.Open(filename)
        if not file or file.IsZombie():
            print(f"Error opening file: {filename}")
            continue

        hist = file.Get("prompt/"+hist_name)
        hist.Add(file.Get("displacedmumu/"+hist_name))
        hist.Add(file.Get("displacedhh/"+hist_name))
        hist.Add(file.Get("displacedhh/"+hist_name))
        
        hist_tot=file.Get(hist_name_tot)
        if not hist:
            print(f"Missing histogram '{hist_name}' in {filename}")
            file.Close()
            continue

        histograms[(mass, ctau)] = hist.GetEntries()
        histograms_tot[(mass, ctau)] = hist_tot.GetEntries()

        file.Close()

# Print the table
# Header row
header = ["mass \\ ctau"] + [str(ctau) for ctau in ctau_values]
print("{:<12}".format(header[0]), end="")
for ctau in header[1:]:
    print("{:>10}".format(ctau), end="")
print()

# Rows
for mass in mass_values:
    print("{:<12}".format(str(mass)), end="")
    for ctau in ctau_values:
        entries = histograms.get((mass, ctau), "N/A")
        if isinstance(entries, float):
            entries = int(entries)
        print("{:>10}".format(entries), end="")
    print()

# Print the table
# Header row
print("Percentage of events left")
header = ["mass \\ ctau"] + [str(ctau) for ctau in ctau_values]
print("{:<12}".format(header[0]), end="")
for ctau in header[1:]:
    print("{:>10}".format(ctau), end="")
print()

# Rows
for mass in mass_values:
    print("{:<12}".format(str(mass)), end="")
    for ctau in ctau_values:
        entries = histograms.get((mass, ctau), "N/A")
        entries_tot=histograms_tot.get((mass,ctau), 1)
        entries = entries/entries_tot
        if isinstance(entries, float):
            entries = round(entries,3)
        print("{:>10}".format(entries), end="")
    print()
