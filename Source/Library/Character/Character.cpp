#include "Character/Character.h"

Character::Character(const std::filesystem::path& filePath)
	: Model(filePath),
	m_targetPosition(XMVectorSet(0.0f, 0.0f, 0.0f,1.0f)),
	m_currentPosition(XMVectorSet(0.0f, 0.0f,0.0f, 1.0f)),
	m_moveLeftRight(0.0f),
	m_moveBackForward(0.0f),
	m_movementSpeed(5.0f),
	m_currentDirection(CharacterDirection::FORWARD)
{}

void Character::HandleInput(const InputDirections& dir, FLOAT dt)
{
    /* 이동량 누적 (world 축) */
    if (dir.bFront)  m_moveBackForward += m_movementSpeed * dt;
    if (dir.bBack)   m_moveBackForward -= m_movementSpeed * dt;
    if (dir.bRight)  m_moveLeftRight += m_movementSpeed * dt;
    if (dir.bLeft)   m_moveLeftRight -= m_movementSpeed * dt;

    CharacterDirection dirNow = m_currentDirection;
    if (fabsf(m_moveBackForward) > fabsf(m_moveLeftRight))
        dirNow = (m_moveBackForward > 0.f) ? CharacterDirection::FORWARD
        : CharacterDirection::BACKWARD;
    else if (fabsf(m_moveLeftRight) > 0.f)
        dirNow = (m_moveLeftRight > 0.f) ? CharacterDirection::RIGHT
        : CharacterDirection::LEFT;

    rotateIfDirectionChanged(dirNow);
}

void Character::rotateIfDirectionChanged(CharacterDirection newDir)
{
    if (newDir == m_currentDirection) return;

    /* 회전 각도 정의 (라디안) */
    static const std::unordered_map<CharacterDirection, FLOAT> angleMap = {
        { CharacterDirection::FORWARD, 3 * XM_PIDIV4 },
        { CharacterDirection::RIGHT,   1 * XM_PIDIV4 },
        { CharacterDirection::BACKWARD,-1 * XM_PIDIV4     },
        { CharacterDirection::LEFT,   -3 * XM_PIDIV4 }
    };

    FLOAT angle = angleMap.at(newDir) - angleMap.at(m_currentDirection);
    /* 자기 중심 회전 */
    RotateYInObjectCoordinate(angle, m_currentPosition);

    m_currentDirection = newDir;
}

void Character::Update(FLOAT /*deltaTime*/)
{
    /* 목표 위치 = 현재 + 누적 이동 */
    XMVECTOR delta =
        m_moveLeftRight * SCREEN_RIGHT +
        m_moveBackForward * SCREEN_FWD;

    Translate(delta);

    m_currentPosition += delta;

    /* --- 2. reset accumulators --- */
    m_moveLeftRight = m_moveBackForward = 0.f;
}