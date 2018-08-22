#ifndef _3DSFILE_H_
#define _3DSFILE_H_

#include <windows.h>
#include <math.h>
#include "Color.h"
#include "Texture.h"
#include "TexturingState.h"
#include "Renderer.h"
#include <vector>

using namespace std;

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D				// This gives the version of the mesh and is found right before the material and object information
#define VERSION       0x0002				// This gives the version of the .3ds file
#define EDITKEYFRAME  0xB000				// This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				// This stored the texture info
#define OBJECT		  0x4000				// This stores the faces, vertices, etc...

//>------ sub defines of MATERIAL
#define MATNAME				0xA000				// This holds the material name
#define MATAMBIENT			0xA010				// This holds the color of the object/material
#define MATDIFFUSE			0xA020
#define MATSPECULAR			0xA030

#define MATSHININESS		0xA040
#define MATEMISSIVE			0xA050
#define MATTRANSPARENCY		0xA052

#define MATMAP        0xA200				// This is a header for a new material
#define MATMAPFILE    0xA300				// This holds the file name of the texture

#define OBJECT_MESH   0x4100				// This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			// The objects vertices
#define OBJECT_FACES		0x4120			// The objects faces
#define OBJECT_MATERIAL		0x4130			// This is found if the object has a material, either texture map or color
#define OBJECT_UV			0x4140			// The UV texture coordinates

#define MAX3DSTEXTURES		1000

// Here is our structure for our 3DS indicies (since .3DS stores 4 unsigned shorts)
struct _Indices {							

	unsigned short a, b, c, bVisible;		// This will hold point1, 2, and 3 index's into the vertex array plus a visible flag
};

// This holds the chunk info
struct _Chunk
{
	unsigned short int ID;					// The chunk's ID		
	unsigned int length;					// The length of the chunk
	unsigned int bytesRead;					// The amount of bytes read within that chunk
};



// This is our face structure.  This is is used for indexing into the vertex
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
class _Face
{
public:
	_Face() {vertIndex[0]=0;vertIndex[1]=0;vertIndex[2]=0;};
	~_Face() {};

	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.
class _MaterialInfo
{
public:
	_MaterialInfo() {memset(strName,'\0',255);memset(strFile,'\0',255);textureFileReferenceNumber=0;};
	~_MaterialInfo() {};

	void operator=(_MaterialInfo mi);
	char  strName[255];			// The texture name
	char  strFile[255];			// The texture file name (If this is set it's a texture map)
	int textureFileReferenceNumber;
	//BYTE  color[3];				// The color of the object (R, G, B)
	Material material;	
//	float uTile;				// u tiling of texture  (Currently not used)
//	float vTile;				// v tiling of texture	(Currently not used)
//	float uOffset;			    // u offset of texture	(Currently not used)
//	float vOffset;				// v offset of texture	(Currently not used)
};

// This holds all the information for our model/scene.
// You should eventually turn into a robust class that 
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
class _3DSObject 
{
public:
	_3DSObject() {numOfVerts=0;numOfFaces=0;numTexVertex=0;materialID=-1;bHasTexture=false;memset(strName,'\0',255);verts=NULL;vertIndices=NULL;vertNormals=NULL;texVerts=NULL;faces=NULL;textureFileReferenceNumber=0;};
	~_3DSObject() {};

	int numOfVerts;			// The number of verts in the model
	int numOfFaces;			// The number of faces in the model
	int numTexVertex;			// The number of texture coordinates
	int materialID;			// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	int textureFileReferenceNumber;
	char strName[255];			// The name of the object
	Vertex *verts;			// The object's vertices
	unsigned short * vertIndices;	//the vertex indices array (the order of each 3 triangle corners)
	Point3D  *vertNormals;		// The object's normals
	UV  *texVerts;		// The texture's UV coordinates	
	_Face *faces;				// The faces information of the object
};

// This holds our model information.  This should also turn into a robust class.
// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
class _3DSModel
{
public:
	_3DSModel();
	~_3DSModel() {};

	char fileName[255];
	//Texture * textures[100];	//max 100 textures per 3ds model
	int numOfObjects;					// The number of objects in the model
	int numOfMaterials;					// The number of materials for the model
	vector<_MaterialInfo> materials;	// The list of material information (Textures and colors)
	vector<_3DSObject> objects;			// The object list for our model

	void Clean();
};


class _3DSLoader
{
public:
	_3DSLoader();
	~_3DSLoader();

