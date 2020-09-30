# HToSS Code

Barebones branch of code used for the VUB/IIHE EOS HToSS analysis.
Contains example C++ program and python script for running over input nTuples

N.B. this code assumes that the user has ROOT installed locally and has the ROOT environmental variables setup and that they are working in an environment that has cvmfs.
It can run without cvmfs, but the user will need to install various programming libraries.

***

## Setup info:

To simply get the example python script and executable program working, follow the instructions below.

1) download this branch of the repository in your prefered terminal interface:

```bash
git clone git@github.com:VUB-EOS-Searches/HToSS_analysis.git -b mastersWork
```

2) setup the environmental variables required by the C++ program libraries and python script (i.e. it finds ROOT for you!)

```bash
source setup.sh
```

3) ensure the library, excutable build and object build directories exist if building the C++ program (not required if using python scripts)
```bash
mkdir lib bin obj
```

4) build the example C++ program
```bash
make
```

## Running the example python script:
A simple python script has been written to produce similar plots to the example C++ program. This script requires no arguements and outputs the histograms in
individual ROOT files to the current directory. It must be noted however, that as this is a very simple python script, it only runs over one input ROOT file
and filepath and name of this ROOT file is hardcoded into the script. Either the user *must* copy their input ROOT nTuple file to the running directory and
rename it OR they must update this script in order for it to run.

To run:

```bash
python scripts/plotGenParticleInfo.py
```


## Running the example C++ program:

# Overview:
The executable =simpleAnalysis.exe=, which has been built in =bin/=, is a stripped down version of the code used in the analysis in the current development branch(es).
The libraries it loads contain a large number of methods used to produce results and plots, which, through command line arguements, are used to run over samples that
are listed and defined in configuration files.

While the program's code is more longer (and admittedly complicated) than the python script, it provides a better structure for conducting larger scale processing of
a larger number of events in a more computationally efficient manner than python (i.e. faster and less resource intensive).
If this simple C++ program is built upon, it'll naturally lend itself to object-orientated programming and the uses of classes - just like the original version of this
code was for the search of a rare single top quark process using 2016+2017 simulated and real data from the CMS experiment!

Try running both the python script and the C++ program and see how long each one takes!

The code consists of .hpp header files, one defining the nTuple tree structure (AnalysisEvent.hpp) and two declaring the configuration parser and the dataset classes (used for reading in configuration 
files and processing the datasets in those configs that are read in by the program); two .cpp source code files that define the configuration parser and the dataset classes, and a .cxx source code file 
that is used by the makefile to create an executable. The .cxx file contains the main() program function and, using the config parser and dataset classes, reads in the nTuple trees detailed in the
configuration file used, and produces histograms.

# Dependencies:
- If you are not on a machine with cvmfs setup, the makefile will need modifying to include the necessary libraries manually. If this is required, it's best to speak to the author!
- ROOT (https://root.cern.ch/) is installed and its enviromental variables are setup in the user's .bashrc file (e.g. =source '/some/path/root/bin/thisroot.sh'=)
- input files: the config scripts for the datasets in (configs/2017/datasets/ggH/) assume that the input ROOT files are stored in a certain location that is definitely different to
where you have yours. These files will need updating. I would recommend the use of the 'sed' command line tool to replace the file path string as appropriate.

# Running the program

```bash
./bin/simpleAnalysis.exe -c configs/2017/ggH/ggH_MS2_ctauS10_Conf.yaml -o ggH_MS2_ctau10.root
```

This first part is calling/running the program simpleAnalysis.exe. The part after '-c' is the command line arguement to use the configuration file.
The part after '-o' is the output name/location of the output ROOT file containing the generator level particle histograms in the example program.

There are several other config files which are provided which you can also use!

# Help!
Use the help arguement initially:
```bash
./bin/simpleAnalysis.exe -h
```
and if that doesn't help, contact the author :)


---

