#pragma once

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>

#include "SpriteDrawCmd.h"
#include "Texture2d.h"
#include "Vbo.h"
#include "Ibo.h"

#include <stdint.h>
#include <vector>
#include <unordered_map>

namespace funk
{
	class SpriteBatchDrawer : public HandledObj<SpriteBatchDrawer>
	{
	private:

		struct SpriteDrawCmdGroup
		{
			// when rendering, rendering is batched by texture_id
			struct SpriteDrawBatch
			{
				int sprite_index_offset;
				int sprite_count;
				SpriteTextureId tex_id; 
			};

			std::vector< SpriteDrawCmd > draw_cmds;
			std::vector< SpriteDrawBatch > batches;
			StrongHandle< Vbo > vbo;
		};

		struct SpriteImmDrawData
		{
			StrongHandle< Vbo > vbo;
			int vertex_insert_index; // where you can insert
		} m_imm_draw_data;

		// all cmds queued to be drawn
		std::vector< SpriteDrawCmdGroup > m_draw_cmd_groups;

		// all textures used
		std::unordered_map< SpriteTextureId, StrongHandle<Texture2d> > m_textures;
		
		// gfx
		std::vector< SpriteVert > m_vert_scratch_space;
		StrongHandle< VertexLayout > m_vert_layout;

	public:

		SpriteBatchDrawer();
		void Init();

		// group
		int AllocGroup(); // returns the index of the group
		void DrawGroup( int a_group_index ); // sorts, builds verts, binds vbo and renders
		void RegisterDraw( int a_group_index, const SpriteDrawCmd & a_draw_cmd ); // registers a draw command to a group

		// draw immediately, this is SLOW, use this only for debug
		void ImmDrawSprite( const SpriteDrawCmd & a_imm_draw_cmd );

		// textures
		void RegisterTexture( SpriteTextureId a_tex_id, StrongHandle< Texture2d> a_tex );

		GM_BIND_TYPEID(SpriteBatchDrawer);

	private:

		void InitVertexLayout();
		void InitIbo();
		void InitImm();

		SpriteDrawCmdGroup & GetGroup( int a_group_index );
		const SpriteDrawCmdGroup & GetGroup( int a_group_index ) const;
		bool DoesGroupExist( int a_group_index ) const { return a_group_index >=0 && a_group_index < (int)m_draw_cmd_groups.size(); }

		// group manipulator
		void SortGroup( SpriteDrawCmdGroup & a_group );
		void BuildGroupVerts( SpriteDrawCmdGroup & a_group );
		void ClearGroup( SpriteDrawCmdGroup & a_group );
	};

	GM_BIND_DECL(SpriteBatchDrawer);
}
