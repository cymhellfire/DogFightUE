#pragma once

/**
 * @brief Convert a boolean value to corresponding string.
 * @param InValue Boolean value to convert
 */
#define BOOL_TO_STR(InValue) \
	(InValue ? TEXT("true") : TEXT("false"))
