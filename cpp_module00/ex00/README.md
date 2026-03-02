# ex00 — Megaphone

## Subject

Write a program that takes command-line arguments, uppercases every character, and prints the result. If no arguments are given, print a loud feedback noise instead.

```
$> ./megaphone "shhhhh... I think the students are asleep..."
SHHHHH... I THINK THE STUDENTS ARE ASLEEP...
$> ./megaphone
* LOUD AND UNBEARABLE FEEDBACK NOISE *
```

---

## What Is the Challenge?

The exercise sounds trivial — call `toupper` on every character — but the real challenge is _how efficiently and generically_ you do it. The naive approach (print one character at a time) hammers `std::cout` in a tight loop, which is slow because each call has function-dispatch overhead and may interact with the stream's internal locking. The implementation here uses a **production-grade buffered pipeline** that:

- Processes multiple arguments as a **single unified stream**
- **Trims** leading/trailing whitespace and **collapses** internal whitespace runs
- Applies a **pluggable character-transform function** (first-class function pointer)
- Writes output in **bulk chunks**, not character by character

---

## Concepts Learned

| Concept | Where used |
|---|---|
| Bitwise enum flags | `TfFlags` enum |
| Function pointers (`typedef`) | `char_fn` type alias |
| Buffered bulk I/O | `CHUNK_SIZE`, `buf[CHUNK_SIZE]`, `flush_buffer` |
| `static inline` helpers | `flush_buffer`, `buffered_transform_chunk` |
| `std::transform` | `other_solution` |
| Macro-gated debug paths | `HAVE_EXAM` toggle |
| C++98 strict compliance | `-std=c++98 -Wall -Wextra -Werror` |

---

## The Bit-Flag System (`TfFlags`)

```cpp
enum TfFlags {
  TF_NONE        = 0,
  TF_TRIM_LEAD   = 1u << 0,              // binary: 0001
  TF_TRIM_TRAIL  = 1u << 1,              // binary: 0010
  TF_TRIM        = TF_TRIM_LEAD | TF_TRIM_TRAIL, // 0011
  TF_COLLAPSE_WS = 1u << 2,              // binary: 0100
  TF_SKIP_CR     = 1u << 3,              // binary: 1000
};
```

### Why bitflags instead of booleans?

Separate `bool` parameters produce unreadable call sites:

```cpp
// ❌ Opaque — what does each true/false mean?
transform(s, fn, true, true, true, false);

// ✅ Self-documenting, composable
transform(s, fn, TF_TRIM | TF_COLLAPSE_WS);
```

Each flag is a **power of two**, so each occupies exactly one unique bit in an integer. You combine flags with `|` (bitwise OR) and test them with `&` (bitwise AND):

```cpp
// Set two flags at once:
unsigned int flags = TF_TRIM | TF_COLLAPSE_WS; // 0011 | 0100 = 0111

// Test one flag:
if (flags & TF_COLLAPSE_WS) { ... } // 0111 & 0100 = 0100 → truthy

// Test a compound flag:
if ((flags & TF_TRIM) == TF_TRIM) { ... } // both trim bits must be set
```

This is the **same pattern used everywhere in systems programming**: Linux `open()` flags (`O_RDONLY | O_CREAT | O_TRUNC`), POSIX `mmap` protection (`PROT_READ | PROT_WRITE`), OpenGL state bits. It's a foundational idiom worth internalizing.

---

## The Chunk Buffer System — Why It's Fast

### The problem with naïve character-by-character output

```cpp
// ❌ Each call to operator<<(char) goes through the stream machinery
for (size_t i = 0; s[i]; i++)
    std::cout << (char)toupper(s[i]);
```

Even though `std::cout` is internally buffered, calling `operator<<(char)` in a loop has overhead on every iteration: virtual dispatch, format checks, and internal state updates. For large inputs this adds up.

### The solution: a stack-allocated chunk buffer + bulk flush

```cpp
#define CHUNK_SIZE 8192          // 8 KB — fits in L1 cache

char buf[CHUNK_SIZE];            // lives on the stack, zero heap overhead
size_t buf_idx = 0;

static inline void flush_buffer(char* buf, size_t& buf_idx) {
  if (buf_idx > 0) {
    std::cout.write(buf, buf_idx); // ONE call writes up to 8192 chars
    buf_idx = 0;
  }
}
```

**Key insight:** `std::cout.write(ptr, n)` hands `n` bytes to the stream in a single call. Instead of 8192 `operator<<(char)` calls, you do **one `write` per 8 KB**. For 1 MB of input that's ~128 flush calls instead of ~1,000,000.

The buffer lives on the **stack** — no `new`, no `delete`, no heap fragmentation. At 8 KB it fits comfortably in L1 cache on any modern CPU, so reads and writes to it are essentially free.

