// Student's name: Le Duc Binh - 1852272
#ifndef CACHE_H
#define CACHE_H

#include "main.h"
class Node
{
public:
    int cnt;
    int idx;
    friend class MFU;
    friend class LFU;
    friend class MFRU;
    friend class LFRU;

public:
    Node(int count, int idx)
    {
        this->cnt = count;
        this->idx = idx;
    }
    ~Node()
    {
        this->cnt = 0;
        this->idx = 0;
    }
};

class HashTableEntry
{
public:
    int key;
    int index;
    friend class HashMapTable;

public:
    HashTableEntry(int index = 0, int key = 0)
    {
        this->key = key;
        this->index = index;
    }
    ~HashTableEntry() {}
};
class HashMapTable
{
private:
    HashTableEntry **hash;
    // Elem **arr;

public:
    HashMapTable()
    {
        hash = new HashTableEntry *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            hash[i] = NULL;
        }
    }
    ~HashMapTable()
    {
        for (int i = 0; i < MAXSIZE; i++)
        {
            delete hash[i];
        }
        delete[] hash;
    }
    int getAddr(Elem *e)
    {
        return e->addr;
    }

    void Insert(int key, int index)
    {
        int h = key % MAXSIZE;
        while (hash[h] != NULL && hash[h]->key != key)
        {
            h = (h + 1) % MAXSIZE;
        }
        if (hash[h] != NULL)
            delete hash[h];
        hash[h] = new HashTableEntry(index, key);
    }
    HashTableEntry *getHash(int index)
    {
        if (index >= 0 && index < MAXSIZE)
        {
            return hash[index];
        }
        else
            return NULL;
    }
};
class ReplacementPolicy
{
protected:
    Elem **arr;
    int count;
    Node **root;

public:
    ReplacementPolicy() {}
    virtual ~ReplacementPolicy() {}
    virtual void print() = 0;
    virtual int insert(Elem *e, int idx) = 0;
    virtual void access(int idx) = 0;
    virtual int remove() = 0;
    Elem *getArr(int index)
    {
        if (index >= 0 && index < MAXSIZE)
        {
            return arr[index];
        }
        else
            return NULL;
    }
    int getCount()
    {
        return this->count;
    }
};

class MFU : public ReplacementPolicy
{
private:
    void reheapUp(int idx)
    {
        if (idx <= 0)
        {
            return;
        }
        else
        {
            // trường hợp con phải = con trái, lấy max bên phải
            int parent = (idx - 1) / 2;
            if (this->root[idx]->cnt > this->root[parent]->cnt && this->root[parent]->cnt != 0)
            {
                Node *temp = root[idx];
                root[idx] = root[parent];
                root[parent] = temp;
                reheapUp(parent);
            }
        }
    }
    void reheapDown(int idx)
    {
        int leftChild = idx * 2 + 1;
        int rightChild = idx * 2 + 2;
        int lastPosition = this->count - 1;

        if (leftChild <= lastPosition)
        {
            int largeChild = leftChild; // by default => leftChild valid but rightChild invalid
            if (rightChild <= lastPosition)
            {
                if (this->root[leftChild]->cnt <= this->root[rightChild]->cnt)
                    largeChild = rightChild;
                else
                    largeChild = leftChild;
            }

            if (this->root[largeChild]->cnt >= this->root[idx]->cnt && this->root[largeChild]->cnt != 0)
            {
                Node *temp = root[largeChild];
                root[largeChild] = root[idx];
                root[idx] = temp;
                reheapDown(largeChild);
            }
        }
    }

public:
    MFU()
    {
        arr = new Elem *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            arr[i] = NULL;
        }
        count = 0;
        root = new Node *[MAXSIZE];
        for (int j = 0; j < MAXSIZE; j++)
        {
            root[j] = NULL;
        }
    }
    ~MFU()
    {
        for (int i = 0; i < count; i++)
        {

            delete arr[root[i]->idx];
            delete root[i];
        }
        delete[] arr;
        delete[] root;
    }

    int insert(Elem *e, int index)
    {
        // Increase the size of Heap by 1
        // insert if index != -1
        /*Insert the new element at the end of the Array*/

        if (index == -1)
        {
            index = this->count;
            count = count + 1;
            root[count - 1] = new Node(1, index);
            /*Heapify function*/
            reheapUp(index);
            arr[index] = new Elem(e->addr, e->data, e->sync);
            return index;
        }
        else
        {
            /*Increase the Size of the Array by 1*/
            count = count + 1;
            root[count - 1] = new Node(1, index);

            /*Heapify function*/
            reheapUp(count - 1);
            arr[index] = new Elem(e->addr, e->data, e->sync);
            return index;
        }
    };
    void access(int index)
    {
        int accessCount = 0;
        if (index < 0 || index >= MAXSIZE)
        {
            return;
        }
        else
        {
            for (int i = 0; i < this->count; i++)
            {
                if (index == root[i]->idx)
                {
                    accessCount = root[i]->cnt;

                    root[i]->cnt = root[count - 1]->cnt;
                    root[i]->idx = root[count - 1]->idx;

                    root[count - 1]->cnt = 0;
                    root[count - 1]->idx = 0;
                    // count--;
                    reheapDown(i);
                    break;
                }
            }
            // count++;
            root[count - 1]->idx = index;
            root[count - 1]->cnt = accessCount + 1;
            reheapUp(count - 1);
        }
    };
    int remove()
    {
        if (count == MAXSIZE)
        {
            int deleteIndex = root[0]->idx;
            root[0]->cnt = root[MAXSIZE - 1]->cnt;
            root[0]->idx = root[MAXSIZE - 1]->idx;
            root[MAXSIZE - 1]->cnt = 0;
            root[MAXSIZE - 1]->idx = 0;
            reheapDown(0);

            count--;
            return deleteIndex;
        }
        return -1;
    };
    void print()
    {
        for (int i = count - 1; i >= 0; i--)
        {
            arr[root[i]->idx]->print();
        }
        // for (int i = 0 ; i < count; i++) {
        //     cout << root[i]->cnt << "("<< root[i]->idx << ")"<<" ";
        // }
        // cout << endl;
    }
};

