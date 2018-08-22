#include <iostream>
#include <vector>
#include <deque>

using namespace std;

/* BellmanFordMoor class */
class BellmanFordMoor
{
	public:
			int NodeNbr;
			/* The 2D Array to store the distances */
			float **W;
			/* The 2D Distance Array */
			float **DM;
			/* Array to store the distances from the current source to all the nodes */
			float *D;
			/* The 2D Array to store the path from any source to any node */
			int **PM;
			int *P;
			/* The Not direct value (#) */
			float Dieze;
			/* The Direct value (d) */
			int d;

			deque<int> L;

			/* Floyd constructor */
			BellmanFordMoor (int NumberOfNodes, int d_=-1, float dieze_value=10000) : NodeNbr(NumberOfNodes), d(d_), Dieze(dieze_value)
			{
				/* Allocating space for the 2D Arrays */
				W = new float *[NodeNbr];
				DM = new float *[NodeNbr];
				PM = new int *[NodeNbr];
				for(int i=0; i<NodeNbr; i++)
				{
					W[i] = new float [NodeNbr];
					PM[i]= new int [NodeNbr];
					DM[i] = new float [NodeNbr];
				}
				/* Init the 2 arrays to default values */
				Init_Arrays();
				/* Set the Weight array (the given) */
				//Initial_DistanceArray();
			}

			~BellmanFordMoor()
			{
				for(int i=0; i<NodeNbr; i++)
				{
					delete [] W[i];
					delete [] PM[i];
					delete [] DM[i];
				}
				delete [] W;
				delete [] PM;
				delete [] DM;
				L.clear();
			}

			/* Function to init the Weight */
			void Init_Arrays (void)
			{
				/* Init the 2 Arrays to -1 (#) -> (not direct edge) */
				for(int i=0; i<NodeNbr; i++)
					for(int j=0; j<NodeNbr; j++)
					{
						W[i][j]=DM[i][j]=Dieze;
						PM[i][j]=(int)Dieze;
						/* Set 0 to the direct edge (d) */
						if(i==j)
						{
							W[i][j]=0;
							DM[i][j]=0;
							PM[i][j]=d;
						}
					}
			}

			void Init_DM (void)
			{
				/* Init the 2 Arrays to -1 (#) -> (not direct edge) */
				for(int i=0; i<NodeNbr; i++)
					for(int j=0; j<NodeNbr; j++)
					{
						DM[i][j]=Dieze;
						/* Set 0 to the direct edge (d) */
						if(i==j)
						{
							DM[i][j]=0;
						}
					}
			}

			/* Set the current HardCoded Weight (distances) + the Directed edges (d) */
			void Initial_DistanceArray (void)
			{
				W[0][1]=1;	W[0][3]=3;
				W[1][2]=1;
				W[3][4]=-2;
				W[4][2]=-1;

				PM[0][1]=d;	PM[0][3]=d;
				PM[1][2]=d;	
				PM[3][4]=d;
				PM[4][2]=d;
			}

			/* Function to compute the Path Array */
			void Compute_Path (int source)
			{
				int State=source;
				float tmp_dist=0;
				unsigned int i=0;
				int tmp_dest=0;

				/* Take the distances of the current source to all the nodes */
				D = DM[source];
				/* Take the Path array */
				P = PM[source];

				while(1)
				{
					/* Loop through all the neighbors of the current State */
					for(int neighbor=0; neighbor<NodeNbr; neighbor++)
					{
						/* the current node is a neighbor */
						if(PM[State][neighbor]==d)
						{
							/* Compute the distance */
							tmp_dist = D[State] + W[State][neighbor];
							/* If the current neighbor distance is > than the path dist */
							if(D[neighbor]>tmp_dist)
							{
								/* Update the distance array */
								D[neighbor]=tmp_dist;

								/* Update the Path array */
								tmp_dest=State;
								while(PM[tmp_dest][neighbor]!=d)
									tmp_dest=PM[tmp_dest][neighbor];
								P[neighbor]=tmp_dest;

								/* Update the path array */
								//P[neighbor]=State;
								/* Add the neighbor to the queue */
								for(i=0; i<L.size(); i++)
								{
									if(L[i]==State)
										break;
								}
								if(i==L.size())
									L.push_back(neighbor);
							}
						}
					}
					/* if the list is not empty */
					if(!L.empty())
					{
						/* Take the back of it */
						State=L.back();
						/* Remove the taken node from the queue */
						L.pop_back();
					}
					else
						break;
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
				Compute_Path(Source);

				/* Set the current path array according to the source */
				P = PM[Source];

				/* Add the Destination */
				tmp_Path.push_back(Destination);
				/* Add the source to the path array */
				Path.push_back(Source);

				/* Loop while the current node is not the source & its not a direct neighbor */
				int n = tmp_Path[tmp_Path.size()-1];
				while(P[n]!=Source && P[n]!=Dieze)
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













