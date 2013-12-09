#include "SpriteBatchDrawer.h"

#include <common/Debug.h>
#include <gm/gmBind.h>
#include <math/ColorUtil.h>

#include "GLTypes.h"
#include "GLDebug.h"
#include "GLIncludes.h"

#include <algorithm>

namespace funk
{

const int SPRITE_BATCH_MAX_NUM_DRAW_CMDS_PER_GROUP = 256;
const int SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD = 6;
const int SPRITE_BATCH_MAX_NUM_VERTS_PER_GROUP = SPRITE_BATCH_MAX_NUM_DRAW_CMDS_PER_GROUP * SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD;

// immediate mode
const int SPRITE_BATCH_IMM_MAX_NUM_DRAW_CMDS = 64;

// cmp function used in std::sort
struct SpriteDrawCmdSortCmp 
{
	// sort by texture id
    inline bool operator() (const SpriteDrawCmd& struct1, const SpriteDrawCmd& struct2)
    {
		return (struct1.tex_id < struct2.tex_id);
    }
};

inline void BuildDrawCmdVerts( const SpriteDrawCmd &a_draw_cmd, SpriteVert out_verts[4] )
{
	// convert command to vertex
	SpriteVert vert_base;
	vert_base.pos_center = a_draw_cmd.pos;
	vert_base.color = a_draw_cmd.color;
	vert_base.rotation = a_draw_cmd.rotation;

	SpriteVert vert0 = vert_base;
	vert0.pos_local = v2(-0.5f) * a_draw_cmd.dimen;
	vert0.texcoord_uv = a_draw_cmd.uv0;

	SpriteVert vert1 = vert_base;
	vert1.pos_local = v2(0.5f, -0.5f) * a_draw_cmd.dimen;
	vert1.texcoord_uv = v2( a_draw_cmd.uv1.x, a_draw_cmd.uv0.y );

	SpriteVert vert2 = vert_base;
	vert2.pos_local = v2(0.5f) * a_draw_cmd.dimen;
	vert2.texcoord_uv = a_draw_cmd.uv1;

	SpriteVert vert3 = vert_base;
	vert3.pos_local = v2(-0.5f, 0.5f) * a_draw_cmd.dimen;
	vert3.texcoord_uv = v2( a_draw_cmd.uv0.x, a_draw_cmd.uv1.y );

	out_verts[0] = vert0;
	out_verts[1] = vert1;
	out_verts[2] = vert2;
	out_verts[3] = vert3;
}

SpriteBatchDrawer::SpriteBatchDrawer()
{}

void SpriteBatchDrawer::Init()
{
	m_vert_scratch_space.reserve( SPRITE_BATCH_MAX_NUM_VERTS_PER_GROUP );
	m_draw_cmd_groups.reserve(8);
	
	InitVertexLayout();
	InitImm();
}

void SpriteBatchDrawer::InitImm()
{
	m_imm_draw_data.vbo = new Vbo();
	m_imm_draw_data.vbo->Init( nullptr, SPRITE_BATCH_IMM_MAX_NUM_DRAW_CMDS * SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD, sizeof( SpriteVert ), m_vert_layout, BUFFER_STREAM );
	m_imm_draw_data.vertex_insert_index = 0;
}

void SpriteBatchDrawer::InitVertexLayout()
{
	m_vert_layout = new VertexLayout;
	m_vert_layout->BeginEntries( sizeof(SpriteVert) );

	m_vert_layout->AddEntry( DATATYPE_FLOAT, 2, offsetof(SpriteVert,pos_center) );
	m_vert_layout->AddEntry( DATATYPE_FLOAT, 2, offsetof(SpriteVert,pos_local) );
	m_vert_layout->AddEntry( DATATYPE_FLOAT, 2, offsetof(SpriteVert,texcoord_uv) );
	m_vert_layout->AddEntry( DATATYPE_UNSIGNED_BYTE, 4, offsetof(SpriteVert,color), true );
	m_vert_layout->AddEntry( DATATYPE_FLOAT, 1, offsetof(SpriteVert,rotation) );
	
	m_vert_layout->EndEntries();
}

void SpriteBatchDrawer::DrawGroup( int a_group_index )
{
	ASSERT( DoesGroupExist( a_group_index ) );

	SpriteDrawCmdGroup & group = GetGroup( a_group_index );
	
	SortGroup( group );
	BuildGroupVerts( group );

	// draw!
	if ( !group.draw_cmds.empty() )
	{
		const int draw_num_sprites = group.draw_cmds.size();

		auto & vbo = group.vbo;
		vbo->Begin();

		// go through each batch and render
		for( auto batch : group.batches )
		{
			// calc the vertex positions and count
			const int vert_index = batch.sprite_index_offset * SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD;
			const int vert_count = batch.sprite_count * SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD;

			// grab the texture of this batch
			auto it_tex = m_textures.find( batch.tex_id );
			ASSERT( it_tex != m_textures.end() );
			auto & tex = it_tex->second;

			// draw!
			tex->Bind(0);
			vbo->Draw( PRIM_TRIANGLES, vert_index, vert_count );
			tex->Unbind();
		}
		
		vbo->End();
	}

	ClearGroup( group );
}

void SpriteBatchDrawer::ImmDrawSprite( const SpriteDrawCmd & a_imm_draw_cmd )
{
	// convert command to vertices
	SpriteVert verts[4];
	BuildDrawCmdVerts( a_imm_draw_cmd, verts );

	// add all 6 verts of quad
	m_vert_scratch_space.clear();
	m_vert_scratch_space.push_back( verts[0] );
	m_vert_scratch_space.push_back( verts[1] );
	m_vert_scratch_space.push_back( verts[2] );
	m_vert_scratch_space.push_back( verts[0] );
	m_vert_scratch_space.push_back( verts[2] );
	m_vert_scratch_space.push_back( verts[3] );

	// calc num bytes
	const uint32_t num_bytes = sizeof(SpriteVert) * SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD;
	const uint32_t byte_offset = sizeof(SpriteVert) * m_imm_draw_data.vertex_insert_index;

	// bind texture
	auto & tex = m_textures.at( a_imm_draw_cmd.tex_id );
	tex->Bind(0);

	// send to gpu
	auto & vbo = m_imm_draw_data.vbo;
	vbo->Bind();
	vbo->SubData( &m_vert_scratch_space.front(), num_bytes, byte_offset ); 

	// draw immediately
	vbo->Draw( PRIM_TRIANGLES, m_imm_draw_data.vertex_insert_index, SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD );
	vbo->Unbind();

	// unbind tex
	tex->Unbind();

	// move index, wrap around if needed
	m_imm_draw_data.vertex_insert_index += SPRITE_BATCH_NUM_VERTS_PER_DRAW_CMD;
	if ( m_imm_draw_data.vertex_insert_index >= (int)vbo->GetNumElems() )
	{
		m_imm_draw_data.vertex_insert_index = 0;
	}
}

int SpriteBatchDrawer::AllocGroup()
{
	m_draw_cmd_groups.resize( m_draw_cmd_groups.size()+1 );

	// init group
	SpriteDrawCmdGroup & group = m_draw_cmd_groups.back();
	group.draw_cmds.reserve( SPRITE_BATCH_MAX_NUM_DRAW_CMDS_PER_GROUP );
	group.batches.reserve( SPRITE_BATCH_MAX_NUM_DRAW_CMDS_PER_GROUP );

	// init vbo
	group.vbo = new Vbo();
	group.vbo->Init( nullptr, SPRITE_BATCH_MAX_NUM_VERTS_PER_GROUP,sizeof(SpriteVert), m_vert_layout, BUFFER_STREAM ); 

	// returns index of the group just created
	return (int)m_draw_cmd_groups.size()-1;
}

SpriteBatchDrawer::SpriteDrawCmdGroup & SpriteBatchDrawer::GetGroup( int a_group_index )
{
	ASSERT( DoesGroupExist( a_group_index ) );
	return m_draw_cmd_groups[a_group_index];
}

const SpriteBatchDrawer::SpriteDrawCmdGroup & SpriteBatchDrawer::GetGroup( int a_group_index ) const
{
	ASSERT( DoesGroupExist( a_group_index ) );
	return m_draw_cmd_groups[a_group_index];
}

void SpriteBatchDrawer::RegisterDraw( int a_group_index, const SpriteDrawCmd & a_draw_cmd )
{
	ASSERT( DoesGroupExist( a_group_index ) );
	SpriteDrawCmdGroup & group = GetGroup( a_group_index );
	ASSERT( group.draw_cmds.size() < SPRITE_BATCH_MAX_NUM_DRAW_CMDS_PER_GROUP );
	group.draw_cmds.push_back( a_draw_cmd );
}

void SpriteBatchDrawer::SortGroup( SpriteDrawCmdGroup & a_group )
{
	auto & cmds = a_group.draw_cmds;
	sort(cmds.begin(), cmds.end(), SpriteDrawCmdSortCmp());
}

void SpriteBatchDrawer::BuildGroupVerts( SpriteDrawCmdGroup & a_group )
{
	// if no draw cmds
	if ( a_group.draw_cmds.empty() ) return;

	m_vert_scratch_space.clear();

	// track when the texture began and ended
	SpriteTextureId curr_tex_id = a_group.draw_cmds.front().tex_id;
	int curr_sprite_index_offset = 0;

	for( size_t i = 0; i < a_group.draw_cmds.size(); ++i )
	{
		auto & draw_cmd = a_group.draw_cmds[i];

		// end of a draw batch
		if ( draw_cmd.tex_id != curr_tex_id )
		{
			SpriteDrawCmdGroup::SpriteDrawBatch batch;
			batch.sprite_index_offset = curr_sprite_index_offset;
			batch.sprite_count = i - curr_sprite_index_offset;
			batch.tex_id = curr_tex_id;

			a_group.batches.push_back( batch );

			// new texture id
			curr_tex_id = draw_cmd.tex_id;
			curr_sprite_index_offset = i;
		}

		// convert command to vertices
		SpriteVert verts[4];
		BuildDrawCmdVerts( draw_cmd, verts );

		// add all 6 verts of quad
		m_vert_scratch_space.push_back( verts[0] );
		m_vert_scratch_space.push_back( verts[1] );
		m_vert_scratch_space.push_back( verts[2] );
		m_vert_scratch_space.push_back( verts[0] );
		m_vert_scratch_space.push_back( verts[2] );
		m_vert_scratch_space.push_back( verts[3] );
	}

	// add last batch
	SpriteDrawCmdGroup::SpriteDrawBatch batch;
	batch.sprite_index_offset = curr_sprite_index_offset;
	batch.sprite_count = a_group.draw_cmds.size() - curr_sprite_index_offset;
	batch.tex_id = curr_tex_id;
	a_group.batches.push_back( batch );

	// send verts to GPU
	auto & vbo = a_group.vbo;
	vbo->Bind();
	vbo->SubData( &m_vert_scratch_space.front(), m_vert_scratch_space.size() * sizeof(m_vert_scratch_space[0]) );
	vbo->Unbind();
}

void SpriteBatchDrawer::ClearGroup( SpriteDrawCmdGroup & a_group )
{
	a_group.batches.clear();
	a_group.draw_cmds.clear();
}

void SpriteBatchDrawer::RegisterTexture( SpriteTextureId a_tex_id, StrongHandle< Texture2d> a_tex )
{
	auto it = m_textures.find( a_tex_id );

	// add if does not exist
	if ( it == m_textures.end() )
	{
		m_textures.insert( std::make_pair( a_tex_id, a_tex ) );
	}
}

GM_REG_NAMESPACE(SpriteBatchDrawer)
{
	GM_MEMFUNC_DECL(CreateSpriteBatchDrawer)
	{
		GM_CHECK_NUM_PARAMS(0);

		StrongHandle<SpriteBatchDrawer> result = new SpriteBatchDrawer();
		result->Init();
		GM_PUSH_USER_HANDLED( SpriteBatchDrawer, result.Get() );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(DrawGroup)
	{
		GM_CHECK_INT_PARAM( a_group_index, 0 );
		
		GM_GET_THIS_PTR(SpriteBatchDrawer, ptr);
		ptr->DrawGroup( a_group_index );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(RegisterDraw)
	{
		GM_CHECK_INT_PARAM( a_group_index, 0 );
		GM_CHECK_INT_PARAM( a_tex_id, 1 );
		GM_CHECK_V2_PARAM( a_pos, 2 );
		GM_CHECK_V2_PARAM( a_dimen, 3 );
		GM_CHECK_V2_PARAM( a_uv0, 4 );
		GM_CHECK_V2_PARAM( a_uv1, 5 );
		GM_CHECK_V3_PARAM( a_color, 6 );
		GM_FLOAT_PARAM( a_alpha, 7, 1.0f );
		GM_FLOAT_PARAM( a_rotation, 8, 0.0f );
		
		GM_GET_THIS_PTR(SpriteBatchDrawer, ptr);

		// populate command
		SpriteDrawCmd cmd;
		cmd.tex_id = a_tex_id;
		cmd.pos = a_pos;
		cmd.dimen = a_dimen;
		cmd.uv0 = a_uv0;
		cmd.uv1 = a_uv1;
		cmd.color =  MakeColorUint32( a_color.x, a_color.y, a_color.z, a_alpha ); 
		cmd.rotation = a_rotation;
		
		ptr->RegisterDraw( a_group_index, cmd );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(ImmDrawSprite)
	{
		GM_CHECK_V2_PARAM( a_pos, 0 );
		GM_CHECK_V2_PARAM( a_dimen, 1 );
		GM_CHECK_V2_PARAM( a_uv0, 2 );
		GM_CHECK_V2_PARAM( a_uv1, 3 );
		GM_CHECK_V3_PARAM( a_color, 4 );
		GM_FLOAT_PARAM( a_alpha, 5, 1.0f );
		GM_FLOAT_PARAM( a_rotation, 6, 0.0f );
		
		GM_GET_THIS_PTR(SpriteBatchDrawer, ptr);

		// populate command
		SpriteDrawCmd cmd;
		cmd.pos = a_pos;
		cmd.dimen = a_dimen;
		cmd.uv0 = a_uv0;
		cmd.uv1 = a_uv1;
		cmd.color =  MakeColorUint32( a_color.x, a_color.y, a_color.z, a_alpha ); 
		cmd.rotation = a_rotation;

		ptr->ImmDrawSprite( cmd );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(RegisterTexture)
	{
		GM_CHECK_INT_PARAM( a_tex_id, 0 );
		GM_CHECK_USER_PARAM_PTR( Texture2d, tex, 1 );
		
		GM_GET_THIS_PTR(SpriteBatchDrawer, ptr);
		ptr->RegisterTexture( a_tex_id, tex );

		return GM_OK;
	}

	GM_GEN_MEMFUNC_INT_VOID( SpriteBatchDrawer, AllocGroup )
}

GM_REG_MEM_BEGIN(SpriteBatchDrawer)
GM_REG_MEMFUNC( SpriteBatchDrawer, DrawGroup )
GM_REG_MEMFUNC( SpriteBatchDrawer, RegisterDraw )
GM_REG_MEMFUNC( SpriteBatchDrawer, ImmDrawSprite )
GM_REG_MEMFUNC( SpriteBatchDrawer, RegisterTexture )
GM_REG_MEMFUNC( SpriteBatchDrawer, AllocGroup )
GM_REG_HANDLED_DESTRUCTORS(SpriteBatchDrawer)
GM_REG_MEM_END()
GM_BIND_DEFINE(SpriteBatchDrawer)

}