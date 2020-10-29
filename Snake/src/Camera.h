#ifndef CAMERA_H
#define CAMERA_H

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

class Camera {
public:
	glm::mat4 getViewMatrix();

	virtual void setPosition(const glm::vec3& positon) {}
	virtual void rotate(float yaw, float pitch) {} //in degrees
	virtual void move(const glm::vec3& offSetPos) {}

	const glm::vec3& getLook();
	const glm::vec3& getRight();
	const glm::vec3& getUp();

	float getFOV() { return mFOV; }
	void setFOV(float fov) { mFOV = fov; }

protected:
	Camera();

	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;
	glm::vec3 mLook;
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;

	float mFOV;

	//Eulers angles (in radians)
	float mYaw;
	float mPitch;
};

class FPS_Camera : public Camera {
public:
	FPS_Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);

	virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch); //in degrees
	virtual void move(const glm::vec3& offsetPos);
private:
	void updateCameraVectors();
};

class Orbit_Camera : public Camera {
public:
	Orbit_Camera();

	virtual void rotate(float yaw, float pitch); //in degrees

	void setLookAt(const glm::vec3& target);
	void setRadius(float radius);

private:
	void updateCameraVectors();
	float mRadius;
};

#endif

