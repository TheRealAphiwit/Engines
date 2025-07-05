#pragma once
#include "VirtualObject.h"

class Component {
public:
    VirtualObject* Owner = nullptr;

    Component() {};
    virtual ~Component() = default;
    virtual void Update() {}
};