#include "Octree.h"
#include "Camera.h"
#include "Support_Functions.h"
#include <GL/gl.h>
#include <GL/glu.h>

#define MAX_FACEPEROCTANT 50
#define MAX_SUBDIVISIONS 2
#define OctreeWorld_Offset -250

int NumLeafOctant=0;
int Current_Subdivision=0;
extern Camera *Main_Camera;

float Absf (float Value)
{
	return (Value<0)?-Value:Value;
}

/* Octree Default Constructor */
Octree::Octree (void)
{
	/* Initializing all the members */
	_leaf=true;
	_width=0;
	_objectCount=0;
	_center.X=_center.Y=_center.Z=0;
	_pLeafIdxVtx=_pFacesPerObject=_pObjectIndex=NULL;
	
	/* Init the children octants */
	for(int i=0; i<8; i++)
		_pOctreeOctants[i]=NULL;
}

/* Octree Default Constructor */
Octree::Octree (_3DSLoader *loader, Renderer * renderer) : _p3DSLoader(loader), _pRenderer(renderer)
{
	/* Initializing all the members */
	_leaf=true;
	_width=0;
	_objectCount=0;
	_center.X=_center.Y=_center.Z=0;
	_pLeafIdxVtx=_pFacesPerObject=_pObjectIndex=NULL;
	
	/* Init the children octants */
	for(int i=0; i<8; i++)
		_pOctreeOctants[i]=NULL;

	/* Create the array of vertices to store the transformed vertices */
	TransformedVertices = new Vertex [loader->numberOfVertices];
}

/* Octree Destructor */
Octree::~Octree (void)
{
	/* Delete the memory allocated for the Index Vertex List */
	if(_pLeafIdxVtx)
	{
		delete [] _pLeafIdxVtx;
		_pLeafIdxVtx=NULL;
	}
	/* Delete the memory allocated for the Faces per object List */
	if(_pFacesPerObject)
	{
		delete [] _pFacesPerObject;
		_pFacesPerObject=NULL;
	}
	/* Delete the memory allocated for the Object Index List */
	if(_pObjectIndex)
	{
		delete [] _pObjectIndex;
		_pObjectIndex=NULL;
	}

	/* Looping through the 8 octants to delete them recursively */
	for(int i=0; i<8; i++)
	{
		if(_pOctreeOctants[i])
		{
			delete _pOctreeOctants[i];
			_pOctreeOctants[i]=NULL;
		}
	}
}

/* Function to get the width of an octant */
float Octree::GetWidth (void) { return _width; }

/* Function to get the center of an octant */
Vertex Octree::GetCenter (void) { return _center; }

/* Function to specify whether the octant is a leaf or not */
bool Octree::IsLeaf (void) { return _leaf; }

/* Function to set the renderer */
void Octree::SetRenderer(Renderer *pr) { _pRenderer = pr; }

/* Function to set the Loader */
void Octree::Set3DSLoader(_3DSLoader *p3DSLoader) { _p3DSLoader=p3DSLoader; }