	//all these arrays references have the same sequence (order).
	int _numberOfModels;
	int numberOfVertices;
	int numberOfObjects;
	int numberOfFaces;
	int numberOfTextureCoordinates;
	Vertex * vertices;//all the models vertices
	//Color * colors;//all the models colors: not used right now
	_MaterialInfo * materials;//each set of n vertices has a different material (for an object in a model).
    unsigned short * objectFaceNumber;//number of faces for each object in all the models
	//example facesNumber[0] = 90 => 90 faces for the first object
	bool * objectHasTexture;//if it has a texture => of course it has a material (because the texture is a subset of the material)
	bool * objectHasMaterial;
	unsigned short * vertexIndices;// a series of 3 components, reserving the indices of each face of all objects in all models.
	Point3D * vertexNormals;// a series of 3 vector3D components of all the vertices in all models.
	UV * vertexTextureCoordinates;
	Texture * textures[MAX3DSTEXTURES];
	Texture ** textureId;

	int textureFileReferenceNumberCounter;
	
	bool Load3DSFiles(_3DSModel *models, int numberOfModels);
	void Render(Renderer * renderer);
	void Clean();
	
	// This computes the vertex normals for the object (used for lighting)
	void ComputeNormals();

	bool Save_3DSLoader(char * fileName);
	bool Load_3DSLoader(char * fileName);

	
private:
	// This reads in a string and saves it in the char array passed in
	int GetString(char *str);
	// This reads the next chunk
	void ReadChunk(_Chunk *chunk);
	// This reads the next large chunk
	void ProcessNextChunk(_3DSModel *model, _Chunk *previousChunk);
	// This reads the object chunks
	void ProcessNextObjectChunk(_3DSModel *model, _3DSObject *object, _Chunk *previousChunk);
	// This reads the material chunks
	void ProcessNextMaterialChunk(_3DSModel *model, _Chunk *previousChunk);
	// This reads the RGB value for the object's color
	void ReadColorChunk(_MaterialInfo *material, _Chunk *chunk, int coltype);
	// This reads the objects vertices
	void ReadVertices(_3DSObject *object, _Chunk *previousChunk);
	// This reads the objects face information
	void ReadVertexIndices(_3DSObject *object, _Chunk *previousChunk);
	// This reads the texture coodinates of the object
	void ReadUVCoordinates(_3DSObject *object, _Chunk *previousChunk);
	// This reads in the material name assigned to the object and sets the materialID
	void ReadObjectMaterial(_3DSModel *model, _3DSObject *object, _Chunk *previousChunk);
	// This frees memory and closes the file
	void CleanUp();

	// The file pointer
	FILE *filePointer;

