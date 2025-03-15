#include "collapsible.hpp"
#include <X11/Xlibint.h>

//ColType to ColType
//wait to be implement
bool Collapsible::isCollide(const Collapsible &other) const{
    //return false;

    switch (m_tcol) {
        case ColType::DOT:
            switch (other.m_tcol) {
                case ColType::DOT:
                    return false;
                case ColType::OVAL:
                    return false;
                case ColType::RECTANGLE:
                    return false;
                default:
                    throw std::invalid_argument( "err on collapsible::isCollide switch" );
                    return false;
            }
            return false;
        case ColType::OVAL:
            switch (Collapsible::m_tcol) {
                case ColType::DOT:
                    return false;
                case ColType::OVAL:
                    return false;
                case ColType::RECTANGLE:
                    return false;
                default:
                    throw std::invalid_argument( "err on collapsible::isCollide switch" );
                    return false;
            }
            return false;
        case ColType::RECTANGLE:
            switch (Collapsible::m_tcol) {
                case ColType::DOT:
                    return false;
                case ColType::OVAL:
                    return false;
                case ColType::RECTANGLE:
                    return false;
                default:
                    throw std::invalid_argument( "err on collapsible::isCollide switch" );
                    return false;
            }
            return false;
        default:
            throw std::invalid_argument( "err on collapsible::isCollide switch" );
            return false;
    }
}

//ColType to dot
//if it trigger every object's isCollide when mouse click
//it will be so odd, doesn't it?
bool Collapsible::isCollide(const glm::vec2 pt) const{
    //return false;
    switch (m_tcol) {
        case ColType::DOT:
            return pt == m_Pivot;
        case ColType::OVAL:
            return pow((pt.x-m_Pivot.x)/m_pcol.x, 2) + pow((pt.y-m_Pivot.y)/m_pcol.y, 2) > 1.0;
        case ColType::RECTANGLE:
            return  (pt.x < m_Pivot.x + m_pcol.x/2)
                  &&(pt.x > m_Pivot.x - m_pcol.x/2)
                  &&(pt.y < m_Pivot.y + m_pcol.y/2)
                  &&(pt.y > m_Pivot.y - m_pcol.y/2);
        default:
            throw std::invalid_argument( "err on collapsible::isCollide switch" );
            return false;
    }
    
}