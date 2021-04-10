#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Window.h"

constexpr float BASE_SPEED = 1000000.0f;

Camera::Camera(Window* window, Settings s) :
	_window			( window   ),
	_settings		( s ),
	_projection		( glm::perspective(s._field_of_view, s._aspect, s._z_near, s._z_far)),
	_view			( glm::mat4(0) ),
	_direction		( glm::vec3(0) ),
	_position		( glm::vec3(0) ),
	_right			( glm::vec3(0) ),
	_up				( glm::vec3(0) )
{}

void Camera::update() {
	_direction = glm::vec3(
		cos(_settings._v_angle) * sin(_settings._h_angle),
		sin(_settings._v_angle),
		cos(_settings._v_angle) * cos(_settings._h_angle)
	);

	_right = glm::vec3(
		sin(_settings._h_angle - M_PI_2),
		0,
		cos(_settings._h_angle - M_PI_2)
	);

	_up = glm::cross(_right, _direction);

	_view = glm::lookAt(
		_position,
		_position + _direction,
		_up
	);
}

void Camera::move(const int direction, float time, float speed) {
	switch(direction) {
	case CAMERA_FORWARD :
		_position.x += _direction.x * (BASE_SPEED + speed) * time;
		_position.z += _direction.z * (BASE_SPEED + speed) * time;
		break;
	case CAMERA_BACKWARD:
		_position.x -= _direction.x * (BASE_SPEED + speed) * time;
		_position.z -= _direction.z * (BASE_SPEED + speed) * time;
		break;
	case CAMERA_LEFT:
		_position.x -= _right.x * (BASE_SPEED + speed) * time;
		_position.z -= _right.z * (BASE_SPEED + speed) * time;
		break;
	case CAMERA_RIGHT:
		_position.x += _right.x * (BASE_SPEED + speed) * time;
		_position.z += _right.z * (BASE_SPEED + speed) * time;
		break;
	case CAMERA_UP:
		_position.y += (BASE_SPEED + speed) * time;
		break;
	case CAMERA_DOWN:
		_position.y -= (BASE_SPEED + speed) * time;
		break;
	default:
		break;
	}
}

void Camera::move_angle(float xpos, float ypos) {
	int width, height;
	glfwGetWindowSize(_window->get(), &width, &height);
	_settings._h_angle += 0.001f * (((float)width / 2.0f) - xpos);
	_settings._v_angle += 0.001f * (((float)height / 2.0f) - ypos);
	glfwSetCursorPos(_window->get(), (double)width / 2.0, (double)height / 2.0);
}

glm::mat4 Camera::get_view() const {
	return _view;
}

glm::mat4 Camera::get_projection() const {
	return _projection;
}

glm::vec3 Camera::get_position() const {
	return _position;
}