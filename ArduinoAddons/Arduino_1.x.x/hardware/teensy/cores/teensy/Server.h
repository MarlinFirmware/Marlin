#if ARDUINO >= 100

#ifndef server_h
#define server_h

class Server : public Print {
public:
  virtual void begin() =0;
};

#endif
#endif
