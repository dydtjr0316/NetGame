#include "ServerFrame.h"

unordered_map<int, Client> ServerFrame::m_Clients;
int ServerFrame::m_id;

ServerFrame::ServerFrame()
{

}

ServerFrame::~ServerFrame()
{
	m_Clients.erase(m_id);

	for (auto& h : m_hCThreads) {
		CloseHandle(h);
		h = NULL;
	}

	closesocket(m_sock);
	WSACleanup();
}

void ServerFrame::err_display(const char* msg)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"에러 " << lpMsgBuf << std::endl;
	while (true);
	LocalFree(lpMsgBuf);
}

int ServerFrame::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int ServerFrame::InitTCPServer()
{
	std::wcout.imbue(locale("korean"));

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	return -1;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) err_display("InitTCPServer() -> socket()");

	SOCKADDR_IN Server_Addr;
	ZeroMemory(&Server_Addr, sizeof(Server_Addr));
	Server_Addr.sin_family = AF_INET;
	Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Server_Addr.sin_port = htons(TCP_SERVERPORT);
	int ret = bind(m_sock, (SOCKADDR*)&Server_Addr, sizeof(Server_Addr));
	if (ret == SOCKET_ERROR) err_display("InitTCPServer() -> bind()");

	ret = listen(m_sock, SOMAXCONN);
	if (ret == SOCKET_ERROR) err_display("InitTCPServer() -> listen()");

	return 1;
}

void ServerFrame::LoginServer()
{
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrlen, u_id;
	
	while (true) {
		addrlen = sizeof(clientAddr);
		clientSock = accept(m_sock, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSock == INVALID_SOCKET) err_display((char*)"LoginServer() -> accept()");

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
		if (ret == SOCKET_ERROR) err_display("LoginServer() -> send()");

		Client client(clientSock, id, clientAddr);
		m_Clients.emplace(id, client);

		u_id = id;
		cout << "Enter " << id << endl;
		m_hCThreads[id] = CreateThread(NULL, 0, this->Process, (LPVOID)m_Clients[id].GetID(), 0, NULL);

		if (NULL == m_hCThreads[id]) closesocket(clientSock);

		LobbyServer(u_id);
	}

}

void ServerFrame::LobbyServer(int id)
{
	CreateMoveThread(id);
	//CreateAttackThread(id);
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

		//int ret = recvn(m_Clients[id].GetSock_TCP(), (char*)&login_packet, sizeof(CS_Client_Login_Packet), 0);
		//if (login_packet.type != ENTER_USER) continue;
		//
		//if (ret == SOCKET_ERROR || ret == 0) {
		//	err_display("Process() -> recv() : login");
		//	closesocket(m_Clients[id].GetSock_TCP());
		//	m_Clients.erase(id);
		//	return 0;
		//}

		int ret = recvn(m_Clients[id].GetSock_TCP(), (char*)&login_packet, sizeof(CS_Client_Login_Packet), 0);
      if (login_packet.type != ENTER_USER) continue;
      if (ret == SOCKET_ERROR) {
         err_display("Process() -> recv() : login");
         closesocket(m_Clients[id].GetSock_TCP());
         m_Clients.erase(id);
         return 0;
      }

		m_Clients[id].SetNickname(login_packet.nickname);
		cout << "Enter : " << id << ", " << m_Clients[id].GetNickname().c_str() << endl;

		SC_Client_LoginOK_Packet p;
		ZeroMemory(&p, sizeof(SC_Client_LoginOK_Packet));
		p.size = sizeof(SC_Client_LoginOK_Packet);
		strcpy_s(p.nickname, m_Clients[id].GetNickname().c_str());
		p.type = NICKNAME_USE;
		cout << "type -> " << p.type << endl;

		ret = send(m_Clients[id].GetSock_TCP(), (char*)&p, sizeof(p), 0);

		if (id != 0) {
			if (m_Clients.count(1) != 0) {
				Send_enter_packet(0, id);
				Send_enter_packet(id, 0);
				//Login = true;
			}
		}
		Login = true;
	}

	return 0;
}

