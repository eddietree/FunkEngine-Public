#include "CamBase.h"

#include "MVP.h"

namespace funk
{
CamBase::CamBase() 
:	m_eyePos( 0.0f, 0.0f, 1.0f ),
	m_orient( 0.0f, 0.0f, -1.0f ),
	m_zNearFar( 0.1f, 100.0f ),
	m_yup(0.0f,1.0f,0.0f)
{
}

void CamBase::Begin()
{
	v3 lookAt = m_eyePos + m_orient;
	
	// push view
	MVP& mvp = MVP::Ref();
	mvp.View().Push();
	mvp.View().Load( matrix::LookAt( m_eyePos, lookAt, m_yup ) );
}

void CamBase::End()
{
	MVP& mvp = MVP::Ref();
	mvp.View().Pop();
}

void CamBase::SetNearFar( v2 zNearFar )
{
	m_zNearFar = zNearFar;
}
}