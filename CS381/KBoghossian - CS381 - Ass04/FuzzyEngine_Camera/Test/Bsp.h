/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Bsp.h 

Purpose:	<Building and traversing the Bsp with frustrum octant clipping>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 4 CS300: Binary space partitioning

Author:	AKE development team

Creation date: <10/15/2004>

- End Header --------------------------------------------------------------*/

#ifndef _BSP_H
#define _BSP_H

//#include "Application.h"
#include <list>
#include "3dsfile.h"
#include "headers.h"

#define EPSILON 0.0005f

extern HWND g_HWND;

struct Middle_Vertex
{
	/* To Store the index of the vertex of the face (0, 1 or 2) - not the real index */
	int Index;
	Vertex V;
	bool IsLeft;
};

//one triangle information
struct Face
{
	/* Face Index */
	int Face_Index;
	/* Indices of the 3 vertices of the face */
	unsigned short index[3];
	/* ID of the object that contains the face */
	int objectID;
	/* The face normal that must be computed from the build bsp */
	Vector3D faceNormal;

	//List of OverlapFaceList
	list<Face*> OverlapFaceList;

	bool Draw;

	Face ()
	{
		Draw=true;
	}
};
/* Structure is used to sace the time and faces,
taken from the Segment Intercept function.
When taking the normal of a face! */
struct TimeFace
{
	float time;
	Point3D ptInter;
	Face *F;
};
/* Tree node class */
class BSP_TreeNode
{
	private:
			/* Default constructor */
			BSP_TreeNode();
			/* Tree Node constructor */
			BSP_TreeNode(Face * pFace);
			/* Tree Node destructor */
			~BSP_TreeNode();

			void RenderNormals_BSP_Tree(Vertex p,_3DSLoader *p3DSLoader,Renderer *renderer);
			/* Function to render the BSP faces */
			void RenderBSP_Tree(Vertex p,_3DSLoader *p3DSLoader,Renderer *renderer);
			//void RenderFinalFaces(Vertex p,_3DSLoader *p3DSLoader);
			//Vector3D GetNormalsSum(int facenum,Vector3D & sum,int & shared);

			/* Function to compute the face normal of a tree node */
			void ComputeFaceNormal (_3DSLoader *p3DSLoader);
			/* Function to convert the tree node into solid (for collision) */
			void BuildSolidTree(void);
			
			/* List of front faces */
			BSP_TreeNode *_front;
			/* List of back faces */
			BSP_TreeNode *_back;
			/* Pointer to the splitter */
			Face *_pFace;
			
			friend class BSP_Tree;
	public:
			
			/* To know whether the node is a leaf or not */
			bool _isLeaf;
			/* To know whether the node is a solid or not (for collision detection) */
			bool _isSolid;
};

/* BSP Tree class */
class BSP_Tree
{
	private:
			/* The BSP root that holds the list treenodes */
			BSP_TreeNode *_root;
			/* Function to build recursively the BSP Tree */
			BSP_TreeNode *BuildBSP_Tree(list<Face*> faceList);
			/* Function to selection a good splitter at each recursion */
			Face * SelectGoodSplitter(list<Face*> &faceList);

	public:
			/* Default constructor */
			BSP_Tree();
			/* BSP Tree constructor to initialize the BSP data base from the original data base by calling the CreateBSP_Tree */
			BSP_Tree(_3DSLoader **_p3DSLoader);
			/* BSP Destructor */
			~BSP_Tree();

			//To save the time and face of each face that was collided with the SegmentIntercept.
			list<TimeFace> TF;
			/* Function to create the BSP Tree by calling recursively the BuildBSP_Tree private function */
			void CreateBSP_Tree(_3DSLoader **p3DSLoader);
			/* Function to render the BSP Tree Faces */
			void RenderBSP_Tree(Vertex p);
			void RenderNormalsBSP_Tree(Vertex p);
			//void RenderFinalFaces(Vertex p);

			/* Function to convert the normal BSP Tree to solid (for collision) */
			void ConvertToSolid(void);
			/* Function to check whether a point is in a solid area or not */
			bool Collision(Vertex p);
			bool SegmentIntercept(Vertex p1, Vertex p2, BSP_TreeNode *n);
			/* Function that search for in the coplanar faces to get the point of intersection and the reflection vector*/
			void SegFaceIntersection(Point3D &ptInter, Vector3D &Reflection);

			BSP_TreeNode * GetRoot(){return _root;}
			void CheckCase_And_ModifyLoader (list<Face *> &frontList, list<Face *> &backList, Face *f, Face *Splitter, CLASSIFY *Vertices_Classifications, int &Front_Number, int &Back_Number);

			//void FreeBSP_Tree();

			Renderer *_pRenderer;					//Application renderer pointer
			_3DSLoader *_p3DSLoader;				//Loader pointer
			Vertex *Modal_Vertices;
			

			void Set3DSLoader(_3DSLoader * loader) { _p3DSLoader=loader; }
			void SetRenderer(Renderer * renderer) { _pRenderer=renderer; }

			/* Function to classify a face to a plane which is the splitter */
			CLASSIFY Classify_Face_Plane(Face *f, Face *splitter);
			CLASSIFY Classify_Face_Plane(Face *f, Face *splitter, CLASSIFY *Vertices_Classifications, int &Front_Number, int &Back_Number);
			/* Function to get the intersection of a triangle's edge and a splitter */
			Vertex Intersect(Vertex source, Vertex destination, Face *pSplitter, float &time);
			void Compute_FaceNormal (Face *f);
			void ComputeNormals(BSP_TreeNode * root);
			/* Updating the bsp vertices to world coordinate */
			void Update_BSP_Vertices(Matrix3D Obj_Matrix);
			bool BSP_Collision(Point3D Source, Point3D Destination, Point3D &ptInter, Vector3D &FaceNormal);

};

/* Function to classify a vertex to a plane which is the splitter */
CLASSIFY Classify_Pt_Plane(Vertex point,Plane3D splitter);
/* Function to classify a face to the camera frustrum */
bool FaceInFrustrum(Face *f, Plane3D FrustrumPlanes[6], _3DSLoader *p3DSLoader);
/*Render any given segment in space*/
void RenderSegment(Vertex p1, Vertex p2, Renderer *renderer);
/*Rendering the point of intersection*/
void Render_InterPoint(Vertex p, Renderer *renderer);

#endif