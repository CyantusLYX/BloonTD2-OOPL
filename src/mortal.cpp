#include "mortal.hpp"
#include "uuid.hpp"

Mortal::Mortal() : m_uuid(Util::generate_uuid()) {}
void Mortal::kill() {
  pre_kill();
  m_life_status = LifeStatus::Dead;
}