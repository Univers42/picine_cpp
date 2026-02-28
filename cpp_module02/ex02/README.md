# Module 02 - Exercise 02: "Now we’re talking"
### Building a Pure Software ALU for Fixed-Point Arithmetic

This project implements a fully functional 32-bit Fixed-Point number class in C++98. Instead of relying on the CPU's high-level operators (`+`, `-`, `*`, `/`, `<`, `>`), this implementation simulates a hardware **Arithmetic Logic Unit (ALU)** in software, using pure bitwise operations to perform all mathematical and relational logic.

---

## Core Architecture: ALU vs. FPU

To understand this project, you must understand the difference between how a computer handles whole numbers versus decimals.

* **The ALU (Arithmetic Logic Unit):** The part of the CPU that handles raw binary integers and bitwise logic gates (AND, OR, XOR). It is incredibly fast and efficient.
* **The FPU (Floating Point Unit):** A specialized coprocessor designed to handle complex decimal math (like `float` and `double` IEEE 754 standards).



**The Goal:** Fixed-point arithmetic allows us to represent decimal fractions using *only* integers. By manually placing a "binary point" (reserving the last 8 bits for the fraction), we can perform decimal math using only the high-speed ALU, bypassing the FPU entirely. This is crucial for embedded systems, old game consoles (like the PS1), or systems lacking an FPU.

---

## The Bitwise Algorithms

Because we refused to use the built-in C++ math operators inside our class methods, we had to implement universally recognized hardware algorithms using pure bitwise logic.

### 1. Addition & Subtraction (The Full Adder)
Addition is performed by simulating hardware logic gates.
* **Sum without carry** is calculated using XOR (`^`).
* **Carry bits** are calculated using AND (`&`) and shifted left (`<< 1`).
* **Subtraction** uses **Two's Complement**. Instead of `A - B`, we perform `A + (~B + 1)`, turning the number negative at the bit level before adding.

### 2. Multiplication (Shift-and-Add / Russian Peasant)
You cannot simply multiply two fixed-point numbers without shifting the decimal point.
We implemented the **Shift-and-Add** algorithm. We iterate through the bits of the multiplier; if a bit is `1`, we add the multiplicand to our product. We then shift the multiplicand left and the multiplier right.
Finally, we shift the result right by 8 bits (`>> 8`) to restore our fixed-point scale.



### 3. Division (Binary Long Division)
We implemented a software division algorithm that mirrors paper-and-pencil long division. We scale the dividend up by 8 bits (`<< 8`) to preserve fractional precision, then iteratively subtract shifted versions of the divisor, accumulating the quotient bit by bit.



---

## The "Epsilon" Limitation: Compounding Truncation Error

During our 1,000-iteration fuzzing stress test, we observed that our fixed-point math occasionally deviated from standard C++ `float` math by a small margin (e.g., yielding `57.4102` instead of `57.3403`).

**This is not a bug; it is a mathematical proof of fixed-point limitations.**

Our `Fixed` class uses **8 bits** for the fractional part.
* The smallest representable value ($\epsilon$) is $1/256 = 0.00390625$.
* Any decimal that is not a perfect power of 2 must be rounded to the nearest $1/256$.

### The Compounding Effect
If you take a float, $46.6502$, and store it in our class, it is forced to round to the nearest representable binary fraction (`46.6484375`). If you store $0.813568$, it rounds to `0.8125`.

When we divide or multiply these *pre-rounded* numbers:
$$46.6484375 \div 0.8125 = 57.41346...$$

The algorithm executed the math **perfectly**. The deviation from the pure float result occurs because the input data lost precision the moment it was stored in an 8-bit fractional format. When you multiply or divide these numbers, the initial rounding errors compound.

This is why we must adjust our test's acceptable `EPSILON` delta to $\approx 0.2f$ when doing heavy arithmetic on 8-bit fixed-point formats.

---

## How to Run

Compile the project using the provided `Makefile`:

```bash
make
./build/bin/ex02
