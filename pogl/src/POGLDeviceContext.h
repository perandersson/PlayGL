#pragma once
#include "config.h"
#include <gl/pogl.h>

class POGLRenderState;
class POGLVertexBuffer;
class POGLIndexBuffer;
class POGLDeviceContext : public IPOGLDeviceContext
{
public:
	POGLDeviceContext(IPOGLDevice* device);
	~POGLDeviceContext();

	/*!
		\brief Destroys this instance and removes the internal release the associated memory with it
	*/
	void Destroy();

	/*!
		\brief Retrieves the render state
	*/
	inline POGLRenderState* GetRenderState() {
		return mRenderState;
	}

	virtual IPOGLDevice* GetDevice();
	virtual IPOGLShaderProgram* CreateShaderProgramFromFile(const POGL_CHAR* path, POGLShaderProgramType::Enum type);
	virtual IPOGLShaderProgram* CreateShaderProgramFromMemory(const POGL_CHAR* memory, POGL_UINT32 size, POGLShaderProgramType::Enum type);
	virtual IPOGLEffect* CreateEffectFromPrograms(IPOGLShaderProgram** programs, POGL_UINT32 numPrograms);
	virtual IPOGLTexture1D* CreateTexture1D();
	virtual IPOGLTexture2D* CreateTexture2D(const POGL_SIZEI& size, POGLTextureFormat::Enum format, const void* bytes);
	virtual IPOGLTexture3D* CreateTexture3D();
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const void* memory, POGL_SIZE memorySize, const POGL_VERTEX_LAYOUT* layout, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_COLOR_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLVertexBuffer* CreateVertexBuffer(const POGL_POSITION_TEXCOORD_VERTEX* memory, POGL_SIZE memorySize, POGLPrimitiveType::Enum primitiveType, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLIndexBuffer* CreateIndexBuffer(const void* memory, POGL_SIZE memorySize, POGLVertexType::Enum type, POGLBufferUsage::Enum bufferUsage);
	virtual IPOGLRenderState* Apply(IPOGLEffect* effect);
	virtual void* Map(IPOGLResource* resource, POGLResourceStreamType::Enum e);
	virtual void* Map(IPOGLResource* resource, POGL_UINT32 offset, POGL_UINT32 size, POGLResourceStreamType::Enum e);
	virtual void Unmap(IPOGLResource* resource);

	/*!
		\brief 
	*/
	void InitializeRenderState();
	
	/*!
		\brief Creates a vertex array ID
	*/
	GLuint GenVertexArray();

	/*!
		\brief Generate a new sampler ID
	*/
	GLuint GenSamplerID();

	/*!
		\brief Generate a new bufferID
	*/
	GLuint GenBufferID();

	/*!
		\brief Generate a new texture ID
	*/
	GLuint GenTextureID();

protected:
	IPOGLDevice* mDevice;
	POGLRenderState* mRenderState;
};
