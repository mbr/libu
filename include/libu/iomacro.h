#ifndef IOMACRO_H
#define IOMACRO_H 

#define DECLARE_SEND(name) void name##_send(const char c)
#define DEFINE_SEND(name, putc, block) DECLARE_SEND(name) { block; putc(c); }

#define DECLARE_SEND_STR(name) void name##_send_str(const char *s)
#define DEFINE_SEND_STR(name, putc, block) DECLARE_SEND_STR(name) { while(*s) { block; putc(*(s++)); } }

#define DECLARE_SEND_DATA(name) void name##_send_data(const char *s, size_t len)
#define DEFINE_SEND_DATA(name, putc, block) DECLARE_SEND_DATA(name) { while(len) { block; putc(*(s++)); --len; } }

#endif /* IOMACRO_H */
