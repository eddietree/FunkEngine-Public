#ifdef ENABLE_OSC

#pragma once

#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>
#include <common/StrongHandle.h>

#include <gm/gmVariable.h>

#include "oscpkt.hh"
#include "udp.hh"

namespace funk
{
	class OscHandler : public funk::HandledObj<OscHandler>
	{
	private:
		oscpkt::UdpSocket m_socket;
		oscpkt::PacketReader m_packet_reader;
		//oscpkt::PacketWriter pw;

		gmTableObject * m_data_table;

	public:
				
		OscHandler();
		~OscHandler();

		bool InitReceiveConnection( int a_port_num = 8000 );
		void SetDataTable( gmTableObject * a_data_table );

		// updates the data by pulling data from the receiver
		void UpdateAddressValues( gmMachine * a_vm );

		GM_BIND_TYPEID(OscHandler);

	private:

	};

	GM_BIND_DECL(OscHandler);
}

#endif // ENABLE_OSC