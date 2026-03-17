//Init the serial line
void serial_init();
//Output a character
void serial_write_char(char a);
//Output null terminated string
void serial_write_string(char* str);
//Output data with a size
void serial_write_data(char* str, int size);
//Output 4 bytes to serial port
void serial_write_int(int data);
//Output 1 byte to serial port
void serial_write_byte(char data);
//Output 4 bytes to serial port with a message
void serial_write_debug(char* str, int data);