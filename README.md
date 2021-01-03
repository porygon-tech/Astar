# Astar
A* algorithm implementation.
## Manual
### Preprocessing

```bash
bash preprocessing/preprocessing.sh cataluna.csv
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
