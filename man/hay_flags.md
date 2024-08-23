hay_flags.h(3) -- a simple command-line argument parser
=======================================================

## SYNOPSIS
`flag_t *hay_flags_parse(flag_t *flags, int argc, char **argv);`  
`flag_t *hay_flags_get(flag_t *flags, char *flag);`  
`char *hay_flags_get_val(flag_t *flags, char *flag);`  
`bool hay_flags_get_check(flag_t *flags, char *flag);`  
`int hay_flags_get_idx(flag_t *flags, char *flag);`

## DESCRIPTION
`hay_flags` is a command-line argument parser that's made to be simple-enough.  
A **Hay Suite** project.

## TYPES
### flag_t
This structure holds the crucial info:
* **`flag`**:
  A `char` pointer (string) representing a flag.
* **`val`**:
  A `char*` pointer (pointer to string) representing the value. This is where the value will be stored.
* **`check`**:
  A `bool` pointer (pointer to integer).  
  Before C99, it'll be a typedef to `int`. Post-C99, it'll be `#include`d from `stddef.h`.
  Sometimes, you might wanna just check for some option (without value). In such a case,
  you can just set it to the memory where you wanna check it, and set `val` to `nullptr`.
* **`idx`**:
  An `int` pointer (pointer to integer). This is where the index (of `argv`) of the specific found flag is stored.  

```c
typedef struct flag {
  char *flag; /**< The flag string (e.g., "-p" or "--port"). */
  char *
      *val;   /**< Pointer to a variable where the flag's value will be stored.
                 This pointer may be nullptr if the flag does not require a value. */
  bool *check; /**< Pointer to an integer used as a check bit. If not nullptr, it
                 will be set to true (1) if the flag is matched. */
  int *idx;   /**< Pointer to an integer where the index of the matched argument
                 is stored. */
} flag_t;
```

### flags_t (deprecated)
This is just an alias of `flag_t*` (a pointer to `flag_t` structures).

## PARAMETERS
The following parameters are required by the `hay_flags_parse()` function. For others, see [FUNCTIONS](#FUNCTIONS).
* **`flags`**: 
  A pointer to an array of **`flag_t`** structures (see [flags_t(3)](#flags_t-deprecated) (**deprecated**)). Each **`flag_t`** (see [flag_t(3)](#flag_t)) structure contains a flag string and a pointer to a location where the flag's value should be stored. If this parameter is nullptr, the function expects that the caller has provided a valid array of **`flag_t`** structures that will be populated with the parsed results. The caller is responsible for freeing this allocated memory when it is no longer needed.

* **`argc`**:
  The number of command-line arguments in the `argv` array. This parameter indicates how many arguments are available for processing.

* **`argv`**:
  An array of command-line arguments. Each element in this array is a string representing a single argument passed to the program. The array should be terminated with a nullptr pointer.

## RETURN VALUE
The `hay_flags_parse` function returns a pointer to the `flags` array on success. If an error occurs during parsing (e.g., a required argument is missing or memory allocation fails), the function sets **`errno`** to an appropriate error code and returns nullptr.

## ERRORS
* **`EINVAL`**:
  This error code is set when an invalid argument is encountered, a required argument for a flag is missing, or if the `flags` parameter is nullptr. The function will print an appropriate error message to `stderr` and return nullptr.

* **`ENOMEM`**:
  This error code is set if memory allocation fails while storing flag values. The function will print an appropriate error message to `stderr` and return nullptr.

## FUNCTIONS
### hay_flags_get()
```c
flag_t *hay_flags_get(flag_t * flags, char *flag);
```
Retrieves the `flag_t` structure associated with a specific flag. This function searches for the specified flag in the `flags` array and returns the corresponding `flag_t` structure if found. If the flag is not found, the function returns nullptr.

### hay_flags_get_val()
```c
char *hay_flags_get_val(flag_t * flags, char *flag);
```
Retrieves the value associated with a specific flag. This function retrieves the value stored in the `val` field of the `flag_t` structure associated with the specified flag. If the flag does not have an associated value, or if the flag is not found, the function returns nullptr.

### hay_flags_get_check()
```c
bool hay_flags_get_check(flag_t * flags, char *flag);
```
Retrieves the check bit associated with a specific flag. This function retrieves the value of the check bit stored in the `check` field of the `flag_t` structure associated with the specified flag. If the flag does not have a check bit, or if the flag is not found, the function returns 0.

### hay_flags_get_idx()
```c
int hay_flags_get_idx(flag_t * flags, char *flag);
```
Retrieves the index of a specific flag. This function retrieves the index of the command-line argument that matches the specified flag. If the flag is not found, or if the index is not set, the function returns -1.

## EXAMPLES
The following example demonstrates how to use the **`hay_flags_parse`** function to parse command-line arguments and extract values for specific flags:

```c
#include <hay/flags.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    // Define variables to store flag values
    char *port = nullptr;
    char *dir = nullptr;
    
    // Define an array of flags with their corresponding storage locations
    flag_t my_flags[] = {
        {"-p", &port},
        {"--port", &port},
        {"-d", &dir},
        {"--dir", &dir}
    };

    // Parse the command-line arguments
    flag_t * parsed_flags = hay_flags_parse(my_flags, argc, argv);

    // Check if parsing was successful
    if (parsed_flags == nullptr) {
        // Handle error
        perror("Failed to parse flags");
        return EXIT_FAILURE;
    } else {
        // Successfully parsed
        printf("Port: %s\n", port);
        printf("Directory: %s\n", dir);
    }

    return EXIT_SUCCESS;
}
```

In this example:
1. `-p` and `--port` flags both set the `port` variable.
2. `-d` and `--dir` flags set the `dir` variable.
3. The program parses the command-line arguments to extract these values.

## SEE ALSO
assert(3), malloc(3), errno(3), free(3), fprintf(3)

## AUTHOR
Written by The Hay Project. Contributions and feedback can be directed to <nobody@rajdeepm.xyz>.

## COPYRIGHT
This manpage is released under the **Mozilla Public License, version 2.0** License.
