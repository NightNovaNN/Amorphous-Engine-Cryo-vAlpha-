# Cryo Syntax Guide

### Based on the Current Amorphous Engine Implementation

This document describes the _exact_ syntax and behavior of the current Cryo compiler as implemented
in the Amorphous Engine.

Cryo is fully static — all expressions are evaluated at compile time.

---

# 1. Variable Definitions

Cryo supports four variable types using prefix symbols:

### Integer

```
.a 123 ..
```

### Float (long double)

```
%f 12.34 ..
```

### Scientific Notation (long double, printed with %e)

```
&s 1.23e+10 ..
```

### Character

```
"h H ..
```

(In Cryo, character variables use their literal character value directly, not `'H'`.)

---

# 2. Variable Names

Variable names may contain:

- letters
- digits
- underscores

Examples:

```
.x
.value
result2
total_sum
```

---

# 3. Expressions (RPN)

Cryo uses Reverse Polish Notation (stack-based).  
Each expression must end with:

```
..
```

### Basic Example

```
.x 10 20 + ..
```

### Multiple Operators

```
.y 5 6 + 7 * ..
```

### Supported Operators

```
+
-
*
/
```

---

# 4. Variable References

Variables can be referenced inside expressions:

```
.z .x .y + ..
```

Cryo resolves variables immediately using their stored constant values.

---

# 5. Character Variables

Cryo defines characters using the `"` prefix:

```
"h H ..
"e e ..
```

Characters are referenced directly:

```
out "h "e "l "l "o ..
```

No quotes or ASCII codes needed.

---

# 6. Output (`out` or `print`)

The Cryo `out` instruction prints values.  
Cryo automatically:

- Generates the correct C `printf()`
- Chooses all format specifiers based on variable types
- Inserts constants directly

You **do not** write format strings yourself.

### Correct Cryo Output Syntax

```
out <vars> ..
```

### Example

```
out .x .y "h ..
```

Cryo will generate something like:

```c
printf("%d%d%c", 60, 20, 'h');
```

### Format specifiers chosen automatically:

- integer → `%d`
- float → `%f`
- scientific → `%e`
- character → `%c`

You never type these yourself.

---

# 7. Complete Example

Cryo program:

```cryo
.a 123 456 + ..
.b 10 5 * ..
.final .a .b + ..
out .final ..
```

Generated C:

```c
#include <stdio.h>
int main() {
    printf("%d", 2600);
    return 0;
}
```

---

Cryo syntax is simple and fully deterministic.  
Every program collapses into constants at compile time.  
There is no runtime computation.
