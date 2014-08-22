#include "MemCheck.h"
#include "POGLDeferredRenderState.h"
#include "POGLDeferredRenderContext.h"
#include "POGLFramebuffer.h"
#include "POGLVertexBuffer.h"
#include "POGLIndexBuffer.h"
#include "uniforms/POGLDeferredUniform.h"

POGLDeferredRenderState::POGLDeferredRenderState(POGLDeferredRenderContext* context)
: mRefCount(1), mRenderContext(context),
mFramebuffer(0), mVertexBuffer(0), mIndexBuffer(0), mDepthTest(false),
mDepthFunc(POGLDepthFunc::DEFAULT), mDepthMask(false), mColorMask(POGLColorMask::ALL), mStencilTest(false),
mStencilMask(BIT_ALL), mSrcFactor(POGLSrcFactor::DEFAULT), mDstFactor(POGLDstFactor::DEFAULT), mBlend(false),
mFrontFace(POGLFrontFace::DEFAULT), mCullFace(POGLCullFace::DEFAULT), mViewport(POGL_RECT(0, 0, 0, 0))

{
}

POGLDeferredRenderState::~POGLDeferredRenderState()
{
}

void POGLDeferredRenderState::Flush()
{
	mFramebuffer.Unset();
	mVertexBuffer.Unset();
	mIndexBuffer.Unset();
	mDepthTest.Unset();
	mDepthFunc.Unset();
	mDepthMask.Unset();
	mColorMask.Unset();
	mStencilTest.Unset();
	mStencilMask.Unset();
	mSrcFactor.Unset();
	mDstFactor.Unset();
	mBlend.Unset();
	mFrontFace.Unset();
	mCullFace.Unset();
	mViewport.Unset();
}

void POGLDeferredRenderState::AddRef()
{
	mRefCount++;
}

void POGLDeferredRenderState::Release()
{
	if (--mRefCount == 0) {
		auto it = mUniforms.begin();
		auto end = mUniforms.end();
		for (; it != end; ++it) {
			delete it->second;
		}

		delete this;
	}
}

void POGLDeferredRenderState::Clear(POGL_UINT32 clearBits)
{
	POGL_CLEAR_COMMAND_DATA* cmd = (POGL_CLEAR_COMMAND_DATA*)mRenderContext->AddCommand(&POGLClear_Command, &POGLNothing_Release,
		sizeof(POGL_CLEAR_COMMAND_DATA));
	cmd->clearBits = clearBits;
}

IPOGLUniform* POGLDeferredRenderState::FindUniformByName(const POGL_CHAR* name)
{
	const POGL_STRING sname(name);
	auto it = mUniforms.find(sname);
	if (it == mUniforms.end()) {
		POGLDeferredUniform* uniform = new POGLDeferredUniform(sname, mRenderContext);
		mUniforms.insert(std::make_pair(sname, uniform));
		return uniform;
	}

	return it->second;
}

void POGLDeferredRenderState::SetFramebuffer(IPOGLFramebuffer* framebuffer)
{
	POGLFramebuffer* impl = static_cast<POGLFramebuffer*>(framebuffer);
	const POGL_UINT32 uid = framebuffer != nullptr ? impl->GetUID() : 0;
	if (mFramebuffer.Set(uid)) {
		POGL_SETFRAMEBUFFER_COMMAND_DATA* cmd = (POGL_SETFRAMEBUFFER_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetFramebuffer_Command, &POGLSetFramebuffer_Release,
			sizeof(POGL_SETFRAMEBUFFER_COMMAND_DATA));
		cmd->framebuffer = impl;
		if (framebuffer)
			framebuffer->AddRef();
	}
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer)
{
	assert_not_null(vertexBuffer);

	Draw(vertexBuffer, nullptr, 0);
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer)
{
	assert_not_null(vertexBuffer);

	Draw(vertexBuffer, indexBuffer, 0);
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex)
{
	assert_not_null(vertexBuffer);

	POGLVertexBuffer* vbImpl = static_cast<POGLVertexBuffer*>(vertexBuffer);
	const POGL_UINT32 vbUID = vbImpl != nullptr ? vbImpl->GetUID() : 0;
	POGLIndexBuffer* ibImpl = static_cast<POGLIndexBuffer*>(indexBuffer);
	const POGL_UINT32 ibUID = ibImpl != nullptr ? ibImpl->GetUID() : 0;

	bool changed = mVertexBuffer.Set(vbUID);
	changed |= mIndexBuffer.Set(ibUID);
	if (changed) {
		POGL_DRAW_COMMAND_DATA* cmd = (POGL_DRAW_COMMAND_DATA*)mRenderContext->AddCommand(&POGLDraw_Command, &POGLDraw_Release,
			sizeof(POGL_DRAW_COMMAND_DATA));
		cmd->vertexBuffer = vertexBuffer;
		vertexBuffer->AddRef();
		cmd->indexBuffer = indexBuffer;
		if (indexBuffer != nullptr)
			indexBuffer->AddRef();
		cmd->startIndex = startIndex;
	}
}