class LFU : public ReplacementPolicy
{
private:
    // Node **root;
    void reheapUp(int idx)
    {
        if (idx <= 0)
            return;
        else
        {
            // trường hợp con phải = con trái, lấy min bên trái
            int parent = (idx - 1) / 2;
            if (this->root[idx]->cnt < this->root[parent]->cnt && this->root[parent]->cnt != 0)
            {
                Node *temp = root[idx];
                root[idx] = root[parent];
                root[parent] = temp;
                reheapUp(parent);
            }
        }
    }
    void reheapDown(int idx)
    {
        int leftChild = idx * 2 + 1;
        int rightChild = idx * 2 + 2;
        int lastPosition = this->count - 1;

        if (leftChild <= lastPosition)
        {
            int smallChild = leftChild; // by default => leftChild valid but rightChild invalid
            if (rightChild <= lastPosition)
            {
                if (this->root[leftChild]->cnt <= this->root[rightChild]->cnt && this->root[rightChild]->cnt != 0 && this->root[leftChild]->cnt != 0)
                {
                    smallChild = leftChild;
                }
                else if (this->root[rightChild]->cnt < this->root[leftChild]->cnt && this->root[rightChild]->cnt != 0 && this->root[leftChild]->cnt != 0)
                {
                    smallChild = rightChild;
                }
                else if ((this->root[leftChild]->cnt > this->root[rightChild]->cnt) && this->root[rightChild]->cnt == 0)
                {
                    smallChild = leftChild;
                }
                else if ((this->root[rightChild]->cnt > this->root[leftChild]->cnt) && this->root[leftChild]->cnt == 0)
                {
                    smallChild = rightChild;
                }
            }

            if (this->root[smallChild]->cnt <= this->root[idx]->cnt && this->root[smallChild]->cnt != 0)
            {
                Node *temp = root[smallChild];
                root[smallChild] = root[idx];
                root[idx] = temp;
                reheapDown(smallChild);
            }
        }
    }

public:
    LFU()
    {
        arr = new Elem *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            arr[i] = NULL;
        }
        count = 0;
        root = new Node *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            root[i] = NULL;
        }
    }
    ~LFU()
    {
        for (int i = 0; i < count; i++)
        {

            delete arr[root[i]->idx];
            delete root[i];
        }
        delete[] arr;
        delete[] root;
    }
    int insert(Elem *e, int index)
    {
        // Increase the size of Heap by 1
        // insert if index != -1
        /*Insert the new element at the end of the Array*/
        if (index == -1)
        {
            index = this->count;
            count = count + 1;
            root[count - 1] = new Node(1, index);
            /*Heapify function*/
            reheapUp(index);
            arr[index] = new Elem(e->addr, e->data, e->sync);
            return index;
        }
        else
        {
            /*Increase the Size of the Array by 1*/
            count = count + 1;
            root[count - 1] = new Node(1, index);

            /*Heapify function*/
            reheapUp(count - 1);
            arr[index] = new Elem(e->addr, e->data, e->sync);
            return index;
        }
    };
    void access(int index)
    {
        int accessCount = 0;
        if (index < 0 || index >= MAXSIZE)
        {
            return;
        }
        else
        {
            for (int i = 0; i < this->count; i++)
            {
                if (index == root[i]->idx)
                {
                    accessCount = root[i]->cnt;

                    root[i]->cnt = root[count - 1]->cnt;
                    root[i]->idx = root[count - 1]->idx;

                    root[count - 1]->cnt = 0;
                    root[count - 1]->idx = 0;

                    count--;
                    reheapDown(i);
                    break;
                }
            }
            count++;
            root[count - 1]->idx = index;
            root[count - 1]->cnt = accessCount + 1;
            reheapUp(count - 1);
        }
    };
    int remove()
    {
        if (count == MAXSIZE)
        {
            int deleteIndex = root[0]->idx;
            root[0]->cnt = root[MAXSIZE - 1]->cnt;
            root[0]->idx = root[MAXSIZE - 1]->idx;
            root[MAXSIZE - 1]->cnt = 0;
            root[MAXSIZE - 1]->idx = 0;
            reheapDown(0);

            count--;
            return deleteIndex;
        }
        return -1;
    };
    void print()
    {
        for (int i = 0; i < count; i++)
        {
            arr[root[i]->idx]->print();
        }
    };
};

