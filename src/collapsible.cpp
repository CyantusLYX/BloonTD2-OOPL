#include "collapsible.hpp"

//rectangle to rectangle
bool Collapsible::isCollide(const Collapsible &other) const{
    if(other.m_Pivot == m_Pivot){//shoud fix the c_box
        return true;
    }
    return false;
}

//rectangle to dot
//if it trigger every object's isCollide when mouse click
//it will be so odd, doesn't it?
bool Collapsible::isCollide(const glm::vec2 pt) const{
    if(pt == m_Pivot){
        return true;
    }
    return false;
}