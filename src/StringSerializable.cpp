#include "StringSerializable.h"

size_t SString::vSStringType = 0;
void SString::see(void * oldPointerValue, void * newPointerValue){
    
}
void SString::serialize(const OutputSimulator & os) const{
    Serializable::serialize(os);
    os(&mLength, sizeof(int));
    os(mString, mLength);
    return;
}
bool SString::deserialize(const InputSimulator & is) {
    Serializable::deserialize(is);
    is(&mLength, sizeof(int));
    mString = (char *) memoryInterface->allocate(mLength+1);
    memset(mString, 0, mLength+1);
    is(mString, mLength);
    return true;
}

void SString::setString(const char * string){
    // we will allocate memory for the string
    mLength = strlen(string);
    mString = (char *) memoryInterface->allocate(mLength + 1);
    memset(mString, 0, mLength + 1);
    strcpy(mString, string);

}

Serializable * SString::newInstance() const {
    return memoryInterface->newInstance<SString>();
}

SString::SString()  {
    this->mSerializableType = SString::vSStringType;
}
char * SString::getString() const{
    return mString;
}