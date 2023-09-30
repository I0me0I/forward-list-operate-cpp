#include <iostream>
#include <functional>


template<typename Type>
class ForwardList
{
private:
    struct Node{
        Type data;
        Node *next;
    };

    struct ConstNode{
        const Type data;
        ConstNode *next;
    };

    template<typename NodeType>
    class Iterator{
    private:
        NodeType *ptr;

    public:
        explicit Iterator(const NodeType *ptr) : ptr(const_cast<NodeType *>(ptr)) {}
        Iterator(const Iterator &it) : ptr(it.ptr) {}
        template<typename OtherNodeType>
        Iterator(const Iterator<OtherNodeType> &it)
            : ptr(reinterpret_cast<NodeType *>(it.ptr))
        {}

        decltype(NodeType::data) &operator*() { return this->ptr->data; }
        const Type &operator*() const { return this->ptr->data; }

        bool operator==(const Iterator &it) const { return this->ptr == it.ptr; }
        bool operator!=(const Iterator &it) const { return !(*this == it); }

        Iterator &operator++(){
            this->ptr = this->ptr->next;
            return *this;
        }

        Iterator operator++(int){
            Iterator it = *this;
            this->ptr = this->ptr->next;
            return it;
        }

        Iterator next() const { return Iterator(this->ptr->next); }

        friend class ForwardList;
    };

    Node beforeHead;
    size_t count;

public:
    using iterator = Iterator<Node>;
    using const_iterator = Iterator<ConstNode>;
    using size_type = size_t;

    ForwardList() : beforeHead({0, nullptr}), count(0) {}
    ForwardList(const ForwardList &fl);
    const ForwardList &operator=(const ForwardList &) = delete;

    void push_front(const Type &value);
    void pop_front();
    iterator insert_after(const_iterator pos, const Type &value);
    iterator erase_after(const_iterator pos);
    void remove(const Type &value);
    void clear();

    template<typename Compare = decltype(std::less<Type>())>
    void sort(Compare comp = std::less<Type>());
    void reverse();
    template<typename CutIf>
    std::array<ForwardList, 2> split(CutIf cutIf) const;

    const Type &front() const { return this->beforeHead->next->data; }
    iterator before_begin() const { return iterator(&this->beforeHead); }
    iterator begin() const { return iterator(this->beforeHead.next); }
    iterator end() const { return iterator(nullptr); }
    const_iterator cbefore_begin() const { return const_iterator(&this->beforeHead); }
    const_iterator cbegin() const { return const_iterator(this->beforeHead->next); }
    const_iterator cend() const { return const_iterator(nullptr); }
    size_type size() const { return this->count; }
    
    ~ForwardList() { this->clear(); }
};


template<typename Type>
ForwardList<Type>::ForwardList(const ForwardList<Type> &fl)
    : beforeHead({0, nullptr}), count(fl.count)
{
    Node *current = nullptr;

    for(auto val : fl){
        current = new Node{val, current};
    }

    this->beforeHead.next = current;
}


template<typename Type>
void ForwardList<Type>::push_front(const Type &value)
{
    this->count++;

    Node *&head = this->beforeHead.next;
    head = new Node{value, head};
}


template<typename Type>
void ForwardList<Type>::pop_front()
{
    this->count--;

    Node *&head = this->beforeHead.next;

    if(head != nullptr){
        Node *tmp = head;
        head = head->next;
        delete tmp;
    }
}


template<typename Type>
auto ForwardList<Type>::insert_after(const_iterator pos, const Type &value) -> iterator
{
    this->count++;

    auto current = reinterpret_cast<Node *>(pos.ptr);
    current->next = new Node{value, current->next};

    return iterator(current->next);
}


template<typename Type>
auto ForwardList<Type>::erase_after(const_iterator pos) -> iterator
{
    this->count--;

    auto current = reinterpret_cast<Node *>(pos.ptr);
    Node *tmp = current->next;
    current->next = tmp->next;
    delete tmp;

    return iterator(current->next);
}


template<typename Type>
void ForwardList<Type>::remove(const Type &value)
{
    auto it = this->before_begin();

    while(it.next() != this->end()){
        if(*it.next() == value){
            this->erase_after(it);
            this->count--;
        }
        else{
            it++;
        }
    }
}


template<typename Type>
void ForwardList<Type>::clear()
{
    this->count = 0;

    Node *&head = this->beforeHead.next;
    std::function<void (Node *)> freeNext;

    freeNext = [&] (Node *node) -> void{
        if(node->next != nullptr){
            freeNext(node->next);
        }

        delete node;
    };

    if(head != nullptr){
        freeNext(head);
        head = nullptr;
    }
}


template<typename Type>
template<typename Compare>
void ForwardList<Type>::sort(Compare comp)
{
    Node *&head = this->beforeHead.next;
    Node *newHead = head;

    if(head == nullptr){
        return;
    }

    head = head->next;
    newHead->next = nullptr;

    while(head != nullptr){
        auto current = head;
        head = head->next;

        if(comp(current->data, newHead->data)){
            current->next = newHead;
            newHead = current;
        }
        else{
            bool inserted = false;
            auto it = newHead;

            while((it->next != nullptr) && (!inserted)){
                auto pos = it;
                it = it->next;

                if(comp(current->data, pos->next->data)){
                    current->next = pos->next;
                    pos->next = current;
                    inserted = true;
                }
            }

            if(!inserted){
                it->next = current;
                current->next = nullptr;
            }
        }
    }

    head = newHead;
}


template<typename Type>
void ForwardList<Type>::reverse()
{
    Node *&head = this->beforeHead.next;
    Node *newHead = nullptr;

    while(head != nullptr){
        auto current = head;
        head = head->next;

        current->next = newHead;
        newHead = current;
    }

    head = newHead;
}

template<typename Type>
template<typename CutIf>
std::array<ForwardList<Type>, 2> ForwardList<Type>::split(CutIf cutIf) const
{
    ForwardList<Type> frist = *this, second;
    Node *current = &frist.beforeHead, *pos = &second.beforeHead;

    while(current->next != nullptr){
        if(cutIf(current->next->data)){
            pos->next = current->next;
            pos = pos->next;
            current->next = pos->next;
            pos->next = nullptr;

            frist.count--;
            second.count++;
        }
        else{
            current = current->next;
        }
    }

    return {frist, second};
}