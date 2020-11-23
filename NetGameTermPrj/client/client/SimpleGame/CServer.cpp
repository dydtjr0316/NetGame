#include "stdafx.h"
#include "CServer.h"
#include "CUDP.h"

SERVER::SERVER()
{
	m_pUdp = new CUDP;
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


