#pragma once
class CClient
{
public:	// »ý¼º ¼Ò¸ê
	CClient();
	CClient(SOCKET, int, SOCKADDR_IN);
	~CClient();

public:
	// // Get
	// NetWork
	SOCKET	GetSocket_TCP() { return m_TcpSocket; }
	SOCKET	GetSocket_UDP() { return m_UdpSocket; }

	//Client Data
	std::string GetNickName() { return m_cNickName; }

	int		GetID() { return m_PlayerId; }
	int		GetPlayerHp() { return m_iPlayerHp; }
	int		GetBossHp() { return m_iBossHp; }

	float	GetPosX() { return m_fPosX; }
	float	GetPosY() { return m_fPosY; }

	bool	GetIsReady() { return m_bisReady; }
	bool	GetIsDamaged() { return m_bisDamaged; }
	bool	GetIsPlayerDead() { return m_bisPlayerDead; }
	bool	GetIsBossDead() { return m_bisBossDead; }

	// // Set
	// NetWork
	void	SetSocket_TCP(SOCKET Socket);
	void	SetSocket_UDP(SOCKET Socket);

	//Client Data
	void	SetNickName(char *NickName);
	void	SetPlayerHp(int hp);

	void	SetPos(float x, float y);

	void	SetIsReady(bool ready);
	void	SetIsDamaged(bool damaged);
	void	SetIsPlayerDead(bool dead);
	void	SetIsBossDead(bool dead);

private:
	// Network
	SOCKET		m_TcpSocket;
	SOCKET		m_UdpSocket;
	SOCKADDR_IN m_addr;
	
	// Client Data
	string	m_cNickName;

	int		m_PlayerId;
	int		m_iPlayerHp;
	int		m_iBossHp;

	float	m_fPosX;
	float	m_fPosY;

	bool	m_bisReady;
	bool	m_bisDamaged;
	bool	m_bisPlayerDead;
	bool	m_bisBossDead;


};

