#include "Bsp.h"

#define N1 0
#define N2 1
#define N3 2

/* Function to modify the loader to add new vertices */
void Modify_LoaderVertices (_3DSLoader *Loader, Vertex *New_VtxList, UV *New_UVList, Point3D *New_VtxNorList, int Numb)
{
	/* Allocating memory for temporary variables to modify the array of vertices */
	Vertex *Temp = new Vertex [Loader->numberOfVertices+Numb];
	/* The array of texture coodinates */
	UV *Temp_UV = new UV [Loader->numberOfTextureCoordinates+Numb];
	/* The array of vertex normals */
	Point3D *Temp_NorList = new Point3D [Loader->numberOfVertices+Numb];

	/* Copy from the loader to the temporary variables */
	memcpy(Temp,Loader->vertices,Loader->numberOfVertices*sizeof(Vertex));
	memcpy(Temp_UV,Loader->vertexTextureCoordinates,Loader->numberOfTextureCoordinates*sizeof(UV));
	memcpy(Temp_NorList,Loader->vertexNormals,Loader->numberOfVertices*sizeof(Point3D));
	
	/* Adding the new vertices */
	for(int i=0; i<Numb; i++)
	{
		Temp[Loader->numberOfVertices+i]=New_VtxList[i];
		Temp_UV[Loader->numberOfTextureCoordinates+i]=New_UVList[i];
		Temp_NorList[Loader->numberOfVertices+i]=New_VtxNorList[i];
	}

	/* Update the loader with the temporary variables */
	delete [] Loader->vertices;
	Loader->vertices=Temp;
	delete [] Loader->vertexTextureCoordinates;
	Loader->vertexTextureCoordinates=Temp_UV;
	delete [] Loader->vertexNormals;
	Loader->vertexNormals=Temp_NorList;
	/* Update the number of vertices and number of texture coordinates */
	Loader->numberOfVertices+=Numb;
	Loader->numberOfTextureCoordinates+=Numb;
}

/* Function to modify the loader to add new faces */
void Modify_LoaderFaces (_3DSLoader *Loader, Face *Remove, Face *F0, Face *F1, Face *F2, int Numb/* (1: for 2 new faces) - (2: for 3 new faces)*/)
{
	/* Get the number of indices */
	int VtxIdxNumb = Loader->numberOfFaces*3;
	/* Allocating memory for temporary variables to modify the array of vertexindex */
	unsigned short *Temp = new unsigned short [VtxIdxNumb+ 3*Numb];

	/* Copy from the loader to the temporary variable */
	memcpy(Temp,Loader->vertexIndices,VtxIdxNumb*sizeof(unsigned short));

	///* Removing the splitted face */
	//int i=0,j=0;
	//for(j=i=Remove->Face_Index*3, j+=3; j<VtxIdxNumb; i++,j++)
	//	Temp[i]=Temp[j];

	/* Adding the 1st new face */
	Temp[VtxIdxNumb - 3]=F0->index[0];
	Temp[VtxIdxNumb - 2]=F0->index[1];
	Temp[VtxIdxNumb - 1]=F0->index[2];
	/* Adding the 2nd new face */
	Temp[VtxIdxNumb	   ]=F1->index[0];
	Temp[VtxIdxNumb + 1]=F1->index[1];
	Temp[VtxIdxNumb + 2]=F1->index[2];
	/* If we are adding 2 new faces */
	if(Numb==2)
	{
		/* Add the 3rd new face */
		Temp[VtxIdxNumb + 3]=F2->index[0];
		Temp[VtxIdxNumb + 4]=F2->index[1];
		Temp[VtxIdxNumb + 5]=F2->index[2];
	}
	
	/* Update the loader */
	delete [] Loader->vertexIndices;
	Loader->vertexIndices=Temp;
	/* Update the number of faces */
	Loader->numberOfFaces+=Numb;
}

inline void Swap (unsigned short &i, unsigned short &j)
{
	i += j;
	j = i-j;
	i -= j;
}

