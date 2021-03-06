#pragma once
#include <gl/pogl.h>

/*!
	\brief Create an example window

	\param size
	\param title
	\return
*/
extern POGL_HANDLE POGLCreateExampleWindow(const POGL_SIZE& size, const POGL_CHAR* title);

/*!
	\brief Process events raised by the example window
*/
extern bool POGLProcessEvents();

/*!
	\brief Displays a alert popup

	\param windowHandle
	\param e
*/
extern void POGLAlert(const POGLException& e);

/*!
	\brief Destroys the example window
*/
extern void POGLDestroyExampleWindow(POGL_HANDLE windowHandle);

/*!

*/
extern POGL_FLOAT POGLGetTimeSinceLastTick();