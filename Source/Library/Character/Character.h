#pragma once

#include "Common.h"
#include "Model/Model.h"

enum class CharacterDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

class Character : public Model
{
public:
	Character() = default;
	Character(const std::filesystem::path& filePath);
	~Character() = default;
	
	void HandleInput(const InputDirections& directions, FLOAT deltaTime);

	HRESULT Initialize(ID3D11Device* dev, ID3D11DeviceContext* ctx) override
	{
		return Model::Initialize(dev, ctx);
	}

	void Update(FLOAT deltaTime) override;

	XMVECTOR GetPosition() const { return m_currentPosition; }
	XMVECTOR GetForward()  const
	{
		// FORWARD/BACKWARD mapping already used in rotateIfDirectionChanged
		switch (m_currentDirection)
		{
		case CharacterDirection::FORWARD:  return XMVectorSet(0, 0, 1, 0);
		case CharacterDirection::BACKWARD: return XMVectorSet(0, 0, -1, 0);
		case CharacterDirection::LEFT:     return XMVectorSet(-1, 0, 0, 0);
		case CharacterDirection::RIGHT:    return XMVectorSet(1, 0, 0, 0);
		default:                           return XMVectorSet(0, 0, 1, 0); // safety
		}
	}

private:
	static constexpr const XMVECTORF32 DEFAULT_FORWARD = { 0.0f, 0.0f, 1.0f, 0.0f };
	static constexpr const XMVECTORF32 DEFAULT_RIGHT = { 1.0f,  0.0f, 0.0f, 0.0f };
	static constexpr const XMVECTORF32 SCREEN_FWD =
	{ 0.70710678f, 0.0f, -0.70710678f, 0.0f };   //  ( +1, 0,  -1 ) / ¡î2
	static constexpr const XMVECTORF32 SCREEN_RIGHT =
	{ 0.70710678f, 0.0f,  0.70710678f, 0.0f };   //  ( +1, 0,  +1 ) / ¡î2

	XMVECTOR m_targetPosition;
	XMVECTOR m_currentPosition;
	FLOAT m_moveLeftRight;
	FLOAT m_moveBackForward;
	FLOAT m_movementSpeed;

	CharacterDirection m_currentDirection;
	void rotateIfDirectionChanged(CharacterDirection newDir);
};