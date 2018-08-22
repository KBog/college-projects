#ifndef _FLOYD_H_
#define _FLOYD_H_

#include <iostream>
#include <vector>

using namespace std;

/* Floyd class */
class Floyd
{
	public:
			int NodeNbr;
			/* The 2D Array to store the distances */
			float **DM;
			/* The 2D Array to store the path of 2 nodes */
			int **PM;
			/* The Not direct value (#) */
			float Dieze;
			/* The Direct value (d) */
			int d;

			/* Floyd constructor */
			Floyd (int NumberOfNodes, int d_=-1, float dieze_value=10000) : NodeNbr(NumberOfNodes), d(d_), Dieze(dieze_value)
			{
				/* Allocating space for the 2D Arrays */
				DM = new float *[NodeNbr];
				PM = new int *[NodeNbr];
				for(int i=0; i<NodeNbr; i++)
				{
					DM[i] = new float [NodeNbr];
					PM[i] = new int [NodeNbr];
				}
				/* Init the 2 arrays to default values */
				Init_Arrays();
				/* Set the Weight array (the given) */
				//Initial_DistanceArray();
			}
			~Floyd()
			{
				for(int i=0; i<NodeNbr; i++)
				{
					delete [] DM[i];
					delete [] PM[i];
				}
				delete [] DM;
				delete [] PM;
			}

			/* Function to init the Weight */
			void Init_Arrays (void)
			{
				/* Init the 2 Arrays to -1 (#) -> (not direct edge) */
				for(int i=0; i<NodeNbr; i++)
					for(int j=0; j<NodeNbr; j++)
					{
						DM[i][j]=Dieze;
						PM[i][j]=(int)Dieze;
						/* Set 0 to the direct edge (d) */
						if(i==j)
						{
							DM[i][j]=0;
							PM[i][j]=d;
						}
					}
			}

			/* Set the current HardCoded Weight (distances) + the Directed edges (d) */
			void Initial_DistanceArray (void)
			{
				DM[0][1]=5;	DM[0][2]=2;	DM[0][5]=3;
				DM[2][1]=2;
				DM[3][2]=6;
				DM[3][4]=1;
				DM[5][1]=1;	DM[5][2]=4;	DM[5][3]=2;	DM[5][4]=4;

				PM[0][1]=d; PM[0][2]=d;	PM[0][5]=d;
				PM[2][1]=d;	
				PM[3][2]=d;	PM[3][4]=d;
				PM[5][1]=d;	PM[5][2]=d;	PM[5][3]=d;
			}

			/* Function to compute the Path Array */
			void Compute_PathArray (void)
			{
				int b,a,c,tmp_dest;
				float Dist_ab_plus_bc=0;

				/* 3 Nested for loops */
				for(b=0; b<NodeNbr; b++)
					for(a=0; a<NodeNbr; a++)
						for(c=0; c<NodeNbr; c++)
						{
							/* Sum the distances ab + bc to compate to ac */
							Dist_ab_plus_bc = DM[a][b] + DM[b][c];
							/* if it's less than the current source to destination distance */
							if(DM[a][c]>Dist_ab_plus_bc)
							{
								/* Update the current source to destination */
								DM[a][c]=Dist_ab_plus_bc;
								/* Update the Path array */
								tmp_dest=b;
								while(PM[a][tmp_dest]!=d)
									tmp_dest=PM[a][tmp_dest];
								PM[a][c]=tmp_dest;
							}
						}
			}

			/* Function to return the Path between 2 nodes in an array */
			vector<int> Get_Path (int Source, int Destination)
			{
				vector<int> Path;

				/* Add the source */
				Path.push_back(Source);

				/* While the current distance is not a (d) direct edge nor a dieze (invalid path) */
				while(PM[Path[Path.size()-1]][Destination] != Dieze && PM[Path[Path.size()-1]][Destination] != d)
					Path.push_back(PM[Path[Path.size()-1]][Destination]);
				/* if the current distance is d -> direct path ==> add the destination to the path array */
				if(PM[Path[Path.size()-1]][Destination]==d)
					Path.push_back(Destination);

				/* Return the path array */
				return Path;

				////////////////////////////////////////////////
				//vector<int> Path, None, RealPath;

				///* Add the source */
				//None.push_back(Source);

				//Path.push_back(Destination);
				//int tmpDest = Destination;

				///* While the current distance is not a (d) direct edge nor a dieze (invalid path) */
				//int pathsize = Path.size();
				//int tmp = Path[pathsize-1];
				//while(PM[Source][tmpDest] != Dieze && PM[Source][tmpDest] != d)
				//{
				//	tmpDest = PM[Source][tmpDest];
				//	Path.push_back(tmpDest);
				//	pathsize = Path.size();
				//	tmp = Path[pathsize-1];
				//}
				///* if the current distance is d -> direct path ==> add the destination to the path array */
				//pathsize = Path.size();
				//tmp = Path[pathsize-1];
				//if(PM[Source][tmpDest]==d)
				//{
				//	Path.push_back(Source);
				//	int s = Path.size();
				//	for(int i=0; i<Path.size(); i++)
				//	{

				//		RealPath.push_back(Path[Path.size()-1-i]);
				//	}
				//}
				//else
				//	return None;
				///* Return the path array */
				//return RealPath;
			}
};

#endif