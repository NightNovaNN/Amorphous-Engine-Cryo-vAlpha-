# Cryo — The Amorphous Programming Language

### Fully Static • Zero Runtime • Hyperfold Execution

Cryo is a pure compile-time language powered by the **Amorphous Engine**, a compiler that evaluates
the entire program during compilation. Cryo performs no runtime computation at all.

Cryo programs consist only of:

- variable definitions
- RPN expressions
- a final `out` statement

The engine resolves every expression using a deterministic stack evaluator (hyperfold). Once all
values are computed, the compiler emits a minimal C program containing only the final `printf()`.

Cryo has no loops, no branching, no runtime input, and no dynamic execution. Everything is resolved
into constants.

---

## How Cryo Works

### 1. Variable Definition

Each variable is defined with a prefix type:

- `.` integer
- `%` float (long double)
- `&` scientific notation
- `"` character

Example:

```cryo
&a 123 456 + ..
&b 10 5 * ..
```

Each definition is evaluated **immediately** using the `hyperfold()` RPN evaluator.

---

### 2. Hyperfold Evaluation

Cryo’s core is the `hyperfold()` function.

Hyperfold performs:

- RPN stack evaluation
- number parsing
- variable lookup
- operator execution

All expressions are folded into a **single long double constant**.

Example:

```cryo
&a 100 200 + ..
```

Hyperfold returns `300`.

Cryo stores this in the variable table.

---

### 3. Output Generation

Cryo recognizes `out` or `print`:

```cryo
out Result=&final ..
```

Cryo:

- builds the `printf` format string
- selects type specifiers (`%d`, `%f`, `%e`, `%c`)
- inserts constant values directly

---

### 4. C Code Output

Cryo emits:

```c
#include <stdio.h>
int main() {
    printf("...", ...);
    return 0;
}
```

This is the entire program.

Cryo does **not** generate:

- variables
- arithmetic
- loops
- runtime logic

Everything has already been folded.

---

## Example

Cryo:

```cryo
&a 123 456 + ..
&b 10 5 * ..
&final &a &b + ..
out Final=&final ..
```

Generated C:

```c
#include <stdio.h>
int main() {
    printf("Final=%d", 2600);
    return 0;
}
```

Runtime execution cost is near zero.

---

## Summary

Cryo is a fully static, zero-runtime language.  
Every variable and expression is resolved during compilation.  
The only thing executed at runtime is a single `printf`.

Cryo demonstrates a programming style where the program becomes a single compile-time constant.
