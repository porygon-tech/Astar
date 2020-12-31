#!/usr/bin/bash
n_nodes=$( cat $1 | grep ^node | wc -l)
n_ways=$(  cat $1 | grep ^way  | wc -l)
cwd=$(pwd)
mkdir parsedfiles -p
echo "parsing nodes"
cat $1 | grep ^node > nodes.csv
echo "parsing edges"
cat $1 | grep ^way  > ways.csv

mv nodes.csv ways.csv parsedfiles
