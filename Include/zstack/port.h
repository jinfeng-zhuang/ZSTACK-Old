#ifndef ZSTACK_PORT_H
#define ZSTACK_PORT_H

#ifdef _WIN32
#define vsnprintf _vsnprintf
#define snprintf _snprintf
#else
#endif

#endif