#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include <RingBuffer.h>

struct ServerInfo {
  TCPsocket socket;
  SDLNet_SocketSet socket_set;
  static constexpr uint32_t max_connections = 1;
  static constexpr uint32_t max_packet_size = 32768;
  TCPsocket sockets[max_connections];
  uint32_t next_index = 0;
};

class RawSocketSerial {
public:
  void stop_listen() {
    if (SDLNet_TCP_DelSocket(server.socket_set, server.socket) == -1) {
      fprintf(stderr, "RawSocketSerial::stop_listen: SDLNet_TCP_DelSocket: %s\n", SDLNet_GetError());
      exit(-1);
    }
    SDLNet_TCP_Close(server.socket);

    for (uint32_t i = 0; i < ServerInfo::max_connections; ++i) {
      if (server.sockets[i] == nullptr) continue;
      close_socket(i);
    }
  }

  void listen_on_port(uint16_t port) {
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, port) == -1) {
      fprintf(stderr, "RawSocketSerial::start: SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(-1);
    }
    start_listen(ip);
    thread_active = true;
    server_thread = std::thread(&RawSocketSerial::execute, this);
  }

  void stop() {
    stop_listen();
    thread_active = false;
    server_thread.join();
  }

  virtual ~RawSocketSerial() {
    if (thread_active) {
      stop();
    }
  }

  void start_listen(IPaddress ip) {
    server.socket = SDLNet_TCP_Open(&ip);
    if (server.socket == nullptr) {
      fprintf(stderr, "RawSocketSerial::start_listen: SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      exit(-1);
    }

    server.socket_set = SDLNet_AllocSocketSet(server.max_connections + 1);
    if (server.socket_set == nullptr) {
      fprintf(stderr, "RawSocketSerial::start_listen: SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
      exit(-1);
    }

    if (SDLNet_TCP_AddSocket(server.socket_set, server.socket) == -1) {
      fprintf(stderr, "RawSocketSerial::start_listen: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
      exit(-1);
    }
  }

  void close_socket(int index) {
    if (server.sockets[index] == nullptr) {
      fprintf(stderr, "RawSocketSerial::close_socket: Attempted to delete a NULL socket.\n");
      return;
    }

    if (SDLNet_TCP_DelSocket(server.socket_set, server.sockets[index]) == -1) {
      fprintf(stderr, "RawSocketSerial::close_socket: SDLNet_TCP_DelSocket: %s\n", SDLNet_GetError());
      exit(-1);
    }

    SDLNet_TCP_Close(server.sockets[index]);
    server.sockets[index] = nullptr;
  }

  int accept_socket(int index) {
    if (server.sockets[index]) {
      fprintf(stderr, "RawSocketSerial::accept_socket: Overriding socket at index %d.\n", index);
      close_socket(index);
    }

    server.sockets[index] = SDLNet_TCP_Accept(server.socket);
    if (server.sockets[index] == nullptr) return 0;

    if (SDLNet_TCP_AddSocket(server.socket_set, server.sockets[index]) == -1) {
      fprintf(stderr, "RawSocketSerial::accept_socket: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
      exit(-1);
    }

    return 1;
  }

  int32_t receive(int index) {
    int32_t length = SDLNet_TCP_Recv(server.sockets[index], receive_buffer, ServerInfo::max_packet_size);
    if (length <= 0) {
      close_socket(index);
      char const* err = SDLNet_GetError();
      if (strlen(err) == 0) {
        printf("RawSocketSerial::receive: client disconnected\n");
      } else {
        fprintf(stderr, "RawSocketSerial::receive: SDLNet_TCP_Recv: %s\n", err);
      }
      return 0;
    } else {
      std::scoped_lock buffer_lock(buffer_mutex);
      rx_buffer.write(receive_buffer, length);
      return length;
    }
  }

  int32_t send(int index) {
    std::size_t length   = tx_buffer.read(transmit_buffer, ServerInfo::max_packet_size);
    std::size_t num_sent = SDLNet_TCP_Send(server.sockets[index], transmit_buffer, length);
    if (num_sent < length) {
      fprintf(stderr, "RawSocketSerial::send: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
      close_socket(index);
    }
    return num_sent;
  }

  void execute() {
    while (thread_active) {
      int num_rdy = SDLNet_CheckSockets(server.socket_set, 0);

      if (num_rdy <= 0) {
        for (uint32_t i = 0; i < server.max_connections; ++i) {
          if (server.sockets[i] != nullptr) send(i);
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        continue;
      }

      if (SDLNet_SocketReady(server.socket)) {
        int got_socket = accept_socket(server.next_index);
        if (!got_socket) {
          num_rdy--;
          continue;
        }

        uint32_t chk_count = 0;
        for (; chk_count < ServerInfo::max_connections; ++chk_count) {
          if (server.sockets[(server.next_index + chk_count) % ServerInfo::max_connections] == nullptr) break;
        }

        server.next_index = (server.next_index + chk_count) % ServerInfo::max_connections;
        printf("RawSocketSerial::thread_main: new connection (server.next_index = %d)\n", server.next_index);
        num_rdy--;
      }

      for (uint32_t ind = 0; (ind < ServerInfo::max_connections) && num_rdy; ++ind) {
        if (server.sockets[ind] == nullptr) continue;
        if (!SDLNet_SocketReady(server.sockets[ind])) continue;
        receive(ind--);
      }
    }
  }

  std::size_t available() {
    return rx_buffer.available();
  }

  int16_t read() {
    uint8_t value = 0;
    uint32_t ret  = rx_buffer.read(&value);
    return (ret ? value : -1);
  }

  size_t readBytes(char* dst, size_t length) {
    return rx_buffer.read((uint8_t*)dst, length);
  }

  size_t write(char c) {
    return tx_buffer.write(c);
  }

  void write(char const* str) {
    while (*str)
      tx_buffer.write(*str++);
  }

  void write(uint8_t const* buffer, size_t size) {
    tx_buffer.write((uint8_t*)buffer, size);
  }

  bool thread_active = false;
  ServerInfo server {};
  uint8_t receive_buffer[ServerInfo::max_packet_size];
  uint8_t transmit_buffer[ServerInfo::max_packet_size];
  RingBuffer<uint8_t, ServerInfo::max_packet_size> rx_buffer;
  RingBuffer<uint8_t, ServerInfo::max_packet_size> tx_buffer;
  std::thread server_thread;
  std::mutex buffer_mutex {};
};