/* Function to compute the width and center of the octant */
void Octree::GetWorldDimensions (float Width)
{
	float X_Av=0, Y_Av=0, Z_Av=0;
	/* Getting the average of the 3 axis */
	for(int i=0; i<_p3DSLoader->numberOfVertices; i++)
	{
		X_Av+=_p3DSLoader->vertices[i].X;
		Y_Av+=_p3DSLoader->vertices[i].Y;
		Z_Av+=_p3DSLoader->vertices[i].Z;
	}
	/* Computing the Center coordinates */
	_center.X = X_Av/_p3DSLoader->numberOfVertices;
	_center.Y = Y_Av/_p3DSLoader->numberOfVertices;
	_center.Z = Z_Av/_p3DSLoader->numberOfVertices;

	/* Translating the center negatively in the Y-Direction */
	_center.Y += OctreeWorld_Offset;

	Point3D C(_center.X,_center.Y,_center.Z);
	/* Computing the Half Width */
	float Max_X, Max_Y, Max_Z;
	Max_X=Max_Y=Max_Z=-1;
	float d=0;
	for(int i=0; i<_p3DSLoader->numberOfVertices; i++)
	{
		/* Getting the Distance from the vertex X coordinate to the Center X coordinate */
		d = fabsf(_p3DSLoader->vertices[i].X-_center.X);
		Max_X = (d>Max_X)?d:Max_X;
		/* Getting the Distance from the vertex Y coordinate to the Center Y coordinate */
		d = fabsf(_p3DSLoader->vertices[i].Y-_center.Y);
		Max_Y = (d>Max_Y)?d:Max_Y;
		/* Getting the Distance from the vertex Z coordinate to the Center Z coordinate */
		d = fabsf(_p3DSLoader->vertices[i].Z-_center.Z);
		Max_Z = (d>Max_Z)?d:Max_Z;
	}
	/* Getting the largest distance from the 3 axis */
	_width = Max_X;
	_width = (Max_Y>_width)?Max_Y:_width;
	_width = (Max_Z>_width)?Max_Z:_width;
	/* Getting the Width */
	_width*=2;
	_width=Width;
}

/* Function to check whether a vertex is in an octant */
bool VertexInOctant (int Octant_ID, Vertex Center, Vertex V)
{
	switch(Octant_ID)
	{
		///////////////// Front ////////////////
		/* Top - Left - Front */
		case Oct_FrontTopLeft:
				if(V.X<=Center.X && V.Y>=Center.Y && V.Z>=Center.Z)
					return true;
				break;
		/* Top - Right - Front */
		case Oct_FrontTopRight:
				if(V.X>=Center.X && V.Y>=Center.Y && V.Z>=Center.Z)
					return true;
				break;
		/* Bottom - Right - Front */
		case Oct_FrontBottomRight:
				if(V.X>=Center.X && V.Y<=Center.Y && V.Z>=Center.Z)
					return true;
				break;
		/* Bottom - Left - Front */
		case Oct_FrontBottomLeft:
				if(V.X<=Center.X && V.Y<=Center.Y && V.Z>=Center.Z)
					return true;
				break;
		///////////////// Back ////////////////
		/* Top - Left - Back */
		case Oct_BackTopLeft:
				if(V.X<=Center.X && V.Y>=Center.Y && V.Z<=Center.Z)
					return true;
				break;
		/* Top - Right - Back */
		case Oct_BackTopRoght:
				if(V.X>=Center.X && V.Y>=Center.Y && V.Z<=Center.Z)
					return true;
				break;
		/* Bottom - Right - Back */
		case Oct_BackBottomRight:
				if(V.X>=Center.X && V.Y<=Center.Y && V.Z<=Center.Z)
					return true;
				break;
		/* Bottom - Left - Back */
		case Oct_BackBottomLeft:
				if(V.X<=Center.X && V.Y<=Center.Y && V.Z<=Center.Z)
					return true;
				break;
	}
	/* Vertex is not in the octant */
	return false;
}

