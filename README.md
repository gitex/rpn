# Reverse Polish Notation

**Reverse Polish Notation (RPN)** is math notation in which operators follow their operands.

Examples:
```
1 2 +          // 1 + 2
2 3 *          // 2 * 3
1 2 + 3 4 + *  // (1 + 2) * (3 + 4)
```

## Build

You can build with `gcc` and `make` as `make` or just use any other way you prefer.

## Usage

Rules:
- Every token divided by at least one space
- Division by zero is allowed (returns 0)

## Todo

- [x] Errors handling (stack not empty, invalid expressions, divide by zero)
- [x] Interpreter
- [] File as source of expression
- [] Support negative numbers
- [] More unary operations (neg, sqrt)
