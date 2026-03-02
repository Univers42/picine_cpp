# ex01 — My Awesome PhoneBook

## Subject

Implement a simple phonebook that stores up to 8 contacts in memory (no files, no dynamic allocation). The program loops on three commands:

- `ADD` — prompt for contact fields and save it
- `SEARCH` — display contacts in a table, then let the user pick one by index
- `EXIT` — quit

Each contact stores: first name, last name, nickname, phone number, darkest secret. When the 9th contact is added it **overwrites the oldest** (circular buffer). Truncate fields longer than 10 characters with a trailing dot when displaying them.

---

## What Is the Challenge?

Several things compound here:

1. **No dynamic allocation** — the contact array is fixed at compile time
2. **Circular overwrite** — the 9th contact must silently replace index 0, the 10th replaces index 1, etc.
3. **Formatted table output** — columns exactly 10 characters wide, right-aligned index, truncated strings with `.`
4. **Input validation** — blank fields must be rejected; phone numbers must match real-world formats
5. **Clean OOP separation** — `Contact` knows nothing about I/O; `PhoneBook` owns the business logic

---

## Concepts Learned

| Concept | Where used |
|---|---|
| Classes, constructors, access control | `Contact`, `PhoneBook` |
| `enum` for field indexing | `Contact::Field` |
| Circular buffer (ring buffer) | `size` / `count` / `cap` in `PhoneBook` |
| `std::iomanip` (`setw`, `setfill`) | Table rendering in `search()` |
| Input validation with a state machine | `isValidPhoneNumber()` |
| `static` local (Meyers singleton) | `Contact::emptyString()` |
| Seeding / fixture data | `PhoneBook::seed()` |
| ANSI escape codes for color | `C_CYAN`, `C_RESET`, etc. |

---

## The `Contact` Class — Field Enum as an Index

Instead of separate `std::string` member variables (`name`, `lastName`, …), the design uses an **enum-indexed array**:

```cpp
// Contact.hpp
enum Field {
  FIELD_NAME = 0,
  FIELD_LASTNAME,
  FIELD_NICKNAME,
  FIELD_PHONE,
  FIELD_SECRET,
  FIELD_COUNT   // ← sentinel: always equals the number of fields
};

std::string fields[FIELD_COUNT]; // array of exactly 5 strings
```

**Why?** The enum value doubles as an array index, so `getField` and `setField` are trivially simple:

```cpp
void Contact::setField(Field f, const std::string& v) {
  int idx = static_cast<int>(f);
  if (idx >= 0 && idx < static_cast<int>(FIELD_COUNT))
    this->fields[idx] = v;
}
```

Adding a new field requires only one change: add a new enum value before `FIELD_COUNT`. The array size, bounds checks, and loop ranges all update automatically. This is the **data-driven design** pattern — structure drives behavior.

### The `emptyString()` static — safe reference return

```cpp
const std::string& Contact::getField(Field f) const {
  int idx = static_cast<int>(f);
  if (idx >= 0 && idx < static_cast<int>(FIELD_COUNT))
    return this->fields[idx];
  return Contact::emptyString(); // safe fallback — never a dangling reference
}

// Implementation:
const std::string& Contact::emptyString() {
  static std::string s; // initialized once, lives for the program lifetime
  return s;
}
```

Returning `""` directly would be returning a reference to a **temporary** — undefined behavior. The `static` local is initialized exactly once on first call (thread-safe in C++11, and simply correct in C++98 single-threaded programs) and persists forever. This is the **Meyers Singleton** idiom applied to a single shared empty string.

---

## The Circular Buffer — How Overwrite Works

`PhoneBook` tracks three integers:

```
cap   = MAX_CONTACT (8) — maximum slots, never changes
count = number of valid contacts (0..8, saturates at 8)
size  = write cursor (0..7, wraps around)
```

```
Initial state: count=0, size=0
After 1st add: count=1, size=1
After 8th add: count=8, size=0   ← size wrapped back to 0
After 9th add: count=8, size=1   ← count stays 8, size keeps cycling
```

The add logic:

```cpp
// Write at current cursor position (overwrites if already occupied):
this->contact[this->size].setField(...);

// Advance the cursor, wrapping around at cap:
this->size = (this->size + 1) % this->cap;

// Increment count only up to the cap:
if (this->count < this->cap) this->count++;
```

