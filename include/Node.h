#pragma once
#include "Serializable.h"
class Node : public Serializable {
public:
    Node();
    void see(void * oldPointerValue, void * newPointerValue) override; 
    void serialize(const OutputSimulator & os) const  override;
    bool deserialize(const InputSimulator & is)  override;
    Serializable * newInstance() const  override;
    static size_t vNodeType;
    void setNext(Serializable * next);
    Serializable * getNext() const;
private:
    Serializable * mNext;
};