/* Function to compute the center of an octant according to its parent */
Vertex ComputeOctantCenter (int Octant_ID, Vertex Center, float Width)
{
	Vertex NewCenter;
	float QuarterWidth = Width/4;
	switch(Octant_ID)
	{
		///////////////// Front ////////////////
		/* Top - Left - Front */
		case Oct_FrontTopLeft:
				NewCenter.X = Center.X - QuarterWidth;
				NewCenter.Y = Center.Y + QuarterWidth;
				NewCenter.Z = Center.Z + QuarterWidth;
				break;
		/* Top - Right - Front */
		case Oct_FrontTopRight:
				NewCenter.X = Center.X + QuarterWidth;
				NewCenter.Y = Center.Y + QuarterWidth;
				NewCenter.Z = Center.Z + QuarterWidth;
				break;
		/* Bottom - Right - Front */
		case Oct_FrontBottomRight:
				NewCenter.X = Center.X + QuarterWidth;
				NewCenter.Y = Center.Y - QuarterWidth;
				NewCenter.Z = Center.Z + QuarterWidth;
				break;
		/* Bottom - Left - Front */
		case Oct_FrontBottomLeft:
				NewCenter.X = Center.X - QuarterWidth;
				NewCenter.Y = Center.Y - QuarterWidth;
				NewCenter.Z = Center.Z + QuarterWidth;
				break;
		///////////////// Back ////////////////
		/* Top - Left - Back */
		case Oct_BackTopLeft:
				NewCenter.X = Center.X - QuarterWidth;
				NewCenter.Y = Center.Y + QuarterWidth;
				NewCenter.Z = Center.Z - QuarterWidth;
				break;
		/* Top - Right - Back */
		case Oct_BackTopRoght:
				NewCenter.X = Center.X + QuarterWidth;
				NewCenter.Y = Center.Y + QuarterWidth;
				NewCenter.Z = Center.Z - QuarterWidth;
				break;
		/* Bottom - Right - Back */
		case Oct_BackBottomRight:
				NewCenter.X = Center.X + QuarterWidth;
				NewCenter.Y = Center.Y - QuarterWidth;
				NewCenter.Z = Center.Z - QuarterWidth;
				break;
		/* Bottom - Left - Back */
		case Oct_BackBottomLeft:
				NewCenter.X = Center.X - QuarterWidth;
				NewCenter.Y = Center.Y - QuarterWidth;
				NewCenter.Z = Center.Z - QuarterWidth;
				break;
	}
	/* Return the Coordinates of the new center */
	return NewCenter;
}

