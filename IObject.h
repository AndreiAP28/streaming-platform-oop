#pragma once

#include <string>
#include <iostream>

class IObject {
    static int counter;
    const int id;

protected:
    IObject(): id(++counter) {}
    IObject(int old_id): id(old_id) {} // used in inheritence for user classes family
    IObject(const IObject& other_IObject): id(++(other_IObject.counter)) {}
public:
    int getID() const noexcept { return id; }

    virtual std::string toString() const = 0;
    
    bool operator==(const IObject& other) const noexcept {
        return id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const IObject& obj) {
        os << obj.toString();
        return os;
    }

    virtual ~IObject() noexcept = default;



};