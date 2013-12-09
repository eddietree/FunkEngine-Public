#include "ImguiTilesheetChunks.h"

#include <common/Debug.h>
#include <gm/gmBind.h>

#include <common/Debug.h>

namespace funk
{

ImguiTilesheetChunks::ImguiTilesheetChunks()
{
}

int ImguiTilesheetChunks::CreateChunk()
{
	m_chunks.resize( m_chunks.size()+1 );
	
	int chunk_id =  (int)m_chunks.size() - 1;
	m_chunks[ chunk_id ].id = chunk_id;

	return chunk_id;
}

void ImguiTilesheetChunks::AddTileToChunk( v2i a_tile_pos, int a_chunk_id )
{
	ASSERT( IsValidChunkId(a_chunk_id) );

	Chunk & chunk = GetChunk( a_chunk_id );
	chunk.tile_pos.push_back( a_tile_pos );

	if ( a_tile_pos == v2i(0) )
	{
		int g =0 ;
	}
	
	auto & tile_data = GetTileDataAt( a_tile_pos );
	tile_data.chunk_id = a_chunk_id;
}

void ImguiTilesheetChunks::Init( v2i a_tilesheet_grid_dimen )
{
	m_tilesheet_grid_dimen = a_tilesheet_grid_dimen;
	m_tile_data.resize( a_tilesheet_grid_dimen.x * a_tilesheet_grid_dimen.y );

	for( auto & tile_data : m_tile_data )
	{
		tile_data.chunk_id = -1;
	}
}

int ImguiTilesheetChunks::GetChunkIdAtTile( v2i a_tile_pos ) const
{
	const TileData & tile_data = GetTileDataAt( a_tile_pos );
	return tile_data.chunk_id;
}

const ImguiTilesheetChunks::TileData & ImguiTilesheetChunks::GetTileDataAt( v2i a_tile_pos ) const
{
	int flat_index = a_tile_pos.y*m_tilesheet_grid_dimen.x + a_tile_pos.x;
	ASSERT( flat_index >= 0 && flat_index < (int)m_tile_data.size() );

	return m_tile_data[flat_index];
}

ImguiTilesheetChunks::TileData & ImguiTilesheetChunks::GetTileDataAt( v2i a_tile_pos )
{
	int flat_index = a_tile_pos.y*m_tilesheet_grid_dimen.x + a_tile_pos.x;
	ASSERT( flat_index >= 0 && flat_index < (int)m_tile_data.size() );

	return m_tile_data[flat_index];
}

bool ImguiTilesheetChunks::IsValidChunkId( int a_chunk_id ) const
{
	return a_chunk_id >= 0 && a_chunk_id < (int)m_chunks.size();
}

const ImguiTilesheetChunks::Chunk & ImguiTilesheetChunks::GetChunk( int a_chunk_id ) const
{
	ASSERT( IsValidChunkId(a_chunk_id) );
	return m_chunks[a_chunk_id];
}

ImguiTilesheetChunks::Chunk & ImguiTilesheetChunks::GetChunk( int a_chunk_id )
{
	ASSERT( IsValidChunkId(a_chunk_id) );
	return m_chunks[a_chunk_id];
}

GM_REG_NAMESPACE(ImguiTilesheetChunks)
{
	GM_MEMFUNC_DECL(CreateImguiTilesheetChunks)
	{
		StrongHandle<ImguiTilesheetChunks> result = new ImguiTilesheetChunks();
		GM_CHECK_V2I_PARAM( a_grid_dimen, 0 );
		result->Init( a_grid_dimen );

		GM_PUSH_USER_HANDLED( ImguiTilesheetChunks, result.Get() );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(AddTileToChunk)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2I_PARAM( a_tile_pos, 0 );
		GM_CHECK_INT_PARAM( a_chunk_id, 1 );

		GM_GET_THIS_PTR(ImguiTilesheetChunks, ptr);
		ptr->AddTileToChunk( a_tile_pos, a_chunk_id );
		
		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_INT_VOID( ImguiTilesheetChunks, CreateChunk )
	GM_GEN_MEMFUNC_INT_V2I( ImguiTilesheetChunks, GetChunkIdAtTile )
}

GM_REG_MEM_BEGIN(ImguiTilesheetChunks)
GM_REG_MEMFUNC( ImguiTilesheetChunks, CreateChunk )
GM_REG_MEMFUNC( ImguiTilesheetChunks, GetChunkIdAtTile )
GM_REG_MEMFUNC( ImguiTilesheetChunks, AddTileToChunk )
GM_REG_HANDLED_DESTRUCTORS(ImguiTilesheetChunks)
GM_REG_MEM_END()
GM_BIND_DEFINE(ImguiTilesheetChunks)


}