void ServerFrame::Send_enter_packet(int to, int id)
{
	SC_Client_Enter_Packet packet;
	packet.id = id;
	packet.size = sizeof(SC_Client_Enter_Packet);
	packet.type = ENTER_USER;

	packet.posX = m_Clients[id].GetX();
	packet.posY = m_Clients[id].GetY();

	strcpy_s(packet.nickname, m_Clients[id].GetNickname().c_str());

	//Send_pakcet(to, &packet);
	send(m_Clients[to].GetSock_TCP(), (char*)&packet, sizeof(packet), 0);
}

void ServerFrame::Send_pakcet(int id, void* p)
{
	unsigned char* packet = reinterpret_cast<unsigned char*>(p);
	send(m_Clients[id].GetSock_TCP(), (char*)&packet, sizeof(packet), 0);
}

void ServerFrame::CreateMoveThread(int id)
{
	m_MOVEThread = CreateThread(NULL, 0, this->MOVEThread, (LPVOID)id, 0, NULL);
}

void ServerFrame::CreateAttackThread(int id)
{
	m_AttackThread = CreateThread(NULL, 0, this->AttackThread, (LPVOID)id, 0, NULL);
}

DWORD __stdcall ServerFrame::MOVEThread(LPVOID arg)
{
	int id = reinterpret_cast<int>(arg);
	while (true) {
		UpdateMove(id);

	}

	return 0;
}

DWORD __stdcall ServerFrame::AttackThread(LPVOID arg)
{
	int id = reinterpret_cast<int>(arg);
	while (true) {
		UpdateAttack(id);
	}

	return 0;
}

void ServerFrame::UpdateMove(int id)
{
	CS_Move_Packet move_packet;
	ZeroMemory(&move_packet, sizeof(CS_Move_Packet));

	int ret = recvn(m_Clients[id].GetSock_TCP(), (char*)&move_packet, sizeof(CS_Move_Packet), 0);
	if (ret == SOCKET_ERROR) err_display("UpdateMove() -> recv()");

	float fX, fY, fZ;
	fX = fY = fZ = 0.0f;
	float fAmount = 20.f;
	float fSize = 0.f;

	if (move_packet.type == CS_PACKET_MOVE)		// 움직임이 있을때만 
	{
		SC_Move_Packet update_packet;
		ZeroMemory(&update_packet, sizeof(SC_Move_Packet));

		switch (move_packet.dir)				// head, state 처리 전부 서버에서
		{
		case DIR::UP:
			fY += 0.1f;
			update_packet.curstate = STATE::UP;
			break;
		case DIR::DOWN:
			fY -= 0.1f;
			update_packet.curstate = STATE::DOWN;
			break;
		case DIR::LEFT:
			fX -= 0.1f;
			update_packet.curstate = STATE::LEFT;
			break;
		case DIR::RIGHT:
			fX += 0.1f;
			update_packet.curstate = STATE::RIGHT;
			break;
		default:
			update_packet.curstate = STATE::IDLE;
			break;
		}

		switch (move_packet.head)
		{
		case UP:
			update_packet.head = STATE::UP;
			break;
		case DOWN:
			update_packet.head = STATE::DOWN;
			break;
		case LEFT:
			update_packet.head = STATE::LEFT;
			break;
		case RIGHT:
			update_packet.head = STATE::RIGHT;
			break;
		default:
			update_packet.head = STATE::DOWN;
			break;
		}

		fSize = sqrtf(fX * fX + fY * fY);

		if (fSize > FLT_EPSILON)
		{
			fX /= fSize;
			fY /= fSize;
			fX *= fAmount;
			fY *= fAmount;

			float accX, accY, accZ;
			accX = accY = accZ = 0.f;

			// 수정
			accX = fX / move_packet.mass;
			accY = fY / move_packet.mass;

			move_packet.velx = move_packet.velx + accX * move_packet.elapsedInSec;
			move_packet.vely = move_packet.vely + accY * move_packet.elapsedInSec;
		}

		update_packet.type = SC_PACKET_MOVE;
		update_packet.id = id;
		update_packet.size = sizeof(update_packet);

		update_packet.x = move_packet.velx;
		update_packet.y = move_packet.vely;

		for (auto& m : m_Clients) {
			ret = send(m_Clients[m.first].GetSock_TCP(), (char*)&update_packet, sizeof(SC_Move_Packet), 0);
			if (ret == SOCKET_ERROR) err_display("UpdateMove -> send()");
		}
	}
}