class DLinkedList
{
public:
    class NodeList; // Forward declaration
public:
    NodeList *head;
    NodeList *tail;
    // NodeList *temp;

public:
    DLinkedList(NodeList *head = nullptr, NodeList *tail = nullptr)
    {
        this->head = head;
        this->tail = tail;
    };
    ~DLinkedList(){
        // this->head = nullptr;
        // this->tail = nullptr;
    };
    void addHead(int index, int cnt)
    {
        NodeList *newNode = new NodeList(index, cnt, NULL, NULL);
        if (head == NULL)
        {
            head = newNode;
            tail = head;
            tail->next = NULL;
        }
        else
        {
            newNode->next = head;
            head->previous = newNode;
            head = newNode;
        }
    };
    NodeList *SearchMFRU(int cnt)
    {
        NodeList *p = this->head;
        while ((p != NULL) && (p->cnt != cnt))
        {
            p = p->next;
        }
        return p;
    };
    NodeList *SearchLFRU(int cnt)
    {
        NodeList *p = this->tail;
        while ((p != NULL) && (p->cnt != cnt))
        {
            p = p->previous;
        }
        return p;
    };
    NodeList *SearchMFRUIndex(int index)
    {
        NodeList *p = this->head;
        while ((p != NULL) && (p->idx != index))
        {
            p = p->next;
        }
        return p;
    };
    NodeList *SearchLFRUIndex(int index)
    {
        NodeList *p = this->tail;
        while ((p != NULL) && (p->idx != index))
        {
            p = p->previous;
        }
        return p;
    };
    void XoaDau()
    {
        NodeList *p = this->head;
        if (p->next == NULL && p->previous == NULL)
        {
            delete p;
            head = tail = NULL;
            return;
        }
        head = head->next;
        head->previous = NULL;
        if (head == NULL)
        {
            tail = NULL;
        }
        p->next = NULL;
        delete p;
    }
    void XoaCuoi()
    {
        NodeList *p = tail;
        if (p->next == NULL && p->previous == NULL)
        {
            delete p;
            head = tail = NULL;
            return;
        }
        tail = tail->previous;
        tail->next = NULL;
        if (tail == NULL)
        {
            head = NULL;
        }
        p->previous = NULL;
        delete p;
    }
    void XoaGiuaMFRU(int index)
    {
        NodeList *p = SearchMFRUIndex(index);
        if (p != NULL)
        {
            if (p->previous == NULL)
            {
                XoaDau();
                return;
            }
            if (p->next == NULL)
            {
                XoaCuoi();
                return;
            }
            p->previous->next = p->next;
            p->next->previous = p->previous;

            p->previous = NULL;
            p->next = NULL;
            delete p;
        }
    };
    void XoaGiuaLFRU(int index)
    {
        NodeList *p = SearchLFRUIndex(index);
        if (p != NULL)
        {
            if (p->previous == NULL)
            {
                XoaDau();
                return;
            }
            if (p->next == NULL)
            {
                
                XoaCuoi();
                return;
            }
            p->previous->next = p->next;
            p->next->previous = p->previous;

            p->previous = NULL;
            p->next = NULL;
            delete p;
        }
    };

public:
    class NodeList
    {
    private:
        int idx;
        int cnt;
        NodeList *next;
        NodeList *previous;
        friend class DLinkedList;
        friend class MFRU;
        friend class LFRU;

