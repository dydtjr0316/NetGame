#include "stdafx.h"
#include "CServer.h"
#include "CUDP.h"
#include "CTCP.h"

SERVER::SERVER()
{
	m_pUdp = new CUDP;
	m_tcp = new CTCP;
}

SERVER::~SERVER()
{
	m_pUdp->~CUDP();
	delete m_pUdp;
}



void SERVER::SendMovePacket(char id, STATE state, STATE head)
{
	m_pUdp->Move(id, state, head);
}

int SERVER::ConnectServer()
{
	int id = m_tcp->ConnectTCP("127.0.0.1");
	m_pUdp->ConnectUDP("127.0.0.1");

	//int id = m_tcp->ConnectTCP("127.0.0.1");
	//m_pUdp->ConnectUDP("127.0.0.1");

	return id;
}