void Octree::CreateOctant  (unsigned short * pVtxIdx, int idxCount, unsigned short *pFacesPerObject, unsigned short *pObjectIndex, int objectCount, Vertex center, float width)
{
	int numberOfFaces = idxCount/3;
	if(numberOfFaces>MAX_FACEPEROCTANT && Current_Subdivision<MAX_SUBDIVISIONS)
	{
		_leaf=false;
		/* Loop through the octants in the current octant */
		for(int oct=0; oct<8; oct++)
		{
			/* List to tag the available faces in the current octant */
			vector<bool> pListVtxIdx(numberOfFaces);
			/* List to tag the available objects in the current octant */
			vector<bool> pListObject(objectCount);
			/* Object Offset */
			int objectOffset=0;
			/* Face and Object Number in the octant */
			int OctantFaceCount=0;
			int OctantObjectCount=0;

			/* For each object */
			for(int obj=0; obj<objectCount; obj++)
			{
				/* For each vertex --> nbr of vertices = number of faces per object * 3 */
				for(int obj_vtx=0; obj_vtx<pFacesPerObject[obj]*3; obj_vtx++)
				{
					/* Getting the current vertex */
					Vertex V = _p3DSLoader->vertices[pVtxIdx[obj_vtx+objectOffset]];
					/* if the vertex lies in this current octant */
					if(VertexInOctant(oct,center,V))
					{
						/* The face containing the current vertex is in the octant */
						pListVtxIdx[(obj_vtx+objectOffset)/3]=true;
						/* The Object containing the current face is ih the octant */
						pListObject[obj]=true;
					}
				}
				/* Update the offset */
				objectOffset += pFacesPerObject[obj]*3;
			}

			/* Counting the Faces in the current Octant */
			for(int F=0; F<numberOfFaces; F++)
				if(pListVtxIdx[F])
					OctantFaceCount++;
			/* Counting the Objects in the current Octant */
			for(int O=0; O<objectCount; O++)
				if(pListObject[O])
					OctantObjectCount++;

			/* If there exist any face in the octant */
			if(OctantFaceCount)
			{
				/* Create a new octant vertex list */
				unsigned short * pOctantVtxIdx = new unsigned short [OctantFaceCount*3];
				/* Create a new list of face count per object */
				unsigned short * pOctantFacesPerObject = new unsigned short [OctantObjectCount];
				/* Create a new object index list */
				unsigned short * pOctantObjectIndex = new unsigned short [OctantObjectCount];
				
				/* Filling the Object Index List */
				int ObjectIndex=0;
				/* Looping through all the objects in the current octant */
				for(int obj=0; obj<objectCount; obj++)
				{
					/* If the current object exists */
					if(pListObject[obj])
					{
						/* Take its index and put it in the current
						   position (ObjectIndex) */
						pOctantObjectIndex[ObjectIndex]=pObjectIndex[obj];
						/* Init the number of faces in the current object */
						pOctantFacesPerObject[ObjectIndex]=0;
						/* Update the Object Index */
						ObjectIndex++;
					}
				}

				/* Filling the Faces per object list */
				int FaceIndex=0;
				int Index_ListVtxIdx=0;
				int FaceObjectIndex=0, FaceObjectIndex2=0;
				int OldFaceCount=0;
				bool takeit=false;
				int numberOfFacesTaken=0;

				while(1)
				{
					if(FaceIndex==(pFacesPerObject[FaceObjectIndex2] + OldFaceCount))
					{
						OldFaceCount += pFacesPerObject[FaceObjectIndex2];
						if(takeit)
						{
							/* Update the number of faces for each object according to the number of faces taken */
							pOctantFacesPerObject[FaceObjectIndex] = numberOfFacesTaken;
							FaceObjectIndex++;
						}
						FaceObjectIndex2++;
						takeit=false;
						numberOfFacesTaken=0;
					}
					/* If the Face index is greater than the current number of faces --> exit the loop */
					if(FaceIndex>=numberOfFaces)
						break;
					/* If the current vertex is tagged */
					if(pListVtxIdx[FaceIndex])
					{
						takeit=true;
						/* Take the 3 indices of the face */
						pOctantVtxIdx[Index_ListVtxIdx] = pVtxIdx[FaceIndex*3];
						pOctantVtxIdx[Index_ListVtxIdx+1] = pVtxIdx[FaceIndex*3+1];
						pOctantVtxIdx[Index_ListVtxIdx+2] = pVtxIdx[FaceIndex*3+2];
						/* Update the index of the list of index vertices */
						Index_ListVtxIdx+=3;
						numberOfFacesTaken++;
					}
					/* Update the index of the faces */
					FaceIndex++;
				}

				/* Creating a new Octant and Settings its properties */
				_pOctreeOctants[oct] = new Octree;
				_pOctreeOctants[oct]->SetRenderer(_pRenderer);
				_pOctreeOctants[oct]->Set3DSLoader(_p3DSLoader);
				/* Compute the new width and center */
				_pOctreeOctants[oct]->_width = width/2;
				_pOctreeOctants[oct]->_center = ComputeOctantCenter(oct,center,width);
				_pOctreeOctants[oct]->_objectCount=OctantObjectCount;
				
				/* Incrementing the number of subdivision */
				Current_Subdivision++;
				/* Calling recursively the function to see if it can partition the same octant further more */
				_pOctreeOctants[oct]->CreateOctant(pOctantVtxIdx,OctantFaceCount*3,pOctantFacesPerObject,pOctantObjectIndex,OctantObjectCount,_pOctreeOctants[oct]->_center,_pOctreeOctants[oct]->_width);
				/* Decrement the number of subdivision */
				Current_Subdivision--;
				/* Releasing the memory allocated for the temporary lists */
				delete [] pOctantVtxIdx;
				delete [] pOctantFacesPerObject;
				delete [] pOctantObjectIndex;
			}
		}
	}
	else
	{
		/* it's a leaf --> Deep copy to the new Octant */
		_leaf=true;
		/* Index Vertex List */
		_pLeafIdxVtx = new unsigned short [idxCount];
		memcpy(_pLeafIdxVtx,pVtxIdx,sizeof(unsigned short)*idxCount);
		/* Faces Per Object List */
		_pFacesPerObject = new unsigned short [objectCount];
		memcpy(_pFacesPerObject,pFacesPerObject,sizeof(unsigned short)*objectCount);
		/* Object Indices */
		_pObjectIndex = new unsigned short [objectCount];
		memcpy(_pObjectIndex,pObjectIndex,sizeof(unsigned short)*objectCount);
	}
}

