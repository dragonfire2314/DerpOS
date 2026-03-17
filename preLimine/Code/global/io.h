extern "C" void write_port(unsigned short port, unsigned char data);
extern "C" char read_port(unsigned short port);

extern "C" void write_port_w(unsigned short port, unsigned short data);
extern "C" unsigned short read_port_w(unsigned short port);