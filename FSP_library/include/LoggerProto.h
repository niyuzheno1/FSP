#pragma once

class LoggerProto{
public:
    virtual void log(const char * message) = 0;
};