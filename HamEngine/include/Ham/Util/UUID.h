#pragma once

#include "Ham/Core/Base.h"

#include <uuid.h>

#include <random>
#include <string>

namespace Ham {
typedef std::string UUID;
class UUIDGenerator {
 public:
  static void Init()
  {
    static std::random_device rd;
    static auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    static std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    static std::mt19937 generator(seq);
    s_Generator = std::make_shared<uuids::uuid_random_generator>(generator);
  }

  static std::shared_ptr<uuids::uuid_random_generator> GetGenerator() { return s_Generator; }
  static UUID Create() { return (UUID)uuids::to_string((*GetGenerator())()); }

 private:
  static std::shared_ptr<uuids::uuid_random_generator> s_Generator;
};
}  // namespace Ham
