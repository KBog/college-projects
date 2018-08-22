#ifndef BUFFERS_H
#define BUFFERS_H

enum BufferUsage
{
	BufferUsageStreamDraw,
	BufferUsageStreamRead,
	BufferUsageStreamCopy,
	BufferUsageStaticDraw,
	BufferUsageStaticRead,
	BufferUsageStaticCopy,
	BufferUsageDynamicDraw,
	BufferUsageDynamicRead,
	BufferUsageDynamicCopy
};

enum BufferAccess
{
    BufferAccessRead,
	BufferAccessWrite,
	BufferAccessReadWrite
};


class VertexBuffer
{
public:
	static VertexBuffer* Construct(unsigned int size, BufferUsage bufferUsage);
	static void Destruct(VertexBuffer*);

	bool Update(unsigned int offset, unsigned int size, const Vertex*) = 0;
	VertexBuffer* Lock(BufferAccess bufferAccess) = 0;
	void Unlock() = 0;
	BufferUsage GetBufferUsage() = 0;
	unsigned int GetSize() = 0;
};


#endif