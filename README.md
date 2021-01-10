# Astar
A* algorithm implementation.
## Manual
### Preprocessing
First of all, the csv file is preprocessed to get just the needed information, so the `csvreader.c` deals with a much lower amount of information. The preprocessing must be performed from inside the `preprocessing/` folder as follows:
```bash
bash preprocessing.sh ../cataluna.csv
```
A folder called `parsedfiles/` will be then generated with two files with the following formats:
\
`nodes_clean.csv`:
```
node_id|node_name|node_lat|node_lon
```
\
`ways_clean.csv`:
```
way_id|oneway|member_nodes
```

These files will be the input of the `csvreader.c` script as is shown below:
```
gcc csvreader.c -o csvread -O3 -lm && ./csvread preprocessing/parsedfiles/nodes_clean.csv preprocessing/parsedfiles/ways_clean.csv bin_out.bin
```
The `bin_out.bin` file is a binary file containing the graph information such that it is much quicker to read. Hence, the original file only needs to be read once.
### Queries

