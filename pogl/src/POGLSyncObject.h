#pragma once
#include "config.h"
#include <gl/pogl.h>
#include <mutex>

class POGLSyncObject
{
public:
	POGLSyncObject(GLsync initSync, IPOGLDevice* device);
	~POGLSyncObject();

	void WaitSyncDriver();
	void WaitSyncClient();
	bool WaitSyncClient(POGL_UINT64 timeout);
	bool WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job);
	
	/*!
		\brief Retrieves the sync object for this resource
	*/
	GLsync GetSyncObject();


private:
	std::recursive_mutex mSyncMutex;
	GLsync mSync;
	IPOGLDevice* mDevice;
};