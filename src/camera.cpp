static const float DEFAULT_YAW = -90.0f;
static const float PITCH_DEFAULT = 0.0f;
static const float DEFAULT_SPEED = 2.5f;
static const float DEFAULT_SENSITIVITY = 0.1f;
static const float DEFAULT_ZOOM = 45.0f;

enum CameraMovement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

struct Camera
{
  float Yaw;
  float Pitch;
  float Zoom;
  float MovementSpeed;
  float MouseSensitivity;
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  Camera(glm::vec3 _Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _WorldUp = glm::vec3(0.0f, 1.0f, 0.0f), float _Yaw = DEFAULT_YAW, float _Pitch = PITCH_DEFAULT) : MovementSpeed(DEFAULT_SPEED), MouseSensitivity(DEFAULT_SENSITIVITY), Zoom(DEFAULT_ZOOM)
  {
    Yaw = _Yaw;
    Pitch = _Pitch;
    Position = _Position;
    WorldUp = _WorldUp;
    UpdateVectors();
  }

  void ProcessMouseMovement(float XOffset, float YOffset, bool ConstrainPitch = true)
  {
    XOffset *= MouseSensitivity;
    YOffset *= MouseSensitivity;

    Yaw += XOffset;
    Pitch += YOffset;

    if (ConstrainPitch)
    {
      if (Pitch > 89.0f)
      {
        Pitch = 89.0f;
      }
      if (Pitch < -89.0f)
      {
        Pitch = -89.0f;
      }
    }

    UpdateVectors();
  }

  void ProcessKeyboard(CameraMovement Direction, float DeltaTime)
  {
    float Velocity = MovementSpeed * DeltaTime;
    if (Direction == FORWARD)
    {
      Position += Front * Velocity;
    }
    if (Direction == BACKWARD)
    {
      Position -= Front * Velocity;
    }
    if (Direction == LEFT)
    {
      Position -= Right * Velocity;
    }
    if (Direction == RIGHT)
    {
      Position += Right * Velocity;
    }
  }

  void ProcessMouseScroll(float YOffset)
  {
    if (Zoom >= 1.0f && Zoom <= 45.0f)
    {
      Zoom -= YOffset;
    }
    if (Zoom <= 1.0f)
    {
      Zoom = 1.0f;
    }
    if (Zoom >= 45.0f)
    {
      Zoom = 45.0f;
    }
  }

  glm::mat4 ViewMatrix()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  void UpdateVectors()
  {
    Front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    Front.y = sin(glm::radians(Pitch));
    Front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    Front = glm::normalize(Front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};
