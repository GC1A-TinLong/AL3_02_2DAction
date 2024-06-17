#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include <assert.h>
#include <cmath>

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

Vector3& operator+=(Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(float scalar, const Vector3& v);
Vector3 operator*(const Vector3& v, float scalar);

float Dot(Vector3& v1, Vector3& v2);
float Length(const Vector3& v);
Vector3 Normailize(const Vector3& v);
Vector3 Cross(const Vector3& v1, const Vector3& v2);

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Inverse(const Matrix4x4& m);
Matrix4x4 Transpose(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Matrix4x4 MakeRotateXMatrix(const float& radian);
Matrix4x4 MakeRotateYMatrix(const float& radian);
Matrix4x4 MakeRotateZMatrix(const float& radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeViewMatrix(const ViewProjection& viewProjection);
Matrix4x4 MakeProjectionMatrix(const ViewProjection& viewProjection);
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);