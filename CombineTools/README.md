# Combine Tools for CB's Masters work

This code WILL NOT run within the analysis code framework and requires the use of CMSSW. The instructions below will detail how to install CMSSW and the Combine Tool.
You do not need to understand how or why the CMS SoftWare works the way it does - these instructions should be able to setup everything and provide the instructions
on how to use the tool.

***

## CMSSW Setup info:

Add the following to your .bashrc file in your home directy on the m-9 machine.

```
alias InitCmsEnv='. /cvmfs/cms.cern.ch/cmsset_default.sh'
```

Either re-log into the machine or execute the following:

```
source ~/.bashrc 
```

This creates a new command 'InitCmsEnv' that sources and sets up the general CMS software environment.

To install the version of CMSSW you need for Combine, execute the following:

```
export SCRAM_ARCH=slc6_amd64_gcc491
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src 
cmsenv
git clone git@github.com:cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v6.3.1
scramv1 b clean
scramv1 b
```

These commands install CMSSW version 7_4_7, setup this version's environment, download the Higgs Analysis Combined Limit (Combine) tool, checks out the recommend tag/version, and compiles everything.

Once this has been done, all you have to do in future is ...

```
cd CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit
InitCmsEnv
cmsenv
```

... and Combine will be ready to use.

Copy over appropriate datacard

```
cp ~/HToSS_analysis/CombineTools/datacard_1GeV_10mm.txt .
```
***

## Using Combine:

In the same directory as this README.md is a sample datacard named 'datacard.txt'. This will need copying to ../yourPathToCMSSW/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/.
Currently not all the systematics are setup within it, but to run it, you will need to execute the following commands.

These six commands cover the observed limit, expected limit, and the upper and lower 1 and 2 sigma bands.

Observed limit:
```
combine datacard_1GeV_10mm.txt -M HybridNew -H ProfileLikelihood --frequentist --testStat LHC --fork 4  -T 5000 --rAbsAcc=0.01 --name output_name
```

Expected (median) limit:
```
combine datacard_1GeV_10mm.txt -M HybridNew -H ProfileLikelihood --frequentist --testStat LHC --expectedFromGrid=0.5 --fork 4  -T 5000 --rAbsAcc=0.01 --name output_name
```

Upper 1 sigma band
```
combine datacard_1GeV_10mm.txt -M HybridNew -H ProfileLikelihood --frequentist --testStat LHC --expectedFromGrid=0.84 --fork 4  -T 5000 --rAbsAcc=0.01 --name output_name
```

Lower 1 sigma band
```
combine datacard_1GeV_10mm.txt -M HybridNew -H ProfileLikelihood --frequentist --testStat LHC --expectedFromGrid=0.16 --fork 4  -T 5000 --rAbsAcc=0.01 --name output_name
```

Upper 2 sigma band
```
combine datacard_1GeV_10mm.txt -M HybridNew -H ProfileLikelihood --frequentist --testStat LHC --expectedFromGrid=0.975 --fork 4  -T 5000 --rAbsAcc=0.01 --name output_name
```

Lower 2 sigma band
```
combine datacard_1GeV_10mm.txt -M HybridNew -H ProfileLikelihood --frequentist --testStat LHC --expectedFromGrid=0.025 --fork 4  -T 5000 --rAbsAcc=0.01 --name output_name
```

## Updating the datacard.

The only bits you will need to modify are the number of events in the datacard -> as this is a simple counting experiment, all you need to do is provide a number of the number of events after your full selection.

My recommendation for this is for you, in your code, to have a counter that increments every time an event passes your full event selection (weighted for luminosity/number of simulated events & cross section), and
prints the final sum (for each sample) to the console. These numbers will then be used to update the datacard before you rerun combine.

# Updating number of data events
On line 8, the number of observed events (in data) is given. This value will need replacing/updating with the number of events you find after your full event selection is applied to data.

# Updating number of simulated signal and background events.
On line 14, the number of expected signal events (simulation) is given in the 2nd column (1st being the definition of the row) and the number of expected background events (simulation) is given in the 3rd column.
These values will need replacing/updating with the number of events you find after your full event selection is applied to your signal/background samples.

#Systematics
These have been predetermined except for the statistical uncertainty uncerts. These depends on the number of signal and QCD events you have and is calculated as:

```
stat uncert = \frac{\sqrt{n}}{n}
```

The stat uncert needs entering onto lines 25 and 26 of the datacard for the signal and QCD samples, respectively.


#TO BE DONE