void Octree::Compute_Octant8Points (Octree *pOctant)
{
	if(pOctant)
	{
		unsigned short Indices [] = {0,1, 1,2, 2,3, 3,0,  0,4, 1,5, 2,6, 3,7,  4,5, 5,6, 6,7, 7,4};
		pOctant->_pRenderer->SetIndexBuffer(Indices, 24);
		float HalfWidth=(pOctant->_width/2);
		
		/* Compute the initial Box octant 8 points */
		Point3D Center(pOctant->_center.X,pOctant->_center.Y,pOctant->_center.Z);
		pOctant->BoxPts[Pt_TopRightFront] = Center + Vector3D(0,0,1)*HalfWidth;
		pOctant->BoxPts[Pt_TopRightFront] = pOctant->BoxPts[Pt_TopRightFront] + Vector3D(1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_TopRightFront] = pOctant->BoxPts[Pt_TopRightFront] + Vector3D(0,1,0)*HalfWidth;

		pOctant->BoxPts[Pt_TopRightBack]= Center + Vector3D(0,0,-1)*HalfWidth;
		pOctant->BoxPts[Pt_TopRightBack] = pOctant->BoxPts[Pt_TopRightBack] + Vector3D(1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_TopRightBack] = pOctant->BoxPts[Pt_TopRightBack] + Vector3D(0,1,0)*HalfWidth;
		
		pOctant->BoxPts[Pt_TopLeftBack] = Center + Vector3D(0,0,-1)*HalfWidth;
		pOctant->BoxPts[Pt_TopLeftBack] = pOctant->BoxPts[Pt_TopLeftBack] + Vector3D(-1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_TopLeftBack] = pOctant->BoxPts[Pt_TopLeftBack] + Vector3D(0,1,0)*HalfWidth;
		
		pOctant->BoxPts[Pt_TopLeftFront] = Center + Vector3D(0,0,1)*HalfWidth;
		pOctant->BoxPts[Pt_TopLeftFront] = pOctant->BoxPts[Pt_TopLeftFront] + Vector3D(-1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_TopLeftFront] = pOctant->BoxPts[Pt_TopLeftFront] + Vector3D(0,1,0)*HalfWidth;

		
		pOctant->BoxPts[Pt_BottomRightFront] = Center + Vector3D(0,0,1)*HalfWidth;
		pOctant->BoxPts[Pt_BottomRightFront] = pOctant->BoxPts[Pt_BottomRightFront] + Vector3D(1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_BottomRightFront] = pOctant->BoxPts[Pt_BottomRightFront] + Vector3D(0,-1,0)*HalfWidth;
		
		pOctant->BoxPts[Pt_BottomRightBack] = Center + Vector3D(0,0,-1)*HalfWidth;
		pOctant->BoxPts[Pt_BottomRightBack] = pOctant->BoxPts[Pt_BottomRightBack] + Vector3D(1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_BottomRightBack] = pOctant->BoxPts[Pt_BottomRightBack] + Vector3D(0,-1,0)*HalfWidth;
		
		pOctant->BoxPts[Pt_BottomLeftBack] = Center + Vector3D(0,0,-1)*HalfWidth;
		pOctant->BoxPts[Pt_BottomLeftBack] = pOctant->BoxPts[Pt_BottomLeftBack] + Vector3D(-1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_BottomLeftBack] = pOctant->BoxPts[Pt_BottomLeftBack] + Vector3D(0,-1,0)*HalfWidth;
		
		pOctant->BoxPts[Pt_BottomLeftFront] = Center + Vector3D(0,0,1)*HalfWidth;
		pOctant->BoxPts[Pt_BottomLeftFront] = pOctant->BoxPts[Pt_BottomLeftFront] + Vector3D(-1,0,0)*HalfWidth;
		pOctant->BoxPts[Pt_BottomLeftFront] = pOctant->BoxPts[Pt_BottomLeftFront] + Vector3D(0,-1,0)*HalfWidth;

		for(int oct=0; oct<8; oct++)
			Compute_Octant8Points(pOctant->_pOctreeOctants[oct]);
	}
}