```cpp
// Inside the transform loop — accumulate, flush only when the buffer is full:
buf[buf_idx++] = static_cast<char>(fn(ch));  // fn = std::toupper
if (buf_idx >= CHUNK_SIZE)
    flush_buffer(buf, buf_idx);              // flush 8 KB at once
// After the loop, drain what's left:
flush_buffer(buf, buf_idx);
```

### Whitespace state machine

The flags drive a mini state-machine inside `buffered_transform_chunk`:

```
has_started = false    → we haven't emitted a real character yet
pending_space = false  → a space is waiting to be emitted

On whitespace:
  if TF_COLLAPSE_WS → set pending_space (don't emit yet)
  else              → emit a space directly

On a real character:
  if pending_space  → emit ONE space first (collapse multiple into one)
  emit the character (transformed by fn)
  has_started = true

TF_TRIM_LEAD: spaces before has_started=true are simply dropped
TF_TRIM_TRAIL: pending_space at end is never flushed
```

This means the transform processes the entire input stream — across all `argv` arguments — in a single linear pass with O(1) extra state.

---

## The `static` Keyword — Three Different Roles

The word `static` appears in three completely different contexts in this codebase:

### 1. `static inline` — internal linkage + inlining hint

```cpp
static inline void flush_buffer(char* buf, size_t& buf_idx) { ... }
static inline void buffered_transform_chunk(...) { ... }
```

`static` here means **internal linkage**: the function is invisible outside this `.cpp` file. This avoids name collisions if another translation unit defines a `flush_buffer`. `inline` asks the compiler to substitute the function body at the call site — no stack frame, no `call`/`ret` instructions — which matters inside tight character-processing loops.

### 2. Static local variable — initialized exactly once

```cpp
// Pattern seen in Contact.cpp (ex01), but the same principle:
const std::string& Contact::emptyString() {
  static std::string s;  // created once on first call, lives forever
  return s;
}
```

A `static` local is initialized the **first time** execution reaches its declaration, and then persists for the rest of the program's lifetime. This is the **Meyers Singleton** pattern: you get a lazily-initialized, guaranteed-single-instance object without any manual lifetime management.

### 3. Static class member — shared across all instances

```cpp
// Account.hpp (ex02):
static int _nbAccounts;  // one copy for the entire class, not per-object
```

Covered in detail in the ex02 README.

---

## Function Pointer — `char_fn`

```cpp
typedef int (*char_fn)(int);
```

This declares `char_fn` as a named type for "pointer to a function taking `int` and returning `int`" — exactly the signature of `std::toupper`, `std::tolower`, `std::isdigit`, etc.

```cpp
// Pass toupper as a first-class value:
buffered_transform_chunk(av[i], std::toupper, flags, ...);

// Inside the function, call it through the pointer:
buf[buf_idx++] = fn ? static_cast<char>(fn(ch)) : static_cast<char>(ch);
//                                      ^^
//                              called like a normal function
```

Passing `NULL` (or `nullptr` in C++11) as `fn` makes the transformer a **pass-through**: characters are buffered unchanged. This makes the entire pipeline reusable for any per-character transformation.

---

## Two Solutions, One Toggle

```cpp
// Simple, idiomatic reference:
int other_solution(int ac, char** av) {
  for (int j = 1; j < ac; ++j) {
    std::string str = av[j];
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    std::cout << str;
  }
  std::cout << "\n";
  return 0;
}

// Optimized: flags + chunk buffer + whitespace normalization:
int optimized_solution(int ac, char** av) {
  unsigned int user_flags = TF_TRIM | TF_COLLAPSE_WS;
  char buf[CHUNK_SIZE];
  // ...
}
```

The `HAVE_EXAM` macro switches between them at compile time:

```cpp
#ifndef HAVE_EXAM
# define HAVE_EXAM 1
#endif

int main(int ac, char** av) {
  if (!HAVE_EXAM)
    // debug: run both and compare
    return (optimized_solution(ac, av), other_solution(ac, av), 0);
  return (optimized_solution(ac, av), 0);
}
```

Pass `-DHAVE_EXAM=0` to the compiler to activate the comparison path and verify both outputs match.

---

## Key Takeaways

The megaphone exercise teaches that a "trivial" problem has multiple solutions at very different performance levels:

1. **Bitflags** — composable, readable, zero-overhead option encoding (power-of-2 enum values + `|` / `&`)
2. **Buffered bulk I/O** — accumulate into a stack buffer, flush in 8 KB chunks; reduces syscall overhead dramatically
3. **Function pointers** — pass behavior as a value, making the entire pipeline generic
4. **`static inline`** — internal linkage to avoid symbol collisions + inlining hint to eliminate call overhead in hot loops
5. **Compile-time toggles** — `#define`-gated paths for switching between exam mode and debug mode without changing source
