#include "FSP.h"
#include "assert.h"
size_t Serializable::pointValueCount = 0;
void ** Serializable::oldPointerValues = nullptr;
void ** Serializable::newPointerValues = nullptr;
size_t * Serializable::serializableIntegerTypes = nullptr;
Serializable ** Serializable::serializables = nullptr;
size_t Serializable::serializableClassCount = 0;
size_t Serializable::observingSerializableCount = 0;
Serializable ** Serializable::observingSerializables = nullptr;
void ** Serializable::observingSerializableOldPointerValues = nullptr;
size_t Serializable::vSerializableType = 0;
LoggerProto * Serializable::logger = nullptr;
FSPMemoryInterface * Serializable::memoryInterface = nullptr;
Serializable::Serializable(){
    if(vSerializableType == 0){
        vSerializableType = registerSerializable(vSerializableType, this);
    }
    this->mSerializableType = vSerializableType;   
     
}
size_t Serializable::registerSerializable(size_t type, Serializable * serializable){
    if(type != 0)  return type;
    assert(memoryInterface != nullptr);
    if(Serializable::serializableClassCount == 0){
        Serializable::serializableIntegerTypes = (size_t *) memoryInterface->allocate(sizeof(size_t));
        Serializable::serializables = (Serializable **)memoryInterface->allocate(sizeof(Serializable *));
    }else{
        Serializable::serializableIntegerTypes = (size_t *) memoryInterface->reallocate(
                                                                Serializable::serializableIntegerTypes, 
                                                                sizeof(size_t), 
                                                                Serializable::serializableClassCount,
                                                                (Serializable::serializableClassCount + 1));
        Serializable::serializables = (Serializable * * ) memoryInterface->reallocate(Serializable::serializables,
                                                                                      sizeof(Serializable*),
                                                                                      Serializable::serializableClassCount, 
                                                                                      (Serializable::serializableClassCount + 1));
    }
    type = Serializable::serializableClassCount+ 1;
    Serializable::serializableIntegerTypes[Serializable::serializableClassCount] = type;
    Serializable::serializables[Serializable::serializableClassCount] = serializable;
    Serializable::serializableClassCount++;
    return Serializable::serializableClassCount;
}
void Serializable::notify(void * oldPointerValue, void * newPointerValue){
    assert(memoryInterface != nullptr);
    for(size_t i = 0; i < Serializable::observingSerializableCount; i++){
        if(Serializable::observingSerializableOldPointerValues[i] == oldPointerValue){
            Serializable::observingSerializables[i]->see(oldPointerValue, newPointerValue);
        }
    }
    if(Serializable::pointValueCount == 0){
        Serializable::oldPointerValues = (void **) memoryInterface->allocate(sizeof(void *));
        Serializable::newPointerValues = (void **) memoryInterface->allocate(sizeof(void *));
    }else{
        Serializable::oldPointerValues = (void **) memoryInterface->reallocate(Serializable::oldPointerValues, 
                                                                              sizeof(void *), 
                                                                              Serializable::pointValueCount, 
                                                                              (Serializable::pointValueCount + 1));
        Serializable::newPointerValues = (void **) memoryInterface->reallocate(Serializable::newPointerValues, 
                                                                              sizeof(void *), 
                                                                              Serializable::pointValueCount, 
                                                                              (Serializable::pointValueCount + 1));
    }
    Serializable::oldPointerValues[Serializable::pointValueCount] = oldPointerValue;
    Serializable::newPointerValues[Serializable::pointValueCount] = newPointerValue;
    Serializable::pointValueCount++;
}
void * Serializable::existsOldPointerValue(void * oldPointerValue){
    for(size_t i = 0; i < Serializable::pointValueCount; i++){
        if(Serializable::oldPointerValues[i] == oldPointerValue){
            return Serializable::newPointerValues[i];
        }
    }
    return nullptr;
}

