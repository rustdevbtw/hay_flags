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
 * - If `flags` is NULL, the function allocates memory for an array of `flag_t`
 *   structures using `malloc`.
 * - It then iterates over the command-line arguments to find and match flags.
 * - If a flag is found, it checks if the `check` bit of it is NULL. If it is,
 *   it then checks if it's corresponding argument is provided. If that's there,
 *   then the value will be stored in `val`, and it's index is stored in `idx`.
 * - If the `check` bit is not NULL, it'll then simply set that to 1 (true),
 *   and set `idx` to the index. It doesn't touch `val` in such a case.
 * - If the `flags` parameter is not NULL, it will be used to store the parsed
 *   results.
 * - On successful parsing, the function returns the `flags` array. If any error
 *   occurs (e.g., missing argument or memory allocation failure), the function
 * sets `errno` to `EINVAL` and returns NULL.
 *
 * @param flags Pointer to an array of `flag_t` structures. If NULL, the
 * function allocates memory for this array. If not NULL, it is assumed to be a
 * valid array of `flag_t` structures that will be filled with the parsed
 * results. The caller is responsible for freeing the allocated memory when it
 * is no longer needed.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return Returns the `flags` array on success. If `flags` was NULL, the
 * function allocates memory for it and returns the pointer. If parsing fails or
 * if any flag is missing its required argument, the function sets `errno` to
 *         `EINVAL` and returns NULL.
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
 * if (parsed_flags == NULL) {
 *     // Handle error
 *     perror("Failed to parse flags");
 * } else {
 *     // Successfully parsed
 *     printf("Port: %s\n", port);
 *     printf("Directory: %s\n", dir);
 * }
 */
flag_t *hay_flags_parse(flag_t *flags, int argc, char **argv) {
  if (argc == 0 || argv == NULL) {
    errno = EINVAL;
    return NULL;
  }
  if (flags == NULL) {
    fprintf(stderr, "hay_flags_parse: flags cannot be NULL\n");
    errno = EINVAL;
    return NULL;
  }

  for (int i = 0; i < argc; i++) {
    if (argv[i] == NULL) {
      fprintf(stderr, "hay_flags_parse: Null argument encountered\n");
      errno = EINVAL;
      return NULL;
    }

    flag_t *found_flag = malloc(sizeof(flag_t));
    found_flag = NULL;
    for (int j = 0; flags[j].flag != NULL; j++) {
      flag_t *flag = &flags[j];
      if (flag->flag == NULL)
        break;
      if (strcmp(flag->flag, argv[i]) == 0) {
        found_flag = flag;
        break;
      }
    }

    if (found_flag == NULL) {
      continue;
    }

    if (found_flag != NULL) {
      flag_t *current_flag = found_flag;

      if (current_flag->check != NULL) {
        *current_flag->check = 1;
        if (current_flag->idx == NULL) {
          current_flag->idx = malloc(sizeof(int));
          if (current_flag->idx == NULL) {
            perror("hay_flags_parse: malloc(current_flag->idx)");
            errno = ENOMEM;
            return NULL;
          }
        }
        *current_flag->idx = i;
      } else {
        // Move to the next argument which should be the value for the flag
        if (++i < argc) {
          if (current_flag->val == NULL) {
            current_flag->val = malloc(sizeof(char *));
            if (current_flag->val == NULL) {
              perror("hay_flags_parse: malloc(current_flag->val)");
              errno = ENOMEM;
              return NULL;
            }
          }

          // Copy the argument value into the appropriate location
          *current_flag->val = argv[i];
          if (current_flag->idx == NULL) {
            current_flag->idx = malloc(sizeof(int));
            if (current_flag->idx == NULL) {
              perror("hay_flags_parse: malloc(current_flag->idx)");
              errno = ENOMEM;
              return NULL;
            }
          }
          *current_flag->idx = i;
        } else {
          // Missing value for the flag
          fprintf(stderr, "hay_flags_parse: Missing argument for %s\n",
                  argv[i]);
          errno = EINVAL;
          return NULL;
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
 * found, the function returns NULL.
 *
 * @param flags Pointer to an array of `flag_t` structures.
 * @param flag The flag string to search for.
 * @return A pointer to the `flag_t` structure associated with the specified
 * flag, or NULL if the flag is not found.
 */
flag_t *hay_flags_get(flag_t *flags, char *flag) {
  if (flags == NULL || flag == NULL) {
    return NULL;
  }

  for (int i = 0; flags[i].flag != NULL; i++) {
    if (strcmp(flags[i].flag, flag) == 0) {
      return &flags[i];
    }
  }
  return NULL;
}

/**
 * @brief Retrieves the value associated with a specific flag.
 *
 * This function retrieves the value stored in the `val` field of the `flag_t`
 * structure associated with the specified flag. If the flag does not have an
 * associated value, or if the flag is not found, the function returns NULL.
 *
 * @param flags Pointer to an array of `flag_t` structures.
 * @param flag The flag string whose value is to be retrieved.
 * @return The value associated with the specified flag, or NULL if the flag
 * does not have an associated value or if the flag is not found.
 */
char *hay_flags_get_val(flag_t *flags, char *flag) {
  flag_t *f = hay_flags_get(flags, flag);
  if (f != NULL) {
    return *(f->val);
  }
  return NULL;
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
int hay_flags_get_check(flag_t *flags, char *flag) {
  flag_t *f = hay_flags_get(flags, flag);
  if (f != NULL && f->check != NULL) {
    return *(f->check);
  }
  return 0;
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
  if (f != NULL && f->idx != NULL) {
    return *(f->idx);
  }
  return -1;
}
