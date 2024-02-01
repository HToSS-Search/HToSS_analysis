#!/bin/bash

dirname=$1
mkdir -p ~/public_html/HToSS_plots/Stacked/$dirname

# Define a list
my_list=( \
"MS2_ctau0" "MS2_ctau1" "MS2_ctau10" "MS2_ctau100" \
"MS1p8_ctau0" "MS1p8_ctau1" "MS1p8_ctau10" "MS1p8_ctau100" \
"MS1p6_ctau0" "MS1p6_ctau1" "MS1p6_ctau10" "MS1p6_ctau100" \
"MS1p4_ctau0" "MS1p4_ctau1" "MS1p4_ctau10" "MS1p4_ctau100" \
"MS1p2_ctau0" "MS1p2_ctau1" "MS1p2_ctau10" "MS1p2_ctau100" \
"MS1p1_ctau0" "MS1p1_ctau1" "MS1p1_ctau10" "MS1p1_ctau100" \
)

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
