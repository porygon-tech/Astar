#include "lib_Astar.h"

int main (int argc, char *argv[]){
   FILE *fin;
   unsigned long n_nodes;
   unsigned long ntotnsucc;
   unsigned long* allsuccessors;
   nodetype *nodes;
   
   if(( fin = fopen (argv[1], "r")) == NULL)
       ExitError("The data files does not exist or cannot be opened", 11);
       
   //Global data --- header
   if( fread(&n_nodes, sizeof(unsigned long), 1, fin) + 
       fread(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2)
           ExitError("When reading the header of the binary data file", 12);
           
   //Getting memory for all data
   if((nodes = (nodetype *) malloc(n_nodes*sizeof(nodetype))) == NULL)
       ExitError("When allocating memory for the nodes vector", 13); 
       
   if((allsuccessors = (unsigned long *) malloc(ntotnsucc*sizeof(unsigned long))) == NULL)
       ExitError("When allocating memory for the edges vector", 15);
       
   //Reading all data from file
   if(fread(nodes, sizeof(nodetype), n_nodes, fin) != n_nodes)
      ExitError("When reading nodes from the binary file", 17);
   
   if(fread(allsuccessors, sizeof(unsigned long), ntotnsucc, fin) != ntotnsucc)
      ExitError("When reading successors from the binary file", 18);
   
   fclose(fin);
   
   //Setting pointers to successors
   for(int i=0; i<n_nodes; i++) if(nodes[i].nsucc) {
       nodes[i].successors = allsuccessors; 
       allsuccessors += nodes[i].nsucc;
   }
}     