
#pragma once

#include <math/v2.h>
#include <math/v3.h>

namespace funk
{
	class CamBase
	{
	public:

		CamBase();

		virtual void Begin();
		virtual void End();

		v3 GetPos() const { return m_eyePos; }
		v3 GetDir() const { return m_orient; }
		v3 GetYup() const { return m_yup; }
		
		void SetPos( const v3 pos ) { m_eyePos = pos; }
		void SetLookAt( const v3 lookAt ) { m_orient = normalize(lookAt-m_eyePos); }
		void SetDir( const v3 a_dir ) { m_orient = normalize(a_dir); }
		void SetNearFar( v2 zNearFar );
		void SetYup( const v3 a_yup ) { m_yup = a_yup; }

	protected:
		v2 m_zNearFar;

	private:
		v3 m_yup;
		v3 m_eyePos;
		v3 m_orient;
	};
}
