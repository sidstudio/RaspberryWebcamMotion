#ifndef TYPE_H
#define TYPE_H

enum class ServerSendCommand: int
{
    SEND_IMAGE_ORIGINAL        = 1,
    SEND_IMAGE_DETECTION_FACE  = 2,
    SEND_IMAGE_MOTION          = 3
};

#endif // TYPE_H

