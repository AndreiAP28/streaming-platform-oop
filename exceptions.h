#pragma once

#include <iostream>
#include <stdexcept>
#include <string>



class StreamingAppException: public std::exception {
protected:
    std::string message;

public:
    StreamingAppException(const std::string& msg): message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }

    virtual ~StreamingAppException() noexcept = default;
};

class ObjectNotFoundException: public StreamingAppException {
    std::string missing_obj;
public:
    ObjectNotFoundException(const std::string& obj_type, const std::string& obj_value)
    : StreamingAppException("[!] 404 Error: " + obj_type + "'" + obj_value + "' was not found in the database."), missing_obj(obj_value) {}

    const std::string& getMissingObj() const { return missing_obj; }
};

class LimitExceededException : public StreamingAppException {
    int maxlimit_size;
public:
    LimitExceededException(const std::string& container_name, int max_size)
    : StreamingAppException( "[!] Action Denied: The limit for " + container_name + " (Max: " + std::to_string(max_size) + ") has been reached!"), maxlimit_size(max_size) {}

    int getLimit() const { return maxlimit_size;}
};

class DuplicateObjectException : public StreamingAppException {
    std::string duplicate_value;

public:
    DuplicateObjectException(const std::string& object_type, const std::string& object_value)
    : StreamingAppException("[!] DUPLICATE: A " + object_type + " with the value '" + object_value + "' already exists!"), duplicate_value(object_value) {}

    const std::string& getDuplicateValue() const { return duplicate_value; }
};

class OutOfRangeException: public StreamingAppException {
    int invalid_index;
    int max_limit;
public:
    OutOfRangeException(int index, int max_size)
    : StreamingAppException("Index out of range!"), invalid_index(index), max_limit(max_size){}
    
    int getInvalidIndex() const { return invalid_index;}

    int getMaxLimit() const { return max_limit; }

};

class InvalidActionException: public StreamingAppException {
public:
    InvalidActionException(const std::string& reason)
    :StreamingAppException("[!] INVALID ACTION: " + reason) {}
};