/* Function to traverse the whole Octree Recursively */
void Octree::TraverseOctree(Octree *pOctant)
{
	/* If the pointer exists */
	if(pOctant)
	{
		if(OctantInFrustrum(pOctant))
		{
			/* If it's not a leaf --> loop through the children */
			if(!pOctant->IsLeaf())
			{
				/* Recursively call the children octants to be tested
				and rendered */
				/* Applying the Back To Front Order */
				for(int oct=0; oct<8; oct++)
					TraverseOctree(pOctant->_pOctreeOctants[oct]);
			}
			/* Render its content */
			else
			{
				/* Increment the current number of visible octants */
				NumLeafOctant++;
				/* Turning the texture state on */
				TexturingState ts;
				ts.TextureFunction = TextureFunctionModulate;
				ts.FilteringMode = FilteringModeLinear;

				int objectOffset=0;
				int objectIndex=0;
				/* Loop through all the objects in the octant */
				for(int obj=0; obj<pOctant->_objectCount; obj++)
				{
					/* Taking the index of the current object */
					objectIndex=pOctant->_pObjectIndex[obj];
					/* Setting the Index Buffer */
					_pRenderer->SetIndexBuffer(pOctant->_pLeafIdxVtx + objectOffset*3, pOctant->_pFacesPerObject[obj]*3);
					/* If the object has materials */
					if(_p3DSLoader->objectHasMaterial[objectIndex])
					{
						/* Set the material of the current object */
						_pRenderer->SetMaterial(_p3DSLoader->materials[objectIndex].material);
						/* if the object has texture */
						if(_p3DSLoader->objectHasTexture[objectIndex])
						{
							/* Enable Textures */
							ts.Status=true;
							_pRenderer->SetState(ts);
							/* Set the texture IDs */
							_pRenderer->SetTexture(pOctant->_p3DSLoader->textureId[objectIndex]);
						}
						else
						{
							/* Disable textures */
							ts.Status=false;
							_pRenderer->SetState(ts);
						}
					}
					/* Drawing the faces */
					_pRenderer->SetPrimitivesType(PrimitiveTypeTriangleList);
					_pRenderer->Draw();
					/* Update the Object Offset according to the number of faces per object */
					objectOffset += pOctant->_pFacesPerObject[obj];
				}
			}
		}
	}
}

/* Function to render the octree */
void Octree::RenderOctree (Octree *pOctant, Matrix3D Transformation)
{
	World_Transformation = Transformation;

	for(unsigned short i=0; i<pOctant->_p3DSLoader->numberOfVertices; i++)
		TransformedVertices[i]=World_Transformation*pOctant->_p3DSLoader->vertices[i];

	/* Setting the Vertex Buffer */
	pOctant->_pRenderer->SetVertexBuffer(TransformedVertices);
	/* Setting the texture coodinates buffer */
	pOctant->_pRenderer->SetTextureCoordBuffer(pOctant->_p3DSLoader->vertexTextureCoordinates);
	/* Setting the color buffer to NULL */
	pOctant->_pRenderer->SetColorBuffer(0);
	/* Setting the normal buffer */
	pOctant->_pRenderer->SetNormalBuffer(pOctant->_p3DSLoader->vertexNormals);

	/* M = PxV */
	Matrix3D M = pOctant->_pRenderer->GetProjectionTransformation()*pOctant->_pRenderer->GetViewingTransformation();
	/* Transforming the Frustrum Vectors from the Model to the world */
	Main_Camera->Update_CameraFrustrum(M);

	NumLeafOctant=0;
	/* Traversing the Octree Recursively */
	TraverseOctree(pOctant);
}

