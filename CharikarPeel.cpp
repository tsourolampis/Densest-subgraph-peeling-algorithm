#include <iostream>
#include <vector>
#include <queue>          // std::priority_queue
#include <iomanip>
#include "IO.h"
#include "HASH.h"

 

using namespace std; 

/*
Compile: g++ CharikarPeel.cpp -o CharikarPeel -std=gnu++0x -O3
Demo: ./CharikarPeel.exe < toy.txt > toy.log
Charalampos E. Tsourakakis, babis@seas.harvard.edu 

*/


// Charalampos E. Tsourakakis, babis@seas.harvard.edu
const int MAXV = 10000000; //10^7 vertices, change it to whatever you like 
const int MAXE = 100000000; //10^8 edges 
const int QUERYBUFFER = 6000000; 

int  MAXDEG=0; 
 

int E, V;
int eList[MAXE+1][2]; // we load all edges on memoer
int degrees[MAXV+1];  
int NumTriangles=0;

// remove multiple edges, self-loops
void SimplifyGraph() 
{
  int E1 = 1;
  int multipleEdges=0;
  HASH::init();
  for(int i = 1; i <= E; ++i) {
    //printf("Testing edge (%d,%d)\n", eList[i][0], eList[i][1]);
    if(1 <= eList[i][0] && eList[i][0] <= V &&
         1 <= eList[i][1] && eList[i][1] <= V &&
         eList[i][0] != eList[i][1] &&
         HASH::insert(eList[i][0], eList[i][1])) {
      eList[E1][0] = eList[i][0];
      eList[E1][1] = eList[i][1];
	  degrees[eList[i][0]]++;
	  degrees[eList[i][1]]++; 
      E1++;
    }
	else
		multipleEdges++; //printf("has appeared before\n");
  }
  E = E1-1;
  printf("number of edges in simple graph: %d\n", E);
  printf("%d\n",multipleEdges);
}



void GraphIn() {
  int u, v;
  IO::read(V);
  IO::read(E);
  printf("Number of vertices and edges (%d,%d)\n",V,E);
  for(int i = 1; i <= E; ++i) 
  {
    IO::read(u);
    IO::read(v);
	//printf("(%d,%d)\n",u,v);
	if(v>u)
	{
		eList[i][0] = u;
		eList[i][1] = v;
	}
    if(u>v)	
	{
		eList[i][0] = v;
		eList[i][1] = u;
	}
  }
}

void PrintEdgeList()
{
   for(int i=1; i<=E; i++) 
   {
        printf("(%d,%d)\n",eList[i][0],eList[i][1]); 
   }
}

void PrintDegrees()
{ 
   printf("***************************\n");
   printf("Vertex id \t Degree\n"); 
   for(int i = 1; i <= V; i++)
      printf("%d \t %d\n",i, degrees[i]); 
   printf("***************************\n");
}



int MaximumDegree()
{  
   for(int i = 1; i <= V; i++)
    if(MAXDEG < degrees[i] )
    	MAXDEG=degrees[i];
  //printf("maximum degree is %d\n", MAXDEG); 
}


void graphinStdio() {
  scanf("%d%d", &V, &E);
  for(int i = 0; i < E; ++i) {
    scanf("%d%d", &eList[i][0], &eList[i][1]);
  }
}

void ElapsedTime(double startTime)
{ 
   printf("Elapsed time to read input: %f\n", (clock()-startTime)/CLOCKS_PER_SEC );
}

void ElapsedTime(double startTime, char* s)
{ 
   printf("Elapsed time for %s: %f\n", s, (clock()-startTime)/CLOCKS_PER_SEC );
}
///////////////adjacency list
int eStart[MAXV];
int mynext[MAXE];

const int NOEDGE = -1;

void BuildGraph() {
  for(int i = 1; i <= V; ++i) {
    eStart[i] = NOEDGE;
  }
  for(int i = 1; i <= E; ++i) {
    mynext[i] = eStart[eList[i][0]];
    eStart[eList[i][0]] = i;
  }
}

 
		 

vector<int> AdjList[MAXV];
 
void BuildAdjList()
{ 
  for(int i = 1; i<=E; i++)
  {
       AdjList[ eList[i][0] ].push_back( eList[i][1] ); 
	   AdjList[ eList[i][1] ].push_back( eList[i][0] );   
  }
}

// This function deletes a vertex from the graph that has the minimum degree 

int permutation[MAXV+1]; //permutation[1] through permutation[V] contains the desired perm 

double  EDGEDENSITY = -1; 
int CharikarSize=-1; 
double CharikarFe = -1; 
double CharikarSizeFraction=-1.0;

double AvgDegreeVals[MAXV+1]; 
double AvgDegreeOptVals[MAXV+1]; 

int OPTIND; 