    public:
        NodeList(int idx, int cnt, NodeList *next, NodeList *previous)
        {
            this->cnt = cnt;
            this->idx = idx;
            this->previous = NULL;
            this->next = NULL;
        };
        ~NodeList()
        {
            this->cnt = -1;
            this->idx = -1;
            this->previous = NULL;
            this->next = NULL;
        };
    };
};

class MFRU : public ReplacementPolicy
{
private:
    DLinkedList *List;
    void reheapUp(int idx)
    {
        if (idx <= 0)
        {
            return;
        }
        else
        {
            // trường hợp con phải = con trái, lấy max bên phải
            int parent = (idx - 1) / 2;
            if (this->root[idx]->cnt > this->root[parent]->cnt && this->root[parent]->cnt != 0)
            {
                Node *temp = root[idx];
                root[idx] = root[parent];
                root[parent] = temp;
                reheapUp(parent);
            }
        }
    }
    void reheapDown(int idx)
    {
        int leftChild = idx * 2 + 1;
        int rightChild = idx * 2 + 2;
        int lastPosition = this->count - 1;
        if (count == 1)
        {
            return;
        }
        if (leftChild <= lastPosition)
        {
            int largeChild = leftChild; // by default => leftChild valid but rightChild invalid
            if (rightChild <= lastPosition)
            {
                if (this->root[leftChild]->cnt <= this->root[rightChild]->cnt)
                    largeChild = rightChild;
                else
                    largeChild = leftChild;
            }

            if (this->root[largeChild]->cnt >= this->root[idx]->cnt && this->root[largeChild]->cnt != 0)
            {
                Node *temp = root[largeChild];
                root[largeChild] = root[idx];
                root[idx] = temp;
                reheapDown(largeChild);
            }
        }
    }

public:
    MFRU()
    {
        arr = new Elem *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            arr[i] = NULL;
        }
        count = 0;
        root = new Node *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            root[i] = NULL;
        }

        List = new DLinkedList(nullptr, nullptr);
    }
    ~MFRU()
    {
        for (int i = 0; i < count; i++)
        {

            delete arr[i];
            delete root[i];
        }
        delete[] arr;
        delete[] root;
        delete List;
    }
    int insert(Elem *e, int index)
    {
        // insert vào heap và double linked-list (add vào đầu) node chứa index và cnt
        if (index == -1)
        {
            index = this->count;
            count = count + 1;
            root[count - 1] = new Node(1, index);
            /*Heapify function*/
            reheapUp(index);
            arr[index] = new Elem(e->addr, e->data, e->sync);

            List->addHead(index, 1);
            return index;
        }
        else
        {
            count = count + 1;
            root[count - 1] = new Node(1, index);
            /*Heapify function*/
            reheapUp(count - 1);
            arr[index] = new Elem(e->addr, e->data, e->sync);

            List->addHead(index, 1);
            return index;
        }
    };
    void access(int index)
    {
        // xóa root[idx] đưa root[count-1] lên heapify lại rồi đứa root[idx] cuối cùng và heapify lại,
        // bên double ll thì xóa node[idx] sau nó add lại lên đầu
        int accessCount = 0;
        if (index < 0 || index >= MAXSIZE)
        {
            return;
        }
        else
        {
            for (int i = 0; i < this->count; i++)
            {
                if (index == root[i]->idx)
                {
                    accessCount = root[i]->cnt;

                    root[i]->cnt = root[count - 1]->cnt;
                    root[i]->idx = root[count - 1]->idx;

                    root[count - 1]->cnt = 0;
                    root[count - 1]->idx = 0;
                    count--;
                    reheapDown(i);
                    break;
                }
            }
            count++;
            root[count - 1]->idx = index;
            root[count - 1]->cnt = accessCount + 1;
            reheapUp(count - 1);

            List->XoaGiuaMFRU(index);
            List->addHead(index, accessCount + 1);
        }
    };
    int remove()
    {
        // pop root[0]->cnt ra sau đó duyệt bên dd linked-list theo cnt gặp phần tử đầu tiên lấy trả về idx, xóa root[idx] đứa root[count-1] lên , heapify
        // xóa arr[idx] ;
        if (count == MAXSIZE)
        {
            int deleteCnt = root[0]->cnt;
            int getIndex = List->SearchMFRU(deleteCnt)->idx;
            for (int i = 0; i < count; i++)
            {
                if (getIndex == root[i]->idx)
                {
                    root[i]->cnt = root[count - 1]->cnt;
                    root[i]->idx = root[count - 1]->idx;

                    root[count - 1]->cnt = 0;
                    root[count - 1]->idx = 0;
                    count--;
                    reheapDown(i);
                    break;
                }
            }

            List->XoaGiuaMFRU(getIndex);
            return getIndex;
        }
        return -1;
    };
    void print()
    {
        // pop root[0]->idx ra sau đó đưa root[count-1] lên, heapify lại,
        // duyệt root[0]->idx trong double link-list từ head->tail gặp đầu tiên thì node[i]->idx
        int count_temp = count;
        int *copyRoot = new int[count_temp];
        for (int i = 0; i < count_temp; i++)
        {
            int takeCount = root[i]->cnt;
            copyRoot[i] = takeCount;
        }
        for (int i = 0; i < count_temp; i++)
        {
            for (int j = i + 1; j < count_temp; j++)
            {
                if (copyRoot[i] > copyRoot[j])
                {
                    copyRoot[i] ^= copyRoot[j] ^= copyRoot[i] ^= copyRoot[j];
                }
                else if (copyRoot[i] == copyRoot[j])
                {

                    copyRoot[j--] = copyRoot[--count_temp];
                }
            }
        }
        
        for (int i = count_temp - 1; i >= 0; i--)
        {
            int deleCnt = copyRoot[i];
            DLinkedList::NodeList *temp = List->head;
            for ( int j = 0; j < count; j++)
            {
                if (temp->cnt == deleCnt)
                {
                    arr[temp->idx]->print();
                }
                temp = temp->next;
            }
        }
        delete[] copyRoot;
    }
};

