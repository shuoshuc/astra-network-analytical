#pragma once

#include <iostream>
#include <string>

namespace NetworkAnalytical {

/**
 * Global verbose logging flag.
 * Set to true to enable debug logging in analytical backend.
 */
inline constexpr bool DEBUG_PRINT = true;

/**
 * Debug logger.
 */
inline void debug_log(const std::string& msg) {
    if constexpr (DEBUG_PRINT) {
        std::cout << msg << std::endl;
    }
}

} // namespace NetworkAnalytical