void Serializable::observe(void * oldPointerValue, Serializable * serializable){
    void * newPointerValues = nullptr;
    assert(memoryInterface != nullptr);
    if((newPointerValues = existsOldPointerValue(oldPointerValue)) == nullptr){
        serializable->see(oldPointerValue, newPointerValues);
        return;
    }
    if(Serializable::observingSerializableCount == 0){
        Serializable::observingSerializables = (Serializable **) memoryInterface->allocate(sizeof(Serializable *));
        Serializable::observingSerializableOldPointerValues = (void **) memoryInterface->allocate(sizeof(void *));
    }else{
        Serializable::observingSerializables = (Serializable **) memoryInterface->reallocate(Serializable::observingSerializables, 
                                                                                             sizeof(Serializable *), 
                                                                                             Serializable::observingSerializableCount, 
                                                                                             (Serializable::observingSerializableCount + 1));
        Serializable::observingSerializableOldPointerValues =  (void **) memoryInterface->reallocate(Serializable::observingSerializableOldPointerValues, 
                                                                                                    sizeof(void *), 
                                                                                                    Serializable::observingSerializableCount, 
                                                                                                    (Serializable::observingSerializableCount + 1));
    }
    Serializable::observingSerializables[Serializable::observingSerializableCount] = serializable;
    Serializable::observingSerializableOldPointerValues[Serializable::observingSerializableCount] = oldPointerValue;
}
void Serializable::see(void * oldPointerValue, void * newPointerValue){
    if(this->logger == nullptr) return;
    char tmp[200];
    memset(tmp, 0, 200);
    sprintf_s(tmp, "Serializable::see: %p, %p", oldPointerValue, newPointerValue);
    this->logger->log(tmp);
}
void Serializable::serialize(const Simulator & os) const{
    
    os((void*)&this->mSerializableType, sizeof(int));
    writePointer(os, (void*) this);
}
bool Serializable::deserialize(const Simulator & is) {
    void * tmp = nullptr;
    readPointer(is, tmp);
    if(tmp == (void*)-1)  return false;
    notify(tmp, this);
    return true;
}

void Serializable::writePointer(const Simulator & os, void * pointer) const{
    unsigned long long tthis = ~(unsigned long long)pointer;
    os(&tthis, sizeof(unsigned long long *));
}
void Serializable::readPointer(const Simulator & is, void * & pointer){
    unsigned long long tthis = 0;
    is(&tthis, sizeof(unsigned long long *));
    if(tthis == -1){
        pointer = (void*)-1;
        return;
    }
    pointer = (void *) tthis;
}

Serializable ** Serializable::deserializeProcess(const Simulator & is, size_t & allSeriablesCount){
    Serializable ** allSeriables = nullptr;
    int type = 0;
    while(true){
        is((char*)&type, sizeof(int));
        if(type == -1) break;
        Serializable * serializable = Serializable::findSerializableByType(type);
        if(serializable == nullptr) break;
        Serializable * newInstance = serializable->newInstance();
        //append to allSeriables
        if(allSeriablesCount == 0){
            allSeriables = (Serializable **) memoryInterface->allocate(sizeof(Serializable *));
        }else{
            allSeriables = (Serializable **) memoryInterface->reallocate(allSeriables, sizeof(Serializable *), allSeriablesCount, (allSeriablesCount + 1));
        }
        allSeriables[allSeriablesCount] = newInstance;
        allSeriablesCount++;
        if(!newInstance->deserialize(is)){
            break;
        }
    }
    return allSeriables;
}
Serializable * Serializable::findSerializableByType(int type){
    for(size_t i = 0; i < Serializable::serializableClassCount; i++){
        if(Serializable::serializableIntegerTypes[i] == type){
            return Serializable::serializables[i];
        }
    }
    return nullptr;
}
Serializable * Serializable::newInstance() const{
    
    return memoryInterface->newInstance<Serializable>();
}
void Serializable::setLogger(LoggerProto * logger){
    Serializable::logger = logger;
}
//setMemoryInterface
void Serializable::setMemoryInterface(FSPMemoryInterface * memoryInterface){
    Serializable::memoryInterface = memoryInterface;
}
