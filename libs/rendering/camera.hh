// (C) 2017 Maciej Gajewski
#pragma once

#include <glm/glm.hpp>

namespace Rendering {

class Camera
{
public:
	Camera();

	const glm::vec3 getPosition() const { return pos_; }
	const glm::vec3 getDirection() const { return direction_; }

	void set(const glm::vec3& pos, const glm::vec3& direction, const glm::vec3& up)
	{
		pos_ = pos;
		direction_ = glm::normalize(direction);
		up_ = glm::normalize(up);
	}

	void setPosition(const glm::vec3& pos) { pos_ = pos; }
	void lookAt(const glm::vec3& center) { direction_ = glm::normalize(center - pos_); }
	void setDirection(glm::vec3 d) { direction_ = glm::normalize(d); }

	glm::mat4 calculateTransformation() const;

private:
	glm::vec3 pos_ = {0.0f, 0.0f, 0.0f};
	glm::vec3 direction_ = {0.0f, 0.0f, 1.0f};
	glm::vec3 up_ = {0.0f, 1.0f, 0.0f};
};

}
