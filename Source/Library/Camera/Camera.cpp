#include "Camera/Camera.h"

Camera::Camera(_In_ const XMVECTOR& position)
	:m_cbChangeOnCameraMovement()
    , m_moveLeftRight(0.0f)
    , m_moveBackForward(0.0f)
    , m_moveUpDown(0.0f)
    , m_movementSpeed(10.0f)
    , m_rotationSpeed(0.002f)
    , m_cameraForward(DEFAULT_FORWARD)
    , m_cameraRight(DEFAULT_RIGHT)
    , m_cameraUp(DEFAULT_UP)
    , m_eye(position)
    , m_target(DEFAULT_TARGET)
    , m_at(XMVectorAdd(position, m_target))
    , m_up(m_cameraUp)
    , m_view(XMMatrixLookAtLH(m_eye, m_at, m_up))
{
}

const XMVECTOR& Camera::GetEye() const
{
    return m_eye;
}

const XMVECTOR& Camera::GetAt() const
{
    return m_at;
}

const XMVECTOR& Camera::GetUp() const
{
    return m_up;
}

const XMMATRIX& Camera::GetView() const
{
    return m_view;
}

ComPtr<ID3D11Buffer>& Camera::GetConstantBuffer()
{
    return m_cbChangeOnCameraMovement;
}


HRESULT Camera::Initialize(_In_  ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBChangeOnCameraMovement);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pDevice->CreateBuffer(&bd, nullptr, m_cbChangeOnCameraMovement.GetAddressOf());
    if (FAILED(hr))
        return hr;

    return hr;
}

void Camera::Update(_In_ FLOAT deltaTime)
{

    m_eye = XMVectorAdd(m_eye, XMVectorScale(Camera::DEFAULT_RIGHT, m_moveLeftRight));
    m_eye = XMVectorAdd(m_eye, XMVectorScale(Camera::DEFAULT_FORWARD, m_moveBackForward));

    // 2. Reset movement accumulators for the next frame.
    m_moveLeftRight = 0.0f;
    m_moveBackForward = 0.0f;
    // m_moveUpDown is not used and remains 0.0f.



    m_at = XMVectorAdd(m_eye, m_target);


    // 5. Recompute the view matrix.
    m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);
}

void Camera::Follow(const Character& ch)
{
    // --- 1. where is the character and which way is he facing? ---
    const XMVECTOR pos = ch.GetPosition();      // world-space feet

    // --- 2. quarter-view offset in character-local axes ---
    constexpr float kRadius = 6.0f;   // horizontal distance
    constexpr float kHeight = 4.0f;   // how high the camera is
	const XMVECTOR offset = XMVectorSet(-kRadius, kHeight, +kRadius, 0.0f); // offset in character-local axes

    // --- 3. build eye, at, view ---
    m_eye = pos + offset;
    m_at = pos + XMVectorSet(0.f, 1.0f, 0.f, 0.f);;    // look at chest-height
    m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);
}