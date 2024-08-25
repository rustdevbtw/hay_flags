/**
 * @file hay/flags.h
 * @brief Flag parsing library for command-line applications.
 *
 * This header defines a simple API for creating and parsing command-line flags
 * in C. It supports string, integer, and boolean flags, with optional short
 * names.
 */

#ifndef HAY_FLAGS_H
#define HAY_FLAGS_H

// C23 compatibility checks
#if __STDC__ != 1
#error "[hay/flags] Not a standard compliant compiler"
#endif

#if __STDC_VERSION__ < 202311L
#include <stddef.h>
#define nullptr NULL
#define true 1
#define false 0
#endif

#if __STDC_VERSION__ < 199901L
#typedef int bool;
#else
#include <stdbool.h>
#include <stddef.h>
#endif

/**
 * @enum flag_ty_t
 * @brief Enum representing the type of a flag.
 */
typedef enum {
  FT_NULL, ///< Represents a null type, used for uninitialized flags.
  FT_STR,  ///< String type flag.
  FT_INT,  ///< Integer type flag.
  FT_BOOL  ///< Boolean type flag.
} flag_ty_t;

/**
 * @union flag_v_t
 * @brief Union to store the value of a flag.
 */
typedef union {
  const char *val_str; ///< Value if the flag is of type FT_STR.
  int val_int;         ///< Value if the flag is of type FT_INT.
  bool val_bool;       ///< Value if the flag is of type FT_BOOL.
} flag_v_t;

/**
 * @struct flag
 * @brief Structure representing a command-line flag.
 *
 * This structure holds information about a command-line flag, including its
 * name, short name, type, value, and whether it has been set by the user.
 */
typedef struct flag {
  char *name;      ///< The long name of the flag (e.g., "verbose").
  char short_name; ///< The short name of the flag (e.g., 'v'), 0 if none.
  flag_ty_t type;  ///< The type of the flag (string, integer, boolean).
  flag_v_t val;    ///< The value of the flag.
  bool is_set;     ///< Boolean indicating if the flag has been set.
} flag_t;

/**
 * @brief Creates a new flag.
 *
 * Initializes a new flag with the specified name, short name, and type.
 *
 * @param name The long name of the flag (e.g., "verbose").
 * @param short_name The short name of the flag (e.g., 'v'), use 0 if none.
 * @param type The type of the flag (FT_STR, FT_INT, FT_BOOL).
 * @return A pointer to the newly created flag.
 *
 * @note The returned pointer must be freed by the caller after use.
 */
flag_t *hay_flags_create(const char *name, const char short_name,
                         flag_ty_t type);

/**
 * @brief Parses command-line arguments and sets the corresponding flags.
 *
 * Iterates over the provided command-line arguments and sets the corresponding
 * flags based on the input.
 *
 * @param flags Array of pointers to flag_t structures, terminated by nullptr.
 * @param argc The argument count from main().
 * @param argv The argument vector from main().
 * @return 0 on success, or a negative error code on failure.
 *
 * @note The flags array must contain all the possible flags that can be set
 *       by the command-line arguments.
 */
int hay_flags_parse(flag_t **flags, int argc, char **argv);

/**
 * @brief Retrieves the value of a null flag, or a default if not set.
 *
 * Returns the value of the given null flag, or the provided default value if
 * the flag is not set.
 *
 * @param flag Pointer to the flag to retrieve the value from.
 * @param defval The default value to return if the flag is not set.
 * @return The null value of the flag, or defval if not set.
 */
bool hay_flags_getnull(flag_t *flag, const bool defval);

/**
 * @brief Retrieves the value of an integer flag, or a default if not set.
 *
 * Returns the value of the given integer flag, or the provided default value
 * if the flag is not set.
 *
 * @param flag Pointer to the flag to retrieve the value from.
 * @param defval The default value to return if the flag is not set.
 * @return The integer value of the flag, or defval if not set.
 */
int hay_flags_getint(flag_t *flag, const int defval);

/**
 * @brief Retrieves the value of a string flag, or a default if not set.
 *
 * Returns the value of the given string flag, or the provided default value
 * if the flag is not set.
 *
 * @param flag Pointer to the flag to retrieve the value from.
 * @param defval The default value to return if the flag is not set.
 * @return The string value of the flag, or defval if not set.
 *
 * @note The returned string is owned by the flag structure and should not
 *       be freed by the caller.
 */
const char *hay_flags_getstr(flag_t *flag, const char *defval);

/**
 * @brief Retrieves the value of a boolean flag, or a default if not set.
 *
 * Returns the value of the given boolean flag, or the provided default value
 * if the flag is not set.
 *
 * @param flag Pointer to the flag to retrieve the value from.
 * @param defval The default value to return if the flag is not set.
 * @return The boolean value of the flag, or defval if not set.
 */
bool hay_flags_getbool(flag_t *flag, const bool defval);

#endif // HAY_FLAGS_H
