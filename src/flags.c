#include <errno.h>
#include <hay/flags.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates a new flag structure.
 *
 * Allocates memory for a new flag and initializes it with the provided name,
 * short name, type, and aliases.
 *
 * @param name The long name of the flag (e.g., "verbose"). This string is
 *             duplicated using `strdup`, so the caller should ensure it is
 *             a valid null-terminated string.
 * @param short_name The short name of the flag (e.g., 'v'). Use 0 if no
 *                   short name is needed.
 * @param type The type of the flag (FT_STR, FT_INT, FT_BOOL). This determines
 *             how the flag value will be stored and processed.
 * @return Pointer to the newly created flag_t structure, or nullptr if an
 *         error occurs. In case of error, `errno` is set to indicate the error.
 *
 * @note The returned `flag_t` structure must be freed by the caller when no
 *       longer needed.
 */
flag_t *hay_flags_create(const char *name, const char short_name,
                         flag_ty_t type) {
  if (!name) {
    errno = EINVAL; // Set errno to EINVAL if the provided name is null.
    return nullptr;
  }

  // Allocate memory for the flag structure.
  flag_t *flag = malloc(sizeof(flag_t));
  if (!flag) {
    perror("hay_flags_create()"); // Print error message if malloc fails.
    errno = ENOMEM; // Set errno to ENOMEM to indicate memory issue.
    return nullptr;
  }

  // Duplicate the flag name and check for errors.
  flag->name = strdup(name);
  if (!flag->name) {
    perror("strdup()"); // Print error message if strdup fails.
    if (flag != nullptr) {
      free(flag);
    }
    errno = ENOMEM; // Set errno to ENOMEM to indicate memory issue.
    return nullptr;
  }

  // Initialize other flag members.
  flag->short_name = short_name;
  flag->type = type;
  flag->is_set = false; // Set flag as not set initially.

  return flag;
}

/**
 * @brief Parses command-line arguments and sets the corresponding flags.
 *
 * Processes the command-line arguments and updates the flag values based on
 * the parsed options. Supports both long and short flags.
 *
 * @param flags Array of pointers to flag_t structures, terminated by nullptr.
 *              Each flag in the array is checked against the arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line argument vector.
 * @return 0 on success, or -1 if an error occurs. In case of error, `errno` is
 *         set to indicate the error.
 *
 * @note The flags array must be terminated with a nullptr to indicate the end
 *       of the array.
 */
int hay_flags_parse(flag_t **flags, int argc, char **argv) {
  if (!flags || !argv) {
    errno = EINVAL; // Set errno to EINVAL if either flags or argv is null.
    return -1;
  }

  // Iterate over command-line arguments starting from index 1 (skip program
  // name).
  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];
    if (!arg) {
      continue; // Skip null arguments.
    }

    // Iterate over the array of flags.
    for (int j = 0; flags[j] != nullptr; j++) {
      flag_t *flag = flags[j];
      if (!flag) {
        continue; // Skip null flag pointers.
      }

      if (arg[0] == '-') {
        if (arg[1] == '-') {
          // Long option (e.g., --verbose)
          if (strcmp(&arg[2], flag->name) == 0) {
            if (flag->type == FT_BOOL) {
              // Treat --flag as --flag true
              flag->val.val_bool = true;
              flag->is_set = true;
            } else if (i + 1 < argc) {
              char *v = argv[++i];
              if (!v) {
                continue; // Skip if no value is provided after the flag.
              }

              // Set the flag value based on its type.
              switch (flag->type) {
              case FT_INT:
                if (sscanf(v, "%d", &flag->val.val_int) != 1) {
                  continue; // Skip if value cannot be parsed as integer.
                }
                break;
              case FT_STR:
                flag->val.val_str = strdup(v);
                if (!flag->val.val_str) {
                  perror("strdup()"); // Print error if strdup fails.
                  errno = ENOMEM;     // Set errno to ENOMEM for memory issues.
                  continue;
                }
                break;
              case FT_BOOL:
                if (strcmp(v, "true") == 0 || strcmp(v, "1") == 0) {
                  flag->val.val_bool = true;
                } else if (strcmp(v, "false") == 0 || strcmp(v, "0") == 0) {
                  flag->val.val_bool = false;
                } else {
                  flag->val.val_bool =
                      true; // Default to true for unknown values.
                }
                break;
              default:
                continue; // Skip unsupported flag types.
              }
              flag->is_set = true; // Mark the flag as set.
            } else if (flag->type == FT_NULL) {
              flag->is_set = true; // Mark null flags as set.
            } else {
              continue; // Skip if value is missing and type is not FT_NULL.
            }
          }
        } else {
          // Short option (e.g., -v or -v value)
          for (int k = 1; arg[k] != '\0'; k++) {
            char f = arg[k];
            if (flag->short_name == f) {
              if (flag->type == FT_BOOL && arg[k + 1] == '\0') {
                // Treat -f as -f true
                flag->val.val_bool = true;
                flag->is_set = true;
              } else if (arg[k + 1] == '\0' && i + 1 < argc &&
                         flag->type != FT_NULL) {
                char *v = argv[++i];
                if (!v) {
                  continue; // Skip if no value is provided after the flag.
                }

                // Set the flag value based on its type.
                switch (flag->type) {
                case FT_INT:
                  if (sscanf(v, "%d", &flag->val.val_int) != 1) {
                    perror("hay_flags_parse(int)");
                    errno = EINVAL;
                    continue; // Skip if value cannot be parsed as integer.
                  }
                  flag->is_set = true;
                  break;
                case FT_STR:
                  flag->val.val_str = strdup(v);
                  if (!flag->val.val_str) {
                    perror("strdup()"); // Print error if strdup fails.
                    errno = ENOMEM; // Set errno to ENOMEM for memory issues.
                    continue;
                  }
                  flag->is_set = true;
                  break;
                case FT_BOOL:
                  if (strcmp(v, "true") == 0 || strcmp(v, "1") == 0) {
                    flag->val.val_bool = true;
                    flag->is_set = true;
                  } else if (strcmp(v, "false") == 0 || strcmp(v, "0") == 0) {
                    flag->val.val_bool = false;
                    flag->is_set = true;
                  } else {
                    flag->val.val_bool =
                        true; // Default to true for unknown values.
                    flag->is_set = true;
                  }
                  break;
                default:
                  continue; // Skip unsupported flag types.
                }
              } else if (flag->type == FT_NULL) {
                flag->is_set = true; // Mark null flags as set.
                continue;
              } else {
                continue; // Skip if value is missing and type is not FT_NULL.
              }
            }
          }
        }
      }
    }
  }

  return 0;
}

