#include <iostream>
#include <string>
using namespace std;
class DLinkedList
{
public:
    class NodeList; // Forward declaration
public:
    NodeList *head;
    NodeList *tail;
    NodeList *newList;

public:
    DLinkedList(NodeList *head = nullptr, NodeList *tail = nullptr)
    {
        this->head = head;
        this->tail = tail;
    };
    ~DLinkedList()
    {
        NodeList *temp = new NodeList(-1, -1, NULL, NULL);
        while (head != NULL)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
    };

    void addHead(int index, int cnt)
    {
        cout << index << " " << cnt << endl;
        NodeList *newNode = new NodeList(index, cnt, NULL, NULL);
        if (head == NULL)
        {
            cout << "abc" << endl;
            head = tail = newNode;
        }
        else
        {
            cout << "def" << endl;
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
        // p->previous = NULL;
        delete p;
    }
    void XoaGiuaMFRU(int index)
    {
        cout << index << endl;
        NodeList *p = SearchMFRUIndex(index);
        cout << p->cnt << endl;
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

            // p->previous = NULL;
            // p->next = NULL;
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

            // p->previous = NULL;
            // p->next = NULL;
            delete p;
        }
    }

    NodeList *copyList(NodeList *head)
    {
        NodeList *current = head; // used to iterate over the original list
        NodeList *newList = NULL; // head of the new list
        NodeList *tail = NULL;    // point to the last node in a new list

        while (current != NULL)
        {
            // special case for the first new node
            if (newList == NULL)
            {
                newList = new NodeList(current->idx, current->cnt, NULL, NULL);
                tail = newList;
            }
            else
            {
                tail->next = new NodeList(current->idx, current->cnt, NULL, NULL);
                tail = tail->next;
            }
            current = current->next;
        }

        return newList;
    }

    NodeList *GetCopyList()
    {
        return this->copyList(this->head);
    }

    void print(NodeList *head)
    {
        NodeList *temp = head;
        while (temp != NULL)
        {
            cout << temp->cnt << "(" << temp->idx << ")" << endl;
            temp = temp->next;
        }
    }

public:
    class NodeList
    {
    public:
        int idx;
        int cnt;
        NodeList *next;
        NodeList *previous;
        friend class DLinkedList;

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
            this->cnt = 0;
            this->idx = 0;
            this->previous = NULL;
            this->next = NULL;
        };
    };
};
int main()
{
    DLinkedList *list = new DLinkedList();
    DLinkedList *newList = new DLinkedList();
    list->addHead(1, 3);
    list->addHead(0, 6);
    // list->addHead(2, 7);
    // list->addHead(3, 5);
    // list->addHead(4, 3);
    // list->print(list->head);
    // cout << list->SearchLFRU(3)->idx << endl;
    // cout << list->SearchMFRU(3)->idx << endl;
    // cout << "search giua" << endl;
    // cout << list->SearchLFRUIndex(3)->cnt << endl;
    // cout << list->SearchMFRUIndex(1)->cnt << endl;
    // cout << "xoa giua" << endl;
    // list->XoaGiuaMFRU(2);
    // list->XoaGiuaMFRU(3);
    // list->XoaGiuaMFRU(4);
    // list->XoaGiuaMFRU(0);
    list->XoaGiuaMFRU(1);
    cout << "in :" << endl;
    // if (list->head ==nullptr && list->tail == nullptr) {
    //     cout << "dung" << endl;
    // }
    // cout << list->head <<endl;
    // cout << list->tail << endl;
    list->print(list->head);
    list->addHead(1, 1);
    list->print(list->head);
    list->XoaGiuaMFRU(0);
    list->print(list->head);
    list->XoaGiuaMFRU(1);
    list->addHead(1, 32);
    list->addHead(3, 45);
    list->print(list->head);
    // cout << "in copyList: " << endl;
    // int n = 5;
    // int *copyRoot = new int[n];
    // copyRoot[0] = 92;
    // copyRoot[1] = 4;
    // copyRoot[2] = 4;
    // copyRoot[3] = 67;
    // copyRoot[4] = 8;
    // for (int i = 0; i < n; i++)
    //     {
    //         for (int j = i + 1; j < n; j++)
    //         {
    //             if (copyRoot[i] > copyRoot[j])
    //             {
    //                 copyRoot[i] ^= copyRoot[j] ^= copyRoot[i] ^= copyRoot[j];
    //             }
    //             else if (copyRoot[i] == copyRoot[j])
    //             {

    //                 copyRoot[j--] = copyRoot[--n];
    //             }
    //         }
    //     }
    // for (int i = 0; i < n; i++) {
    //     cout << copyRoot[i] << endl;
    // }

    // list->print(list->GetCopyList());

    return 0;
}
