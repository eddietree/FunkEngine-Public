#pragma once

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>

#include <math/v2i.h>

#include <vector>

namespace funk
{
	class ImguiTilesheetChunks : public HandledObj<ImguiTilesheetChunks>
	{
	private:
		struct Chunk
		{
			int id;
			std::vector< v2i > tile_pos;
		};

		struct TileData
		{
			int chunk_id;
		};

		v2i m_tilesheet_grid_dimen;

		std::vector<TileData> m_tile_data;
		std::vector<Chunk> m_chunks;

	public:
		ImguiTilesheetChunks();
		void Init( v2i a_tilesheet_grid_dimen );

		int		CreateChunk(); // returns the ID of the chunk
		int		GetChunkIdAtTile( v2i a_tile_pos ) const;
		void	AddTileToChunk( v2i a_tile_pos, int a_chunk_id );

		// getting chunky!
		bool IsValidChunkId( int a_chunk_id ) const;
		const Chunk & GetChunk( int a_chunk_id ) const;
		Chunk & GetChunk( int a_chunk_id );
		
		GM_BIND_TYPEID(ImguiTilesheetChunks);

	private:

		const TileData & GetTileDataAt( v2i a_tile_pos ) const;
		TileData & GetTileDataAt( v2i a_tile_pos );
	};

	GM_BIND_DECL(ImguiTilesheetChunks);
}