/* Function to draw the octant boundaries */
void Octree::DrawOctantBoundaries (Octree *pOctant, Matrix3D Transformation)
{
	/* Only draw the created octants */
	if(pOctant)
	{	
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.5f,0.5f,0,1);

		for(int i=0; i<8; i++)
			pOctant->World_BoxPts[i] = Transformation*pOctant->BoxPts[i];

		Vertex Vertices[8];
		Vertices[0] = Vertex(pOctant->World_BoxPts[0].X,pOctant->World_BoxPts[0].Y,pOctant->World_BoxPts[0].Z);	Vertices[4] = Vertex(pOctant->World_BoxPts[4].X,pOctant->World_BoxPts[4].Y,pOctant->World_BoxPts[4].Z);
		Vertices[1] = Vertex(pOctant->World_BoxPts[1].X,pOctant->World_BoxPts[1].Y,pOctant->World_BoxPts[1].Z);	Vertices[5] = Vertex(pOctant->World_BoxPts[5].X,pOctant->World_BoxPts[5].Y,pOctant->World_BoxPts[5].Z);
		Vertices[2] = Vertex(pOctant->World_BoxPts[2].X,pOctant->World_BoxPts[2].Y,pOctant->World_BoxPts[2].Z);	Vertices[6] = Vertex(pOctant->World_BoxPts[6].X,pOctant->World_BoxPts[6].Y,pOctant->World_BoxPts[6].Z);
		Vertices[3] = Vertex(pOctant->World_BoxPts[3].X,pOctant->World_BoxPts[3].Y,pOctant->World_BoxPts[3].Z);	Vertices[7] = Vertex(pOctant->World_BoxPts[7].X,pOctant->World_BoxPts[7].Y,pOctant->World_BoxPts[7].Z);

		pOctant->_pRenderer->SetVertexBuffer(Vertices);	

		pOctant->_pRenderer->SetPrimitivesType(PrimitiveTypeLineList);
		pOctant->_pRenderer->Draw();

		glEnable(GL_TEXTURE_2D);

		for(int oct=0; oct<8; oct++)
			DrawOctantBoundaries (pOctant->_pOctreeOctants[oct],Transformation);
	}
}

/* Function to check the octant points with the camera frustrum */
bool Octree::OctantInFrustrum(Octree *pOctant)
{
	/* Take the 6 frustrum planes and check them with the 8 points */
	for(int pl=0; pl<6; pl++)
	{
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_TopRightFront])!=BACK)
			continue;
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_TopRightBack])!=BACK)
			continue;
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_TopLeftBack])!=BACK)
			continue;
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_TopLeftFront])!=BACK)
			continue;
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_BottomRightFront])!=BACK)
			continue;
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_BottomRightBack])!=BACK)
			continue;
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_BottomLeftBack])!=BACK)
			continue;
		/* If the 1st Point is to the front of the current plane --> Take another plane
		   else --> check the same plane with another point */
		if(Custom_Classify_PtToPlane(Main_Camera->FrustrumPlanes[pl],pOctant->World_BoxPts[Pt_BottomLeftFront])!=BACK)
			continue;

		/* If all the 8 points where in the back of the current plane --> the octant is
		   outside the frustrum */
		return false;
	}
	/* 1- This case will be reached if all the planes where to the front of a point (the
	      loop has ended) --> A point is inside the frustrum.
	   2- Or some points where in the front of some planes --> Octant envelopping the
	      frustrum 
	*/
	return true;
}




