#include <iostream>
#include <vector>
#include <list>

using namespace std;

/* Floyd class */
class Dijkstra
{
	public:
			int NodeNbr;
			/* The 2D Array to store the distances */
			float **W;
			/* Array to store the distances from the current source to all the nodes */
			float *D;
			/* The 2D Array to store the path from any source to any node */
			int **PM;
			int *P;
			/* The Not direct value (#) */
			float Dieze;
			/* The Direct value (d) */
			int d;

			list<int> A;
			vector<int> C;

			/* Floyd constructor */
			Dijkstra (int NumberOfNodes, int d_=-1, float dieze_value=10000) : NodeNbr(NumberOfNodes), d(d_), Dieze(dieze_value)
			{
				/* Allocating space for the 2D Arrays */
				W = new float *[NodeNbr];
				PM = new int *[NodeNbr];
				for(int i=0; i<NodeNbr; i++)
				{
					W[i] = new float [NodeNbr];
					PM[i]= new int [NodeNbr];
				}
				/* Init the 2 arrays to default values */
				Init_Arrays();
				/* Set the Weight array (the given) */
				//Initial_DistanceArray();
			}

			~Dijkstra()
			{
				for(int i=0; i<NodeNbr; i++)
				{
					delete [] W[i];
					delete [] PM[i];
				}
				delete [] W;
				delete [] PM;
				A.clear();
				C.clear();
			}

			/* Function to init the Weight */
			void Init_Arrays (void)
			{
				/* Init the 2 Arrays to -1 (#) -> (not direct edge) */
				for(int i=0; i<NodeNbr; i++)
					for(int j=0; j<NodeNbr; j++)
					{
						W[i][j]=Dieze;
						PM[i][j]=(int)Dieze;
						/* Set 0 to the direct edge (d) */
						if(i==j)
						{
							W[i][j]=0;
							PM[i][j]=d;
						}
					}
			}

			/* Set the current HardCoded Weight (distances) + the Directed edges (d) */
			void Initial_DistanceArray (void)
			{
				W[0][1]=5;	W[0][2]=2;	W[0][5]=3;
				W[2][1]=2;
				W[3][2]=6;
				W[3][4]=1;
				W[5][1]=1;	W[5][2]=4;	W[5][3]=2;	W[5][4]=4;

				PM[0][2]=d;	PM[0][5]=d;
				PM[2][1]=d;	
				PM[3][2]=d;	PM[3][4]=d;
				PM[5][1]=d;	PM[5][2]=d;	PM[5][3]=d;
			}

			void Set_AC_Arrays (int source)
			{
				/* Clear the A list and C array */
				A.clear();	C.clear();

				/* Loop through all the nodes */
				for(int i=0; i<NodeNbr; i++)
				{
					/* if the current node is not the source */
					if(i!=source)
						/* Add it to the A */
						A.push_back(i);
				}
				/* Add only the source */
				C.push_back(source);
			}

			/* Function to return the node that has the min distance to the source node */
			int ShortestNode_ToSource (int source)
			{
				list<int>::iterator it=A.begin();
				int min_node=*it;

				/* Loop through the content of A */
				for(it++; it!=A.end(); it++)
				{
					/* Check if the current chosen node has a distance less than the min node */
					if(D[*it]<D[min_node])
						min_node=*it;
				}
				/* remove the min node */
				A.remove(min_node);
				/* Add the min node to the visited nodes */
				C.push_back(min_node);
				return min_node;
			}

			/* Function to compute the Path Array */
			void Compute_Path (int source)
			{
				int n;
				float tmp_dist=0;
				int tmp_dest=0;

				/* Take the distances of the current source to all the nodes */
				D = W[source];
				/* Take the Path array */
				P = PM[source];
				/* Init the A list & the C Array depending on the current source */
				Set_AC_Arrays(source);

				/* Loop i-2 times (without the source and the last node in A has the max distance) */
				for(int i=1; i<NodeNbr-1; i++)
				{
					/* Get the node having the min distance to the source then update the A & C */
					n = ShortestNode_ToSource(source);

					/* Loop through the nodes in A */
					int s;
					for(list<int>::iterator e=A.begin(); e!=A.end(); e++)
					{
						s=(*e);
						/* Compute the distance from the min node to the current node in A */
						tmp_dist = D[n] + W[n][s];
						/* If it has a less distance than the min node */
						if(D[s]>tmp_dist)
						{
							/* Update the distance array */
							D[s]=tmp_dist;
							/* Update the Path array */
							tmp_dest=n;
							while(PM[tmp_dest][s]!=d)
								tmp_dest=PM[tmp_dest][s];
							P[s]=tmp_dest;
							/* Update the path array */
							//P[s]=n;
						}
					}
				}
			}

			/* Function to compute all the pathes from all the nodes */
			void Compute_AllPaths (void)
			{
				/* Take the distances of the current source to all the nodes */
				for(int cur_source=0; cur_source<NodeNbr; cur_source++)
					/* Compute the current path from the current source */
					Compute_Path(cur_source);
			}

			/* Function to return the Path between 2 nodes in an array */
			vector<int> Get_Path (int Source, int Destination)
			{
				vector<int> Path, tmp_Path;

				/* Set the current path array according to the source */
				P = PM[Source];

				/* Add the Destination */
				tmp_Path.push_back(Destination);
				/* Add the source to the path array */
				Path.push_back(Source);

				/* Loop while the current node is not the source & its not a direct neighbor */
				int n = tmp_Path[tmp_Path.size()-1];
				while(P[n]!=Source && P[n]!=d && P[n]!=Dieze)
				{
					tmp_Path.push_back(P[n]);
					n = tmp_Path[tmp_Path.size()-1];
				}

				/* If the current node has a Max distance --> there is not path --> exit */
				if(P[tmp_Path[tmp_Path.size()-1]]==Dieze)
					return Path;
		
				/* Get the real path which is in reverse of the tmp */
				for(int i=(int)tmp_Path.size()-1; i>=0; i--)
					Path.push_back(tmp_Path[i]);

				/* Return the path array */
				return Path;
			}
};













