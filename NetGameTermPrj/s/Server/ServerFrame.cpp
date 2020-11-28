#include "ServerFrame.h"

unordered_map<int, Client> ServerFrame::m_Clients;
int ServerFrame::m_id;
set<string> ServerFrame::m_nick;

ServerFrame::ServerFrame()
{

}

ServerFrame::~ServerFrame()
{
	for (auto& h : m_hCThreads) {
		CloseHandle(h);
		h = NULL;
	}

	closesocket(m_sock);
	WSACleanup();
}

void ServerFrame::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void ServerFrame::err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int ServerFrame::InitTCPServer()
{
	std::wcout.imbue(locale("korean"));

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	return -1;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) err_display("InitTCPServer() -> socket() Error");

	SOCKADDR_IN Server_Addr;
	ZeroMemory(&Server_Addr, sizeof(Server_Addr));
	Server_Addr.sin_family = AF_INET;
	Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Server_Addr.sin_port = htons(TCP_SERVERPORT);
	int ret = bind(m_sock, (SOCKADDR*)&Server_Addr, sizeof(Server_Addr));
	if (ret == SOCKET_ERROR) err_display("InitTCPServer() -> bind() Error");

	ret = listen(m_sock, SOMAXCONN);
	if (ret == SOCKET_ERROR) err_display("InitTCPServer() -> listen() Error");

	return 1;
}

void ServerFrame::LoginServer()
{
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrlen, u_id;

	while (true) {
		clientSock = accept(m_sock, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSock == INVALID_SOCKET) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) continue;
			else err_display("LoginServer() -> accept() Error");
		}

		int id = 2;
		for (int i = 0; i < id; ++i) {
			if (m_Clients.count(i) == 0) {
				id = i;
				break;
			}
		}

		if (2 <= id) {
			closesocket(clientSock);
			continue;
		}

		int ret = send(clientSock, (char*)&id, sizeof(int), 0);
		if (ret == SOCKET_ERROR) err_display("LoginServer() -> send() Error");

		Client client(clientSock, id, clientAddr);
		m_Clients.emplace(id, client);

		u_id = id;
		m_hCThreads[id] = CreateThread(NULL, 0, this->Process, (LPVOID)m_Clients[id].GetID(), 0, NULL);
		if (NULL == m_hCThreads[id]) closesocket(clientSock);		
	}

	LobbyServer(u_id);
}

void ServerFrame::LobbyServer(int id)
{
	CreateMoveThread(id);
	// 입구 간 사람 몇명인지 확인
	// 패킷 주고받으면서 두명 다 입구에 있는거 확인되면
	// 인게임으로 넘어가기?

	//InGameServer();
}

void ServerFrame::InGameServer()
{
	while (true) {
		UpdateStatus();
		UpdateCollision();
		UpdateBoss();
	}
}

DWORD __stdcall ServerFrame::Process(LPVOID arg)
{
	int id = reinterpret_cast<int>(arg);

	SOCKADDR_IN Client_Addr;
	int addrlen = sizeof(Client_Addr);
	getpeername(m_Clients[id].GetSock_TCP(), (SOCKADDR*)&Client_Addr, &addrlen);

	bool Login = false;
	while (!Login) {
		CS_Client_Login_Packet login_packet;
		ZeroMemory(&login_packet, sizeof(CS_Client_Login_Packet));

		int ret = recv(m_Clients[id].GetSock_TCP(), (char*)&login_packet, sizeof(CS_Client_Login_Packet), 0);
		if (ret == SOCKET_ERROR || ret == 0) {
			err_display("Process() -> recv() : login Error");
			closesocket(m_Clients[id].GetSock_TCP());
			m_Clients.erase(id);
			return 0;
		}

		m_Clients[id].SetNickname(login_packet.nickname);

		int nick_size = m_nick.size();
		m_nick.insert(login_packet.nickname);

		SC_Client_LoginOK_Packet loginok_packet;
		loginok_packet.size = sizeof(SC_Client_LoginOK_Packet);

		if (nick_size == m_nick.size()) {
			loginok_packet.type = NICKNAME_UNUSE;
			ret = send(m_Clients[id].GetSock_TCP(), (char*)&loginok_packet, sizeof(SC_Client_LoginOK_Packet), 0);
		}
		else {
			loginok_packet.type = NICKNAME_USE;
			ret = send(m_Clients[id].GetSock_TCP(), (char*)&loginok_packet, sizeof(SC_Client_LoginOK_Packet), 0);
			Login = true;
		}

		//for (auto& cl : m_Clients) {
		//	int i = cl.second.GetID();
		//	m_ready.id[i] = cl.second.GetReady();
		//	strcpy_s(m_ready.nickname[i], m_Clients[i].GetNickname().c_str());
		//}
		//m_Clients[id].SetReady(true);

		//m_ready.type = 


	}

	return 0;
}

void ServerFrame::CreateMoveThread(int id)
{
	m_MOVEThread = CreateThread(NULL, 0, this->MOVEThread, (LPVOID)m_Clients[id].GetID(), 0, NULL);
}

DWORD __stdcall ServerFrame::MOVEThread(LPVOID arg)
{
	int id = reinterpret_cast<int>(arg);
	while (true) {
		UpdateMove(id);
	}

	return 0;
}

void ServerFrame::UpdateMove(int id)
{
	SOCKADDR_IN clientAddr;
	int addrlen;

	CS_Move_Packet move_packet;
	ZeroMemory(&move_packet, sizeof(CS_Move_Packet));

	addrlen = sizeof(clientAddr);
	int ret = recv(m_Clients[id].GetSock_TCP(), (char*)&move_packet, sizeof(CS_Move_Packet), 0);
	if (ret == SOCKET_ERROR) err_display("UpdateMove() -> recv() Error");

	float x = m_Clients[id].GetX();
	float y = m_Clients[id].GetY();

	char dir = move_packet.type;
	switch (dir) {
	case MOVE_UP: if (y > 0) y = y - 0.1f; break;
	case MOVE_DOWN: if (y < (HEIGHT - 1)) y = y + 0.1f; break;
	case MOVE_LEFT: if (x > 0) x = x - 0.1f; break;
	case MOVE_RIGHT: if (x < (WIDTH - 1)) x = x + 0.1f; break;
	default: while (true);
	}
	m_Clients[id].SetPos(x, y);

	SC_Move_Packet update_packet;
	ZeroMemory(&update_packet, sizeof(SC_Move_Packet));

	update_packet.type = SC_PACKET_MOVE;
	update_packet.id = id;
	update_packet.x = x;
	update_packet.y = y;
	update_packet.size = sizeof(update_packet);

	ret = send(m_Clients[id].GetSock_TCP(), (char*)&update_packet, sizeof(SC_Move_Packet), 0);
	if (ret == SOCKET_ERROR) err_display("UpdateMove -> send() Error");
}

void ServerFrame::UpdateStatus()
{
}

void ServerFrame::SendAllStatus()
{
}

void ServerFrame::IsAllReady()
{
}

void ServerFrame::SendBulletRoute()
{
}

void ServerFrame::UpdateCollision()
{
}

void ServerFrame::UpdateBoss()
{
}

