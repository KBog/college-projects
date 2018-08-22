#include "Bsp.h"
#include "Support_Functions.h"
#include <GL/gl.h>
#include <GL/glu.h>

#define SPLIT_CONST 8

int Leaf_Number=0;
int current_depth=0, Depth=0;

//Floating segment
Point3D p1, p2;
//======================================================================================================
void RenderSegment(Vertex p1, Vertex p2, Renderer *renderer)
{
	TexturingState ts;
	ts.TextureFunction = TextureFunctionModulate;
	ts.FilteringMode = FilteringModeLinear;

	glDisable(GL_TEXTURE_2D);
	glColor4f(1,0,0,1);

	Vertex NormalVtx [2];
	NormalVtx[0] = p1;
	NormalVtx[1] = p2;

	unsigned short indexbuff [] = {0,1};
	renderer->SetIndexBuffer(indexbuff,2);
	renderer->SetVertexBuffer(NormalVtx);
	renderer->SetPrimitivesType(PrimitiveTypeLineList);
	renderer->Draw();

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
}
//======================================================================================================
/*Displaying point of intersection between segment and face*/
void Render_InterPoint(Vertex p, Renderer *renderer)
{
	//For TESTING: CREATIVE PROBLEM SOLVING
	Point3D ps(p.X+0.1f, p.Y+0.1f, p.Z+0.1f);
	//Displaying a Segment as if displaying a point ;)!
	RenderSegment(p,ps,renderer);
}
//======================================================================================================
/********************************************************* BSP TREE NODE *********************************************************/
/* Default constructor */
BSP_TreeNode::BSP_TreeNode()
{
	/* Init the members */
	_pFace=NULL;
	_front=_back=NULL;
	_isLeaf=_isSolid=false;
}
//======================================================================================================
/* Tree Node constructor */
BSP_TreeNode::BSP_TreeNode(Face * pFace)
{
	_pFace=pFace;
	_front=_back=NULL;
	_isLeaf=_isSolid=false;
}
//======================================================================================================
/* Tree Node Destructor */
BSP_TreeNode::~BSP_TreeNode()
{
	/* if the front list exists --> delete recursively */
	if(_front)
		delete _front;
	/* if the back list exists --> delete recursively */
	if(_back)
		delete _back;
	/* delete the splitter */
	if(_pFace)
		delete _pFace;
}
//======================================================================================================
/* Function to compute the face normal */
void BSP_TreeNode::ComputeFaceNormal (_3DSLoader *p3DSLoader)
{
	/* Compute the face normal */
	Vector3D V1(p3DSLoader->vertices[_pFace->index[1]].X-p3DSLoader->vertices[_pFace->index[0]].X, p3DSLoader->vertices[_pFace->index[1]].Y-p3DSLoader->vertices[_pFace->index[0]].Y, p3DSLoader->vertices[_pFace->index[1]].Z-p3DSLoader->vertices[_pFace->index[0]].Z);
	Vector3D V2(p3DSLoader->vertices[_pFace->index[2]].X-p3DSLoader->vertices[_pFace->index[0]].X, p3DSLoader->vertices[_pFace->index[2]].Y-p3DSLoader->vertices[_pFace->index[0]].Y, p3DSLoader->vertices[_pFace->index[2]].Z-p3DSLoader->vertices[_pFace->index[0]].Z);
	_pFace->faceNormal = V1.GetCrossProduct(V2);
	_pFace->faceNormal.SetNormalize();
	/* if the front list exists --> recursive call and compute the face normal */
	if(_front)
		_front->ComputeFaceNormal(p3DSLoader);
	/* if the back list exists --> recursive call and compute the face normal */
	if(_back)
		_back->ComputeFaceNormal(p3DSLoader);
}
//======================================================================================================
/* Function to render only the front BSP faces (FaceCulling is not needed) */
void BSP_TreeNode::RenderBSP_Tree(Vertex p,_3DSLoader *p3DSLoader,Renderer *renderer)
{
	TexturingState ts;
	ts.TextureFunction = TextureFunctionModulate;
	ts.FilteringMode = FilteringModeLinear;

	/* Classify the Point against the node's plane */
	CLASSIFY Test = Classify_Pt_Plane(p,Plane3D(p3DSLoader->vertices[_pFace->index[0]],_pFace->faceNormal));
	if(Test==FRONT || Test==INTERSECT)
	{
		/* if the current node has a list of back faces */
		if(_back)
			if(_back->_pFace)
			{
				current_depth++;
				_back->RenderBSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
		Depth=(Depth<current_depth)?current_depth:Depth;
		/* Render the face */
		/* Set index buffer */
		if (_pFace->Draw==true)
		{
			renderer->SetIndexBuffer(_pFace->index,3);
			/* Set material if the face has material */
			if(p3DSLoader->objectHasMaterial[_pFace->objectID])
			{
				renderer->SetMaterial(p3DSLoader->materials[_pFace->objectID].material);
				if(p3DSLoader->objectHasTexture[_pFace->objectID])
				{
					ts.Status=true;
					renderer->SetState(ts);
					renderer->SetTexture(p3DSLoader->textureId[_pFace->objectID]);
				}
				else
				{
					ts.Status=false;
					renderer->SetState(ts);
				}
				renderer->SetPrimitivesType(PrimitiveTypeTriangleList);
				renderer->Draw();
			}
		}
		/* if the current node has a list of front faces */
		if(_front)
			if(_front->_pFace)
			{
				current_depth++;
				_front->RenderBSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
	}
	else
	{
		/* if the current node has a list of front faces */
		if(_front)
			if(_front->_pFace)
			{
				current_depth++;
				_front->RenderBSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
		/* if the current node has a list of back faces */
		if(_back)
			if(_back->_pFace)
			{
				current_depth++;
				_back->RenderBSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
	}
}
//======================================================================================================
/* Function to convert the tree node into solid (for collision) */
void BSP_TreeNode::BuildSolidTree(void)
{
	/* If the front list is empty --> set to leaf and solid to false */
	if(!_front)
	{
		/* Create a new node and assign it to the front list */
		_front = new BSP_TreeNode;
		/* Set to leaf */
		_front->_isLeaf=true;
		/* Not solid */
		_front->_isSolid=false;
	}
	/* call recursively to build the rest */
	else
		_front->BuildSolidTree();
	/* If the back list is empty --> set to leaf and solid to true */
	if(!_back)
	{
		/* Create a new node and assign it to the front list */
		_back = new BSP_TreeNode;
		/* Set to leaf */
		_back->_isLeaf=true;
		/* Set to solid */
		_back->_isSolid=true;
	}
	/* call recursively to build the rest */
	else
		_back->BuildSolidTree();
}
//======================================================================================================
/********************************************************* BSP TREE *********************************************************/
/* Function to classify a face to a plane */
CLASSIFY BSP_Tree::Classify_Face_Plane(Face *f, Face *splitter)
{
	int Front_Number=0, Back_Number=0;
	Plane3D Splitter_Pln(Plane3D(_p3DSLoader->vertices[splitter->index[0]],splitter->faceNormal));

	CLASSIFY T;
	/* Test 3 vertices of the current face with the splitter */
	for(int i=0; i<3; i++)
	{
		/* Classify the current vertex with the splitter */
		T=Classify_Pt_Plane(_p3DSLoader->vertices[f->index[i]],Splitter_Pln);
		/* if it's to the front --> increment the number of front vertices */
		if(T==FRONT)
			Front_Number++;
		/* if it's to the back --> increment the number of back vertices */
		else if(T==BACK)
			Back_Number++;
	}
	
	/* There exist front and back --> intersect --> Split in 2 or 3 triangles */
	if(Front_Number && Back_Number)
		return INTERSECT;
	/* Only front --> add to front list */
	else if(Front_Number)
		return FRONT;
	/* Only Back --> add to back list */
	else if(Back_Number)
		return BACK;
	/* The face lies in the splitter --> add to front list (for consistency) */
	return OVERLAP;
}
//======================================================================================================
/* Function to classify a face to a plane */
CLASSIFY BSP_Tree::Classify_Face_Plane(Face *f, Face *splitter, CLASSIFY *Vertices_Classifications, int &Front_Number, int &Back_Number)
{
	Front_Number=Back_Number=0;
	Plane3D Splitter_Pln(Plane3D(_p3DSLoader->vertices[splitter->index[0]],splitter->faceNormal));

	/* Test 3 vertices of the current face with the splitter */
	for(int i=0; i<3; i++)
	{
		/* Classify the current vertex with the splitter */
		Vertices_Classifications[i]=Classify_Pt_Plane(_p3DSLoader->vertices[f->index[i]],Splitter_Pln);
		/* if it's to the front --> increment the number of front vertices */
		if(Vertices_Classifications[i]==FRONT)
			Front_Number++;
		/* if it's to the back --> increment the number of back vertices */
		else if(Vertices_Classifications[i]==BACK)
			Back_Number++;
	}
	
	/* There exist front and back --> intersect --> Split in 2 or 3 triangles */
	if(Front_Number && Back_Number)
		return INTERSECT;
	/* Only front --> add to front list */
	else if(Front_Number)
		return FRONT;
	/* Only Back --> add to back list */
	else if(Back_Number)
		return BACK;
	/* The face lies in the splitter --> add to front list (for consistency) */
	return OVERLAP;
}
//======================================================================================================
/* Function that returns the intersection point of a segment with a plane */
Vertex BSP_Tree::Intersect(Vertex source, Vertex destination, Face *pSplitter, float &time)
{
	/* Vector going from the source vertex to the destination one */
	Vector3D V1(destination.X-source.X, destination.Y-source.Y, destination.Z-source.Z);
	float Denominator = pSplitter->faceNormal.GetDotProduct(V1);
	/* Vector going from a splitter's vertex to the source vertex */
	Vector3D V2(source.X-_p3DSLoader->vertices[pSplitter->index[0]].X, source.Y-_p3DSLoader->vertices[pSplitter->index[0]].Y, source.Z-_p3DSLoader->vertices[pSplitter->index[0]].Z);
	float Numerator = pSplitter->faceNormal.GetDotProduct(V2);
	/* Compute the time at which the collision occured */
	time = -Numerator/Denominator;

	/* Return the intersection vertex */
	return source + time*(V1);
}

//======================================================================================================
/* Function to select a good splitter using "Approach A" (see class notes P165) */
Face * BSP_Tree::SelectGoodSplitter(list<Face*> &faceList)
{
	/* Init the splitter face */
	Face *Sp =  *faceList.begin();
	/* Init the number of front, back and intersecting faces */
	int Front=0, Back=0, Intersect=0;
	/* Setting the initial equation value to the list size */
	float Prev_Result=(float)faceList.size()*10, Current_Result=0;
	CLASSIFY Test;

	/* Loop through all the faces to take one by one as a splitter */
	for(list<Face*>::iterator it_i=faceList.begin(); it_i!=faceList.end(); it_i++)
	{
		Front=Back=Intersect=0;
		/* And check with all the remaining faces */
		for(list<Face*>::iterator it_j=faceList.begin(); it_j!=faceList.end(); it_j++)
		{
			/* Storing the result of the classification */
			Test=Classify_Face_Plane(*it_j,*it_i);
			/* Update the classification counters according to the result */
			if(Test==FRONT || Test==OVERLAP)
				Front++;
			else if(Test==BACK)
				Back++;
			else
				Intersect++;
		}
		/* Compute the result after choosing the current splitter and splitting the other faces 
		   using "Approach A" */
		Current_Result = (float)abs(Front-Back) + Intersect*SPLIT_CONST;
		/* If the result if less than the previous result --> Update the splitter and the equation */
		if(Current_Result<Prev_Result)
		{
			Sp=*it_i;
			Prev_Result=Current_Result;
		}
	}

	/* Remove the splitter from the list */
	faceList.remove(Sp);
	/* Return the Splitter */
	return Sp;
}  
//======================================================================================================
/* Function to compute the normal of a face */
void BSP_Tree::Compute_FaceNormal (Face *f)
{
	/* Compute the face normal */
	Vector3D V1(_p3DSLoader->vertices[f->index[1]].X-_p3DSLoader->vertices[f->index[0]].X, _p3DSLoader->vertices[f->index[1]].Y-_p3DSLoader->vertices[f->index[0]].Y, _p3DSLoader->vertices[f->index[1]].Z-_p3DSLoader->vertices[f->index[0]].Z);
	Vector3D V2(_p3DSLoader->vertices[f->index[2]].X-_p3DSLoader->vertices[f->index[0]].X, _p3DSLoader->vertices[f->index[2]].Y-_p3DSLoader->vertices[f->index[0]].Y, _p3DSLoader->vertices[f->index[2]].Z-_p3DSLoader->vertices[f->index[0]].Z);
	f->faceNormal = V1.GetCrossProduct(V2);
	f->faceNormal.SetNormalize();
}
//======================================================================================================
/* Function to compute the face normals after building the tree */
void BSP_Tree::ComputeNormals(BSP_TreeNode * root)
{
	/* if the root exists --> compute its face normal and call recursively
	   its children */
	if(root)
		root->ComputeFaceNormal(_p3DSLoader);
}
//======================================================================================================
/* BSP Tree default constructor */
BSP_Tree::BSP_Tree ()
{
	/* Init the root pointer */
	_root=NULL;
	/* Init the renderer pointer */
	_pRenderer=NULL;
	/* Init the loader pointer */
	_p3DSLoader=NULL;

}
//======================================================================================================
/* BSP Tree constructor that starts the building process */
BSP_Tree::BSP_Tree (_3DSLoader **p3DSLoader)
{
	/* Init the root pointer */
	_root=NULL;
	/* create the list of faces then recursively builds the tree */
	CreateBSP_Tree(p3DSLoader);
	Modal_Vertices = new Vertex [_p3DSLoader->numberOfVertices];
	memcpy(Modal_Vertices,_p3DSLoader->vertices,_p3DSLoader->numberOfVertices*sizeof(Vertex));
}
//======================================================================================================
/* BSP Tree destructor */
BSP_Tree::~BSP_Tree()
{
	/* Delete the root node if it exists */
	if(_root)
		delete _root;
}
//======================================================================================================
/* Function the creates the BSP tree using the recursive buildbsp_tree function */
void BSP_Tree::CreateBSP_Tree (_3DSLoader **p3DSLoader)
{
	/* Assign the loader to the class member */
	_p3DSLoader=*p3DSLoader;

	/* The list that stores the faces in the current world */
	list<Face *> faceList;
	/* A pointer to the current face that will be added to the list */
	Face *f;
	/* Face offset */
	int offset=0;
	/* object offset */
	int nextObject=0;

	/* Loop through all the faces */
	for(int Fi=0; Fi<_p3DSLoader->numberOfFaces; Fi++)
	{
		/* Allocate a space for the new face */
		f=new Face;
		/* Store the face index */
		f->Face_Index=Fi;
		/* Store the current 3 vertex index in the new face */
		f->index[0]=_p3DSLoader->vertexIndices[Fi*3];
		f->index[1]=_p3DSLoader->vertexIndices[Fi*3+1];
		f->index[2]=_p3DSLoader->vertexIndices[Fi*3+2];
		/* Store the current objectID in the new face */
		f->objectID=nextObject;
		/* Compute the face normal */
		Compute_FaceNormal(f);

		/* Add the new face to the list of faces */
		faceList.push_back(f);

		/* Update the objectID (current object offset) if all the faces are visited */
		if(Fi>=(offset+_p3DSLoader->objectFaceNumber[nextObject]-1))
		{
			/* Take the face number of the next object */
			offset+=_p3DSLoader->objectFaceNumber[nextObject];
			/* Take another object */
			nextObject++;
		}
	}

	/* Recursively build the BSP Tree and return its root */
	_root=BuildBSP_Tree(faceList);
	/* Convert the tree to solid for collision detection */
	ConvertToSolid();
}
//======================================================================================================
/* Function to build the BSP Tree recursively */
BSP_TreeNode *BSP_Tree::BuildBSP_Tree(list<Face*> faceList)
{
	/* 1st Base case, if the list contains 0 element */
	if(faceList.size()==0)
		return NULL;
	/* 2nd Base case, if the list contains 1 element */
	if(faceList.size()==1)
		return new BSP_TreeNode(*(faceList.begin()));

	/* else the list contains faces --> fill the front and back lists */
	list<Face *> frontList, backList;
	/* choosing a good splitter and remove it from the list (done in the SelectGoodSplitter) */
	Face *Splitter=SelectGoodSplitter(faceList);
	
	/* Classify the current world to the Splitter */
	CLASSIFY Test;
	CLASSIFY Vertices_Classifications[3];
	/* Init the number of front, back vertices and the middle index vertex */
	int Front=0, Back=0;
	/* Loop through all the faces in the current world */
	for(list<Face*>::iterator it_i=faceList.begin(); it_i!=faceList.end(); it_i++)
	{
		/* Classify the current face to the splitter and store the result of the 3 vertices
		   classifications in the array */
		Test=Classify_Face_Plane(*it_i,Splitter,Vertices_Classifications,Front,Back);
		/* The face is in the front or coplanar to the splitter (4 cases) */
		if(Test==FRONT)
			frontList.push_back(*it_i);
		/* The face is in the back of the splitter (3 cases) */
		else if(Test==BACK)
			backList.push_back(*it_i);
		else if(Test== OVERLAP)
		{
			frontList.push_back(*it_i);
			Splitter->OverlapFaceList.push_back(*it_i);
		}
		/* The face intersects with the splitter 2 triangles (12 cases) - 3 triangles (24 cases) */
		else
			CheckCase_And_ModifyLoader(frontList,backList,*it_i,Splitter,Vertices_Classifications,Front,Back);
	}

	/* Create a new node holding the splitter */
	BSP_TreeNode *node=new BSP_TreeNode(Splitter);
	/* Assign the 2 lists previously computed to the new node */
	node->_front=BuildBSP_Tree(frontList);
	node->_back=BuildBSP_Tree(backList);

	/* return the new node */
	return node;
}
//======================================================================================================
/* Function to render the BSP Tree according to a point in the world */
void BSP_Tree::RenderBSP_Tree(Vertex p)
{
	/* Set the vertex buffer */
	_pRenderer->SetVertexBuffer(_p3DSLoader->vertices);
	/* Set the texture coordinates */
	_pRenderer->SetTextureCoordBuffer(_p3DSLoader->vertexTextureCoordinates);
	/* Set the normal buffer */
	_pRenderer->SetNormalBuffer(_p3DSLoader->vertexNormals);
	
	/* Render the BSP recursively (2 ways: 1 BackFaceCulling is needed, 2 BackFaceCulling is not needed) */
	_root->RenderBSP_Tree(p,_p3DSLoader,_pRenderer);
}
/* Function to render the normals */
void BSP_Tree::RenderNormalsBSP_Tree(Vertex p)
{
	_root->RenderNormals_BSP_Tree(p,_p3DSLoader,_pRenderer);
}
//======================================================================================================
/* Function to convert the normal BSP Tree to solid (for collision) */
void BSP_Tree::ConvertToSolid(void)
{
	/* Build the solid tree starting from the root node */
	_root->BuildSolidTree();
}
//======================================================================================================
/* Function to check whether a point is in a solid area or not */
bool BSP_Tree::Collision(Vertex p)
{
	/* Init the temporary node to the tree root */
	BSP_TreeNode *n=_root;
	CLASSIFY Test;
	
	/* Loop untill we reach a leaf */
	while(!n->_isLeaf)
	{
		/* classify the current node with the vertex */
		Test=Classify_Pt_Plane(p,Plane3D(_p3DSLoader->vertices[n->_pFace->index[0]],n->_pFace->faceNormal));
		/* if the vertex is to the front --> we go front */
		if(Test==FRONT || Test==INTERSECT)
			n=n->_front;
		/* if the vertex is to the back --> we go back */
		else
			n=n->_back;
	}
	/* return the Solid status of the reached leaf */
	return n->_isSolid;
}

//======================================================================================================
/* Function to return whether a segment intersepts a solid */
bool BSP_Tree::SegmentIntercept(Vertex p1, Vertex p2, BSP_TreeNode *n)
{
	//Variable to save the point of intersection
	//where the segment intercepts the face.
 	Point3D pointIntersection;
	//time variable is considered as the distance
	//normalized btw source pt and pt of inter.
	float time;
	//A structure that saves the time and the face 
	//and point of intersection.
	//2 cases FRONT_BACK and BACK_FRONT.
	TimeFace timeFace;

    //When reaching the end of the BSP, that means we reached a solid area!
	if(n->_isLeaf)
		return !n->_isSolid;
	
	/* Classify the 2 vertices with the current node */
 	CLASSIFY cla1 = Classify_Pt_Plane(p1,Plane3D(_p3DSLoader->vertices[n->_pFace->index[0]],n->_pFace->faceNormal));
	CLASSIFY cla2 = Classify_Pt_Plane(p2,Plane3D(_p3DSLoader->vertices[n->_pFace->index[0]],n->_pFace->faceNormal));

	/* if the 2 vertices intersect --> go front */
	if(cla1==INTERSECT && cla2==INTERSECT)
		return SegmentIntercept(p1,p2,n->_front);
	/* if v1 front and v2 back */
	if(cla1==FRONT && cla2==BACK)
	{
		//Compute the intersection point
		pointIntersection=Intersect(p1,p2,n->_pFace,time);
		//Saving the distance or time
		timeFace.time= time;
		//Saving the face
		timeFace.F= n->_pFace;
		//Saving the pt of intersection
		timeFace.ptInter= pointIntersection;
		//Building the list of TimeFace
		TF.push_back(timeFace);
		/* return the result of the 2 new edges classification */
		return SegmentIntercept(p1,pointIntersection,n->_front)&& SegmentIntercept(p2,pointIntersection,n->_back);
	}
	/* if v1 back and v2 front */
	if(cla1==BACK && cla2==FRONT)
	{
		//Compute the intersection point
		pointIntersection=Intersect(p1,p2,n->_pFace,time);
		//Saving the distance or time
		timeFace.time= time;
		//Saving the face
		timeFace.F= n->_pFace;
		//Saving the pt of intersection
		timeFace.ptInter= pointIntersection;
		//Building the list of TimeFace
		TF.push_back(timeFace);
		/* return the result of the 2 new edges classification */
		return SegmentIntercept(p2,pointIntersection,n->_front)&& SegmentIntercept(p1,pointIntersection,n->_back);
	}
	/* if the 2 vertices are to the front side --> go front */
	if(cla1==FRONT || cla2==FRONT)
		return SegmentIntercept(p1,p2,n->_front);
	/* if the 2 vertices are to the back side --> go back */
	else
		return SegmentIntercept(p1,p2,n->_back);
	return true;
}

//======================================================================================================
/* Function that search for in the coplanar faces to get the point of intersection and the reflection vector*/
void BSP_Tree::SegFaceIntersection(Point3D &ptInter, Vector3D &Reflection)
{
	//If the list is empty
	if(TF.empty())
	{
		//Set the vector normal to zero.
		Reflection.SetXYZ(0,0,0);
		//Get out from the function.
		return;
	}
	//Minimum Time variable set a very big minimum value
  	float MaxTime= -10000.0f;
	list<TimeFace>::iterator it;
	//Checking in the list of faces, which face that has 
	//the maximum time or distance to the source point.
	for(it= TF.begin(); it!= TF.end(); it++)
		if(it->time > MaxTime)
			MaxTime= it->time;
	//Looping through all the faces that are considered as 
	//coplanar faces, in all BSP.
  	for(it= TF.begin(); it!= TF.end(); it++)
	{
		//Which of the faces that has the MaxTime.
		//When find it, get its point and face normal.
		if(it->time == MaxTime)
		{
			//3 points for the triangle
			Point3D p1,p2,p3;
			//Getting the triangle that is constructed
			//from the vertices of the current face.
			p1= _p3DSLoader->vertices[(it)->F->index[0]];
			p2= _p3DSLoader->vertices[(it)->F->index[1]];
			p3= _p3DSLoader->vertices[(it)->F->index[2]];
			//Triangle3D tri
			Triangle3D tri(p1,p2,p3);		
			/*Checking the current face with the point of 
			intersection. Because the current face is not
			taken in consideration with the overlapped face list */
			//Set the point of intersection
			Point3D point(it->ptInter);
			//Set the reflection of the current face
			Reflection= (it)->F->faceNormal;
			//pln is the plane that holds the tri and the Reflection vector.
			Plane3D pln(p1, Reflection);
			//pt is considered the point of projection to the current face
			Point3D pt;
			point.ProjectionOnPlane(pln, pt);
			//For TEST: ptInter is a public member inside the BSPTree, to display in the window title.
			ptInter= pt;
			//For TEST: Display where the segment is intercepting!
			Render_InterPoint(pt,_pRenderer);

			/*Checking the list of neighbors of the current face,
			its neighbors are overlaped with the current face.
			Like that we can identify 100% the point of intersection
			and the Reflection Normal*/
			list<Face*>::iterator it2;
			//Looping through all the coplanar faces of this current face
			for(it2= it->F->OverlapFaceList.begin(); it2!= it->F->OverlapFaceList.end(); it2++)
			{
				//3 points for the triangle
				Point3D p1,p2,p3;
				//Getting the triangle that is constructed
				//from the vertices of the current face.
				p1= _p3DSLoader->vertices[(*it2)->index[0]];
				p2= _p3DSLoader->vertices[(*it2)->index[1]];
				p3= _p3DSLoader->vertices[(*it2)->index[2]];
				//Triangle3D tri
				Triangle3D tri(p1,p2,p3);
				/*Checking the current face with the point of 
				intersection. Because the current face is not
				taken in consideration with the overlapped face list */
				//Set the point of intersection
				Point3D point(it->ptInter);
				//Set the reflection of the current face
				Reflection= (*it2)->faceNormal;
				//pln is the plane that holds the tri and the Reflection vector
				Plane3D pln(p1, Reflection);
				//pt is considered the point of projection to the current face
				Point3D pt;
				point.ProjectionOnPlane(pln, pt);
				//For TEST: ptInter is a public member inside the BSPTree, to display in the window title.
				ptInter= pt;
				//For TEST: Display where the segment is intercepting!
				Render_InterPoint(pt,_pRenderer);
			}	
		}
	}
	//Freeing all the faces at the end, because each GameLoop the TF list is being filled!
	for(size_t i= TF.size(); i>0; i--)
		TF.pop_front();
}
//======================================================================================================
/* Function to render only the front BSP faces (FaceCulling is not needed) */
void BSP_TreeNode::RenderNormals_BSP_Tree(Vertex p,_3DSLoader *p3DSLoader,Renderer *renderer)
{
	TexturingState ts;
	ts.TextureFunction = TextureFunctionModulate;
	ts.FilteringMode = FilteringModeLinear;

	/* Classify the Point against the node's plane */
	CLASSIFY Test = Classify_Pt_Plane(p,Plane3D(p3DSLoader->vertices[_pFace->index[0]],_pFace->faceNormal));
	if(Test==FRONT || Test==INTERSECT)
	{
		/* if the current node has a list of back faces */
		if(_back)
			if(_back->_pFace)
			{
				current_depth++;
				_back->RenderNormals_BSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
		Depth=(Depth<current_depth)?current_depth:Depth;
		/* Render the face */
		/* Set index buffer */

		if(_pFace)
		{
		glDisable(GL_TEXTURE_2D);
		glColor4f(1,0,0,1);

		Vertex NormalVtx [2];
		NormalVtx[0] = (p3DSLoader->vertices[_pFace->index[0]] + p3DSLoader->vertices[_pFace->index[1]] + p3DSLoader->vertices[_pFace->index[2]])/3;
		NormalVtx[1] = NormalVtx[0] + _pFace->faceNormal*5;

		unsigned short indexbuff [] = {0,1};
		renderer->SetIndexBuffer(indexbuff,2);
		renderer->SetVertexBuffer(NormalVtx);
		renderer->SetPrimitivesType(PrimitiveTypeLineList);
		renderer->Draw();

		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		}
		/* if the current node has a list of front faces */
		if(_front)
			if(_front->_pFace)
			{
				current_depth++;
				_front->RenderNormals_BSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
	}
	else
	{
		/* if the current node has a list of front faces */
		if(_front)
			if(_front->_pFace)
			{
				current_depth++;
				_front->RenderNormals_BSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
		/* if the current node has a list of back faces */
		if(_back)
			if(_back->_pFace)
			{
				current_depth++;
				_back->RenderNormals_BSP_Tree(p,p3DSLoader,renderer);
				current_depth--;
			}
	}
}
//======================================================================================================
/* Updating the bsp vertices to world coordinate */
void BSP_Tree::Update_BSP_Vertices(Matrix3D Obj_Matrix)
{
	for(int i=0; i<_p3DSLoader->numberOfVertices; i++)
		_p3DSLoader->vertices[i] = Obj_Matrix*Modal_Vertices[i];
}

//Function that sets the point of intersection with its normal,
//and takes also the parameters that the SegmentIntercept function needs!
bool BSP_Tree::BSP_Collision(Point3D Source, Point3D Destination, Point3D &ptInter, Vector3D &FaceNormal)
{
	if(!SegmentIntercept(Source,Destination,_root))
	{
		SegFaceIntersection(ptInter, FaceNormal);
		return true;
	}
	return false;
}