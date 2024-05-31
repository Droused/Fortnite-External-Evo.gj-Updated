#include "sdk.h"

Vector3 SDK::GetBoneWithRotation(uintptr_t mesh, int id)
{
	uintptr_t bone_array = read<uintptr_t>(mesh + 0x5B8 + (read<int>(mesh + 0x600) * 0x10));
	FTransform bone = read<FTransform>(bone_array + (id * 0x60));
	FTransform component_to_world = read<FTransform>(mesh + 0x1c0);
	D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
	return Vector3(matrix._41, matrix._42, matrix._43);
}
struct CamewaDescwipsion
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
	char Useless[0x18];
};
//Camera SDK::GetViewAngles() {
//	char v1; // r8
//	CamewaDescwipsion ViewPoint = read<CamewaDescwipsion>(BaseId + 0x10CB8D0);
//	BYTE* v2 = (BYTE*)&ViewPoint;
//	int i; // edx
//	__int64 result; // rax
//
//	v1 = 0x40;
//	for (i = 0; i < 0x40; ++i)
//	{
//		*v2 ^= v1;
//		result = (unsigned int)(i + 0x17);
//		v1 += i + 0x17;
//		v2++;
//	}
//
//	return { ViewPoint.Location, ViewPoint.Rotation, ViewPoint.FieldOfView };
//}

Camera get_camera()
{
	Camera camera;

	auto uworld = LocalPtrs::Gworld;
	auto location_pointer = read<uintptr_t>(uworld + 0x110);
	auto rotation_pointer = read<uintptr_t>(uworld + 0x120);

	double fnRot[3];
	fnRot[0] = read<double>(rotation_pointer);
	fnRot[1] = read<double>(rotation_pointer + 0x20);
	fnRot[2] = read<double>(rotation_pointer + 0x1d0);

	camera.Location = read<Vector3>(location_pointer);

	camera.Rotation.x = asin(fnRot[2]) * (180.0 / M_PI);
	camera.Rotation.y = ((atan2(fnRot[0] * -1, fnRot[1]) * (180.0 / M_PI)) * -1) * -1;

	camera.FieldOfView = read<float>((uintptr_t)LocalPtrs::PlayerController + 0x394) * 90.f;

	return camera;
}


Vector2 SDK::ProjectWorldToScreen(Vector3 WorldLocation)
{

	vCamera = get_camera(); //get ur players newest view angles
	if (Debug::PrintLocations) {
		Util::Print3D("Location: ", vCamera.Location);
		Util::Print3D("Rotation: ", vCamera.Rotation);
		Util::Print2D("Fov: ", Vector2(vCamera.FieldOfView, 69));
	}

	D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector2((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z);
}
