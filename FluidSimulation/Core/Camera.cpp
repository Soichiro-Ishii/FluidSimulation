#include "pch.h"
#include "Camera.h"
void Camera::changeDim(CAMERA_DIMENSION dim) {
	m_dim = dim;
	if (m_dim == CAMERA_DIMENSION::DIM_3D) {
		m_data.cam3d = Camera3DData{};
	}
	else if (m_dim == CAMERA_DIMENSION::DIM_2D) {
		m_data.cam2d = Camera2DData{};
	}
}
void Camera::setPos3D(glm::vec3 pos) {
	if (m_dim != CAMERA_DIMENSION::DIM_3D) {
		spdlog::error("The camera is not for 3D.");
		return;
	}
	m_data.cam3d.pos = pos;
}
void Camera::setAng3D(glm::vec3 ang) {
	if (m_dim != CAMERA_DIMENSION::DIM_3D) {
		spdlog::error("The camera is not for 3D.");
		return;
	}
	m_data.cam3d.ang = ang;
}
void Camera::setCameraDist(float cameraDist) {
	if (m_dim != CAMERA_DIMENSION::DIM_3D) {
		spdlog::error("The camera is not for 3D.");
		return;
	}
	m_data.cam3d.cameraDist = cameraDist;
}
void Camera::setCamera3DMode(CAMERA3D_MODE mode) {
	if (m_dim != CAMERA_DIMENSION::DIM_3D) {
		spdlog::error("The camera is not for 3D.");
		return;
	}
	m_data.cam3d.camMode = mode;
}
glm::mat4 Camera::getView3D() {
	if (m_dim != CAMERA_DIMENSION::DIM_3D) {
		spdlog::error("The camera is not for 3D.");
		return glm::mat4(1.0f);
	}

	glm::vec3& pos = m_data.cam3d.pos;
	glm::vec3& ang = m_data.cam3d.ang;

	glm::mat4 rotation =
		glm::yawPitchRoll(ang.y, ang.x, ang.z);

	glm::vec3 forward =
		glm::vec3(rotation * glm::vec4(0, 0, 1, 0));

	glm::vec3 up =
		glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

	if (m_data.cam3d.camMode == CAMERA3D_MODE::FirstPerson) {
		return glm::lookAt(
			pos,
			pos + forward,
			up
		);
	}
	else {
		return glm::lookAt(
			pos - forward * m_data.cam3d.cameraDist,
			pos,
			up
		);
	}
}

void Camera::setPos2D(glm::vec2 pos) {
	if (m_dim != CAMERA_DIMENSION::DIM_2D) {
		spdlog::error("The camera is not for 2D.");
		return;
	}
	m_data.cam2d.pos = pos;
}
void Camera::setAng2D(float ang) {
	if (m_dim != CAMERA_DIMENSION::DIM_2D) {
		spdlog::error("The camera is not for 2D.");
		return;
	}
	m_data.cam2d.ang = ang;
}
void Camera::setScale2D(float scale) {
	if (m_dim != CAMERA_DIMENSION::DIM_2D) {
		spdlog::error("The camera is not for 2D.");
		return;
	}
	m_data.cam2d.scale = scale;
}
glm::mat4 Camera::getView2D() {
	if (m_dim != CAMERA_DIMENSION::DIM_2D) {
		spdlog::error("The camera is not for 2D.");
		return glm::mat4(1.0f);
	}
	glm::vec2& pos = m_data.cam2d.pos;
	float& ang = m_data.cam2d.ang;
	float& scale = m_data.cam2d.scale;
	glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(-pos, 0.0f));
	glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(1 / scale, 1 / scale, 1.0f));
	glm::mat4 r = glm::rotate(glm::mat4(1.0f), -ang, glm::vec3(0.0f, 0.0f, 1.0f));
	return r * s * t;
}