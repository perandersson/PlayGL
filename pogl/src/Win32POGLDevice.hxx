#pragma once
#include "POGLDevice.hxx"
#include "Win32POGLDeviceContext.hxx"
#include <vector>
#include <mutex>

class Win32POGLDevice : public POGLDevice
{
public:
	Win32POGLDevice();
	~Win32POGLDevice();

	IPOGLDeviceContext* GetContext();
	void SwapBuffers();
	bool Initialize(POGL_DEVICE_INFO* info);

private:
	HWND mWindowHandle;
	HDC mDeviceContext;
	Win32POGLDeviceContext* mMainThreadDeviceContext;

	std::vector<IPOGLDeviceContext*> mDeviceContexts;
	std::recursive_mutex mDeviceContextsMutex;
};
