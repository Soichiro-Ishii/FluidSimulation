#pragma once
#include<glm/glm.hpp>
enum class CAMERA3D_MODE {
	FirstPerson,
	ThirdPerson
};
struct Camera3DData {
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ang = glm::vec3(0.0f, 0.0f, 0.0f);
	float cameraDist = 5.0f;					//3人称用
	CAMERA3D_MODE camMode = CAMERA3D_MODE::FirstPerson;
};
struct Camera2DData {
	glm::vec2 pos = glm::vec2(0.0f, 0.0f);
	float ang = 0.0f;
	float scale = 1.0f;
};
union CameraData {
	Camera3DData cam3d;
	Camera2DData cam2d;
	CameraData() {
		cam3d = Camera3DData{};
	};
	~CameraData() = default;
};
enum class CAMERA_DIMENSION {
	DIM_3D,
	DIM_2D
};
class Camera
{
private:
	CameraData m_data;
	CAMERA_DIMENSION m_dim = CAMERA_DIMENSION::DIM_3D;
public:
	Camera() = default;
public:
	//---------------3D---------------//
	void changeDim(CAMERA_DIMENSION dim);
	void setPos3D(glm::vec3 pos);
	void setAng3D(glm::vec3 ang);
	void setCameraDist(float cameraDist);
	void setCamera3DMode(CAMERA3D_MODE mode);
	glm::mat4 getView3D();
	//---------------3D---------------//
public:
	//---------------2D---------------//
	void setPos2D(glm::vec2 pos);
	void setAng2D(float ang);
	void setScale2D(float scale);
	glm::mat4 getView2D();
	//---------------2D---------------//;
};

