indirname1=$1
indirname2=$2
dirname=$3
#python3 scripts/massbounds.py --input1 $indirname1 --input2 $indirname2 -o $dirname -r 4 --xlow 0. --xhigh 2.5 --yhigh 1.5 -n 1 --updatecuts
python3 scripts/massbounds.py --input1 $indirname1 --input2 $indirname2 -o $dirname -r 2 --xlow 0. --xhigh 2.5 --yhigh 1.5 -n 1 --updatecuts
#python3 scripts/massbounds.py --input1 $indirname1 --input2 $indirname2 -o $dirname -r 4 --xlow 0. --xhigh 2.5 --yhigh 1.5 -n 1 --log 1
#cp $dirname/function_combined.txt /user/sdansana/CMSSW_10_6_27/src/HToSS_analysis/configs/2017/cuts/function_combined.txt