void ServerFrame::UpdateAttack(int id)
{
	CS_Attack_Packet attack_packet;
	ZeroMemory(&attack_packet, sizeof(CS_Attack_Packet));

	int ret = recvn(m_Clients[id].GetSock_TCP(), (char*)&attack_packet, sizeof(CS_Attack_Packet), 0);
	if (ret == SOCKET_ERROR) err_display("UpdateAttack() -> recv()");

	float vBulletX, vBulletY, vBulletZ;
	vBulletX = vBulletY = vBulletZ = 0.f;
	if (attack_packet.type == CS_PACKET_ATTACK)
	{
		switch (attack_packet.head) {
		case UP:
			vBulletY += 0.2f;
			break;
		case DOWN:
			vBulletY -= 0.2f;
			break;
		case LEFT:
			vBulletX -= 0.2f;
			break;
		case RIGHT:
			vBulletX += 0.2f;
			break;
		default:
			break;
		}


		float vBulletSize = sqrtf(vBulletX * vBulletX + vBulletY * vBulletY + vBulletZ * vBulletZ);

		if (vBulletSize > 0.000001f)
		{
			vBulletX /= vBulletSize;
			vBulletY /= vBulletSize;
			vBulletZ /= vBulletSize;

			vBulletX *= attack_packet.bulletvel;
			vBulletY *= attack_packet.bulletvel;
			vBulletZ *= attack_packet.bulletvel;
		}

		//

		SC_Attack_Packet update_attack_packet;
		ZeroMemory(&update_attack_packet, sizeof(SC_Attack_Packet));

		update_attack_packet.bulletx = vBulletX;
		update_attack_packet.bullety = vBulletY;
		update_attack_packet.bulletz = vBulletZ;
		update_attack_packet.bulletsize = vBulletSize;
		update_attack_packet.type = SC_PACKET_ATTACK;
		update_attack_packet.id = id;
		update_attack_packet.size = sizeof(SC_Attack_Packet);


		for (auto& m : m_Clients) {
			ret = send(m_Clients[m.first].GetSock_TCP(), (char*)&update_attack_packet, sizeof(SC_Move_Packet), 0);
			if (ret == SOCKET_ERROR) err_display("UpdateAttack -> send()");
		}
	}
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

CS_Move_Packet ServerFrame::AddForce(CS_Move_Packet& move_packet)
{
	float fX, fY, fZ;
	fX = fY = fZ = 0.0f;
	float fAmount = 20.f;
	float fSize = 0.f;

	fSize = sqrtf(fX * fX + fY * fY);

	fX /= fSize;
	fY /= fSize;
	fX *= fAmount;
	fY *= fAmount;

	float accX, accY, accZ;
	accX = accY = accZ = 0.f;

	accX = fX / move_packet.mass;
	accY = fY / move_packet.mass;
	accZ = fZ / move_packet.mass;

	move_packet.velx = move_packet.velx + accX * move_packet.elapsedInSec;
	move_packet.vely = move_packet.vely + accY * move_packet.elapsedInSec;

	CS_Move_Packet temp = move_packet;

	return temp;
}