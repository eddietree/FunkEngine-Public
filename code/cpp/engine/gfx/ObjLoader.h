
#pragma once

#include <math/v3.h>
#include <math/v2.h>
#include <common/StrongHandle.h>
#include "Vbo.h"

#include <vector>

namespace funk
{
	class ObjLoader
	{
	public:

		struct VertObj
		{
			v3 pos;
			v3 normal;
			v2 uv;
		};


		ObjLoader();	
		
		// create vbo from file data
		StrongHandle<Vbo> CreateVbo(); 

		// grab flattened verts raw data
		void GetVboVertPos( std::vector<VertObj> & out ) const;

		void LoadFile( const char * file );
		void SetSwizzleZY( bool val ) { m_swizzleZY = val; }
		void SetGenerateNormals( bool val ) { m_bGenNormals = val; }

	private:

		// settings
		bool m_swizzleZY;
		bool m_bGenNormals;

		struct Face
		{
			static const int kMaxPts = 4;

			int pos[kMaxPts];
			int normal[kMaxPts];
			int uv[kMaxPts];

			Face()
			{
				for ( int i = 0; i < kMaxPts; ++i ) pos[i] = normal[i] = uv[i] = -1;
			}
		};

		std::vector< v3 > m_vertPos;
		std::vector< v3 > m_vertNormal;
		std::vector< v2 > m_vertUV;
		std::vector< Face > m_faces;
	};
}
