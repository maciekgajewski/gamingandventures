// (C) 2017 Maciej Gajewski
#include "camera.hh"

#include <glm/gtc/matrix_transform.hpp>

namespace Rendering {

Camera::Camera()
{

}

glm::mat4 Camera::calculateTransformation() const
{
	return glm::lookAt(pos_, pos_+direction_, up_);
}

}
