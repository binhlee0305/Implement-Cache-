//Student's name: Le Duc Binh - 1852272
#include "Cache.h"
Cache::Cache(SearchEngine *s, ReplacementPolicy *r) : rp(r), s_engine(s) {}
Cache::~Cache()
{
    delete rp;
    delete s_engine;
}
Data *Cache::read(int addr)
{
    int idxFound = s_engine->search(addr);
    if (idxFound != -1)
    {
        rp->access(idxFound);
        // s_engine->newData = false;
        return rp->getArr(idxFound)->data;
    }
    else
    {
        s_engine->newData = false;
    }
    return NULL;
}
Elem *Cache::put(int addr, Data *cont)
{
    int replaceIndex = rp->remove();
    int index = 0;
    
    if (replaceIndex == -1)
    {

        if (s_engine->newData == true)
        {
            Elem *e = new Elem(addr, cont, false);
            index = rp->insert(e, replaceIndex);
        }
        else
        {
            Elem *e = new Elem(addr, cont, true);
            index = rp->insert(e, replaceIndex);
        }
        s_engine->insert(rp->getArr(index), index);

        return NULL;
    }
    else
    {
        Elem *check = rp->getArr(replaceIndex);
        if (check != nullptr)
        {
            s_engine->remove(rp->getArr(replaceIndex)->addr);
            delete check;
            check = nullptr;
            
            if (s_engine->newData == true)
            {
                Elem *e = new Elem(addr, cont, false);
                index = rp->insert(e, replaceIndex);
            }
            else
            {
                Elem *e = new Elem(addr, cont, true);
                index = rp->insert(e, replaceIndex);
            }
            s_engine->insert(rp->getArr(index), index);
        }
        return check;
    }
}
Elem *Cache::write(int addr, Data *cont)
{
    int findIdx = s_engine->search(addr);
    if (findIdx != -1)
    {
        // thêm access vào heap
        rp->getArr(findIdx)->addr = addr;

        rp->getArr(findIdx)->data = cont;

        rp->getArr(findIdx)->sync = false;
        rp->access(findIdx);
        return rp->getArr(findIdx);
    }
    else
    {
        s_engine->newData = true;
        put(addr, cont);
        s_engine->newData = false;
    }
    return rp->getArr(findIdx);
}
void Cache::printRP()
{
    rp->print();
}
void Cache::printSE()
{
    cout << "Print BST in inorder:\n";
    s_engine->inOrder();
    cout << "Print BST in preorder:\n";
    s_engine->preOrder();
}
void Cache::printLP()
{
    int CountArr = rp->getCount();
    cout << "Prime memory\n";
    for (int k = 0; k < CountArr; k++)
    {
        rp->getArr(k)->print();
    }
    cout << "Hash table memory\n";
    HashMapTable *HashMap = new HashMapTable();
    for (int i = 0; i < CountArr; i++)
    {
        int key = HashMap->getAddr(rp->getArr(i));
        HashMap->Insert(key, i);
    }
    for (int j = 0; j < MAXSIZE; j++)
    {
        HashTableEntry *hash = HashMap->getHash(j);
        if (hash != nullptr) {
            rp->getArr(hash->index)->print();
        }
        continue;
    }
    delete HashMap;
}
