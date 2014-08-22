#include <gl/pogl.h>
#include <thread>
#include "POGLExampleWindow.h"

int main()
{
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOCHAR("Example: Blending"));
	POGL_DEVICE_INFO deviceInfo = { 0 };
#ifdef _DEBUG
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#else
	deviceInfo.flags = 0;
#endif
	deviceInfo.windowHandle = windowHandle;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;
	IPOGLDevice* device = POGLCreateDevice(&deviceInfo);

	try {
		IPOGLRenderContext* context = device->GetRenderContext();

		IPOGLShader* vertexShader = context->CreateShaderFromFile(POGL_TOCHAR("simple.vs"), POGLShaderType::VERTEX_SHADER);
		IPOGLShader* fragmentShader = context->CreateShaderFromFile(POGL_TOCHAR("simple.fs"), POGLShaderType::FRAGMENT_SHADER);
		IPOGLShader* shaders[] = { vertexShader, fragmentShader, nullptr };
		IPOGLProgram* program = context->CreateProgramFromShaders(shaders);
		vertexShader->Release();
		fragmentShader->Release();

		//
		// Create two triangles with different colors.
		//

		const POGL_POSITION_COLOR_VERTEX VERTICES[] = {
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-0.5f, -0.5f, 0.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.0f, 0.5f, 0.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.5f, -0.5f, 0.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		const POGL_POSITION_COLOR_VERTEX VERTICES_INV[] = {
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.5f, 0.5f, 0.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.0f, -0.5f, 0.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-0.5f, 0.5f, 0.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f))
		};
		IPOGLVertexBuffer* vertexBufferInv = context->CreateVertexBuffer(VERTICES_INV, sizeof(VERTICES_INV), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		//
		// Setup program properties. This can be done on the associated RenderState as well, but we do it on the program since 
		// we want this to be applied globally for this program.
		//
		// Changes made on the program will NOT be applied until the next time we call IPOGLRenderContext::Apply(IPOGLProgram*)
		//

		program->SetBlend(true);
		program->SetBlendFunc(POGLSrcFactor::SRC_COLOR, POGLDstFactor::ONE_MINUS_SRC_COLOR);

		//
		// Apply the program to this context
		// 
		// Since we are only using one program, and we are not changing any parameters on the program while rendering
		// it's recommended to apply the state only once.
		// 
		// POGL will prevent all unneccessary state changes, so trying to apply the same program more than once will not really do anything.
		//

		IPOGLRenderState* state = context->Apply(program);

		// 
		// Render loop
		//

		while (POGLProcessEvents()) {
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			//
			// Bind the normal vertex buffer and draw it onto the screen
			//

			state->Bind(vertexBuffer);
			state->Draw();

			//
			// Bind the inverted vertex buffer and draw it onto the screen
			//

			state->Bind(vertexBufferInv);
			state->Draw();

			//
			// End the current frame
			//

			device->EndFrame();
		}

		state->Release();

		//
		// Release resources
		//

		vertexBuffer->Release();
		vertexBufferInv->Release();
		program->Release();
		context->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	device->Release();
	POGLDestroyExampleWindow(windowHandle);
	return 0;
}
