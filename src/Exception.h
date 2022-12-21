#ifndef EXCEPTION_H
#define EXCEPTION_H


class KeyAlreadyExists : public std::exception{
public:
    const char* what() const noexcept override{
        return "Key Already Exists: Key already exists in the tree";
    }
};

class BadKey{
public:
    const char* what(){
        return "Bad Key: Key is not valid";
    }
};

class KeyDoesntExists{
public:
    const char* what(){
        return "Key Doesn't Exists: No such key in the tree";
    }
};

class IdDoesntExists{
public:
    const char* what(){
        return "Id Doesn't Exists: No such Id in the tree";
    }
};

class CantFitInArray{
public:
    const char* what(){
        return "Can't Fit In Array: the allocated array is too small for the tree";
    }
};

class EmptyTree{
public:
    const char* what(){
        return "Empty Tree: the tree is empty";
    }
};

#endif