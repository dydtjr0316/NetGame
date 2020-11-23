#include "stdafx.h"
#include "CServerFrame.h"

SOCKET	CServerFrame::m_UDP_Sock;
CError* CServerFrame::m_Error;
std::unordered_map <int, CClient> CServerFrame::m_mClients;

CServerFrame::CServerFrame()
{
	// �̴ϼȶ����� �ϳ� ������� ������ �������
	//

	m_Error = new CError;
}

CServerFrame::~CServerFrame()
{
	// �÷��̾�, ���� �� 
	// ��۸� ������ �ȳ����� ����

	if (m_Error != nullptr) {
		delete m_Error;
		m_Error = nullptr;
	}

	for (auto& h : m_hClientsThreads) {
		CloseHandle(h);
		h = NULL;
	}

	closesocket(m_ListenSock);

	WSACleanup();
}

int CServerFrame::InitTCPServer()
{
	wcout.imbue(locale("korean"));

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return -1;
	}

	// ListenSock ����
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ListenSock == INVALID_SOCKET)
	{
		m_Error->err_display("ServerFrame::InitTCPServer Sock() Error");
	}

	// Bind()
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(TCP_SERVERPORT/*����*/);
	int retval = bind(m_ListenSock, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	if (retval == SOCKET_ERROR) {
		m_Error->err_display("ServerFrame::InitTCPServer Bind() Error");

	}

	retval = listen(m_ListenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		m_Error->err_display("ServerFrame::InitTCPServer Listen() Error");
	}

	return 1;
}

int CServerFrame::InitUDPServer()
{
	// socket ����
	m_UDP_Sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_UDP_Sock == INVALID_SOCKET) {
		m_Error->err_display("ServerFrame::InitUDPServer Sock() Error");

	}

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(UDP_SERVERPORT);
	int retval = bind(m_UDP_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		m_Error->err_display("ServerFrame::InitUDPServer Sock() Error");
	}

	return 1;
}

void CServerFrame::UDP_Socket()
{
	while (true)
	{
		// move�Լ�
		UpdateMovePos();
		SendMovePos();
	}
}

void CServerFrame::UpdateMovePos()
{
	SOCKADDR_IN clientAddr;
	int addrLength;
	int retval;

	CS_Move_Packet move_packet;
	ZeroMemory(&move_packet, sizeof(CS_Move_Packet));

	addrLength = sizeof(clientAddr);
	retval = recvfrom(m_UDP_Sock, (char*)&move_packet, sizeof(CS_Move_Packet), 0, (SOCKADDR*)&clientAddr, &addrLength);

	if (retval == SOCKET_ERROR) m_Error->err_display("recvfrom() UpdateMovePos()");
	
	for (auto& cl : m_mClients) {
		if (cl.first == move_packet.id) {
			short x = cl.second.GetPosX();
			short y = cl.second.GetPosY();

			char dir = move_packet.state;
			switch (dir) {
			case UP: if (y > 0) y = y - 0.1f; break;
			case DOWN: if (y < (HEIGHT - 1)) y = y + 0.1f; break;
			case LEFT: if (x > 0) x = x - 0.1f; break;
			case RIGHT: if (x < (WIDTH - 1)) x = x + 0.1f; break;
			default: while (true);
			}
			cl.second.SetPos(x, y);

			SC_Move_Packet update_packet;
			ZeroMemory(&update_packet, sizeof(SC_Move_Packet));

			update_packet.id = cl.first;
			update_packet.size = sizeof(update_packet);
			update_packet.type = SC_PACKET_MOVE;
			update_packet.x = x;
			update_packet.y = y;

			retval = sendto(cl.second.GetSocket_UDP(), (char*)&update_packet, sizeof(SC_Move_Packet), 0, (SOCKADDR*)&clientAddr, addrLength);
		}
	}
}

void CServerFrame::LoginServer()
{
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLength;

	u_long ul_BlockingMode = NonBlocking;
	// 0�� ��� ���ŷ 0�̾ƴҰ�� �� ���ŷ

	int iChangeSocketMode = ioctlsocket(m_ListenSock, FIONBIO, &ul_BlockingMode);

	addrLength = sizeof(clientAddr);

	while (true)
	{
		// �̺�Ʈ ��� ���
		clientSock = accept(m_ListenSock, (SOCKADDR*)&clientAddr, &addrLength);
		if (clientSock == INVALID_SOCKET) {
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			else {
				m_Error->err_display("CServerFrame::RunServer() accept()");
				return;
			}
		}


		// ���ŷ ��� ��ȯ
		ul_BlockingMode = Blocking;

		int chagesocket = ioctlsocket(clientSock, FIONBIO, &ul_BlockingMode);
		if (chagesocket == SOCKET_ERROR)
			m_Error->err_display("CServerFrame::RunServer() changesocket1");

		// �ѹ���
		int id = 2;
		for (int i = 0; i < id; ++i) {
			if (m_mClients.count(i) == 0) {
				id = i;
				break;
			}
		}

		{
			// �������� Ŭ�� 2�� �̻��̸� ���� ����
			if (2 <= id) {
				closesocket(clientSock);
				continue;
			}
		}

		{
			// Print client Information
		/*	printf("\nŬ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d, id=%d\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), id
			);*/

			// std::cout << "send" << std::endl;

			// ID ����
			int retval = send(clientSock, (char*)&id, sizeof(int), 0);
			if (SOCKET_ERROR == retval) {
				/*if (WSAGetLastError() == WSAEWOULDBLOCK) {
					continue;
				}*/
				//else {
				m_Error->err_display("CServerFrame::RunLoginServer send()");
				return;
				//}
			}

			// Ŭ���̾�Ʈ ������ �����ϴ� map�� ����
			CClient client(clientSock, id, clientAddr);
			m_mClients.emplace(id, client);


			
			//m_hClientsThreads[id] = CreateThread(NULL, 0, /* ��Ŀ ������ */, (LPVOID)m_mClients[id].GetID(), 0, NULL);
			//if (NULL == m_hClientsThreads[id]) {
			//	closesocket(clientSock);
			//}
		}
	}
}

DWORD __stdcall CServerFrame::Process(LPVOID arg)
{
	int user_id = reinterpret_cast<int>(arg);

	SOCKADDR_IN	 Client_Addr;
	int addrLength = sizeof(Client_Addr);

	addrLength = sizeof(Client_Addr);
	getpeername(m_mClients[user_id].GetSocket_TCP(), (SOCKADDR*)&Client_Addr, &addrLength);



	return 0;
}

void CServerFrame::SaveNickName()
{

}

void CServerFrame::sendNickName()
{

}

void CServerFrame::UpdateStatus()
{

}

void CServerFrame::SendAllStatus()
{

}

void CServerFrame::IsAllReady()
{

}

void CServerFrame::BulletRoute()
{

}

void CServerFrame::Collision()
{

}

void CServerFrame::UpdatePosition()
{

}