void BSP_Tree::CheckCase_And_ModifyLoader (list<Face *> &frontList, list<Face *> &backList, Face *f, Face *Splitter, CLASSIFY *Vertices_Classifications, int &Front_Number, int &Back_Number)
{
	Vertex *New_VtxList;
	UV *New_UVList;
	Point3D *New_VtxNorList;
	Face *F0, *F1, *F2;
	float time1, time2;
	int Top, Middle, Bottom;

	Plane3D Splitter_Pln(Plane3D(_p3DSLoader->vertices[Splitter->index[0]],Splitter->faceNormal));

	/* 3 triangles --> 2 Front - 1 Back (12 cases) */
	if(Front_Number>Back_Number)
	{
		New_VtxList = new Vertex[2];
		New_UVList = new UV[2];
		New_VtxNorList = new Point3D[2];
		F0 = new Face;
		F1 = new Face;
		F2 = new Face;

		if(Vertices_Classifications[0]==BACK)
		{
			Bottom=0;
			Top=1;	
			Middle=2;
		}
		else if(Vertices_Classifications[1]==BACK)
		{
			Bottom=1;
			Top=2;	
			Middle=0;
		}
		else	//2=BACK
		{
			Bottom=2;
			Top=0;	
			Middle=1;
		}

		/* Get the intesection vertices */
		New_VtxList[0] = Intersect(_p3DSLoader->vertices[f->index[Middle]],_p3DSLoader->vertices[f->index[Bottom]],Splitter,time1);
		New_VtxList[1] = Intersect(_p3DSLoader->vertices[f->index[Top]],_p3DSLoader->vertices[f->index[Bottom]],Splitter,time2);
		/* Get the intersection vertices texture coordinates */
		New_UVList[0].U = _p3DSLoader->vertexTextureCoordinates[f->index[Middle]].U + (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].U - _p3DSLoader->vertexTextureCoordinates[f->index[Middle]].U)*time1;
		New_UVList[0].V = _p3DSLoader->vertexTextureCoordinates[f->index[Middle]].V + (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].V - _p3DSLoader->vertexTextureCoordinates[f->index[Middle]].V)*time1;
		New_UVList[1].U = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U	+ (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].U - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U)*time2;
		New_UVList[1].V = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V	+ (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].V - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V)*time2;
		/* Get the intersection vertices normal buffer */
		New_VtxNorList[0].X = _p3DSLoader->vertexNormals[f->index[Middle]].X		+ (_p3DSLoader->vertexNormals[f->index[Bottom]].X - _p3DSLoader->vertexNormals[f->index[Middle]].X)*time1;
		New_VtxNorList[0].Y = _p3DSLoader->vertexNormals[f->index[Middle]].Y		+ (_p3DSLoader->vertexNormals[f->index[Bottom]].Y - _p3DSLoader->vertexNormals[f->index[Middle]].Y)*time1;
		New_VtxNorList[0].Z = _p3DSLoader->vertexNormals[f->index[Middle]].Z		+ (_p3DSLoader->vertexNormals[f->index[Bottom]].Z - _p3DSLoader->vertexNormals[f->index[Middle]].Z)*time1;
		New_VtxNorList[1].X = _p3DSLoader->vertexNormals[f->index[Top]].X			+ (_p3DSLoader->vertexNormals[f->index[Bottom]].X - _p3DSLoader->vertexNormals[f->index[Top]].X)*time2;
		New_VtxNorList[1].Y = _p3DSLoader->vertexNormals[f->index[Top]].Y			+ (_p3DSLoader->vertexNormals[f->index[Bottom]].Y - _p3DSLoader->vertexNormals[f->index[Top]].Y)*time2;
		New_VtxNorList[1].Z = _p3DSLoader->vertexNormals[f->index[Top]].Z			+ (_p3DSLoader->vertexNormals[f->index[Bottom]].Z - _p3DSLoader->vertexNormals[f->index[Top]].Z)*time2;

		/* Create 3 new faces */
		/* 1st new face */
		F0->Face_Index = _p3DSLoader->numberOfFaces+N1;
		F0->objectID = f->objectID;
		F0->index[0] = f->index[Top];
		F0->index[1] = f->index[Middle];
		F0->index[2] = _p3DSLoader->numberOfVertices+N2;
		/* 2nd new face */
		F1->Face_Index = _p3DSLoader->numberOfFaces+N2;
		F1->objectID = f->objectID;
		F1->index[0] = _p3DSLoader->numberOfVertices+N2;
		F1->index[1] = f->index[Middle];
		F1->index[2] = _p3DSLoader->numberOfVertices+N1;
		/* 3rd new face */
		F2->Face_Index = _p3DSLoader->numberOfFaces+N3;
		F2->objectID = f->objectID;
		F2->index[0] = _p3DSLoader->numberOfVertices+N1;
		F2->index[1] = f->index[Bottom];
		F2->index[2] = _p3DSLoader->numberOfVertices+N2;

		/* Modify the loader for the changes */
		Modify_LoaderVertices(_p3DSLoader,New_VtxList,New_UVList,New_VtxNorList,2);
		Modify_LoaderFaces(_p3DSLoader,f,F0,F1,F2,2);
		
		/* Compute the normal of each new face */
		Compute_FaceNormal(F0);
		/* Check if the normal has the same direction 
		   of the normal of the parent, if not --> swap(1,2) */
		if(F0->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F0->index[1],F0->index[2]);
		F0->faceNormal=f->faceNormal;
		Compute_FaceNormal(F1);
		if(F1->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F1->index[1],F1->index[2]);
		F1->faceNormal=f->faceNormal;
		Compute_FaceNormal(F2);
		if(F2->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F2->index[1],F2->index[2]);
		F2->faceNormal=f->faceNormal;

		/* Add 2 Front Faces and 1 back faces */
		frontList.push_back(F0);
		frontList.push_back(F1);
		backList.push_back(F2);
	}
	/* 3 triangles --> 2 Back - 1 Front (12 Cases) */
	else if(Back_Number>Front_Number)
	{
		New_VtxList = new Vertex[2];
		New_UVList = new UV[2];
		New_VtxNorList = new Point3D[2];
		F0 = new Face;
		F1 = new Face;
		F2 = new Face;

		if(Vertices_Classifications[0]==FRONT)
		{
			Top=0;
			Bottom=1;
			Middle=2;
		}
		else if(Vertices_Classifications[1]==FRONT)
		{
			Top=1;
			Bottom=2;	
			Middle=0;
		}
		else	//2=FRONT
		{
			Top=2;
			Bottom=0;	
			Middle=1;
		}
	
		/* Get the intesection vertices */
		New_VtxList[0] = Intersect(_p3DSLoader->vertices[f->index[Top]],_p3DSLoader->vertices[f->index[Middle]],Splitter,time1);
		New_VtxList[1] = Intersect(_p3DSLoader->vertices[f->index[Top]],_p3DSLoader->vertices[f->index[Bottom]],Splitter,time2);
		/* Get the intersection vertices texture coordinates */
		New_UVList[0].U = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U + (_p3DSLoader->vertexTextureCoordinates[f->index[Middle]].U - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U)*time1;
		New_UVList[0].V = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V + (_p3DSLoader->vertexTextureCoordinates[f->index[Middle]].V - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V)*time1;
		New_UVList[1].U = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U + (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].U - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U)*time2;
		New_UVList[1].V = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V + (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].V - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V)*time2;
		/* Get the intersection vertices normal buffer */
		New_VtxNorList[0].X = _p3DSLoader->vertexNormals[f->index[Top]].X + (_p3DSLoader->vertexNormals[f->index[Middle]].X - _p3DSLoader->vertexNormals[f->index[Top]].X)*time1;
		New_VtxNorList[0].Y = _p3DSLoader->vertexNormals[f->index[Top]].Y + (_p3DSLoader->vertexNormals[f->index[Middle]].Y - _p3DSLoader->vertexNormals[f->index[Top]].Y)*time1;
		New_VtxNorList[0].Z = _p3DSLoader->vertexNormals[f->index[Top]].Z + (_p3DSLoader->vertexNormals[f->index[Middle]].Z - _p3DSLoader->vertexNormals[f->index[Top]].Z)*time1;
		New_VtxNorList[1].X = _p3DSLoader->vertexNormals[f->index[Top]].X + (_p3DSLoader->vertexNormals[f->index[Bottom]].X - _p3DSLoader->vertexNormals[f->index[Top]].X)*time2;
		New_VtxNorList[1].Y = _p3DSLoader->vertexNormals[f->index[Top]].Y + (_p3DSLoader->vertexNormals[f->index[Bottom]].Y - _p3DSLoader->vertexNormals[f->index[Top]].Y)*time2;
		New_VtxNorList[1].Z = _p3DSLoader->vertexNormals[f->index[Top]].Z + (_p3DSLoader->vertexNormals[f->index[Bottom]].Z - _p3DSLoader->vertexNormals[f->index[Top]].Z)*time2;
		
		/* Create 3 new faces */
		/* 1st new face */
		F0->Face_Index = _p3DSLoader->numberOfFaces+N1;
		F0->objectID = f->objectID;
		F0->index[0] = f->index[Top];
		F0->index[1] = _p3DSLoader->numberOfVertices+N1;
		F0->index[2] = _p3DSLoader->numberOfVertices+N2;
		/* 2nd new face */
		F1->Face_Index = _p3DSLoader->numberOfFaces+N2;
		F1->objectID = f->objectID;
		F1->index[0] = _p3DSLoader->numberOfVertices+N2;
		F1->index[1] = _p3DSLoader->numberOfVertices+N1;
		F1->index[2] = f->index[Bottom];
		/* 3rd new face */
		F2->Face_Index = _p3DSLoader->numberOfFaces+N3;
		F2->objectID = f->objectID;
		F2->index[0] = f->index[Bottom];
		F2->index[1] = _p3DSLoader->numberOfVertices+N1;
		F2->index[2] = f->index[Middle];

		/* Modify the loader for the changes */
		Modify_LoaderVertices(_p3DSLoader,New_VtxList,New_UVList,New_VtxNorList,2);
		Modify_LoaderFaces(_p3DSLoader,f,F0,F1,F2,2);
		
		/* Compute the normal of each new face */
		Compute_FaceNormal(F0);
		/* Check if the normal has the same direction 
		   of the normal of the parent, if not --> swap(1,2) */
		if(F0->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F0->index[1],F0->index[2]);
		F0->faceNormal=f->faceNormal;
		Compute_FaceNormal(F1);
		if(F1->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F1->index[1],F1->index[2]);
		F1->faceNormal=f->faceNormal;
		Compute_FaceNormal(F2);
		if(F2->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F2->index[1],F2->index[2]);
		F2->faceNormal=f->faceNormal;

		/* Add 1 front face and 2 Back faces */
		frontList.push_back(F0);
		backList.push_back(F1);
		backList.push_back(F2);
	}
	/* 2 triangles --> 1 Front - 1 Back (12 Cases) */
	else
	{
		New_VtxList = new Vertex[1];
		New_UVList = new UV[1];
		New_VtxNorList = new Point3D[1];

		F0 = new Face;
		F1 = new Face;

		if(Vertices_Classifications[0]==INTERSECT)
		{
			Middle=0;
			Bottom=1;
			Top=2;
		}
		else if(Vertices_Classifications[1]==INTERSECT)
		{
			Middle=1;
			Bottom=2;
			Top=0;
		}
		else	//2 = INTERSECT
		{
			Middle=2;
			Bottom=0;
			Top=1;
		}
		
		/* Get the intesection vertex */
		New_VtxList[0] = Intersect(_p3DSLoader->vertices[f->index[Top]],_p3DSLoader->vertices[f->index[Bottom]],Splitter,time1);
		/* Get the intersection vertex texture coordinates */
		New_UVList[0].U = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U + (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].U - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].U)*time1;
		New_UVList[0].V = _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V + (_p3DSLoader->vertexTextureCoordinates[f->index[Bottom]].V - _p3DSLoader->vertexTextureCoordinates[f->index[Top]].V)*time1;
		/* Get the intersection vertices normal buffer */
		New_VtxNorList[0].X = _p3DSLoader->vertexNormals[f->index[Top]].X + (_p3DSLoader->vertexNormals[f->index[Bottom]].X - _p3DSLoader->vertexNormals[f->index[Top]].X)*time1;
		New_VtxNorList[0].Y = _p3DSLoader->vertexNormals[f->index[Top]].Y + (_p3DSLoader->vertexNormals[f->index[Bottom]].Y - _p3DSLoader->vertexNormals[f->index[Top]].Y)*time1;
		New_VtxNorList[0].Z = _p3DSLoader->vertexNormals[f->index[Top]].Z + (_p3DSLoader->vertexNormals[f->index[Bottom]].Z - _p3DSLoader->vertexNormals[f->index[Top]].Z)*time1;

		/* Create 2 new faces */
		/* 1st new face */
		F0->Face_Index = _p3DSLoader->numberOfFaces+N1;
		F0->objectID = f->objectID;
		F0->index[0] = f->index[Top];
		F0->index[1] = f->index[Middle];
		F0->index[2] = _p3DSLoader->numberOfVertices+N1;
		/* 2nd new face */
		F1->Face_Index = _p3DSLoader->numberOfFaces+N2;
		F1->objectID = f->objectID;
		F1->index[0] = _p3DSLoader->numberOfVertices+N1;
		F1->index[1] = f->index[Middle];
		F1->index[2] = f->index[Bottom];
		
		/* Modify the loader for the changes */
		Modify_LoaderVertices(_p3DSLoader,New_VtxList,New_UVList,New_VtxNorList,1);
		Modify_LoaderFaces(_p3DSLoader,f,F0,F1,NULL,1);
		
		/* Compute the normal of each new face */
		Compute_FaceNormal(F0);
		/* Check if the normal has the same direction 
		   of the normal of the parent, if not --> swap(1,2) */
		if(F0->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F0->index[1],F0->index[2]);
		F0->faceNormal=f->faceNormal;
		Compute_FaceNormal(F1);
		if(F1->faceNormal.GetDotProduct(f->faceNormal)<0)
			Swap(F1->index[1],F1->index[2]);
		F1->faceNormal=f->faceNormal;

		/* Add 1 Front and 1 Back */
		/* if Vtx1 is in the front --> add to frontlist F0 & add to backlist F1 
		   (to include the case that the splitter can be to the front or back)*/
		if(Vertices_Classifications[Top]==FRONT)
		{
			frontList.push_back(F0);
			backList.push_back(F1);
		}
		else
		{
			frontList.push_back(F1);
			backList.push_back(F0);
		}
	}
	delete [] New_VtxList;
	delete [] New_UVList;
	delete [] New_VtxNorList;
}