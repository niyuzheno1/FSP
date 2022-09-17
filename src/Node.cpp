#include "Node.h"

Node::Node(){
    mNext = nullptr;
    this->mSerializableType = Node::vNodeType;
}
void Node::see(void * oldPointerValue, void * newPointerValue){
    if (oldPointerValue == mNext){
        mNext = (Serializable *) newPointerValue;
    }
} 
void Node::serialize(const OutputSimulator & os) const{
    Serializable::serialize(os);
    writePointer(os, mNext);
    Serializable::appendToQueue(this, mNext);
    return;
}
bool Node::deserialize(const InputSimulator & is)  {
    Serializable::deserialize(is);
    is(&mNext, sizeof(Serializable *));
    Serializable::observe( mNext, this);
    return true;
}
Serializable * Node::newInstance() const  {
    return memoryInterface->newInstance<Node>();
}
void Node::setNext(Serializable * next){
    mNext = next;   
}
Serializable * Node::getNext() const{
    return mNext;
}

size_t Node::vNodeType; 