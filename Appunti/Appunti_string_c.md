Di seguito sono illustrate 15 funzioni della libreria `<string.h>` in C, ciascuna accompagnata da un esempio di codice e dall'output prodotto.

### 1. `strlen`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, World!";
    size_t len = strlen(str);
    printf("Length of '%s' is %zu\n", str, len);
    return 0;
}
```
**Output**:
```
Length of 'Hello, World!' is 13
```

### 2. `strcpy`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char src[] = "Hello";
    char dest[10];
    strcpy(dest, src);
    printf("Source: %s\nDestination: %s\n", src, dest);
    return 0;
}
```
**Output**:
```
Source: Hello
Destination: Hello
```

### 3. `strncpy`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char src[] = "Hello";
    char dest[10];
    strncpy(dest, src, 3);
    dest[3] = '\0'; // Ensure null termination
    printf("Source: %s\nDestination: %s\n", src, dest);
    return 0;
}
```
**Output**:
```
Source: Hello
Destination: Hel
```

### 4. `strcat`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char dest[20] = "Hello, ";
    char src[] = "World!";
    strcat(dest, src);
    printf("Result: %s\n", dest);
    return 0;
}
```
**Output**:
```
Result: Hello, World!
```

### 5. `strncat`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char dest[20] = "Hello, ";
    char src[] = "World!";
    strncat(dest, src, 3);
    printf("Result: %s\n", dest);
    return 0;
}
```
**Output**:
```
Result: Hello, Wor
```

### 6. `strcmp`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "Hello";
    char str2[] = "World";
    int result = strcmp(str1, str2);
    printf("Comparison result: %d\n", result);
    return 0;
}
```
**Output**:
```
Comparison result: -15
```
(Note: The output value may vary based on the lexicographical difference between the strings.)

### 7. `strncmp`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "Hello";
    char str2[] = "Helium";
    int result = strncmp(str1, str2, 3);
    printf("Comparison result: %d\n", result);
    return 0;
}
```
**Output**:
```
Comparison result: 0
```

### 8. `strchr`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, World!";
    char *pos = strchr(str, 'W');
    if (pos) {
        printf("First occurrence of 'W' is at position: %ld\n", pos - str);
    }
    return 0;
}
```
**Output**:
```
First occurrence of 'W' is at position: 7
```

### 9. `strrchr`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, World!";
    char *pos = strrchr(str, 'o');
    if (pos) {
        printf("Last occurrence of 'o' is at position: %ld\n", pos - str);
    }
    return 0;
}
```
**Output**:
```
Last occurrence of 'o' is at position: 8
```

### 10. `strstr`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, World!";
    char *pos = strstr(str, "World");
    if (pos) {
        printf("Substring 'World' found at position: %ld\n", pos - str);
    }
    return 0;
}
```
**Output**:
```
Substring 'World' found at position: 7
```

### 11. `strtok`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, World!";
    char *token = strtok(str, " ,!");
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = strtok(NULL, " ,!");
    }
    return 0;
}
```
**Output**:
```
Token: Hello
Token: World
```

### 12. `strspn`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "abcde312$#@";
    char str2[] = "abcde";
    size_t len = strspn(str1, str2);
    printf("Initial segment length containing only characters from '%s': %zu\n", str2, len);
    return 0;
}
```
**Output**:
```
Initial segment length containing only characters from 'abcde': 5
```

### 13. `strcspn`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "abcde312$#@";
    char str2[] = "312";
    size_t len = strcspn(str1, str2);
    printf("Initial segment length containing no characters from '%s': %zu\n", str2, len);
    return 0;
}
```
**Output**:
```
Initial segment length containing no characters from '312': 5
```

### 14. `strpbrk`

```c
#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "abcde312$#@";
    char str2[] = "123";
    char *pos = strpbrk(str1, str2);
    if (pos) {
        printf("First occurrence of any character in '%s' is at position: %ld\n", str2, pos - str1);
    }
    return 0;
}
```
**Output**:
```
First occurrence of any character in '123' is at position: 5
```

### 15. `strdup`

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char str[] = "Hello, World!";
    char *dup = strdup(str);
    if (dup) {
        printf("Original: %s\nDuplicate: %s\n", str, dup);
        free(dup);
    }
    return 0;
}
```
**Output**:
```
Original: Hello, World!
Duplicate: Hello, World!
```

Questi esempi coprono le funzioni principali per la manipolazione delle stringhe in C utilizzando `<string.h>`. Ognuna di queste funzioni è essenziale per diverse operazioni che coinvolgono stringhe, come la ricerca, la copia, la concatenazione e il confronto.