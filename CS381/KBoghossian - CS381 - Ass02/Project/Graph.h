#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <string>
#include "Viewing.h"
#include "Floyd.h"
#include "BellmanFordMoor.h"
#include "Dijkstra.h"
#include "Graphics.h"

using namespace std;

extern Character *NodeShape;
extern Character *MainCharacter;

extern int Source;
extern Vertex destVtx;
extern int Source_TrapIdx, Destination_TrapIdx;

#define INTERP_FACT 15

enum ShortestPath_Type { Floyd_Type, BellmanFordMoor_Type, Dijkstra_Type };

/* Graph Node class */
class GraphNode
{
	public:
			/* The node position in the world (all the nodes will use the same character, 
				but will change its position so it will change the matrix) */
			VERTEX Position;
			/* Index in the array of nodes in the Graph */
			int Index;
			/* The array of neighbors */
			vector<int> Neighbors;
			/* A pointer to the character shape */
			Character *Shape;

			GraphNode(VERTEX position, int index) : Position(position), Index(index)
			{
				/* Setting the pointer to the shape */
				Shape=NodeShape;
			}

			void Draw (ViewPort *ViewPort_List)
			{
				Matrix<3> Object_Matrix, Mapping_Matrix, Final;
				
				/* Building the viewing matrix */
				Viewing_Transformation(ViewPort_List, Mapping_Matrix);

				Shape->GetSettings().Position = Position;
				/* Transforming the objects from their model coodinates to the world coord */
				Shape->BuildCharacterMatrix(Object_Matrix.M);
				/* Concatinating the final matrix */
				Final=Mapping_Matrix*Object_Matrix;
				/* Drawing the object */
				Shape->Draw(Final.M);
			}

			/* Function to display the index of the current node */
			void DisplayIndex (ViewPort *VP)
			{
				/* Transforming the position to the screen coordinates */
				Matrix<3> Mapping_Matrix;
				Point_Float TransfVtx;
				TransfVtx.x = Position.x-2.5f;
				TransfVtx.y = Position.y+15;
				
				/* Building the viewing matrix */
				Viewing_Transformation(VP, Mapping_Matrix);
				TransfVtx = Mapping_Matrix.M*TransfVtx;

				char numb[5];
				sprintf(numb,"%i",Index);
				Graphics_TextOut((int)TransfVtx.x,(int)TransfVtx.y,numb, 0, 200, 0);
			}
};

/* Graph class */
class Graph
{
	public:
			/* The nodes in the graph */
			vector<GraphNode *> Nodes;
			/* The current number of nodes */
			int NodeNbr;
			/* The Shortest Path AI Algorithm */
			Floyd *floyd;
			BellmanFordMoor *fordmoor;
			Dijkstra *dijkstra;

			ShortestPath_Type ShortestPathType;

			/* The stored path */
			vector<int> Path;
			/* The interpolation factor */
			float Interpolate_Factor;
			/* A bool to know if we are interpolating a path */
			bool Traversing_Path, External_Path_S, External_Path_D;
			/* An index in the path array to know which 2 current nodes we are interpolating */
			int CurrentNode;
			/* The distance factor */
			float Step;
			/* The direction of the path */
			Vector VectDir;

			Graph () : NodeNbr(0)
			{
				Traversing_Path=false;
				External_Path_S=External_Path_D=false;
				Interpolate_Factor=0;
				ShortestPathType=Floyd_Type;
			}
			Graph (int nodenbr) : NodeNbr(0)
			{
				Traversing_Path=false;
				External_Path_S=External_Path_D=false;
				Interpolate_Factor=0;
				/*VERTEX position;
				position.x=position.y=0;

				for(int i=0; i<NodeNbr; i++)
					Nodes.push_back(new GraphNode(position,i));*/
				//floyd = new Floyd(nodenbr);
			}

			/* Function to add a node according to a position */
			void Add_Node (float NodePos_X, float NodePos_Y)
			{
				VERTEX Node_Position;
				Node_Position.x = NodePos_X;
				Node_Position.y = NodePos_Y;

				/* Adding the node to the vector */
				Nodes.push_back(new GraphNode(Node_Position,NodeNbr));
				NodeNbr++;
			}
			/* Function to add a node according to a position */
			void Add_Node (float NodePos_X, float NodePos_Y, int nodeNbr)
			{
				VERTEX Node_Position;
				Node_Position.x = NodePos_X;
				Node_Position.y = NodePos_Y;

				/* Adding the node to the vector */
				Nodes.push_back(new GraphNode(Node_Position,nodeNbr));
				NodeNbr++;
			}
			
			void Link_Nodes (int N1_Idx, int N2_Idx)
			{
				Nodes[N1_Idx]->Neighbors.push_back(N2_Idx);
			}

