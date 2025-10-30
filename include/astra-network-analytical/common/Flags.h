#pragma once

#include <iostream>
#include <string>

namespace NetworkAnalytical {

/**
 * Global verbose logging flag.
 * Set to true to enable debug logging in analytical backend.
 */
constexpr bool DEBUG_PRINT = false;

/**
 * Ring topology bidirectional flag.
 * Set to false to make ring unidirectional.
 */
 constexpr bool RING_BIDIRECTIONAL = false;

} // namespace NetworkAnalytical
