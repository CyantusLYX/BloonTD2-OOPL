#include "mortal.hpp"
#include "uuid.hpp"

Mortal::Mortal()
    : m_uuid(Util::generate_uuid()) {
}