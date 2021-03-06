#include "MemCheck.h"
#include "POGLDefaultBufferResourceProvider.h"
#include "POGLDefaultBufferResource.h"
#include "POGLEnum.h"

POGLDefaultBufferResourceProvider::POGLDefaultBufferResourceProvider()
{
}

POGLDefaultBufferResourceProvider::~POGLDefaultBufferResourceProvider()
{
}

IPOGLBufferResource* POGLDefaultBufferResourceProvider::CreateBuffer(POGL_UINT32 memorySize, GLenum target, POGLBufferUsage::Enum bufferUsage)
{
	return new POGLDefaultBufferResource(memorySize, target, bufferUsage);
}
