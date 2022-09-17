#pragma once
#include "Serializable.h"
class SString : public Serializable {
public:
    SString();
    void see(void * oldPointerValue, void * newPointerValue) override; 
    void serialize(const OutputSimulator & os) const  override;
    bool deserialize(const InputSimulator & is)  override;
    Serializable * newInstance() const  override;
    static size_t vSStringType;
    void setString(const char * string);
    char * getString() const;
private:
    char * mString;
    int mLength;
};