The modulo `% cap` is the entire ring-buffer mechanism. `count` tells how many slots are valid; `size` tells where the *next* write goes. When `count == cap`, the phonebook is full and every new `add` silently overwrites the oldest entry (the one at `size` before it's incremented).

---

## The Table — Is It Dynamic?

### What the table does

```
┌──────────┬──────────┬──────────┬──────────┐
│  index   │first name│ last name│ nickname │
├──────────┼──────────┼──────────┼──────────┤
│         0│Alice     │Smith     │Al        │
│         1│Maximilia…│Von-Trapp │Maxi      │
└──────────┴──────────┴──────────┴──────────┘
```

### How the column width is enforced

Two helper functions cooperate:

```cpp
// Truncate to 9 chars + "." if too long, else return as-is
static std::string formatField(const std::string& s) {
  return (s.size() > 10 ? s.substr(0, 9) + "." : s);
}

// Pad with spaces to exactly `width` characters, centered
static std::string centerField(const std::string& s, int width = 10) {
  if (s.length() >= (size_t)width) return s.substr(0, width);
  int pad_left  = (width - s.length()) / 2;
  int pad_right = width - s.length() - pad_left;
  return std::string(pad_left, ' ') + s + std::string(pad_right, ' ');
}
```

`centerField` is used for **header cells** (centered text). `formatField` + `std::setw(10)` with `std::left` is used for **data cells** (left-aligned, truncated).

### Is the table static or dynamic?

**Static in structure, dynamic in content.**

- The number of columns (4) and their widths (10 each) are **hardcoded** in the `#define` box strings and the `setw(10)` calls. Adding a fifth column would require changing the `BOX_TOP/MID/BOT` macros and adding another `V_BAR << setw(10) << ...` segment in `search()`.
- The number of **rows** is dynamic — driven by `this->count` at runtime. The loop iterates over however many contacts exist.

To make the table truly dynamic (variable columns, variable widths), you would need a `vector<vector<string>>` data structure and a two-pass algorithm: first pass computes the max width per column, second pass renders. The current design is deliberately simple and fits the constraints of C++98 with no dynamic allocation.

### The rendering algorithm (step by step)

```
1. Print BOX_TOP line
2. Print header row:
   │ centerField("index") │ centerField("first name") │ ...
3. Print BOX_MID separator
4. For each contact i in 0..count-1:
   │ right-aligned index │ formatField(name) │ formatField(last) │ formatField(nick) │
5. Print BOX_BOT line
6. Enter input loop: read index, validate, display full contact detail
```

`std::setw(10)` combined with `std::right` / `std::left` lets `iomanip` do the padding arithmetic. `formatField` ensures no cell ever exceeds 10 characters (the truncation + dot rule from the subject).

---

## Phone Number Validation

```cpp
static bool isValidPhoneNumber(const std::string& phone) {
  // 1. Strip allowed separators (space, dash, dot) from a copy
  std::string cleaned;
  for (size_t i = 0; i < phone.length(); ++i) {
    if (std::isdigit(phone[i]) || (i == 0 && phone[i] == '+'))
      cleaned += phone[i];
    else if (phone[i] != ' ' && phone[i] != '-' && phone[i] != '.')
      return false; // unexpected character → reject immediately
  }

  // 2. Must not be empty or just "+"
  if (cleaned.empty() || cleaned == "+") return false;

  // 3. Match known national formats:
  bool isFR = (cleaned.length() == 10 && cleaned[0] == '0')        // 06 12 34 56 78
           || (cleaned.length() == 12 && cleaned.substr(0,3)=="+33"); // +33...

  bool isES = (cleaned.length() == 9 && cleaned[0] >= '6')          // 612 345 678
           || (cleaned.length() == 12 && cleaned.substr(0,3)=="+34"); // +34...

  // 4. E.164 fallback: 9–15 digits (covers +1, +44, etc.)
  size_t digit_count = 0;
  for (size_t i = 0; i < cleaned.length(); i++)
    if (std::isdigit(cleaned[i])) digit_count++;
  bool isE164 = (digit_count >= 9 && digit_count <= 15);

  return isFR || isES || isE164;
}
```

The algorithm is a **clean-then-classify** pipeline: normalize the input by stripping separators, then test it against known structural patterns. The `seed()` data was chosen specifically to exercise all branches: French local, French international, Spanish local, Spanish international, North American (+1), and dotted format.

---

## The `seed()` Function — Fixture Data

```cpp
const char* db[][5] = {
  {"Alice",      "Smith",      "Al",    "06 12 34 56 78",    "French local"},
  {"Maximilian", "Von-Trapp",  "Maxi",  "+33-6-11-22-33-44", "French Intl"},
  {"O'Connor",   "John-Paul",  "JP",    "612 345 678",        "Spanish local"},
  {"..........", "----------", "____",  "+34 699 888 777",    "Spanish Intl"},
  {"Short",      "Cut",        "S",     "+1 (800) 123-4567",  "Universal"},
  {"1234567890", "0987654321", "Nums",  "01.23.45.67.89",     "Dotted"}
};
```

`seed()` is a **developer convenience command** (type `seed` at the prompt) that fills the phonebook with pre-verified contacts so you can immediately test `search` without manually typing 8 contacts. It also demonstrates the table's truncation rules: `"Maximilian"` is exactly 10 chars (no truncation), `"Von-Trapp"` is 9 chars, `"1234567890"` is exactly 10 chars.

---

## Key Takeaways

1. **Enum-indexed arrays** — `Field` enum + `fields[FIELD_COUNT]` creates a self-sizing, bounds-checked field table
2. **Circular buffer** — `size = (size + 1) % cap` is the entire overwrite mechanism; `count` saturates at `cap`
3. **Static local as safe sentinel** — `emptyString()` returns a reference to a forever-valid empty string, avoiding dangling references
4. **Table rendering** — `formatField` + `std::setw` + `std::left`/`std::right` handles all column alignment; the table is structurally static but row-count dynamic
5. **Clean-then-classify validation** — strip separators first, then test structural patterns; keeps validation logic readable
6. **OOP separation of concerns** — `Contact` is a pure data container; `PhoneBook` owns all I/O and business rules
