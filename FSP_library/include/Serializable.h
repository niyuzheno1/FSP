#pragma once
#include <functional>
#include "LoggerProto.h"
#include "FSPMemoryInterface.h"

//Note: This is c++11 feature and is not supported by all compilers
//Note: simulator's writing into the file depends on the endianness of the machine
using Simulator = std::function<void(void *, size_t)>;

//TODO: add a class for memory allocation

class Serializable {
public:
    static size_t pointValueCount;
    static void ** oldPointerValues;
    static void ** newPointerValues;

    static size_t * serializableIntegerTypes; 
    static Serializable ** serializables;
    static size_t serializableClassCount;

    static size_t observingSerializableCount;
    static Serializable ** observingSerializables;
    static void ** observingSerializableOldPointerValues;

    Serializable();
    static size_t registerSerializable(size_t type, Serializable * serializable);
    static void observe(void * oldPointerValue, Serializable * serializable);
    static void notify(void * oldPointerValue, void * newPointerValue);
    virtual void see(void * oldPointerValue, void * newPointerValue) ; // informed the serialable about the pointer has been read 
    virtual void serialize(const Simulator & os) const;
    virtual bool deserialize(const Simulator & is);
    static Serializable ** Serializable::deserializeProcess(const Simulator & is, size_t & allSeriablesCount);
    static Serializable * findSerializableByType(int type);
    virtual Serializable * newInstance() const;
    static void * existsOldPointerValue(void * oldPointerValue);
    static void setLogger(LoggerProto * logger);
    static void setMemoryInterface(FSPMemoryInterface * memoryInterface);
    void writePointer(const Simulator & os, void * pointer) const;
    void readPointer(const Simulator & is, void * & pointer);
private:    
    //needed to be set
    static FSPMemoryInterface * memoryInterface;
    static size_t vSerializableType; 
    static LoggerProto * logger;
    size_t mSerializableType;
    
};