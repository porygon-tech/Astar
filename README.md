# Astar
A* algorithm implementation on the spanish road network for the Optimisation subject at the official master in modelling for science and engineering at the autonomous university of Barcelona (UAB).
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
The `bin_out.bin` file is a binary file containing the graph information such that it is much quicker to read for the `pathfinder.c`. Hence, the original file only needs to be read once.
### Queries
The pathfinder is compiled and executed as follows:
```
gcc pathfinder.c -o pathfind -Ofast -lm && ./pathfind bin_out_spain.bin
```
after running it, the command line asks the user to enter a start node ID and a goal node ID. Then, prints the haversine distance (the heuristic algorithm used in our implementation) between both nodes and starts expanding successors. When the goal node is reached, the resulting path is outputted into a csv file.
### Results
The optimal path between Santa Maria del Mar and the Giralda has 2665 nodes and the distance is 897623.2919 meters.
This implementation of the A* algorithm takes approximately 100s.

