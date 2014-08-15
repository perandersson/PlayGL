#pragma once
#include "POGLEffectState.h"
#include <memory>

class POGLDeviceContext;
class POGLVertexBuffer;
class POGLIndexBuffer;
class POGLTextureResource;
class POGLSamplerObject;
class POGLFramebuffer;
class POGLRenderState : public IPOGLRenderState
{
	typedef std::hash_map<POGL_UINT32, std::shared_ptr<POGLEffectState>> EffectStates;

public:
	POGLRenderState(POGLDeviceContext* context);
	virtual ~POGLRenderState();
	
	/*!
		\brief Applies the supplied effect to this render state

		\param effect
	*/
	void Apply(IPOGLEffect* effect);

	/*!
		\brief Retrieves the effect state for the supplied effect

		\return effect
	*/
	POGLEffectState* GetEffectState(POGLEffect* effect);


	/*!
		\brief Check to see if the current effect is of the supplied type
	*/
	inline bool IsEffectActive(POGL_UINT32 effectUID) const {
		return mEffectUID == effectUID;
	}

	/*!
		\brief Bind the supplied sampler object
	*/
	void BindSamplerObject(POGLSamplerObject* samplerObject, POGL_UINT32 idx);
	
	/*!
		\brief Bind the supplied texture handle

		\param texture
		\param idx
	*/
	void BindTextureResource(POGLTextureResource* texture, POGL_UINT32 idx);
	
	/*!
		\brief Set the currently bound texture resource

		\param texture
	*/
	void SetTextureResource(POGLTextureResource* texture);

	/*!
		\brief Set the currently bound vertex buffer

		\param vertexBuffer
	*/
	void SetVertexBuffer(POGLVertexBuffer* vertexBuffer);
	
	/*!
		\brief Set the currently bound vertex buffer

		\param vertexBuffer
	*/
	void SetIndexBuffer(POGLIndexBuffer* indexBuffer);

	/*!
		\brief Set the currently bound framebuffer

		\param framebuffer
	*/
	void SetFramebuffer(POGLFramebuffer* framebuffer);

	/*!
		\brief Retrieves the next active texture for this render state.
	*/
	POGL_UINT32 NextActiveTexture();
	
	/*!
		\brief Bind the supplied vertex buffer

		\param buffer
	*/
	void BindVertexBuffer(POGLVertexBuffer* buffer);

	/*!
		\brief Bind the supplied index buffer

		\param buffer
	*/
	void BindIndexBuffer(POGLIndexBuffer* buffer);

// IPOGLInterface
public:
	virtual void AddRef();
	virtual void Release();

// IPOGLRenderState
public:
	virtual IPOGLDevice* GetDevice();
	virtual IPOGLDeviceContext* GetDeviceContext();
	virtual void Clear(POGL_UINT32 clearBits);
	virtual IPOGLUniform* FindUniformByName(const POGL_CHAR* name);
	virtual void SetFramebuffer(IPOGLFramebuffer* framebuffer);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex);
	virtual void Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count);
	virtual void SetDepthTest(bool b);
	virtual void SetDepthFunc(POGLDepthFunc::Enum depthFunc);
	virtual void SetDepthMask(bool b);
	virtual void SetColorMask(POGL_UINT8 mask);
	virtual void SetStencilTest(bool b);
	virtual void SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor);
	virtual void SetBlend(bool b);
	virtual void SetViewport(const POGL_RECTI& viewport);

private:
	/*!
		\brief Binds the supplied effect to this state

		\param effect
	*/
	void BindEffect(POGLEffect* effect);

	/*!
		\brief Binds the supplied buffers
	*/
	void BindBuffers(POGLVertexBuffer* vertexBuffer, POGLIndexBuffer* indexBuffer);
	
private:
	std::atomic<POGL_UINT32> mRefCount;
	POGLDeviceContext* mDeviceContext;

	IPOGLEffect* mEffect;
	POGL_UINT32 mEffectUID;
	POGLEffectState* mCurrentEffectState;
	EffectStates mEffectStates;
	bool mApplyCurrentEffectState;

	IPOGLVertexBuffer *mVertexBuffer;
	POGL_UINT32 mVertexBufferUID;
	IPOGLIndexBuffer* mIndexBuffer;
	POGL_UINT32 mIndexBufferUID;

	//
	// Properties
	//

	bool mDepthTest;
	POGLDepthFunc::Enum mDepthFunc;
	bool mDepthMask;
	POGL_UINT8 mColorMask;
	bool mStencilTest;
	POGLSrcFactor::Enum mSrcFactor;
	POGLDstFactor::Enum mDstFactor;
	bool mBlending;
	POGL_RECTI mViewport;

	//
	// Textures
	//

	POGL_UINT32 mMaxActiveTextures;
	POGL_UINT32 mNextActiveTexture;
	std::vector<POGL_UINT32> mTextureUID;
	std::vector<POGLTextureResource*> mTextures;
	std::vector<POGL_UINT32> mSamplerObjectUID;
	POGL_UINT32 mActiveTextureIndex;

	//
	// Framebuffer
	//

	POGLFramebuffer* mFramebuffer;
	POGL_UINT32 mFramebufferUID;
};
