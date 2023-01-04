#ifndef LIST_H
#define LIST_H

template <class T>
class List{
public:
    class Node;

    List();

    ~List();

    void append(T* data);

    Node* get_first();

private:
    Node* m_first;
};

template <class T>
class List<T>::Node{
public:
    Node(const Node&) = delete;
    Node& operator=(const Node& other) = delete;
    Node* get_next();
    T* get_data();
    Node(T* data);
private:
    T* m_data;
    Node* m_next;
    friend class List<T>;
};

template <class T>
List<T>::Node::Node(T* data) : m_data(data), m_next(nullptr){}

template <class T>
typename List<T>::Node* List<T>::Node::get_next(){
    return m_next;
}

template <class T>
T* List<T>::Node::get_data(){
    return m_data;
}

template <class T>
List<T>::List() : m_first(nullptr){}

template <class T>
List<T>::~List(){
    while(m_first){
        Node* temp = m_first;
        m_first = m_first->m_next;
        delete temp;
    }
}

template <class T>
typename List<T>::Node* List<T>::get_first(){
    return m_first;
}

template <class T>
void List<T>::append(T* data){
    Node* newNode (new List<T>::Node(data));
    newNode->m_next = m_first;
    m_first = newNode;
}

#endif