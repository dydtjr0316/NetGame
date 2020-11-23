#include "CServerFrame.h"

SOCKET	CServerFrame::m_UDP_Sock;
CError* CServerFrame::m_Error;
unordered_map <int, CClient> CServerFrame::m_mClients;

CServerFrame::CServerFrame()
{
	// 이니셜라이저 하나 만들던가 생성자 오버사용
}

CServerFrame::~CServerFrame()
{
	// 플레이어, 보스 등 
	// 댕글링 포인터 안나오게 삭제
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

	// ListenSock 생성
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
	ServerAddr.sin_port = htons(TCP_SERVERPORT/*수정*/);
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
	// socket 생성
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
		// move함수
		UpdateMovePos();
		SendMovePos();
	}
}

void CServerFrame::UpdateMovePos()
{
	SOCKADDR_IN clientAddr;
	int addrLength;
	int retval;

	// 보낼 MovePos Packet
	SC_Move_Packet move_packet;
	// 0초기화
	ZeroMemory(&move_packet, sizeof(SC_Move_Packet));

	addrLength = sizeof(clientAddr);
	retval = recvfrom(m_UDP_Sock, (char*)&move_packet, sizeof(SC_Move_Packet), 0, (SOCKADDR*)&clientAddr, &addrLength);

	if (retval == SOCKET_ERROR) m_Error->err_display("recvfrom() UpdateMovePos()");

	int id = move_packet.id;
	float hp = move_packet.hp;
	short x = move_packet.x;
	short y = move_packet.y;

	for (auto& client : m_mClients) {
		if (client.first == id) client.second.SetPos(x, y);
	}
}

void CServerFrame::SendMovePos()
{

}

void CServerFrame::LoginServer()
{
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLength;

	u_long ul_BlockingMode = NonBlocking;
	// 0일 경우 블로킹 0이아닐경우 논 블로킹

	int iChangeSocketMode = ioctlsocket(m_ListenSock, FIONBIO, &ul_BlockingMode);

	addrLength = sizeof(clientAddr);

	while (true)
	{
		// 이벤트 모드 대기
		clientSock = accept(m_ListenSock, (SOCKADDR*)&clientAddr, &addrLength);
		if (clientSock == INVALID_SOCKET) {
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			else {
				m_Error->err_display("CServerFrame::RunServer() accept()");
				return;
			}
		}


		// 블로킹 모드 변환
		ul_BlockingMode = Blocking;

		int chagesocket = ioctlsocket(clientSock, FIONBIO, &ul_BlockingMode);
		if (chagesocket == SOCKET_ERROR)
			m_Error->err_display("CServerFrame::RunServer() changesocket1");

		// 넘버링
		int id = 2;
		for (int i = 0; i < id; ++i) {
			if (m_mClients.count(i) == 0) {
				id = i;
				break;
			}
		}

		{
			// 접속중인 클라가 2명 이상이면 접속 끊음
			if (2 <= id) {
				closesocket(clientSock);
				continue;
			}
		}

		{
			// Print client Information
		/*	printf("\n클라이언트 접속: IP 주소=%s, 포트 번호=%d, id=%d\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), id
			);*/

			// std::cout << "send" << std::endl;

			// ID 전송
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

			// 클라이언트 정보를 저장하는 map에 저장
			CPlayer client(clientSock, id, clientAddr);
			m_mClients.emplace(id, client);


			
			//m_hClientsThreads[id] = CreateThread(NULL, 0, /* 워커 스레드 */, (LPVOID)m_mClients[id].GetID(), 0, NULL);
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