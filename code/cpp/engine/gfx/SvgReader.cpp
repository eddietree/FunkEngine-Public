#include "SvgReader.h"

#include <common/Debug.h>
#include <system/DataFile.h>
#include <ext/rapidxml/rapidxml_funk.h>

namespace funk
{

struct ProcessNodeParams
{
	std::vector<SvgVert>&		verts;
	std::vector<uint32_t>&		indices;
	std::vector<SvgDrawCmd>&	draw_cmds;
};

struct StrokeParams
{
	uint32_t color;
	float thickness;
};

struct FillParams
{
	uint32_t color;
};

uint32_t ConvertStringToUint32Color( const char* a_str, float a_opacity = 1.0f )
{
	if ( a_str == nullptr ) return 0x0;
	
	uint32_t result = strtoul( a_str+1, nullptr, 16 );

	// add alpha
	uint8_t alpha_byte = (uint8_t)(a_opacity * 255);
	result |= (alpha_byte <<24 );

	return result;
}

// grabs fill color, returns true if exists
bool GetFillColor( rapidxml::xml_node<>* a_node, FillParams * a_params )
{
	rapidxml::xml_attribute<>* attrib_color = a_node->first_attribute("fill");
	rapidxml::xml_attribute<>* attrib_opacity = a_node->first_attribute("opacity");

	if ( attrib_color == nullptr ) 
		return false;

	// grab color
	float opacity = attrib_opacity ? (float)atof(attrib_opacity->value()) : 1.0f;
	a_params->color = ConvertStringToUint32Color( attrib_color->value(), opacity );

	return true;
}

// grabs stroke color, returns if exists
bool GetStrokeColor( rapidxml::xml_node<>* a_node, StrokeParams * a_params )
{
	rapidxml::xml_attribute<>* attrib_color = a_node->first_attribute("stroke");
	rapidxml::xml_attribute<> * attrib_opacity = a_node->first_attribute("opacity");

	if ( attrib_color == nullptr ) 
		return false;

	// color
	float opacity = attrib_opacity ? (float)atof(attrib_opacity->value()) : 1.0f;
	a_params->color = ConvertStringToUint32Color( attrib_color->value(), opacity );

	// thickness
	rapidxml::xml_attribute<>* attrib_stroke_width =  a_node->first_attribute( "stroke-width" );
	a_params->thickness = attrib_stroke_width ? (float)atof( attrib_stroke_width->value() ) : 1.0f;

	return true;
}

void ProcessPath( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
}

void ProcessRect( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
}

void ProcessLine( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
}

void ProcessPolyLine( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
}

void ProcessCircle( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
}

void ProcessEllipse( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
}

void ProcessPolygon( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
	StrokeParams stroke_params;
	FillParams fill_params;

	// TODO: grab verts

	if ( GetFillColor( a_node, &fill_params ) )
	{
		SvgDrawCmd cmd;
		cmd.vert_offset = a_params.indices.size();

		// set indices

		cmd.vert_count = a_params.indices.size() - cmd.vert_offset;
		
		a_params.draw_cmds.push_back(cmd);
	}

	if ( GetStrokeColor( a_node, &stroke_params ) )
	{
		SvgDrawCmd cmd;

		// set indices

		a_params.draw_cmds.push_back(cmd);
	}
}

void ProcessNode( rapidxml::xml_node<>* a_node, ProcessNodeParams & a_params )
{
	if ( a_node == nullptr ) 
		return;

	do
	{
		const char* text = a_node->name();
		TRACE("text: %s\n", text);

		if ( strcmp( text, "path" ) == 0 )
			ProcessPath( a_node, a_params );
		else if ( strcmp( text, "rect" ) == 0 )
			ProcessRect( a_node, a_params );
		else if ( strcmp( text, "line" ) == 0 )
			ProcessLine( a_node, a_params );
		else if ( strcmp( text, "polyline" ) == 0 )
			ProcessPolyLine( a_node, a_params );
		else if ( strcmp( text, "circle" ) == 0 )
			ProcessCircle( a_node, a_params );
		else if ( strcmp( text, "ellipse" ) == 0 )
			ProcessEllipse( a_node, a_params );
		else if ( strcmp( text, "polygon" ) == 0 )
			ProcessPolygon( a_node, a_params );

		// handle children
		ProcessNode( a_node->first_node(), a_params );	
	} 
	while( a_node = a_node->next_sibling() );
}

void SvgReader::LoadFile( const char * a_filename )
{
	// load data of file
	DataFile data_file;
	data_file.Load( a_filename );
	
	// parse XML
	rapidxml::xml_document<> doc;
	const int flags = 0x0;
	doc.parse<flags>((char*)data_file.GetData());
	
	// params passed throughout
	ProcessNodeParams params = 
	{
		m_verts,
		m_indices,
		m_draw_cmds
	};

	//  begin
	rapidxml::xml_node<>* svg_node = doc.first_node(); // <svg>
	ProcessNode( svg_node, params );
	
	// TODO: flip verts with the matrix: translate(0,y) scale(1,-1)
}

void SvgReader::GenerateFlattenedVerticesArray( std::vector<SvgVert>& a_output_vertices )
{
	a_output_vertices.resize( m_indices.size() );

	for( size_t i = 0; i < m_indices.size(); ++i )
	{
		const uint32_t index = m_indices[i];
		a_output_vertices[i] = m_verts[index];
	}
}

StrongHandle<SvgDrawer> SvgReader::GenerateSvgDrawer()
{
	StrongHandle<SvgDrawer> result = new SvgDrawer;

	// vertex layout
	StrongHandle< VertexLayout > layout = new VertexLayout;
	layout->BeginEntries( sizeof(SvgVert) );
	layout->AddEntry( DATATYPE_FLOAT, 2, offsetof(SvgVert,pos) );
	layout->AddEntry( DATATYPE_UNSIGNED_BYTE, 4, offsetof(SvgVert,color), true );
	layout->EndEntries();

	// flatten vertices into one VBO, no indices
	std::vector<SvgVert> vertices_flattened;
	GenerateFlattenedVerticesArray( vertices_flattened );

	// vbo
	StrongHandle<Vbo> vbo = new Vbo;
	if ( !vertices_flattened.empty() ) vbo->Init( &(vertices_flattened.front()), vertices_flattened.size(), sizeof(SvgVert), layout );
	result->SetVbo(vbo);

	// copy draw commands
	result->m_draw_cmds.resize( m_draw_cmds.size() );
	memcpy( &(result->m_draw_cmds.front()), &(m_draw_cmds.front()), sizeof(SvgDrawCmd)*m_draw_cmds.size() );

	return result;
}

}