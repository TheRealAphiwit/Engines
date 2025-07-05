#pragma once
class Component {
public:
    Component() {};
    virtual ~Component() = default;
    virtual void Update() {}
};