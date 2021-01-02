#!/usr/bin/bash
#	bash preprocessing.sh ../cataluna.csv


#n_nodes=$( cat $1 | grep ^node | wc -l)
#n_ways=$(  cat $1 | grep ^way  | wc -l)
cwd=$(pwd)
echo "generating parsedfiles folder"
mkdir parsedfiles -p

echo "extracting nodes from $1"
cat $1 | grep ^node > nodes.csv

echo "extracting ways from $1"
cat $1 | grep ^way  > ways.csv


echo "cleaning files... "
perl edgecleaner2.pl nodes.csv ways.csv
mv nodes_clean.csv ways_clean.csv parsedfiles
rm nodes.csv ways.csv

echo "files stored at" "$cwd/parsedfiles"