	_3DSModel *_models;
};

//void Init3DSModel(_3DSModel &mod,char *fname);
//void Render3DSModel(Renderer * renderer,_3DSModel & mod);
//void Remove3DSModel(_3DSModel &mod);

// CHUNKS: What is a chunk anyway?
// 
// "The chunk ID is a unique code which identifies the type of data in this chunk
// and also may indicate the existence of subordinate chunks. The chunk length indicates
// the length of following data to be associated with this chunk. Note, this may
// contain more data than just this chunk. If the length of data is greater than that
// needed to fill in the information for the chunk, additional subordinate chunks are
// attached to this chunk immediately following any data needed for this chunk, and
// should be parsed out. These subordinate chunks may themselves contain subordinate chunks.
// Unfortunately, there is no indication of the length of data which is owned by the current
// chunk, only the total length of data attached to the chunk, which means that the only way
// to parse out subordinate chunks is to know the exact format of the owning chunk. On the
// other hand, if a chunk is unknown, the parsing program can skip the entire chunk and
// subordinate chunks in one jump. " - Jeff Lewis (werewolf@worldgate.com)
//
// Below is a list of the order that you will find the chunks and all the know chunks.
// If you go to www.wosit.org you can find the 3DS file format for a huge document explaining
// the little things I skipped.
//
//
//
//      MAIN3DS  (0x4D4D)
//     |
//     +--EDIT3DS  (0x3D3D)
//     |  |
//     |  +--EDIT_MATERIAL (0xAFFF)
//     |  |  |
//     |  |  +--MAT_NAME01 (0xA000) (See mli Doc) 
//     |  |
//     |  +--EDIT_CONFIG1  (0x0100)
//     |  +--EDIT_CONFIG2  (0x3E3D) 
//     |  +--EDIT_VIEW_P1  (0x7012)
//     |  |  |
//     |  |  +--TOP            (0x0001)
//     |  |  +--BOTTOM         (0x0002)
//     |  |  +--LEFT           (0x0003)
//     |  |  +--RIGHT          (0x0004)
//     |  |  +--FRONT          (0x0005) 
//     |  |  +--BACK           (0x0006)
//     |  |  +--USER           (0x0007)
//     |  |  +--CAMERA         (0xFFFF)
//     |  |  +--LIGHT          (0x0009)
//     |  |  +--DISABLED       (0x0010)  
//     |  |  +--BOGUS          (0x0011)
//     |  |
//     |  +--EDIT_VIEW_P2  (0x7011)
//     |  |  |
//     |  |  +--TOP            (0x0001)
//     |  |  +--BOTTOM         (0x0002)
//     |  |  +--LEFT           (0x0003)
//     |  |  +--RIGHT          (0x0004)
//     |  |  +--FRONT          (0x0005) 
//     |  |  +--BACK           (0x0006)
//     |  |  +--USER           (0x0007)
//     |  |  +--CAMERA         (0xFFFF)
//     |  |  +--LIGHT          (0x0009)
//     |  |  +--DISABLED       (0x0010)  
//     |  |  +--BOGUS          (0x0011)
//     |  |
//     |  +--EDIT_VIEW_P3  (0x7020)
//     |  +--EDIT_VIEW1    (0x7001) 
//     |  +--EDIT_BACKGR   (0x1200) 
//     |  +--EDIT_AMBIENT  (0x2100)
//     |  +--EDIT_OBJECT   (0x4000)
//     |  |  |
//     |  |  +--OBJ_TRIMESH   (0x4100)      
//     |  |  |  |
//     |  |  |  +--TRI_VERTEXL          (0x4110) 
//     |  |  |  +--TRI_VERTEXOPTIONS    (0x4111)
//     |  |  |  +--TRI_MAPPINGCOORS     (0x4140) 
//     |  |  |  +--TRI_MAPPINGSTANDARD  (0x4170)
//     |  |  |  +--TRI_FACEL1           (0x4120)
//     |  |  |  |  |
//     |  |  |  |  +--TRI_SMOOTH            (0x4150)   
//     |  |  |  |  +--TRI_MATERIAL          (0x4130)
//     |  |  |  |
//     |  |  |  +--TRI_LOCAL            (0x4160)
//     |  |  |  +--TRI_VISIBLE          (0x4165)
//     |  |  |
//     |  |  +--OBJ_LIGHT    (0x4600)
//     |  |  |  |
//     |  |  |  +--LIT_OFF              (0x4620)
//     |  |  |  +--LIT_SPOT             (0x4610) 
//     |  |  |  +--LIT_UNKNWN01         (0x465A) 
//     |  |  | 
//     |  |  +--OBJ_CAMERA   (0x4700)
//     |  |  |  |
//     |  |  |  +--CAM_UNKNWN01         (0x4710)
//     |  |  |  +--CAM_UNKNWN02         (0x4720)  
//     |  |  |
//     |  |  +--OBJ_UNKNWN01 (0x4710)
//     |  |  +--OBJ_UNKNWN02 (0x4720)
//     |  |
//     |  +--EDIT_UNKNW01  (0x1100)
//     |  +--EDIT_UNKNW02  (0x1201) 
//     |  +--EDIT_UNKNW03  (0x1300)
//     |  +--EDIT_UNKNW04  (0x1400)
//     |  +--EDIT_UNKNW05  (0x1420)
//     |  +--EDIT_UNKNW06  (0x1450)
//     |  +--EDIT_UNKNW07  (0x1500)
//     |  +--EDIT_UNKNW08  (0x2200)
//     |  +--EDIT_UNKNW09  (0x2201)
//     |  +--EDIT_UNKNW10  (0x2210)
//     |  +--EDIT_UNKNW11  (0x2300)
//     |  +--EDIT_UNKNW12  (0x2302)
//     |  +--EDIT_UNKNW13  (0x2000)
//     |  +--EDIT_UNKNW14  (0xAFFF)
//     |
//     +--KEYF3DS (0xB000)
//        |
//        +--KEYF_UNKNWN01 (0xB00A)
//        +--............. (0x7001) ( viewport, same as editor )
//        +--KEYF_FRAMES   (0xB008)
//        +--KEYF_UNKNWN02 (0xB009)
//        +--KEYF_OBJDES   (0xB002)
//           |
//           +--KEYF_OBJHIERARCH  (0xB010)
//           +--KEYF_OBJDUMMYNAME (0xB011)
//           +--KEYF_OBJUNKNWN01  (0xB013)
//           +--KEYF_OBJUNKNWN02  (0xB014)
//           +--KEYF_OBJUNKNWN03  (0xB015)  
//           +--KEYF_OBJPIVOT     (0xB020)  
//           +--KEYF_OBJUNKNWN04  (0xB021)  
//           +--KEYF_OBJUNKNWN05  (0xB022)  
//
// Once you know how to read chunks, all you have to know is the ID you are looking for
// and what data is stored after that ID.  You need to get the file format for that.
// I can give it to you if you want, or you can go to www.wosit.org for several versions.
// Because this is a proprietary format, it isn't a official document.
//
// I know there was a LOT of information blown over, but it is too much knowledge for
// one tutorial.  In the animation tutorial that I eventually will get to, some of
// the things explained here will be explained in more detail.

#endif