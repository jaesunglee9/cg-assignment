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

    /* 방향 결정 (가장 큰 성분) */
    CharacterDirection dirNow = m_currentDirection;
    if (fabsf(m_moveBackForward) > fabsf(m_moveLeftRight))
        dirNow = (m_moveBackForward > 0) ? CharacterDirection::FORWARD
        : CharacterDirection::BACKWARD;
    else if (fabsf(m_moveLeftRight) > 0.f)
        dirNow = (m_moveLeftRight > 0) ? CharacterDirection::RIGHT
        : CharacterDirection::LEFT;

    rotateIfDirectionChanged(dirNow);
}

void Character::rotateIfDirectionChanged(CharacterDirection newDir)
{
    if (newDir == m_currentDirection) return;

    /* 회전 각도 정의 (라디안) */
    static const std::unordered_map<CharacterDirection, FLOAT> angleMap = {
        { CharacterDirection::FORWARD,  0.f },
        { CharacterDirection::RIGHT,   XM_PIDIV2 },
        { CharacterDirection::BACKWARD,XM_PI     },
        { CharacterDirection::LEFT,   -XM_PIDIV2 }
    };

    FLOAT angle = angleMap.at(newDir) - angleMap.at(m_currentDirection);
    /* 자기 중심 회전 */
    RotateYInObjectCoordinate(angle, m_currentPosition);

    m_currentDirection = newDir;
}

void Character::Update(FLOAT /*deltaTime*/)
{
    /* 목표 위치 = 현재 + 누적 이동 */
    m_targetPosition = m_currentPosition +
        m_moveLeftRight * XMVectorSet(1, 0, 0, 0) +
        m_moveBackForward * XMVectorSet(0, 0, 1, 0);

    /* Translate = (target - current) */
    XMVECTOR delta = m_targetPosition - m_currentPosition;
    Translate(delta);

    m_currentPosition = m_targetPosition;

    /* 누적값 리셋 */
    m_moveLeftRight = 0.f;
    m_moveBackForward = 0.f;
}