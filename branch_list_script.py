fin = open('include/AnalysisEvent.hpp','r')
lines = fin.readlines()
for line in lines:
	if ("fChain->SetBranchAddress" not in line) or (line=='') :
		continue
	tmp = line.split('(')[1].split(',')[0].replace('"','')
	print(tmp)
