# ❄️ Cryo — The Amorphous Programming Language
### Zero Runtime • Full Hyperfold • Fully Static Execution

Cryo is an experimental programming language built on top of the **Amorphous Engine**, a compiler that evaluates *the entire program at compile time*.  

Cryo performs **full-program constant folding**, reducing all expressions into constants and generating a minimal C file containing only a `printf()` statement.

There is **no runtime execution** in Cryo.  
All computation happens during compilation.

---

# 🚀 Features

### • Fully static language  
Cryo does not execute code at runtime. Every expression is evaluated while compiling.

### • RPN (Reverse Polish Notation) expression system  
Simple stack-based expressions like:

```
.x 10 20 + ..
```

### • Auto-typed variables  
Types are chosen by prefix:
- `.` → integer  
- `%` → float (long double)  
- `&` → scientific (printed with `%e`)  
- `"` → character  

### • Automatic `printf` generation  
Cryo produces a C program that directly prints the final constant results.

### • Zero-runtime paradigm  
Once Cryo collapses all expressions, the output program often looks like:

```c
printf("%d", 2600);
```

This makes Cryo effectively one of the **fastest possible languages**, since nothing is computed at runtime.

---

# 🧠 How Cryo Works

1. **Parse each variable definition**
2. **Evaluate the RPN expression with a stack**
3. **Store the result as a constant**
4. **When `out` is encountered**, Cryo:
   - picks correct format specifiers (`%d`, `%f`, `%e`, `%c`)
   - inserts the constant values
   - outputs a `printf()` call as the entire program

All variables are constants.  
All computation is done during compile time.

---

# 📦 Example

Cryo code (`program.c25`):

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

Output:
```
2600
```

Runtime cost: **almost zero**.

---

# 🔧 Building Cryo

### Windows
```
gcc src/amorphous.c -o cryo.exe
cryo.exe program.c25 out.c
gcc out.c -o out.exe
out.exe
```

### Linux / macOS
```
gcc src/amorphous.c -o cryo
./cryo program.c25 out.c
gcc out.c -o out
./out
```

Cryo has **no dependencies** except a C compiler (GCC or Clang).

---

# 📜 Language Syntax

Detailed syntax is in [`syntax.md`](syntax.md), but here’s a quick summary:

### Define variables
```
.x 10 20 + ..
%pi 3.1415 ..
&sci 1e+10 ..
"h H ..
```

### Use variables in expressions
```
.sum .x %pi + ..
```

### Output values
```
out .sum "h ..
```

Cryo automatically generates:
```
printf("%d%c", 123, 'h');
```

---

# ❄️ Why “Amorphous”?

Cryo destroys traditional runtime execution.  
Your entire program **morphs** into a constant during compilation.  
Everything collapses into a single print statement.

Cryo doesn’t *run* your program.  
Cryo *solves* your program.

This creates a new conceptual model:

### ▶ Amorphous Programming  
A fully static paradigm where programs shrink into their simplest possible form.

---

# 🔥 Status: MVP  
Cryo is currently minimal and experimental:
- no runtime variables  
- no branching  
- no loops  
- no dynamic input  
- no error reporting  

But the **Amorphous Engine already works**, and the core idea is proven.

---

# 📚 Docs

- [`cryo.md`](cryo.md) — Full explanation  
- [`syntax.md`](syntax.md) — Language syntax  

---

# 🧊 License

Released under the MIT License.

---

Cryo is an experiment in “zero-runtime programming.”  
It is simple, extreme, and intentionally limited — but it demonstrates a powerful idea:

> If computation can be done at compile time,  
> then runtime should not exist.

# Future
Cryo update coming out tomorrow (4/12/2025)

