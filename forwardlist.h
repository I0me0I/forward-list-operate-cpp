#include<iostream>
#include<functional>

template<typename Type>
class ForwardList
{
private:
    struct Node{
        Type data;
        Node *next;
    };

    class ConstIterator;

    class Iterator{
    private:
        Node *ptr;

    public:
        explicit Iterator(Node *ptr) : ptr(ptr) {}

        Type &operator*() { return this->ptr->data; }
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

        friend ConstIterator::ConstIterator(const Iterator &it);
    };

    class ConstIterator{
    private:
        const Node *ptr;

    public:
        explicit ConstIterator(const Node *ptr) : ptr(ptr) {}
        ConstIterator(const Iterator &it) : ptr(it.ptr) {}

        const Type &operator*() const { return this->ptr->data; }

        bool operator==(const ConstIterator &it) const { return this->ptr == it.ptr; }
        bool operator!=(const ConstIterator &it) const { return !(*this == it); }

        ConstIterator &operator++(){
            this->ptr = this->ptr->next;
            return *this;
        }

        ConstIterator operator++(int){
            ConstIterator it = *this;
            this->ptr = this->ptr->next;
            return it;
        }
    };

    Node *head, *tail;
    size_t count;

public:
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using size_type = size_t;

    ForwardList() : head(nullptr), tail(nullptr), count(0) {}

    void push_back(const Type &value);
    void pop_front();
    void insert(const_iterator it, const Type &value);
    void erase(const_iterator it);
    void clear();

    iterator begin() const { return iterator(this->head); }
    iterator end() const { return iterator(this->tail->next); }
    const_iterator cbegin() const { return const_iterator(this->head); }
    const_iterator cend() const { return const_iterator(this->tail->next); }
    size_type size() const { return this->count; }

    ~ForwardList() { this->clear(); }
};


template<typename Type>
void ForwardList<Type>::push_back(const Type &value)
{
    this->count++;

    if(this->head == nullptr){
        this->head = this->tail = new Node;
    }
    else{
        this->tail->next = new Node;
        this->tail = this->tail->next;
    }

    this->tail->data = value;
    this->tail->next = nullptr;
}


template<typename Type>
void ForwardList<Type>::pop_front()
{
    this->count--;

    if(this->head == nullptr){
        return;
    }

    if(this->head == this->tail){
        delete this->head;
        this->head = this->tail = nullptr;
    }

    Node *tmp = this->head;
    this->head = tmp->next;
    delete tmp;
}


template<typename Type>
void ForwardList<Type>::insert(const_iterator it, const Type &value)
{
    this->count++;

    auto current = reinterpret_cast<const Node *>(&(*it));

    Node *newNode = new Node;
    newNode->data = value;

    if(current == this->head){
        newNode->next = this->head;
        this->head = newNode;
    }
    else{
        Node *prev;
        for(
            prev = this->head;
            prev->next != current;
            prev = prev->next
        );

        newNode->next = prev->next;
        prev->next = newNode;
    }
}


template<typename Type>
void ForwardList<Type>::erase(const_iterator it)
{
    this->count--;

    auto current = reinterpret_cast<const Node *>(&(*it));

    if(current == this->head){
        this->pop_front();
        return;
    }

    Node *prev;
    for(
        prev = this->head;
        prev->next != current;
        prev = prev->next
    );

    if(current == this->tail){
        prev->next = nullptr;
    }
    else{
        prev->next = prev->next->next;
    }

    delete current;
}


template<typename Type>
void ForwardList<Type>::clear()
{
    this->count = 0;

    std::function<void (Node *)> freeNext;

    freeNext = [&, this] (Node *ptr) -> void{
        if(ptr != this->tail){
            freeNext(ptr->next);
        }

        delete ptr;
    };

    if(this->head != nullptr){
        freeNext(this->head);
        this->head = this->tail = nullptr;
    }
}