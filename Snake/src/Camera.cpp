#include "Camera.h"

const float DEF_FOV = 45.0f;

glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

const glm::vec3& Camera::getLook(){
	return mLook;
}

const glm::vec3& Camera::getRight()
{
	return mRight;
}

const glm::vec3& Camera::getUp()
{
	return mUp;
}

Camera::Camera() : WORLD_UP(glm::vec3(0.0f, 1.0f, 0.0f)){
	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::vec3(0.0f, 0.0f, 0.0f);
	mYaw = glm::pi<float>();
	mPitch = 0.0f;
	mFOV = DEF_FOV;
}

//Orbit_camera

Orbit_Camera::Orbit_Camera(){
	mRadius = 10.0f;
}

void Orbit_Camera::rotate(float yaw, float pitch){
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
}

void Orbit_Camera::setLookAt(const glm::vec3& target){
	mTargetPos = target;
}

void Orbit_Camera::setRadius(float radius){
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void Orbit_Camera::updateCameraVectors(){
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}

//FPS camera

FPS_Camera::FPS_Camera(glm::vec3 position, float yaw, float pitch){
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

void FPS_Camera::setPosition(const glm::vec3& position){
	mPosition = position; 
}

void FPS_Camera::rotate(float yaw, float pitch){
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}

void FPS_Camera::move(const glm::vec3& offsetPos){
	mPosition += offsetPos;
	updateCameraVectors();
}

void FPS_Camera::updateCameraVectors(){
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
}