class LFRU : public ReplacementPolicy
{
private:
    // Node **root;
    DLinkedList *List;
    void reheapUp(int idx)
    {
        if (idx <= 0)
            return;
        else
        {
            // trường hợp con phải = con trái, lấy min bên trái
            int parent = (idx - 1) / 2;
            if (this->root[idx]->cnt < this->root[parent]->cnt && this->root[parent]->cnt != 0)
            {
                Node *temp = root[idx];
                root[idx] = root[parent];
                root[parent] = temp;
                reheapUp(parent);
            }
        }
    }
    void reheapDown(int idx)
    {
        int leftChild = idx * 2 + 1;
        int rightChild = idx * 2 + 2;
        int lastPosition = this->count - 1;
        if (count == 1)
        {
            return;
        }

        if (leftChild <= lastPosition)
        {
            int smallChild = leftChild; // by default => leftChild valid but rightChild invalid
            if (rightChild <= lastPosition)
            {
                if (this->root[leftChild]->cnt <= this->root[rightChild]->cnt && this->root[rightChild]->cnt != 0 && this->root[leftChild]->cnt != 0)
                {
                    smallChild = leftChild;
                }
                else if (this->root[rightChild]->cnt < this->root[leftChild]->cnt && this->root[rightChild]->cnt != 0 && this->root[leftChild]->cnt != 0)
                {
                    smallChild = rightChild;
                }
                else if ((this->root[leftChild]->cnt > this->root[rightChild]->cnt) && this->root[rightChild]->cnt == 0)
                {
                    smallChild = leftChild;
                }
                else if ((this->root[rightChild]->cnt > this->root[leftChild]->cnt) && this->root[leftChild]->cnt == 0)
                {
                    smallChild = rightChild;
                }
            }

            if (this->root[smallChild]->cnt <= this->root[idx]->cnt && this->root[smallChild]->cnt != 0)
            {
                Node *temp = root[smallChild];
                root[smallChild] = root[idx];
                root[idx] = temp;
                reheapDown(smallChild);
            }
        }
    }

public:
    LFRU()
    {
        arr = new Elem *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            arr[i] = NULL;
        }
        count = 0;
        root = new Node *[MAXSIZE];
        for (int i = 0; i < MAXSIZE; i++)
        {
            root[i] = NULL;
        }
        List = new DLinkedList(nullptr, nullptr);
    }
    ~LFRU()
    {
        for (int i = 0; i < count; i++)
        {

            delete arr[i];
            delete root[i];
        }
        delete[] arr;
        delete[] root;
        delete List;
    }
    int insert(Elem *e, int index)
    {
        // insert vào heap và double linked-list (add vào đầu) node chứa index và cnt
        if (index == -1)
        {
            index = this->count;
            count = count + 1;
            root[count - 1] = new Node(1, index);
            /*Heapify function*/
            reheapUp(index);
            arr[index] = new Elem(e->addr, e->data, e->sync);

            List->addHead(index, 1);
            return index;
        }
        else
        {
            count = count + 1;
            root[count - 1] = new Node(1, index);
            /*Heapify function*/
            reheapUp(count - 1);
            arr[index] = new Elem(e->addr, e->data, e->sync);

            List->addHead(index, 1);
            
            return index;
        }
    };
    void access(int index)
    {
        // xóa root[idx] đưa root[count-1] lên heapify lại rồi đứa root[idx] cuối cùng và heapify lại,
        // bên double ll thì xóa node[idx] sau nó add lại lên đầu
        int accessCount = 0;
        if (index < 0 || index >= MAXSIZE)
        {
            return;
        }
        else
        {
            for (int i = 0; i < this->count; i++)
            {
                if (index == root[i]->idx)
                {
                    accessCount = root[i]->cnt;

                    root[i]->cnt = root[count - 1]->cnt;
                    root[i]->idx = root[count - 1]->idx;

                    root[count - 1]->cnt = 0;
                    root[count - 1]->idx = 0;
                    count--;
                    reheapDown(i);
                    break;
                }
            }
            count++;
            root[count - 1]->idx = index;
            root[count - 1]->cnt = accessCount + 1;
            reheapUp(count - 1);

            List->XoaGiuaLFRU(index);
            List->addHead(index, accessCount + 1);
        }
    }
    int remove()
    {
        // pop root[0]->idx ra sau đó đưa root[count-1] lên, heapify lại,
        // duyệt root[0]->idx trong double link-list từ tail->head gặp node[i]->idx đầu tiên thì lấy
        if (count == MAXSIZE)
        {
            int deleteCnt = root[0]->cnt;
            int getIndex = List->SearchLFRU(deleteCnt)->idx;
            for (int i = 0; i < count; i++)
            {
                if (getIndex == root[i]->idx)
                {
                    root[i]->cnt = root[count - 1]->cnt;
                    root[i]->idx = root[count - 1]->idx;

                    root[count - 1]->cnt = 0;
                    root[count - 1]->idx = 0;
                    count--;
                    reheapDown(i);
                    break;
                }
            }
            List->XoaGiuaLFRU(getIndex);
            return getIndex;
        }
        return -1;
    };
    void print()
    {
        int count_temp = count;
        int *copyRoot = new int[count_temp];
        for (int i = 0; i < count_temp; i++)
        {
            int takeCount = root[i]->cnt;
            copyRoot[i] = takeCount;
        }
        for (int i = 0; i < count_temp; i++)
        {
            for (int j = i + 1; j < count_temp; j++)
            {
                if (copyRoot[i] > copyRoot[j])
                {
                    copyRoot[i] ^= copyRoot[j] ^= copyRoot[i] ^= copyRoot[j];
                }
                else if (copyRoot[i] == copyRoot[j])
                {

                    copyRoot[j--] = copyRoot[--count_temp];
                }
            }
        }

        for (int i = 0; i < count_temp; i++)
        {
            int deleCnt = copyRoot[i];
            DLinkedList::NodeList *temp = List->tail;
            for ( int j = 0; j < count; j++)
            {
                if (temp->cnt == deleCnt)
                {
                    arr[temp->idx]->print();
                }
                temp = temp->previous;
            }
        }

        delete[] copyRoot;
    }
};
class SearchEngine
{
public:
    Elem **arr;
    bool newData = false;

public:
    SearchEngine(){};
    virtual ~SearchEngine(){

    };
    virtual void insert(Elem *e, int index) = 0;
    virtual int search(int addr) = 0;
    virtual void remove(int addr) = 0;
    virtual void preOrder() = 0;
    virtual void inOrder() = 0;
    virtual bool checkData() = 0;
};
class BST : public SearchEngine
{
public:
    class ElemNode; // forward declare

protected:
    ElemNode *root;

public:
    BST()
    {
        newData = false;
        this->root = nullptr;
    };

