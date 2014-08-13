#include "MemCheck.h"
#include "POGLUniformFloat.h"
#include "POGLDeviceContext.h"

POGLUniformFloat::POGLUniformFloat(const POGLEffect* effect, POGLRenderState* state, POGLDeviceContext* context, GLint componentID)
: POGLDefaultUniform(effect, state, context, componentID), mCount(0)
{
	mValues[0] = 0;
	mValues[1] = 0;
	mValues[2] = 0;
	mValues[3] = 0;

	mValuesSet[0] = FLT_MAX;
	mValuesSet[1] = FLT_MAX;
	mValuesSet[2] = FLT_MAX;
	mValuesSet[3] = FLT_MAX;
}

POGLUniformFloat::~POGLUniformFloat()
{
}

void POGLUniformFloat::Apply()
{
	switch (mCount) {
	case 3:
		if (FLT_EQ(mValuesSet[0], mValues[0]) && FLT_EQ(mValuesSet[1], mValues[1]) && FLT_EQ(mValuesSet[2], mValues[2])) return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		glUniform3fv(mComponentID, 1, mValues);
		break;
	case 2:
		if (FLT_EQ(mValuesSet[0], mValues[0]) && FLT_EQ(mValuesSet[1], mValues[1])) return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		glUniform2fv(mComponentID, 1, mValues);
		break;
	case 4:
		if (FLT_EQ(mValuesSet[0], mValues[0]) && FLT_EQ(mValuesSet[1], mValues[1]) && FLT_EQ(mValuesSet[2], mValues[2]) && FLT_EQ(mValuesSet[3], mValues[3])) return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		mValuesSet[3] = mValues[3];
		glUniform4fv(mComponentID, 1, mValues);
		break;
	case 1:
		if (FLT_EQ(mValuesSet[0], mValues[0])) return;
		mValuesSet[0] = mValues[0];
		glUniform1fv(mComponentID, 1, mValues);
		break;
	default:
		return;
	}

	CHECK_GL("Could not assign float uniform values");
}

void POGLUniformFloat::SetFloat(POGL_FLOAT a)
{
	mCount = 1;
	mValues[0] = a;
	mValues[1] = FLT_MAX;
	mValues[2] = FLT_MAX;
	mValues[3] = FLT_MAX;

	if (IsEffectActive())
		POGLUniformFloat::Apply();
}

void POGLUniformFloat::SetFloat(POGL_FLOAT a, POGL_FLOAT b)
{
	mCount = 2;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = FLT_MAX;
	mValues[3] = FLT_MAX;

	if (IsEffectActive())
		POGLUniformFloat::Apply();
}

void POGLUniformFloat::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c)
{
	mCount = 3;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = FLT_MAX;

	if (IsEffectActive())
		POGLUniformFloat::Apply();
}

void POGLUniformFloat::SetFloat(POGL_FLOAT a, POGL_FLOAT b, POGL_FLOAT c, POGL_FLOAT d)
{
	mCount = 4;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = d;

	if (IsEffectActive())
		POGLUniformFloat::Apply();
}

void POGLUniformFloat::SetFloat(POGL_FLOAT* ptr, POGL_UINT32 count)
{
	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mValues[i] = ptr[i];
}

void POGLUniformFloat::SetDouble(POGL_DOUBLE a)
{
	SetFloat((POGL_FLOAT)a);
}

void POGLUniformFloat::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b)
{
	SetFloat((POGL_FLOAT)a, (POGL_FLOAT)b);
}

void POGLUniformFloat::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c)
{
	SetFloat((POGL_FLOAT)a, (POGL_FLOAT)b, (POGL_FLOAT)c);
}

void POGLUniformFloat::SetDouble(POGL_DOUBLE a, POGL_DOUBLE b, POGL_DOUBLE c, POGL_DOUBLE d)
{
	SetFloat((POGL_FLOAT)a, (POGL_FLOAT)b, (POGL_FLOAT)c, (POGL_FLOAT)d);
}

void POGLUniformFloat::SetDouble(POGL_DOUBLE* ptr, POGL_UINT32 count)
{
	const POGL_UINT32 clampedCount = count > 4 ? 4 : count;
	for (POGL_UINT32 i = 0; i < clampedCount; ++i)
		mValues[i] = (POGL_FLOAT)ptr[i];
}

void POGLUniformFloat::SetVector2F(const POGL_VECTOR2F& vec)
{
	SetFloat(vec.x, vec.y);
}

void POGLUniformFloat::SetVector2D(const POGL_VECTOR2D& vec)
{
	SetFloat((POGL_FLOAT)vec.x, (POGL_FLOAT)vec.y);
}

void POGLUniformFloat::SetVector3F(const POGL_VECTOR3F& vec)
{
	SetFloat(vec.x, vec.y, vec.z);
}

void POGLUniformFloat::SetVector3D(const POGL_VECTOR3D& vec)
{
	SetFloat((POGL_FLOAT)vec.x, (POGL_FLOAT)vec.y, (POGL_FLOAT)vec.z);
}

void POGLUniformFloat::SetVector4F(const POGL_VECTOR4F& vec)
{
	SetFloat(vec.x, vec.y, vec.z, vec.w);
}

void POGLUniformFloat::SetVector4D(const POGL_VECTOR4D& vec)
{
	SetFloat((POGL_FLOAT)vec.x, (POGL_FLOAT)vec.y, (POGL_FLOAT)vec.z, (POGL_FLOAT)vec.w);
}
