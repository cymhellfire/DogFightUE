#pragma once

/** Test a single flag with given mask. */
#define TEST_SINGLE_FLAG(Bitmask, Bit) (((Bitmask) & (1 << static_cast<int32>(Bit))) > 0)
/** Test multiple flags with given mask. */
#define TEST_MULTIPLE_FLAGS(Bitmask, DesiredFlags) (((Bitmask) & (DesiredFlags)) == (DesiredFlags))
/** Merge flags into given mask. */
#define MERGE_FLAGS(Bitmask, FlagsToAdd) Bitmask = Bitmask | FlagsToAdd
/** Remove flags from given mask. */
#define REMOVE_FLAGS(Bitmask, FlagsToRemove) Bitmask = Bitmask & (~FlagsToRemove)