#pragma once
#include "IPOGLBufferResource.h"
#include "POGLBufferResourceLock.h"

class POGLRenderState;
class POGLDefaultBufferResource : public IPOGLBufferResource
{
public:
	POGLDefaultBufferResource(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage);
	virtual ~POGLDefaultBufferResource();
	
	/*!
		\brief Retrieves the internal OpenGL buffer ID
	*/
	inline GLuint GetBufferID() const {
		return mBufferID;
	}

// IPOGLBufferResource
public:
	virtual GLuint PostConstruct(POGLRenderState* renderState);
	virtual void* Map(POGLResourceMapType::Enum e);
	virtual void* Map(POGL_UINT32 offset, POGL_UINT32 length, POGLResourceMapType::Enum e);
	virtual void Unmap();
	virtual void Lock();
	virtual void Lock(POGL_UINT32 offset, POGL_UINT32 length);
	virtual void Unlock();

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

private:
	REF_COUNTER mRefCount;
	GLuint mBufferID;
	POGL_UINT32 mMemorySize;
	GLenum mTarget;
	POGLBufferUsage::Enum mBufferUsage;

	POGLBufferResourceLock mLock;
};