priority_queue< pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > q;
void PQCharikarPeel()
{
	//vector<int> subgraph;
	EDGEDENSITY= 2.0*E/V; 
	CharikarFe = (double) 2*E/(V*(V-1)); 
	CharikarSize=V; 
	CharikarSizeFraction=1.0;
	double numedges=(double)E;
	double numvertices=(double)V;
	for (int i = 1; i <= V; ++i) 
        q.push(make_pair(degrees[i], i));

	int c = q.top().second;
	int counter = 0; 
	while (!q.empty()) 
	{
		int c = q.top().second;
        q.pop();
         if (degrees[c] < 0 )// == -1) 
		{ 
            continue;
        }
		else
		{ 
			AvgDegreeOptVals[counter] = EDGEDENSITY;
			AvgDegreeVals[counter] = 2.0*numedges / (double) numvertices;  
			permutation[counter++]=c;  
		}
        numedges -= degrees[c];  
        --numvertices;  
        degrees[c] = -1;  
		if (numvertices > 0) {
            double cur = 2.0*numedges / (double) numvertices;  
            if (cur >= EDGEDENSITY ) {
                EDGEDENSITY = cur;
				CharikarFe = EDGEDENSITY/(numvertices-1);
				CharikarSize = numvertices;
				CharikarSizeFraction = numvertices/V;
               // subgraph.clear();
				OPTIND = counter; 
            }
        }
       // subgraph.push_back(c); 
	
		for (int i=0; i < AdjList[c].size();i++)
		{
			int w = AdjList[c][i];
			if( degrees[w]>0)
			{
				q.push(make_pair(--degrees[w], w));
			}
		}
		if( counter == V)
		    break;
	}


}



void PrintAdjList()
{
	for(int j = 1; j<=V; j++)
	{
	    cout<< "Neighbors of "<<j<<" [";
		for (int i=0; i<AdjList[j].size();i++)
		{
		cout << AdjList[j][i]<< " ";
		}
        cout<< "]" <<endl;
		
	}
}


 
 
void PrintPermutationStatistics()
{ 
   int breakline = 0;
   cout<<"Peeling permutation order"<<endl; 
   cout<<"[";
   for(int i=0; i < V; i++) 
   {
		cout<<" "<< permutation[i];
		if( ++breakline > 25 ) // entries per line 
		{
		        cout<<endl;
				breakline = 0;
		}
   }
   cout<<"]"<<endl;
   /*
	breakline = 0;
   cout<<"Avg Degree values"<<endl;
   cout<<"[";
   for(int i=0; i < V; i++) 
   {
		cout<<" "<< AvgDegreeVals[i];
		if( ++breakline > 25 ) // entries per line 
		{
		        cout<<endl;
				breakline = 0;
		}
   }
   cout<<"]"<<endl;*/
   
  breakline = 0;
   cout<<"Optimal avg degree values"<<endl;
   cout<<"[";
   for(int i=0; i < V; i++) 
   {
		cout<<" "<< AvgDegreeOptVals[i];
		if( ++breakline > 25 ) // entries per line 
		{
		        cout<<endl;
				breakline = 0;
		}
   }
   cout<<"]"<<endl;
   
}



void printOptSubgraph()
{ 
   cout<<"[";
   for(int i=OPTIND; i < V; i++)  
   {
		cout<<" "<< permutation[i];
   }
   cout<<"]"<<endl;

}


int main(int argc, char **argv)
{
  double startTime = clock();
  GraphIn();
  ElapsedTime(startTime);
  SimplifyGraph(); // besides making G simple, we compute the degree sequence here 
  BuildGraph();
  BuildAdjList();
  MaximumDegree(); // what is the max degree? 
  startTime = clock();
  PQCharikarPeel();
  cout<<"********** CHARIKAR PEELING ***************"<<endl;
  ElapsedTime(startTime,"Peel Charikar"); 
  cout<<"Charikar's Results"<<endl; 
  cout<<"n:"<<V<<endl;
  cout<<"m:"<<E<<endl;  
  cout<<"Avg. degree 2m/n:"<<setprecision(3) << fixed <<(double)2.0*E/V<<endl;  
  cout<<"Optimal Avg. Degree:"  <<setprecision(3) << fixed <<EDGEDENSITY<<endl;
  cout<<"Size:"  <<setprecision(3) << fixed <<CharikarSize<<endl;
  cout<<"Size/|V|:"  <<setprecision(3) << fixed <<CharikarSizeFraction<<endl;
  cout<<"fe:"  <<setprecision(3) << fixed <<CharikarFe<<endl;
  cout<<"*******************************************"<<endl;
  
  bool subgraphflag = false; //false 
  if( subgraphflag ) 
  {
      cout<<"********** Optimal Subgraph *********************"<<endl;
      printOptSubgraph();
	  cout<<"*************************************************"<<endl;
  }

  bool statsflag = false; //false 
  if( statsflag ) 
  {
     
	  cout<<"********** Permutation Statistics *********************"<<endl;
	  PrintPermutationStatistics();
	  cout<<"*************************************************"<<endl;
  }
  
  
  /* Alternative version, slower than then one with priority queues, kept it for sake of completeness
  startTime = clock();
  CharikarPeel();
  cout<<"********** CHARIKAR PEELING ***************"<<endl;
  ElapsedTime(startTime,"Peel Charikar"); 
  cout<<"Charikar's Results"<<endl; 
  cout<<"n:"<<V<<endl;
  cout<<"m:"<<E<<endl;  
  cout<<"Avg. degree 2m/n:"<<(double)2.0*E/V<<endl;  
  cout<<"Optimal Avg. Degree:"  <<EDGEDENSITY<<endl;
  cout<<"Size:"  <<CharikarSize<<endl;
  cout<<"fe:"  <<CharikarFe<<endl;
  cout<<"*******************************************"<<endl;
  */ 
  return 0;
}
 
