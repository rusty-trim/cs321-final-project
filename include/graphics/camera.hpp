#include "matrix_float4x4.hpp"
#include "matrix_transform.hpp"
#include "vector_float3.hpp"

enum CameraDirection
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
  private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    float m_yaw = -90.0f, m_pitch = 0.0f;
    float m_speed = 5.0f, m_sensitivity = 0.1f;

  public:
    Camera(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &up);
    ~Camera();

    glm::mat4 get_view_matrix() const
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void handle_mouse(float dx, float dy);
    void handle_keyboard(CameraDirection op, float dt, bool sprint = false);
};
