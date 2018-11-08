#include "netvar.hpp"

netvar *netvar::_netvar = nullptr;

namespace netvars {
	uintptr_t m_iTeamNum, m_iHealth, m_lifeState, m_vecViewoffset, m_aimPunchangle,
		m_vecOrigin, m_Collision;
}

std::shared_ptr< netvar > netvar::get() {
	if( !_netvar )
		_netvar = new netvar();

	return std::shared_ptr<netvar>( _netvar );
}

uintptr_t recurse( recvtable *table, const char *propname, uintptr_t addoffset = 0 ) {
	for( int i = 0; i < table->nprops; ++i ) {
		const auto *prop = &table->props[ i ];

		if( std::strcmp( prop->varname, propname ) == 0 )
			return addoffset + prop->offset;

		if( prop->recvtype != DPT_DataTable )
			continue;

		auto ret = recurse( prop->datatable, propname, addoffset + prop->offset );

		if( ret != 0 )
			return ret;
	}

	return 0x0;
}

uintptr_t get_netvar( const char *table, const char *prop ) {
	clientclass *classes = globals::client->get_allclasses();
	while( classes ) {
		if( strcmp( classes->networkname, table ) == 0 ) {
#ifdef _DEBUG
			utility::get()->printtoconsole( "\t%s::%s -> 0x%X \n", table, prop, recurse( classes->recvtable, prop ) );
#endif // _DEBUG
			return recurse( classes->recvtable, prop );
		}

		classes = classes->next;
	}

	return 0x0;
}

void netvar::setup() {
	netvars::m_iHealth = get_netvar( "CCSPlayer", "m_iHealth" );
	netvars::m_lifeState = get_netvar( "CCSPlayer", "m_lifeState" );
	netvars::m_vecOrigin = get_netvar( "CCSPlayer", "m_vecOrigin" );
	netvars::m_vecViewoffset = get_netvar( "CCSPlayer", "m_vecViewOffset[0]" );

	netvars::m_iTeamNum = get_netvar( "CBaseEntity", "m_iTeamNum" );
	netvars::m_Collision = get_netvar( "CBaseEntity", "m_Collision" );

	netvars::m_aimPunchangle = get_netvar( "CBasePlayer", "m_aimPunchAngle" );
}