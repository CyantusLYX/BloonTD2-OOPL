#ifndef BLOON_HOLDER_HPP
#define BLOON_HOLDER_HPP
#include "pch.hpp"
#include "bloon.hpp"
#include "interfaces.hpp"
#include "Util/Position.hpp"
#include "Util/GameObject.hpp"
#include <memory>
class BloonHolder : public Interface::ICollapsible, public Util::GameObject {
    private:
        std::shared_ptr<Bloon> m_Bloon;
       
    public:
        void bloon_spawn(Bloon::Type type, ) {
            m_Bloon = std::make_shared<Bloon>(type);
        }
};
#endif // BLOONS_HOLDER_HPP