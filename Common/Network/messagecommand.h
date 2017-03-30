#ifndef MESSAGECOMMAND_H
#define MESSAGECOMMAND_H


enum OP_CODE_CMD_REQ
{
    REQ_CONNECT_USER    = 0x00010001,
    REQ_LOGIN_USER      = 0x00020001,
    REQ_GET_VERSION     = 0x00030002,
};

enum OP_CODE_CMD_RES
{
    ACK_CONNECT_USER    = 0x10010001,
    ACK_LOGIN_USER      = 0x10020001,
    ACK_GET_VERSION     = 0x10030002,
    ACK_NOT_LOGGED_IN   = 0x1F00FFFF,
};

class MessageCommand
{
public:
    MessageCommand();
};

#endif // MESSAGECOMMAND_H