/**
 * @deprecated Use hay_flags_getbool() with FT_BOOL as the type
 * @brief Retrieves the value of a null flag or returns a default value.
 *
 * Checks if the specified flag is a null type and if it is set. If so, returns
 * true; otherwise, returns the provided default value.
 *
 * @param flag Pointer to the flag to check.
 * @param defval The default value to return if the flag is not set or not a
 * null type.
 * @return true if the flag is a null type and is set, otherwise returns defval.
 */
bool hay_flags_getnull(flag_t *flag, const bool defval) {
  if (flag && flag->is_set && flag->type == FT_NULL) {
    return true;
  }
  return defval;
}

/**
 * @brief Retrieves the value of an integer flag or returns a default value.
 *
 * Checks if the specified flag is of integer type and if it is set. If so,
 * returns the flag's integer value; otherwise, returns the provided default
 * value.
 *
 * @param flag Pointer to the flag to check.
 * @param defval The default value to return if the flag is not set or not an
 * integer type.
 * @return The integer value of the flag if set, otherwise returns defval.
 */
int hay_flags_getint(flag_t *flag, const int defval) {
  if (flag && flag->is_set && flag->type == FT_INT) {
    return flag->val.val_int;
  }
  return defval;
}

/**
 * @brief Retrieves the value of a string flag or returns a default value.
 *
 * Checks if the specified flag is of string type and if it is set. If so,
 * returns the flag's string value; otherwise, returns the provided default
 * value.
 *
 * @param flag Pointer to the flag to check.
 * @param defval The default value to return if the flag is not set or not a
 * string type.
 * @return The string value of the flag if set, otherwise returns defval.
 */
const char *hay_flags_getstr(flag_t *flag, const char *defval) {
  if (flag && flag->is_set && flag->type == FT_STR) {
    return flag->val.val_str;
  }
  return defval;
}

/**
 * @brief Retrieves the value of a boolean flag or returns a default value.
 *
 * Checks if the specified flag is of boolean type and if it is set. If so,
 * returns the flag's boolean value; otherwise, returns the provided default
 * value.
 *
 * @param flag Pointer to the flag to check.
 * @param defval The default value to return if the flag is not set or not a
 * boolean type.
 * @return The boolean value of the flag if set, otherwise returns defval.
 */
bool hay_flags_getbool(flag_t *flag, const bool defval) {
  if (flag && flag->is_set && flag->type == FT_BOOL) {
    return flag->val.val_bool;
  }
  return defval;
}
