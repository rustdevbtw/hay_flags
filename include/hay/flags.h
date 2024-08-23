#ifndef HAY_FLAGS_H
#define HAY_FLAGS_H

// C23 compat
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

#define MAX_FLAGS                                                              \
  1024 /**< Maximum number of flags or characters for temporary storage. */

/**
 * @brief Represents a single flag and its associated value.
 *
 * This structure holds a flag string and a pointer to where the value for
 * that flag will be stored. It also optionally holds a check bit and index.
 */
typedef struct flag {
  char *flag;  /**< The flag string (e.g., "-p" or "--port"). */
  char **val;  /**< Pointer to a variable where the flag's value will be stored.
                  This pointer may be nullptr if the flag does not require a
                  value. */
  bool *check; /**< Pointer to an integer used as a check bit. If not nullptr
                 (NULL), it will be set to 1 (true) if the flag is matched. */
  int *idx; /**< Pointer to an integer where the index of the matched argument
               is stored. */
} flag_t;

__attribute__((deprecated(
    "not recommended. use flag_t* instead"))) typedef flag_t *flags_t; /**< Type
      definition for a pointer to an array of `flag_t` structures. */

/**
 * @brief Parses command-line arguments and matches them with predefined flags.
 *
 * This function processes the command-line arguments (`argv`) and attempts
 * to match each argument with the flags specified in the `flags` array.
 * If a flag is found, its corresponding value is stored in the `val` field
 * of the `flag_t` structure associated with that flag. Memory is allocated
 * for the values as needed.
 *
 * @param flags Pointer to an array of `flag_t` structures. It is assumed to be
 * a valid array of `flag_t` structures that will be filled with the parsed
 * results. The caller is responsible for freeing the allocated memory when it
 * is no longer needed.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return Returns the `flags` array on success. If parsing fails or if any flag
 * is missing its required argument, the function sets `errno` to an appropriate
 * error code and returns nullptr.
 *
 * @retval nullptr If an error occurs. The global `errno` is set to indicate the
 * specific error.
 * @retval EINVAL If `flags` or `argv` is nullptr, or if an unrecognized flag is
 * encountered, or if a required argument is missing.
 * @retval ENOMEM If memory allocation fails while storing flag values.
 *
 * @note The `val` field in `flag_t` structures can be nullptr if the flag does
 * not require a value. The function will handle memory allocation for `val` and
 * should be managed properly.
 *
 * @example
 * // Example usage
 * char port[256];
 * char dir[256];
 * int verbosity = 0;
 * flag_t my_flags[] = {
 *     {"-p", &port},
 *     {"--port", &port},
 *     {"-d", &dir},
 *     {"--dir", &dir},
 *     {"--verbose", nullptr, &verbosity}
 * };
 *
 * int argc = 10;
 * char *argv[] = {"program", "-p", "8080", "--verbose", "--port", "5000", "-d",
 * "/path/to/dir"};
 *
 * // Parse the flags
 * flag_t *parsed_flags = hay_flags_parse(my_flags, argc, argv);
 *
 * if (parsed_flags == nullptr) {
 *     // Handle error
 *     perror("Failed to parse flags");
 * } else {
 *     // Successfully parsed
 *     printf("Port: %s\n", port);
 *     printf("Directory: %s\n", dir);
 *     printf("Verbosity: %d\n", verbosity);
 * }
 */
flag_t *hay_flags_parse(flag_t *flags, int argc, char **argv);

/**
 * @brief Retrieves the `flag_t` structure associated with a specific flag.
 *
 * This function searches for the specified flag in the `flags` array and
 * returns the corresponding `flag_t` structure if found. If the flag is not
 * found, the function returns nullptr.
 *
 * @param flags Pointer to an array of `flag_t` structures.
 * @param flag The flag string to search for.
 * @return A pointer to the `flag_t` structure associated with the specified
 * flag, or nullptr if the flag is not found.
 */
flag_t *hay_flags_get(flag_t *flags, char *flag);

/**
 * @brief Retrieves the value associated with a specific flag.
 *
 * This function retrieves the value stored in the `val` field of the `flag_t`
 * structure associated with the specified flag. If the flag does not have an
 * associated value, or if the flag is not found, the function returns nullptr.
 *
 * @param flags Pointer to an array of `flag_t` structures.
 * @param flag The flag string whose value is to be retrieved.
 * @return The value associated with the specified flag, or nullptr if the flag
 * does not have an associated value or if the flag is not found.
 */
char *hay_flags_get_val(flag_t *flags, char *flag);

/**
 * @brief Retrieves the check bit associated with a specific flag.
 *
 * This function retrieves the value of the check bit stored in the `check`
 * field of the `flag_t` structure associated with the specified flag. If the
 * flag does not have a check bit, or if the flag is not found, the function
 * returns 0.
 *
 * @param flags Pointer to an array of `flag_t` structures.
 * @param flag The flag string whose check bit is to be retrieved.
 * @return The value of the check bit associated with the specified flag, or 0
 * if the flag does not have a check bit or if the flag is not found.
 */
bool hay_flags_get_check(flag_t *flags, char *flag);

/**
 * @brief Retrieves the index of a specific flag.
 *
 * This function retrieves the index of the command-line argument that matches
 * the specified flag. If the flag is not found, or if the index is not set,
 * the function returns -1.
 *
 * @param flags Pointer to an array of `flag_t` structures.
 * @param flag The flag string whose index is to be retrieved.
 * @return The index of the command-line argument that matches the specified
 * flag, or -1 if the flag is not found or if the index is not set.
 */
int hay_flags_get_idx(flag_t *flags, char *flag);

#endif // HAY_FLAGS_H
