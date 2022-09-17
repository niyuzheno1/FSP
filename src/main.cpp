#include "FSP/export.h"
#include "FSPMemoryInstance.h"
FSPMemoryInstance memoryInstance;
char tmpx[100];
int tsz; 
void run(){
    Serializable tmp;
    tsz = 0;
    tmp.serialize([&](void * _c, size_t sz){
        unsigned char * c = (unsigned char *) _c;
        for(size_t i = 0; i < sz; i++){
            tmpx[tsz++] = c[i];
        }
    });
    int led = 0 ;
    size_t cnt = 0;
    Serializable ** allSeriable = Serializable::deserializeProcess(
        [&](void * _c, size_t sz){
            unsigned char * c = (unsigned char *) _c;

            if(led >= tsz){
                for (size_t i = 0; i < sz; i++)
                {
                    c[i] = 0xff;
                }
                return;
            }
            for(size_t i = 0; i < sz; i++){
                if(led >= tsz){
                    c[i] = 0xff;
                    continue;
                }
                c[i] = tmpx[led++];
            }
        }, 
        cnt
    );
    if(allSeriable == nullptr){
        return;
    }

}
int main(){
    Serializable::setMemoryInterface(&memoryInstance);
    run();
    return 0;
}