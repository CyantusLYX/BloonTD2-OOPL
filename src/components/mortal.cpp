#include "components/mortal.hpp"
#include "core/uuid.hpp"

Mortal::Mortal()
    : m_uuid(Util::generate_uuid()) {}
void Mortal::kill() {
  kill_children();
  pre_kill();
  m_life_status = LifeStatus::Dead;
}
