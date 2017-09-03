// (C) 2017 Maciej Gajewski
#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	const glm::vec3 GetPosition() const { return pos_; }
	const glm::vec3 GetDirection() const { return direction_; }

	void Set(const glm::vec3& pos, const glm::vec3& direction, const glm::vec3& up)
	{
		pos_ = pos;
		direction_ = glm::normalize(direction);
		up_ = glm::normalize(up);
	}

	void SetPosition(const glm::vec3& pos) { pos_ = pos; }
	void LookAt(const glm::vec3& center) { direction_ = glm::normalize(center - pos_); }

	glm::mat4 CalculateTransformation() const;

private:
	glm::vec3 pos_ = {0.0f, 0.0f, 0.0f};
	glm::vec3 direction_ = {0.0f, 0.0f, 1.0f};
	glm::vec3 up_ = {0.0f, 1.0f, 0.0f};
};

