#include <Application.h>
#include <3dsfile.h>
#include <headers.h>

using namespace std;

enum OctantName { Oct_FrontTopLeft, Oct_FrontTopRight, Oct_FrontBottomRight, Oct_FrontBottomLeft,
				  Oct_BackTopLeft, Oct_BackTopRoght, Oct_BackBottomRight, Oct_BackBottomLeft };
enum PointName { Pt_TopRightFront, Pt_TopRightBack, Pt_TopLeftBack, Pt_TopLeftFront, 
				 Pt_BottomRightFront, Pt_BottomRightBack, Pt_BottomLeftBack, Pt_BottomLeftFront };

class Octree
{
	private:
			/* Octant Leaf Status */
			bool _leaf;
			/* Octant Width */
			float _width;
			/* Octant center */
			Vertex _center;
			/* Number of objects per octant */
			int _objectCount;
			/* List of vertex indices */
			unsigned short * _pLeafIdxVtx;		
			/* List of number of faces per object */
			unsigned short * _pFacesPerObject;
			/* List of Object Indices */
			unsigned short * _pObjectIndex;
			/* Array of octants in each octant */
			Octree * _pOctreeOctants [8];

			Matrix3D World_Transformation;
			
			/* Function to traverse the Octree recursively */
			void TraverseOctree(Octree *pOctant);

			Renderer * _pRenderer;
			_3DSLoader * _p3DSLoader;

			/* 6 Points forming the octant */
			Point3D BoxPts[8];

			Point3D World_BoxPts[8];
			Vertex *TransformedVertices;

	public:
			Octree();
			Octree(_3DSLoader *loader,Renderer *renderer);
			~Octree();

			float GetWidth (void);
			Vertex GetCenter (void);
			bool IsLeaf (void);

			void SetRenderer(Renderer *pr);
			void Set3DSLoader(_3DSLoader *p3DSLoader);
			void Octree::GetWorldDimensions (float Width);
			
			void CreateOctant  (unsigned short * pVtxIdx,		//Vertex index buffer
								int idxCount,					//Number of elements in the index vertex buffer
								unsigned short *pFacesPerObject,//List of number of faces per object
								unsigned short *pObjectIndex,	//List of Object Index
								int objectCount,				//Number of Objects
								Vertex center,					//Octant Center
								float width);					//Octant Width
			
			/* Will call the recursive traverse */
			void RenderOctree (Octree *pOctant, Matrix3D Transformation);
			bool OctantInFrustrum(Octree *pOctant);
			void Compute_Octant8Points (Octree *pOctant);
			void DrawOctantBoundaries (Octree *pOctant, Matrix3D Transformation);
};