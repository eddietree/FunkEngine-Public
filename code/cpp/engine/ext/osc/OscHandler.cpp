#ifdef ENABLE_OSC

#include "OscHandler.h"

#include <gm/gmBind.h>
#include <common/Debug.h>

#include <stdint.h>

namespace funk
{

OscHandler::OscHandler() : m_data_table(nullptr)
{}

OscHandler::~OscHandler()
{
	m_socket.close();
	m_data_table = nullptr;
}

bool OscHandler::InitReceiveConnection( int a_port_num )
{
	m_socket.bindTo( a_port_num );

	if ( !m_socket.isOk() )
	{
		MESSAGE_BOX("Socket Bind Failed", "Cannot bind to socket %d", a_port_num );
		LOG( "Cannot bind to socket %d", a_port_num );
		HALT_ERROR();
		return false;
	}

	return true;
}

void OscHandler::SetDataTable( gmTableObject * a_data_table )
{
	ASSERT( a_data_table != nullptr );
	m_data_table = a_data_table;
}

bool GetArgIntVal( oscpkt::Message::ArgReader &a_arg_reader, int &a_result )
{
	if ( a_arg_reader.isInt32() )
	{
		int val;
		a_arg_reader.popInt32(val);
		a_result = val;
	}
	else if ( a_arg_reader.isInt64() )
	{
		// downcasted 64->32 bit int
		int64_t val;
		a_arg_reader.popInt64(val);
		a_result = (int)val;
	}
	else
	{
		return false;
	}

	return true;
}

bool GetArgFloatOrIntVal( oscpkt::Message::ArgReader &a_arg_reader, float &a_result )
{
	if ( a_arg_reader.isInt32() )
	{
		int val;
		a_arg_reader.popInt32(val);
		a_result = (float)val;
	}
	else if ( a_arg_reader.isInt64() )
	{
		// downcasted 64->32 bit int
		int64_t val;
		a_arg_reader.popInt64(val);
		a_result = (float)val;
	}
	else if ( a_arg_reader.isFloat() )
	{
		// downcasted 64->32 bit int
		float val;
		a_arg_reader.popFloat(val);
		a_result = (float)val;
	}
	else if ( a_arg_reader.isDouble() )
	{
		// downcasted 64->32 bit
		double val;
		a_arg_reader.popDouble(val);
		a_result = (float)val;
	}
	else
	{
		return false;
	}

	return true;
}

void OscHandler::UpdateAddressValues( gmMachine * a_vm )
{
	// an example address data is like:
	// "/address" = { value = v2(1.0f), age = 0.0f };

	ASSERT( m_socket.isOk() );

	const int timeout = 1;  /* timeout, in ms */
	
	while ( m_socket.receiveNextPacket( timeout ) ) 
	{
		m_packet_reader.init( m_socket.packetData(), m_socket.packetSize());
		oscpkt::Message *msg;

		while (m_packet_reader.isOk() && (msg = m_packet_reader.popMessage()) != 0) 
		{
			ASSERT(msg->isOk());

			// grab parameters
			const std::string & address = msg->addressPattern();
			const std::string & type_tags = msg->typeTags();
			oscpkt::Message::ArgReader arg_reader = msg->match( address );
			const size_t num_args = arg_reader.nbArgRemaining();
			
			// grab the table that corresponds to this address
			gmVariable address_table_var = m_data_table->Get( a_vm, address.c_str() );

			// if address not registered
			if ( address_table_var.IsNull() )
			{
				TRACE( "OSC address '%s' not registered!\n",  address.c_str() );
				continue;
			}

			// var at address must be be a table
			ASSERT( address_table_var.IsTable() );
			gmTableObject * address_table = address_table_var.GetTableObjectSafe();
			gmVariable curr_address_val = address_table->Get( a_vm, "val" );

			ASSERT( !curr_address_val.IsNull() );

			// result value
			gmVariable next_address_val;
			next_address_val.Nullify();

			if ( num_args == 0 ) continue;
			else if ( num_args == 1 ) // handle 1 arg
			{
				if ( arg_reader.isBool() )
				{
					bool val;
					arg_reader.popBool(val);
					next_address_val.SetInt( val ? 1 : 0 );
				}
				else if ( arg_reader.isInt32() )
				{
					int val;
					arg_reader.popInt32(val);
					next_address_val.SetInt( val );
				}
				else if ( arg_reader.isInt64() )
				{
					// downcasted 64->32 bit int
					int64_t val;
					arg_reader.popInt64(val);
					next_address_val.SetInt( (int)val );
				}
				else if ( arg_reader.isFloat() )
				{
					// downcasted 64->32 bit int
					float val;
					arg_reader.popFloat(val);
					next_address_val.SetFloat( val );
				}
				else if ( arg_reader.isDouble() )
				{
					// downcasted 64->32 bit
					double val;
					arg_reader.popDouble(val);
					next_address_val.SetFloat( (float)val );
				}

				else if ( arg_reader.isStr() )
				{
					std::string val;
					arg_reader.popStr(val);
					next_address_val.SetString( a_vm, val.c_str() );
				}
				else
				{
					TRACE("OSC unrecognzed arg type for of address '%s'", address.c_str(), num_args );
				}
			}
			else if ( num_args == 2 ) // can only be v2 or v2i
			{
				if ( type_tags == "ii" )
				{
					v2i result;

					if ( GetArgIntVal( arg_reader, result.x ) && GetArgIntVal( arg_reader, result.y ) )
					{
						next_address_val.SetV2i(result);
					}
					else
					{
						TRACE("OSC unable to grab v2i from address '%s'", address.c_str() );
					}
				}
				else
				{
					v2 result;

					if ( GetArgFloatOrIntVal( arg_reader, result.x ) && GetArgFloatOrIntVal( arg_reader, result.y ) )
					{
						next_address_val.SetV2(result);
					}
					else
					{
						TRACE("OSC unable to grab v2 from address '%s'", address.c_str() );
					}
				}
			}

			else if ( num_args == 3 ) // handle 3 args, should be v3()
			{
				v3 result;

				if ( GetArgFloatOrIntVal( arg_reader, result.x ) && GetArgFloatOrIntVal( arg_reader, result.y ) && GetArgFloatOrIntVal( arg_reader, result.z ) )
				{
					next_address_val.SetV3(result);
				}
				else
				{
					TRACE("OSC: unable to grab v3 from address '%s'", address.c_str() );
				}
			}
			else
			{
				TRACE("OSC: unrecognzed number of args for '%s': %d\n", address.c_str(), num_args );
			}

			if ( curr_address_val.GetType() != next_address_val.GetType() )
			{
				// was not able to grab value
				if ( next_address_val.IsNull() ) 
				{
					TRACE("OSC: unable to parse value for address '%s'\n", address.c_str() );
				}

				TRACE("OSC: address '%s' expected type '%s' but instead got '%s'\n", address.c_str(), a_vm->GetTypeName(curr_address_val.GetType()), a_vm->GetTypeName(next_address_val.GetType()) );
			}
			else
			{
				// set the address variable and reset the age timer
				address_table->Set( a_vm, "val", next_address_val );
				address_table->Set( a_vm, "age", gmVariable(0.0f) );
			}
		}
	}
}

GM_REG_NAMESPACE(OscHandler)
{
	GM_MEMFUNC_CONSTRUCTOR(OscHandler)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( OscHandler, new OscHandler() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(UpdateAddressValues)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(OscHandler, ptr);
		ptr->UpdateAddressValues( a_thread->GetMachine() );
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetDataTable)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_TABLE_PARAM( a_table, 0 );

		GM_GET_THIS_PTR(OscHandler, ptr);
		ptr->SetDataTable( a_table );
		
		return GM_OK;
	}

	GM_GEN_MEMFUNC_BOOL_INT( OscHandler, InitReceiveConnection )
}

GM_REG_MEM_BEGIN(OscHandler)
GM_REG_MEMFUNC( OscHandler, InitReceiveConnection )
GM_REG_MEMFUNC( OscHandler, SetDataTable )
GM_REG_MEMFUNC( OscHandler, UpdateAddressValues )
GM_REG_HANDLED_DESTRUCTORS(OscHandler)
GM_REG_MEM_END()
GM_BIND_DEFINE(OscHandler)

}

#endif // ENABLE_OSC