#include "CubeGameObject.h"
#include "../RenderEngine/CubeRenderProxy.h"

CubeGameObject::CubeGameObject(float st)
{
	start_t = st;
	m_pRenderProxy = new CubeRenderProxy();
}

MoveCube::MoveCube(float st) : CubeGameObject(st) {}
ControlCube::ControlCube(float st) : CubeGameObject(st) {}
JumpCube::JumpCube(float st) : CubeGameObject(st) {}

void CubeGameObject::Event(float t, float dt, EInputCommand e) {}
void CubeGameObject::Update(float t, float dt) {}


void MoveCube::Update(float t, float dt)
{
	if (std::isnan(start_pos[0])) {
		start_pos[0] = m_vPosition[0];
		start_pos[1] = m_vPosition[1];
		start_pos[2] = m_vPosition[2];
	}

	SetPosition(start_pos[0], start_pos[1], start_pos[2] + sinf((t - start_t) * 10.0f)) ;
}


void JumpCube::Update(float t, float dt)
{
	if (std::isnan(start_pos[0])) {
		start_pos[0] = m_vPosition[0];
		start_pos[1] = m_vPosition[1];
		start_pos[2] = m_vPosition[2];
	}

	float tt = (t - start_t) * 0.5f - floor((t  - start_t) * 0.5f);
	float jump = -(tt - 1) * (tt) * 20.f;
	SetPosition(start_pos[0], start_pos[1] + jump, start_pos[2]);
}


void ControlCube::Event(float t, float dt, EInputCommand e) {
	float speed = 10.f;
	if (e == eIC_GoLeft) {
		SetPosition(m_vPosition[0] - dt * speed, m_vPosition[1], m_vPosition[2]);
	}
	if (e == eIC_GoRight) {
		SetPosition(m_vPosition[0] + dt * speed, m_vPosition[1], m_vPosition[2]);
	}
}