			void Init_ShortestPath (void)
			{
				floyd = new Floyd(NodeNbr);
				fordmoor = new BellmanFordMoor(NodeNbr);
				dijkstra = new Dijkstra(NodeNbr);

				int neighborIdx=0;
				float Distance=0;

				for(int n=0; n<NodeNbr; n++)
				{
					for(unsigned int nbor=0; nbor<Nodes[n]->Neighbors.size(); nbor++)
					{
						neighborIdx = Nodes[n]->Neighbors[nbor];
						Distance = Nodes[n]->Position.GetDistance(Nodes[neighborIdx]->Position);

						floyd->DM[n][neighborIdx] = Distance;
						floyd->PM[n][neighborIdx]=floyd->d;
						
						//fordmoor->W[n][neighborIdx] = Distance;
						// fordmoor->PM[n][neighborIdx] = fordmoor->d;

						dijkstra->W[n][neighborIdx] = Distance;
						dijkstra->PM[n][neighborIdx] = dijkstra->d;
					}
				}

				/* Compute the path array according to the given DM */
				floyd->Compute_PathArray();
				//fordmoor->Compute_AllPaths();
				dijkstra->Compute_AllPaths();
				MainCharacter->GetSettings().Position=Nodes[0]->Position;
			}

			string Get_Path (int Source, int Destination)
			{
				switch(ShortestPathType)
				{
					case Floyd_Type:
						Path = floyd->Get_Path(Source,Destination);
						break;
					case BellmanFordMoor_Type:
					{
						int neighborIdx=0;
						float Distance=0;

						fordmoor->Init_Arrays();

						for(int n=0; n<NodeNbr; n++)
						{
							for(unsigned int nbor=0; nbor<Nodes[n]->Neighbors.size(); nbor++)
							{
								neighborIdx = Nodes[n]->Neighbors[nbor];
								Distance = Nodes[n]->Position.GetDistance(Nodes[neighborIdx]->Position);
								
								fordmoor->W[n][neighborIdx] = Distance;
								fordmoor->PM[n][neighborIdx] = fordmoor->d;								
							}
						}
						Path = fordmoor->Get_Path(Source,Destination);
					}
					break;
					case Dijkstra_Type:
						Path = dijkstra->Get_Path(Source,Destination);
						break;
				}
				
				//MainCharacter->GetSettings().Position=Nodes[Source]->Position;
				CurrentNode=0;
				Traversing_Path=true;

				if(Source==Destination || Source_TrapIdx==Destination_TrapIdx)
				{
					Path.clear();
					Path.push_back(Source_TrapIdx);
					Path.push_back(Source_TrapIdx);
				}

				char arr[100];
				string result;
				for(unsigned int i=0; i<Path.size()-1; i++)
				{
					sprintf(arr,"%i - ", Path[i]);
					result+=arr;
					arr[0]='\0';
				}
				sprintf(arr,"%i", Path[i]);
				result+=arr;

				/* if the user chose 2 different nodes */
				if(Source!=Destination && Source_TrapIdx!=Destination_TrapIdx)
					External_Path_S=true;
				/* If the user chose 2 points belonging to the same node */
				else
				{
					/* Set the properties for the last interpolation */
					External_Path_S=false;
					External_Path_D=true;
					/* Set the interpolation factor */
					Interpolate_Factor=INTERP_FACT;
					
					/* Compute the vector direction of the path to follow */
					VectDir.x = destVtx.x - MainCharacter->GetSettings().Position.x;
					VectDir.y = destVtx.y - MainCharacter->GetSettings().Position.y;
					/* Compute the path's length */
					float Magnitude = sqrtf(VectDir.x*VectDir.x + VectDir.y*VectDir.y);
					/* Normalize the vector direction of the path */
					VectDir.x/=Magnitude;
					VectDir.y/=Magnitude;
					/* Set the step factor according to the interpolation factor */
					Step = Magnitude/Interpolate_Factor;
			
					/* Reset the path array */
					Path.clear();
				}

				return result;
			}

			/* Function to get the nearest node to the cursor position */
			int Get_NearestNode (int source, Vertex pt)
			{
				/* Init the index of the nearest node as the source (node center of trap) */
				int Nearest=source;
				/* Init the min distance as the distance from the mouse pos --> node of the trapezoid */
				float min_dist=pt.GetDistance(Nodes[source]->Position);
				float cur_dist=0;

				/* Loop through the neighbors of the source index */
				for(unsigned int i=0; i<Nodes[source]->Neighbors.size(); i++)
				{
					/* Compute the current distance from the current node to the point */
					cur_dist = pt.GetDistance(Nodes[Nodes[source]->Neighbors[i]]->Position);
					/* if it is less than the min --> save the node's index with the min_dist */
					if(cur_dist<min_dist)
					{
						min_dist=cur_dist;
						Nearest=Nodes[source]->Neighbors[i];
					}
				}
				return Nearest;
			}

