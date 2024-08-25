hay_flags.h(3) - Command-line Flag Parsing Library
===============================================

## NAME

`hay/flags` - A library for creating, parsing, and handling command-line flags.

## SYNOPSIS

```c
flag_t *hay_flags_create(const char *name, const char short_name, flag_ty_t type);
int hay_flags_parse(flag_t **flags, int argc, char **argv);
bool hay_flags_getnull(flag_t *flag, const bool defval);
int hay_flags_getint(flag_t *flag, const int defval);
const char *hay_flags_getstr(flag_t *flag, const char *defval);
bool hay_flags_getbool(flag_t *flag, const bool defval);
```

## DESCRIPTION

The `hay_flags` library provides functions for managing command-line flags. It supports creating flags, parsing command-line arguments, and retrieving flag values.  
A **Hay Suite** project.

### hay_flags_create()

**Synopsis:**

```c
flag_t *hay_flags_create(const char *name, const char short_name, flag_ty_t type);
```

**Description:**

Creates a new flag with the specified name, short name, and type.

- `name`: The long name of the flag (e.g., "verbose"). This string is duplicated using `strdup`, so it should be a valid null-terminated string.
- `short_name`: The short name of the flag (e.g., 'v'). Use 0 if no short name is needed.
- `type`: The type of the flag (`FT_STR`, `FT_INT`, `FT_BOOL`). Determines how the flag value is stored and processed.

**Returns:**

A pointer to the newly created `flag_t` structure, or `NULL` if an error occurs. If an error occurs, `errno` is set to indicate the error.

**Errors:**

- `EINVAL`: Invalid argument provided.
- `ENOMEM`: Memory allocation failed.

### hay_flags_parse()

**Synopsis:**

```c
int hay_flags_parse(flag_t **flags, int argc, char **argv);
```

**Description:**

Parses command-line arguments and sets the corresponding flags.

- `flags`: Array of pointers to `flag_t` structures, terminated by `NULL`. Each flag in the array is checked against the arguments.
- `argc`: The number of command-line arguments.
- `argv`: The command-line argument vector.

**Returns:**

0 on success, or -1 if an error occurs. If an error occurs, `errno` is set to indicate the error.

**Errors:**

- `EINVAL`: Invalid arguments provided.

### hay_flags_getnull()

**Synopsis:**

```c
bool hay_flags_getnull(flag_t *flag, const bool defval);
```

**Description:**

Retrieves the value of a null flag or returns a default value.

- `flag`: Pointer to the `flag_t` structure to check.
- `defval`: Default value to return if the flag is not set or not a null type.

**Returns:**

`true` if the flag is a null type and is set; otherwise, returns `defval`.

### hay_flags_getint()

**Synopsis:**

```c
int hay_flags_getint(flag_t *flag, const int defval);
```

**Description:**

Retrieves the value of an integer flag or returns a default value.

- `flag`: Pointer to the `flag_t` structure to check.
- `defval`: Default value to return if the flag is not set or not an integer type.

**Returns:**

The integer value of the flag if set; otherwise, returns `defval`.

### hay_flags_getstr()

**Synopsis:**

```c
const char *hay_flags_getstr(flag_t *flag, const char *defval);
```

**Description:**

Retrieves the value of a string flag or returns a default value.

- `flag`: Pointer to the `flag_t` structure to check.
- `defval`: Default value to return if the flag is not set or not a string type.

**Returns:**

The string value of the flag if set; otherwise, returns `defval`.

### hay_flags_getbool()

**Synopsis:**

```c
bool hay_flags_getbool(flag_t *flag, const bool defval);
```

**Description:**

Retrieves the value of a boolean flag or returns a default value.

- `flag`: Pointer to the `flag_t` structure to check.
- `defval`: Default value to return if the flag is not set or not a boolean type.

**Returns:**

The boolean value of the flag if set; otherwise, returns `defval`.

## EXAMPLES

**Example 1: Creating and Parsing Flags**

```c
#include <hay/flags.h>

int main(int argc, char **argv) {
    flag_t *verbose = hay_flags_create("verbose", 'v', FT_BOOL);
    flag_t *count = hay_flags_create("count", 'c', FT_INT);
    flag_t *flags[] = {verbose, count, nullptr}; // You can use nullptr, even in C99, because hay_flags defines it

    if (hay_flags_parse(flags, argc, argv) != 0) {
        perror("hay_flags_parse");
        return EXIT_FAILURE;
    }

    if (hay_flags_getbool(flags[0], false)) {
        printf("Verbose mode enabled\n");
    }

    int count = hay_flags_getint(flags[1], 0);
    printf("Count: %d\n", count);

    hay_flags_destroy(flags[0]);
    hay_flags_destroy(flags[1]);

    return EXIT_SUCCESS;
}
```

## SEE ALSO
malloc(3), strdup(3), free(3)

## AUTHOR
Written by The Hay Project. Contributions and feedback can be directed to <nobody@rajdeepm.xyz>.

## COPYRIGHT
This manpage is released under the **Mozilla Public License, version 2.0** License.
