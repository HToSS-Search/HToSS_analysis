#!/bin/bash

dirname=$1
mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname

# Define a list
my_list_kaon=( \
"MS2_ctau0" "MS2_ctau1" "MS2_ctau10" "MS2_ctau100" \
"MS1p8_ctau0" "MS1p8_ctau1" "MS1p8_ctau10" "MS1p8_ctau100" \
"MS1p6_ctau0" "MS1p6_ctau1" "MS1p6_ctau10" "MS1p6_ctau100" \
"MS1p4_ctau0" "MS1p4_ctau1" "MS1p4_ctau10" "MS1p4_ctau100" \
"MS1p2_ctau0" "MS1p2_ctau1" "MS1p2_ctau10" "MS1p2_ctau100" \
"MS1p1_ctau0" "MS1p1_ctau1" "MS1p1_ctau10" "MS1p1_ctau100" \
)
# Define a list; other ctau points to be added
my_list_pion=( \
"MS0p4_ctau0" "MS0p4_ctau1" "MS0p4_ctau10" "MS0p4_ctau100" \
"MS0p6_ctau0" "MS0p6_ctau1" "MS0p6_ctau10" "MS0p6_ctau100" \
"MS0p8_ctau0" "MS0p8_ctau1" "MS0p8_ctau10" "MS0p8_ctau100" \
"MS0p9_ctau0" "MS0p9_ctau1" "MS0p9_ctau10" "MS0p9_ctau100" \
"MS1_ctau0" "MS1_ctau1" "MS1_ctau10" "MS1_ctau100" \
)
my_list=()
if [[ $dirname == *"Kaon"* ]]; then
  for tmp in "${my_list_kaon[@]}"; do
    my_list+=("$tmp")
  done
else
  for tmp in "${my_list_pion[@]}"; do
    my_list+=("$tmp")
  done
fi
# Loop over the list
for item in "${my_list[@]}"
do
#   echo "Processing item: $item"
  # Add your code to process each item here
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/prompt
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/displacedmumu
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/displacedhh
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/displaced
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/prompt
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/displacedmumu
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/displacedhh
  mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/displaced
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/prompt/.
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/displacedmumu/.
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/displacedhh/.
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LogYaxis/$item/displaced/.
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/prompt/.
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/displacedmumu/.
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/displacedhh/.
  cp ~/index.php ~/public_html/HToSS_plots/Stacked/$dirname/LinearYaxis/$item/displaced/.
done