			/* Function to traverse the path while interpolating */
			void Traverse_Path (void)
			{
				/* If we are traversing the path */
				if(Traversing_Path)
				{
					if(External_Path_S && !Interpolate_Factor)
					{
						External_Path_S=false;
						/* Set the interpolation factor */
						Interpolate_Factor=INTERP_FACT;
						
						/* Compute the vector direction of the path to follow */
						GraphNode *tmp = Nodes[Source];
						VectDir.x = Nodes[Source]->Position.x - MainCharacter->GetSettings().Position.x;
						VectDir.y = Nodes[Source]->Position.y - MainCharacter->GetSettings().Position.y;
						/* Compute the path's length */
						float Magnitude = sqrtf(VectDir.x*VectDir.x + VectDir.y*VectDir.y);
						/* Normalize the vector direction of the path */
						VectDir.x/=Magnitude;
						VectDir.y/=Magnitude;
						/* Set the step factor according to the interpolation factor */
						Step = Magnitude/Interpolate_Factor;
					}
					/* If the index in the Path array is less than the path array size && the interpolation of the prev node has ended */
					if(CurrentNode<(int)Path.size() && !Interpolate_Factor && !External_Path_D)
					{
						/* Take the next index */
						int n2=CurrentNode+1;
						/* Check if the next index is in the path array */
						if(n2<(int)Path.size())
						{
							/* Set the interpolation factor */
							Interpolate_Factor=INTERP_FACT;
							/* Update the index in the path array */
							CurrentNode++;

							/* Compute the vector direction of the path to follow */
							VectDir.x = Nodes[Path[CurrentNode]]->Position.x - MainCharacter->GetSettings().Position.x;
							VectDir.y = Nodes[Path[CurrentNode]]->Position.y - MainCharacter->GetSettings().Position.y;
							/* Compute the path's length */
							float Magnitude = sqrtf(VectDir.x*VectDir.x + VectDir.y*VectDir.y);
							/* Normalize the vector direction of the path */
							VectDir.x/=Magnitude;
							VectDir.y/=Magnitude;
							/* Set the step factor according to the interpolation factor */
							Step = Magnitude/Interpolate_Factor;
						}
						else
						{
							/* we have finished the previous interpolation */
							External_Path_D=true;
							/* Set the interpolation factor */
							Interpolate_Factor=INTERP_FACT;
							
							/* Compute the vector direction of the path to follow */
							VectDir.x = destVtx.x - MainCharacter->GetSettings().Position.x;
							VectDir.y = destVtx.y - MainCharacter->GetSettings().Position.y;
							/* Compute the path's length */
							float Magnitude = sqrtf(VectDir.x*VectDir.x + VectDir.y*VectDir.y);
							/* Normalize the vector direction of the path */
							VectDir.x/=Magnitude;
							VectDir.y/=Magnitude;
							/* Set the step factor according to the interpolation factor */
							Step = Magnitude/Interpolate_Factor;
					
							/* Reset the path array */
							Path.clear();
						}
					}
					else if(External_Path_D && !Interpolate_Factor)
					{
						External_Path_D=false;
						Traversing_Path=false;
					}
					else
						Interpolate_Path();
				}
			}

			/* Function to interpolate the character's position according to the node */
			void Interpolate_Path (void)
			{
				/* Update the position with the vector multiplied with the step factor */
				MainCharacter->GetSettings().Position.x += VectDir.x*Step;
				MainCharacter->GetSettings().Position.y += VectDir.y*Step;
				Interpolate_Factor--;
			}

			/* Function to draw the graph with its content */
			void Draw (ViewPort *ViewPort_List)
			{
				Matrix<3> Mapping_Matrix;
				Point_Float V[2];
				
				/* Building the viewing matrix */
				Viewing_Transformation(ViewPort_List, Mapping_Matrix);

				/* Draw all the nodes */
				for(int gn=0; gn<NodeNbr; gn++)
					Nodes[gn]->Draw(ViewPort_List);

				/* Draw the edges */
				for(int gn=0; gn<NodeNbr; gn++)
				{
					/* Loop through the neighbors of the current node */
					for(unsigned int nbor=0; nbor<Nodes[gn]->Neighbors.size(); nbor++)
					{
						/* Transform the vertices to the screen */
						V[0].x = Nodes[gn]->Position.x/*+40*NodeShape->GetSettings().Scale*/;	V[1].x = Nodes[Nodes[gn]->Neighbors[nbor]]->Position.x-0*NodeShape->GetSettings().Scale;
						V[0].y = Nodes[gn]->Position.y-20*NodeShape->GetSettings().Scale;	V[1].y = Nodes[Nodes[gn]->Neighbors[nbor]]->Position.y-10*NodeShape->GetSettings().Scale;

						V[0] = Mapping_Matrix.M*V[0];
						V[1] = Mapping_Matrix.M*V[1];

						/* Draw the edge */
						LINE(V[0].x,V[0].y,V[1].x,V[1].y,200,0,0);
					}
				}
			}

			/* Function to display the node indices */
			void DisplayNodeIndices (ViewPort *ViewPort_List)
			{
				/* Loop through the nodes */
				for(int gn=0; gn<NodeNbr; gn++)
					/* Display the node's index */
					Nodes[gn]->DisplayIndex(ViewPort_List);
			}

};

#endif