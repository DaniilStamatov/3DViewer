#include "../transformations/transformation.h"

#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 2.5f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
   public:
    Camera(s21::Vector3 position = s21::Vector3(0.0f, 0.0f, -3.0f), s21::Vector3 up = s21::Vector3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH)
        : m_front(s21::Vector3(0.0f, 0.0f, 1.0f)),
          m_movementSpeed(SPEED),
          m_mouseSensitivity(SENSITIVITY),
          m_zoom(ZOOM) {
        m_position = position;
        m_worldUp = up;
        m_yaw = yaw;
        m_pitch = pitch;
        updateCameraVectors();
    }

    s21::Matrix4x4 GetViewMatrix() { return s21::LookAt(m_position, m_position + m_front, m_up); }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = m_movementSpeed * deltaTime;
        if (direction == FORWARD) m_position = m_position + (m_front * velocity);
        if (direction == BACKWARD) m_position = m_position - (m_front * velocity);
        if (direction == LEFT) m_position = m_position - (m_right * velocity);
        if (direction == RIGHT) m_position = m_position + (m_right * velocity);
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (constrainPitch) {
            if (m_pitch > 89.0f) m_pitch = 89.0f;
            if (m_pitch < -89.0f) m_pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void updateCameraVectors() {
        // calculate the new Front vector
        s21::Vector3 front;
        front.x = cos(m_yaw * (M_PI / 180.0)) * cos(m_pitch * (M_PI / 180.0));
        front.y = sin(m_pitch * (M_PI / 180.0));
        front.z = sin(m_yaw * (M_PI / 180.0)) * cos(m_pitch * (M_PI / 180.0));
           if (front.length() > 0) {
        m_front = s21::normalize(front);
    }

    // also re-calculate the Right and Up vector
    m_right = s21::normalize(m_front.cross(m_worldUp));
    m_up = s21::normalize(m_right.cross(m_front));
    }

    void ProcessMouseScroll(float yoffset) {
        m_zoom -= (float)yoffset;
        if (m_zoom < 1.0f) m_zoom = 1.0f;
        if (m_zoom > 45.0f) m_zoom = 45.0f;
    }

    float GetZoom() const { return m_zoom; }
    s21::Vector3 GetPosition() const {return m_position;}
   private:
    s21::Vector3 m_position;
    s21::Vector3 m_front;
    s21::Vector3 m_up;
    s21::Vector3 m_right;
    s21::Vector3 m_worldUp;
    // euler Angles
    float m_yaw;
    float m_pitch;
    // camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
};