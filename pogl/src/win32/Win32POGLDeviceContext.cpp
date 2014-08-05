#include "MemCheck.h"
#include "Win32POGLDeviceContext.h"
#include <vector>

Win32POGLDeviceContext::Win32POGLDeviceContext(IPOGLDevice* device, HDC deviceContext)
: POGLDeviceContext(device), mRefCount(0), mDeviceContext(deviceContext),
mRenderContext(nullptr), mBoundToThread(false),
// Extensions
wglCreateContextAttribsARB(nullptr)
{

}

Win32POGLDeviceContext::~Win32POGLDeviceContext()
{
}

bool Win32POGLDeviceContext::Initialize(Win32POGLDeviceContext* parentContext)
{
	// Create a temp render context so that you can load the extensions
	HGLRC temp = wglCreateContext(mDeviceContext);
	if (!temp)
		return false;
	wglMakeCurrent(mDeviceContext, temp);

	// Load extensions
	this->LoadExtensions();

	// Verify that your graphics card support OpenGL 3.3
	if (wglCreateContextAttribsARB == nullptr) {
		wglMakeCurrent(0, 0);
		return false;
	}

	// Attributes
	std::vector<int> attributes;
	attributes.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB); attributes.push_back(3);
	attributes.push_back(WGL_CONTEXT_MINOR_VERSION_ARB); attributes.push_back(3);
	if (BIT_ISSET(mDevice->GetDeviceInfo()->flags, POGLDeviceInfoFlags::DEBUG_MODE)) {
		attributes.push_back(WGL_CONTEXT_FLAGS_ARB); attributes.push_back(WGL_CONTEXT_DEBUG_BIT_ARB);
	}
	attributes.push_back(0);

	// Create an OpenGL 3.3 render context
	HGLRC sharedRenderContext = parentContext == nullptr ? nullptr : parentContext->GetHandlePtr();
	mRenderContext = wglCreateContextAttribsARB(mDeviceContext, sharedRenderContext, &attributes[0]);
	wglMakeCurrent(0, 0);
	wglDeleteContext(temp);
	if (mRenderContext == nullptr)
		return false;
	
	return POGLDeviceContext::Initialize();
}

void Win32POGLDeviceContext::AddRef()
{
	mRefCount++;
}

void Win32POGLDeviceContext::Release()
{
	if (--mRefCount == 0) {
		assert_with_message(mBoundToThread, "Why is this not bound to the current thread?");
		wglMakeCurrent(0, 0);
		mBoundToThread = false;
	}
}

Win32POGLDeviceContext* Win32POGLDeviceContext::BindContextIfNeccessary()
{
	if (!mBoundToThread) {
		wglMakeCurrent(mDeviceContext, mRenderContext);
		mBoundToThread = true;
	}

	AddRef();
	return this;
}

void Win32POGLDeviceContext::LoadExtensions()
{
	SET_EXTENSION_FUNC(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);

	POGLDeviceContext::LoadExtensions();
}

void* Win32POGLDeviceContext::GetProcAddress(const char* functionName)
{
	return wglGetProcAddress(functionName);
}