#include <errno.h>
#include <hay/flags.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Parses command-line arguments and matches them with predefined flags.
 *
 * This function processes the command-line arguments (`argv`) and attempts
 * to match each argument with the flags specified in the `flags` array.
 * If a flag is found, its corresponding value is stored in the `val` field
 * of the `flag_t` structure associated with that flag.
 *
 * The function operates as follows:
 * - If `flags` is nullptr, the function allocates memory for an array of
 * `flag_t` structures using `malloc`.
 * - It then iterates over the command-line arguments to find and match flags.
 * - If a flag is found, it checks if the `check` bit of it is nullptr. If it
 * is, it then checks if it's corresponding argument is provided. If that's
 * there, then the value will be stored in `val`, and it's index is stored in
 * `idx`.
 * - If the `check` bit is not nullptr, it'll then simply set that to 1 (true),
 *   and set `idx` to the index. It doesn't touch `val` in such a case.
 * - If the `flags` parameter is not nullptr, it will be used to store the
 * parsed results.
 * - On successful parsing, the function returns the `flags` array. If any error
 *   occurs (e.g., missing argument or memory allocation failure), the function
 * sets `errno` to `EINVAL` and returns nullptr.
 *
 * @param flags Pointer to an array of `flag_t` structures. If nullptr, the
 * function allocates memory for this array. If not nullptr, it is assumed to be
 * a valid array of `flag_t` structures that will be filled with the parsed
 * results. The caller is responsible for freeing the allocated memory when it
 * is no longer needed.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return Returns the `flags` array on success. If `flags` was nullptr, the
 * function allocates memory for it and returns the pointer. If parsing fails or
 * if any flag is missing its required argument, the function sets `errno` to
 *         `EINVAL` and returns nullptr.
 *
 * @note The caller must ensure that `flag_t` structures have valid pointers for
 *       `flag` and `val`. Specifically, `flag->val` should be a valid pointer
 * to a memory area where the argument value can be copied.
 *
 * @see flag_t
 *
 * @example
 * // Example usage
 * char port[256];
 * char dir[256];
 * flag_t my_flags[] = {
 *     {"-p", &port},
 *     {"-d", &dir}
 * };
 *
 * int argc = 5;
 * char *argv[] = {"program", "-p", "8080", "-d", "/path/to/dir"};
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
 * }
 */
flag_t *hay_flags_parse(flag_t *flags, int argc, char **argv) {
  if (argc == 0 || argv == nullptr) {
    errno = EINVAL;
    return nullptr;
  }
  if (flags == nullptr) {
    fprintf(stderr, "hay_flags_parse: flags cannot be nullptr\n");
    errno = EINVAL;
    return nullptr;
  }

  for (int i = 0; i < argc; i++) {
    if (argv[i] == nullptr) {
      fprintf(stderr, "hay_flags_parse: Null argument encountered\n");
      errno = EINVAL;
      return nullptr;
    }

    flag_t *found_flag = malloc(sizeof(flag_t));
    found_flag = nullptr;
    for (int j = 0; flags[j].flag != nullptr; j++) {
      flag_t *flag = &flags[j];
      if (flag->check == nullptr && flag->val == nullptr)
        flag->val = malloc(sizeof(char *));
      if (flag->flag == nullptr)
        break;
      if (strcmp(flag->flag, argv[i]) == 0) {
        found_flag = flag;
        break;
      }
    }

    if (found_flag == nullptr) {
      continue;
    }

    if (found_flag != nullptr) {
      flag_t *current_flag = found_flag;

      if (current_flag->check != nullptr) {
        *current_flag->check = true;
        if (current_flag->idx == nullptr) {
          current_flag->idx = malloc(sizeof(int));
          if (current_flag->idx == nullptr) {
            perror("hay_flags_parse: malloc(current_flag->idx)");
            errno = ENOMEM;
            return nullptr;
          }
        }
        *current_flag->idx = i;
      } else {
        // Move to the next argument which should be the value for the flag
        if (++i < argc) {
          if (current_flag->val == nullptr) {
            current_flag->val = malloc(sizeof(char *));
            if (current_flag->val == nullptr) {
              perror("hay_flags_parse: malloc(current_flag->val)");
              errno = ENOMEM;
              return nullptr;
            }
          }

          // Copy the argument value into the appropriate location
          *current_flag->val = argv[i];
          if (current_flag->idx == nullptr) {
            current_flag->idx = malloc(sizeof(int));
            if (current_flag->idx == nullptr) {
              perror("hay_flags_parse: malloc(current_flag->idx)");
              errno = ENOMEM;
              return nullptr;
            }
          }
          *current_flag->idx = i;
        } else {
          // Missing value for the flag
          fprintf(stderr, "hay_flags_parse: Missing argument for %s\n",
                  current_flag->flag);
          errno = EINVAL;
          return nullptr;
        }
      }
    }
  }

  return flags;
}

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
flag_t *hay_flags_get(flag_t *flags, char *flag) {
  if (flags == nullptr || flag == nullptr) {
    return nullptr;
  }

  for (int i = 0; flags[i].flag != nullptr; i++) {
    if (strcmp(flags[i].flag, flag) == 0) {
      return &flags[i];
    }
  }
  return nullptr;
}

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
char *hay_flags_get_val(flag_t *flags, char *flag) {
  flag_t *f = hay_flags_get(flags, flag);
  if (f != nullptr) {
    if (f->val != nullptr) {
      return *(f->val);
    } else {
      return nullptr;
    }
  }
  return nullptr;
}

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
bool hay_flags_get_check(flag_t *flags, char *flag) {
  flag_t *f = hay_flags_get(flags, flag);
  if (f != nullptr) {
    if (f->check == nullptr)
      return true;
    return *(f->check);
  }
  return false;
}

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
int hay_flags_get_idx(flag_t *flags, char *flag) {
  flag_t *f = hay_flags_get(flags, flag);
  if (f != nullptr && f->idx != nullptr) {
    return *(f->idx);
  }
  return -1;
}
