

#ifndef SERVER_MESSAGETYPES_H
#define SERVER_MESSAGETYPES_H

#define DEFAULT_PORT 10001

enum MessageType
{
    STATUS,
    ACQUIRE,
    SET_EXPOSURE,
    SET_GAIN
};

#endif //SERVER_MESSAGETYPES_H