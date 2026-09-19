#include "rng.h"

#include <random>

namespace jsh::rng {
        int rInt(int a, int b) {
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist(a, b);
                int num = dist(rng);
                return num;
        }
} // namespace jsh::rng
