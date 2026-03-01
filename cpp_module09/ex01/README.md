# 🧮 C++ Module 09 - Exercise 01: Reverse Polish Notation (RPN)

## 📖 Introduction
Welcome to **RPN**! If you've ever looked at a mathematical equation and thought, *"Wow, parentheses and the order of operations (PEMDAS) are really annoying to code,"* you aren't alone. Early computer scientists felt the exact same way.

Their solution? **Reverse Polish Notation (Postfix Notation).**

This program takes a mathematical expression written in RPN, parses it strictly left-to-right, and evaluates it instantly using a single, highly efficient Data Structure: a **Stack**.

---

## 🤔 What exactly is RPN?
In standard math (Infix Notation), we put the operator *between* the numbers:
`1 + 1`

In **Reverse Polish Notation**, we put the operator *after* the numbers:
`1 1 +`

Because the operator always comes after its targets, **parentheses are completely unnecessary**. The order of the tokens mathematically guarantees the order of operations.

For example, standard math `(8 * 9) - 9` becomes `8 9 * 9 -` in RPN.

---

## 🧠 Our Strategy: The Stack Algorithm
To solve this, we need a Data Structure that remembers numbers as we read them, but always gives us the most *recent* numbers when we hit an operator.

The perfect tool for this is a **Stack (LIFO - Last-In, First-Out)**.

### The Golden Rules of our Algorithm:
We read the string token by token from left to right in **O(N) time complexity**.
1. **If the token is a number:** We `push` it onto the top of the stack.
2. **If the token is an operator (`+`, `-`, `*`, `/`):** * We `pop` the top number off the stack (this is the **Right Operand**).
   * We `pop` the *next* number off the stack (this is the **Left Operand**).
   * We do the math: `Left [operator] Right`.
   * We `push` the result back onto the top of the stack.
3. **When the string ends:** There should be exactly **one** number left in the stack. That is our final answer!

### 🕵️‍♂️ Step-by-Step Trace: `"7 7 * 7 -"`
Let's see the stack in action:
* Read `7` ➔ **PUSH**. *(Stack: `[7]`)*
* Read `7` ➔ **PUSH**. *(Stack: `[7, 7]`)*
* Read `*` ➔ **OPERATOR!**
  * Pop `7` (Right)
  * Pop `7` (Left)
  * Calculate `7 * 7 = 49`
  * **PUSH 49**. *(Stack: `[49]`)*
* Read `7` ➔ **PUSH**. *(Stack: `[49, 7]`)*
* Read `-` ➔ **OPERATOR!**
  * Pop `7` (Right)
  * Pop `49` (Left)
  * Calculate `49 - 7 = 42`
  * **PUSH 42**. *(Stack: `[42]`)*

End of string. The stack has exactly one item: **`42`**.

---

## 🚧 The "Forbidden Container" Trap
Module 09 has a tricky rule: *"The container(s) you used in the previous exercise are forbidden here."* If you used `std::map` or `std::vector` in `ex00` (Bitcoin Exchange), you cannot use them here. Because `ex02` (PmergeMe) will heavily require `std::vector` and `std::deque`, we need to protect them!

**Our Hack:** `std::stack` is actually a *Container Adaptor*. By default, it wraps a `std::deque`. To save `std::deque` for the next exercise, we explicitly forced our stack to wrap a `std::list` like this:
```cpp
std::stack<int, std::list<int> > _stack;