    ~BST()
    {
        clear(this->root);
    };

    void insert(Elem *e, int index)
    {
        this->root = insert(this->root, e, index);
    }
    int search(int addr)
    {
        return search(this->root, addr);
    };
    void remove(int addr)
    {
        this->root = remove(this->root, addr);
    }
    void preOrder()
    {
        preOrder(this->root);
    }
    void inOrder()
    {
        inOrder(this->root);
    }
    bool checkData()
    {
        return this->newData;
    }

public:
    class ElemNode
    {
    protected:
        Elem *e;
        ElemNode *left;
        ElemNode *right;
        int index;
        friend class BST;

    public:
        ElemNode(Elem *e = nullptr, ElemNode *left = nullptr, ElemNode *right = nullptr, int index = 0)
        {
            this->e = e;
            this->left = left;
            this->right = right;
            this->index = index;
        }
        ~ElemNode()
        {
        }
    };

private:
    void clear(ElemNode *root)
    {
        if (root == nullptr)
            return;
        if (root->left != nullptr)
            clear(root->left);
        if (root->right != nullptr)
            clear(root->right);
        delete root;
    }

    // Insert function.
    ElemNode *insert(ElemNode *root, Elem *e, int index)
    {
        if (root == nullptr)
        {
            // Insert the first node, if root is NULL.

            return new ElemNode(e, nullptr, nullptr, index);
        }

        if (e->addr >= root->e->addr)
        {
            root->right = insert(root->right, e, index);
        }
        else
        {
            root->left = insert(root->left, e, index);
        }
        return root;
    };

