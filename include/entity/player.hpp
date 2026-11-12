#pragma once
#include "game/inventory.hpp"
#include "vector_float3.hpp"

struct MoveInput
{
    float forward = 0.0f;
    float right = 0.0f;
    float up = 0.0f;
    bool sprint = false;
};

// TODO: Should find a better place to put this.
enum GameMode
{
    GAMEMODE_SURVIVAL = 0,
    GAMEMODE_CREATIVE,
    GAMEMODE_COUNT
};

class Player
{
  private:
    glm::vec3 m_pos;            // feet position
    float m_yaw = -90.0f;       // degrees
    float m_pitch = 0.0f;       // degrees
    float m_speed = 4.3f;       // bps (blocks per second)
    float m_sprint_mult = 2.3f; // ~6.6 bps
    float m_sensitivity = 0.1f;
    float m_eye_height = 1.62f;
    Inventory m_inventory;
    GameMode m_gamemode = GAMEMODE_CREATIVE;

  public:
    static constexpr float WIDTH = 0.6f;
    static constexpr float HEIGHT = 1.8f;

    explicit Player(const glm::vec3 &pos) : m_pos(pos) {};

    void look(float dx, float dy);
    void update(const MoveInput &in, float dt);

    glm::vec3 get_look_dir() const;

    glm::vec3 get_position() const
    {
        return m_pos;
    }

    glm::vec3 get_eye_position() const
    {
        return m_pos + glm::vec3(0.0f, m_eye_height, 0.0f);
    }

    float get_yaw() const
    {
        return m_yaw;
    }

    float get_pitch() const
    {
        return m_pitch;
    }

    Inventory &get_inventory()
    {
        return m_inventory;
    }

    GameMode get_gamemode() const
    {
        return m_gamemode;
    };

    void set_gamemode(GameMode gamemode)
    {
        m_gamemode = gamemode;
    }
};