void POGLDeferredRenderState::Draw(IPOGLVertexBuffer* vertexBuffer, IPOGLIndexBuffer* indexBuffer, POGL_UINT32 startIndex, POGL_UINT32 count)
{
	assert_not_null(vertexBuffer);

	POGLVertexBuffer* vbImpl = static_cast<POGLVertexBuffer*>(vertexBuffer);
	const POGL_UINT32 vbUID = vbImpl != nullptr ? vbImpl->GetUID() : 0;
	POGLIndexBuffer* ibImpl = static_cast<POGLIndexBuffer*>(indexBuffer);
	const POGL_UINT32 ibUID = ibImpl != nullptr ? ibImpl->GetUID() : 0;

	bool changed = mVertexBuffer.Set(vbUID);
	changed |= mIndexBuffer.Set(ibUID);
	if (changed) {
		POGL_DRAW_COMMAND_DATA* cmd = (POGL_DRAW_COMMAND_DATA*)mRenderContext->AddCommand(&POGLDrawCount_Command, &POGLDraw_Release,
			sizeof(POGL_DRAW_COMMAND_DATA));
		cmd->vertexBuffer = vertexBuffer;
		vertexBuffer->AddRef();
		cmd->indexBuffer = indexBuffer;
		if (indexBuffer != nullptr)
			indexBuffer->AddRef();
		cmd->startIndex = startIndex;
		cmd->count = count;
	}
}

void POGLDeferredRenderState::SetDepthTest(bool b)
{
	if (mDepthTest.Set(b)) {
		POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetDepthTest_Command, &POGLNothing_Release,
			sizeof(POGL_BOOLEAN_COMMAND_DATA));
		cmd->value = b;
	}
}

void POGLDeferredRenderState::SetDepthFunc(POGLDepthFunc::Enum depthFunc)
{
	if (mDepthFunc.Set(depthFunc)) {
		POGL_SETDEPTHFUNC_COMMAND_DATA* cmd = (POGL_SETDEPTHFUNC_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetDepthFunc_Command, &POGLNothing_Release,
			sizeof(POGL_SETDEPTHFUNC_COMMAND_DATA));
		cmd->depthFunc = depthFunc;
	}
}

void POGLDeferredRenderState::SetDepthMask(bool b)
{
	if (mDepthMask.Set(b)) {
		POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetDepthMask_Command, &POGLNothing_Release,
			sizeof(POGL_BOOLEAN_COMMAND_DATA));
		cmd->value = b;
	}
}

void POGLDeferredRenderState::SetColorMask(POGL_UINT8 mask)
{
	if (mColorMask.Set(mask)) {
		POGL_COLORMASK_COMMAND_DATA* cmd = (POGL_COLORMASK_COMMAND_DATA*)mRenderContext->AddCommand(&POGLColorMask_Command, &POGLNothing_Release,
			sizeof(POGL_COLORMASK_COMMAND_DATA));
		cmd->mask = mask;
	}
}

void POGLDeferredRenderState::SetStencilTest(bool b)
{
	if (mStencilTest.Set(b)) {
		POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetStencilTest_Command, &POGLNothing_Release,
			sizeof(POGL_BOOLEAN_COMMAND_DATA));
		cmd->value = b;
	}
}

void POGLDeferredRenderState::SetStencilMask(POGL_UINT32 mask)
{
	if (mStencilMask.Set(mask)) {
		POGL_STENCILMASK_COMMAND_DATA* cmd = (POGL_STENCILMASK_COMMAND_DATA*)mRenderContext->AddCommand(&POGLStencilMask_Command, &POGLNothing_Release,
			sizeof(POGL_STENCILMASK_COMMAND_DATA));
		cmd->mask = mask;
	}
}

void POGLDeferredRenderState::SetBlendFunc(POGLSrcFactor::Enum sfactor, POGLDstFactor::Enum dfactor)
{
	bool changed = mSrcFactor.Set(sfactor);
	changed |= mDstFactor.Set(dfactor);
	if (changed) {
		POGL_SETBLENDFUNC_COMMAND_DATA* cmd = (POGL_SETBLENDFUNC_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetBlendFunc_Command, &POGLNothing_Release,
			sizeof(POGL_SETBLENDFUNC_COMMAND_DATA));
		cmd->sfactor = sfactor;
		cmd->dfactor = dfactor;
	}
}

void POGLDeferredRenderState::SetBlend(bool b)
{
	if (mBlend.Set(b)) {
		POGL_BOOLEAN_COMMAND_DATA* cmd = (POGL_BOOLEAN_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetBlend_Command, &POGLNothing_Release,
			sizeof(POGL_BOOLEAN_COMMAND_DATA));
		cmd->value = b;
	}
}

void POGLDeferredRenderState::SetFrontFace(POGLFrontFace::Enum e)
{
	if (mFrontFace.Set(e)) {
		POGL_SETFRONTFACE_COMMAND_DATA* cmd = (POGL_SETFRONTFACE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetFrontFace_Command, &POGLNothing_Release,
			sizeof(POGL_SETFRONTFACE_COMMAND_DATA));
		cmd->frontFace = e;
	}
}

void POGLDeferredRenderState::SetCullFace(POGLCullFace::Enum e)
{
	if (mCullFace.Set(e)) {
		POGL_SETCULLFACE_COMMAND_DATA* cmd = (POGL_SETCULLFACE_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetCullFace_Command, &POGLNothing_Release,
			sizeof(POGL_SETCULLFACE_COMMAND_DATA));
		cmd->cullFace = e;
	}
}

void POGLDeferredRenderState::SetViewport(const POGL_RECT& viewport)
{
	if (mViewport.Set(viewport)) {
		POGL_SETVIEWPORT_COMMAND_DATA* cmd = (POGL_SETVIEWPORT_COMMAND_DATA*)mRenderContext->AddCommand(&POGLSetViewport_Command, &POGLNothing_Release,
			sizeof(POGL_SETVIEWPORT_COMMAND_DATA));
		cmd->viewport = viewport;
	}
}

