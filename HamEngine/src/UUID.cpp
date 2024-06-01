#include "Ham/Util/UUID.h"

namespace Ham {
std::shared_ptr<uuids::uuid_random_generator> UUIDGenerator::s_Generator;
}  // namespace Ham
