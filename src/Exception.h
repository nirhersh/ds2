#ifndef EXCEPTION_H
#define EXCEPTION_H


class KeyAlreadyExists : public std::exception{
public:
    const char* what() const noexcept override{
        return "Key Already Exists: Key already exists in the tree";
    }
};

class BadKey : public std::exception{
public:
    const char* what() const noexcept override{
        return "Bad Key: Key is not valid";
    }
};

class KeyDoesntExists : public std::exception{
public:
    const char* what() const noexcept override{
        return "Key Doesn't Exists: No such key in the tree";
    }
};

class CantFitInArray : public std::exception{
public:
    const char* what() const noexcept override{
        return "Can't Fit In Array: the allocated array is too small for the tree";
    }
};

class EmptyTree : public std::exception{
public:
    const char* what() const noexcept override{
        return "Empty Tree: the tree is empty";
    }
};

class InvalidArguments : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid Arguments: invalid arguments for player or a team";
    }
};

#endif //EXCEPTION_H