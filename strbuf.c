
#define sbuf_append(sbuf, ptr, len) \
    if(((sbuf.head + sbuf.size) - sbuf.tail) < len); \
    memcpy(sbuf.tail, ptr, len);


struct strbuf {
union
{
struct{ wchar_t* head; wchar_t* tail;} w;	
struct{ char* head; char* tail;}       a;
};
unsigned size: 24;
char wide: 1;
unsigned short attr;
};

typedef struct string{
    union {char *ascii; wchar_t *unicode;};
    unsigned length;
} string;

#define STRBUF_APPEND1(strbuf, s1, p1)\
    if( (strbuf).wide ){ \
        if (s1) {\
        memcpy( (strbuf).w.tail, p1, s1<<1 );\
        (strbuf).w.tail += s1<<1;\
        }\
        else{\
        strcpy( (strbuf).w.tail, p1 );\
        (strbuf).w.tail += strlen(p1);\
        }\
    }\
    else{\
        if (s1) {\
        memcpy( (strbuf).a.tail, p1, s1 );\
        (strbuf).a.tail += s1;\
        }\
        else{\
        strcpy( (strbuf).a.tail, p1 );\
        (strbuf).a.tail += strlen(p1);\
        }\
    }

#define STRBUF_APPEND2(strbuf, s1, p1, s2, p2)\
    STRBUF_APPEND1(strbuf, s1, p1);\
    STRBUF_APPEND1(strbuf, s2, p2);

//return the pointer to the head of a string buffer
#define STRBUF_HEAD(strbuf)\
    ( (strbuf).wide ? (strbuf).w.head : (strbuf).a.head )

//return the pointer to the tail of a string buffer
#define STRBUF_TAIL(strbuf)\
    ( (strbuf).wide ? (strbuf).a.tail : (strbuf).a.tail )

//eraase <count> characters from a string buffer
#define STRBUF_ERASE(strbuf, count)\
    ((strbuf).wide\
        ?\
    ((count >= (strbuf).w.tail - (strbuf).w.head) ?\
    ((strbuf).w.tail - ((strbuf).w.tail = (strbuf).w.head), (strbuf).w.tail = 0) :\
    ((strbuf).w.tail -= count, (strbuf).w.tail = 0))\
        :\
    ((count >= (strbuf).a.tail - (strbuf).a.head) ?\
    ((strbuf).a.tail - ((strbuf).a.tail = (strbuf).a.head), (strbuf).a.tail = 0) :\
    ((strbuf).a.tail -= count, (strbuf).a.tail = 0)))