    // Inorder traversal. //LNR
    void inOrder(ElemNode *root)
    {
        if (root == nullptr)
        {
            return;
        }
        // Traverse left
        inOrder(root->left);

        // Traverse root
        root->e->print();
        // Traverse right
        inOrder(root->right);
    };

    void preOrder(ElemNode *root)
    {
        if (root == nullptr)
        {
            return;
        }
        // Traverse root
        root->e->print();
        // Traverse left
        preOrder(root->left);
        // Traverse right
        preOrder(root->right);
    };

    int search(ElemNode *root, int addr)
    {
        if (root == nullptr)
        {
            return -1;
        }

        if (addr == root->e->addr)
        {
            return root->index;
        }
        else if (addr < root->e->addr)
        {
            return search(root->left, addr);
        }
        else
        {
            return search(root->right, addr);
        }
        return -1;
    };

    ElemNode *remove(ElemNode *root, int addr)
    {
        // Return if the tree is empty
        if (root == NULL)
            return root;
        // Find the node to be deleted
        if (addr < root->e->addr)
            root->left = remove(root->left, addr);
        else if (addr > root->e->addr)
            root->right = remove(root->right, addr);
        else
        {
            // If the node is with only one child or no child
            if (root->left == NULL)
            {
                ElemNode *temp = root->right;
                delete (root);
                return temp;
            }
            else if (root->right == NULL)
            {
                ElemNode *temp = root->left;
                delete (root);
                return temp;
            }
            // Find the inorder successor

            ElemNode *current = root->right;

            // Find the leftmost leaf
            while (current && current->left != NULL)
                current = current->left;

            // Place the inorder successor in position of the node to be deleted
            root->e = current->e;
            root->index = current->index;
            // Delete the inorder successor
            root->right = remove(root->right, current->e->addr);
        }
        return root;
    };
};
#endif