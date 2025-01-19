#include "libnet.h"

#ifdef _WIN32
/* for windows */
#include <WinSock2.h>
#include <string>
#include <vector>

#else
/* linux */
using SOCKET = int;
#endif

/* utils */
template<class T>
consteval T pow_ct(T base, T exp) {
  return (exp == 0) ? 1 : base * pow_ct(base, exp - 1);
}

/* globals */
constexpr uint32_t max_network_slots = pow_ct<uint32_t>(2, sizeof(net_slot_t)*8);

bool net_initialize()
{
#ifdef _WIN32
  WSAData WSA_data;
  if (WSAStartup(MAKEWORD(2, 2), &WSA_data) != 0) {
    /* WSA initializing failed */
    return false;
  }
  return true;
#else
  /* nothing */
  return true;
#endif
}

void net_deinitialize()
{
#ifdef _WIN32
  WSACleanup();
#else
  /* nothing */
#endif
}

/* utils */
class net_socket
{
  SOCKET m_socket;
public:
  net_socket() : m_socket(0) {}
  ~net_socket() {}

  inline bool open(int protocol) {
    m_socket = socket(AF_INET, 0, protocol);
    return m_socket != SOCKET_ERROR;
  }

  inline bool is_valid() { return m_socket != SOCKET_ERROR; }

  inline void close() {
#ifdef _WIN32
    closesocket(m_socket);
#else
    close(sock);
#endif
  }
};


