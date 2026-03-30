# Wizard Language Keywords (Final 64)

## Program Structure
1. ENTER_HOGWARTS        → main() {
2. EXIT_HOGWARTS         → }

3. HOUSE                 → {
4. ENDHOUSE              → }

## House Roles (Semantic Use)
5. Gryffindor            → Declaration Block
6. Slytherin             → Function Block
7. Hufflepuff            → Condition Block
8. Ravenclaw             → Loop Block

---

## Data Types
9. INT                   → int
10. FLOAT                → float
11. DOUBLE               → double
12. LONG                 → long
13. CHAR                 → char
14. BOOL                 → bool
15. VOID                 → void

---

## Declaration & Assignment
16. DECLARE              → type variable =
17. AS                   → (type binding keyword)
18. =                    → =
19. :=                   → initialization assignment

---

## Control Flow
20. IF                   → if
21. FI                   → }
22. ELSE                 → else
23. CHECK                → if (condition)
24. THEN                 → {
25. ENDCHECK             → }

---

## Loops (Ravenclaw)
26. LOOP                 → while
27. DO                   → {
28. ENDLOOP              → }
29. BREAK                → break
30. CONTINUE             → continue

---

## Functions (Slytherin)
31. SPELL                → function definition
32. ENDSPELL             → }
33. SUMMON               → function call
34. WITH                 → argument passing
35. RETURN               → return

---

## Input / Output
36. CAST                 → printf()
37. PROPHECY             → printf (formatted output)
38. INPUT                → scanf()

---

## Scope Management
39. POTION               → {
40. ENDPOTION            → }

---

## Semantic Validation
41. SORTING_HAT          → semantic_check()
42. ENDHAT               → end semantic block
43. CHECK_TYPES          → type checking
44. CHECK_DECLARATIONS   → symbol table validation

---

## Arithmetic Operators
45. +                    → +
46. -                    → -
47. *                    → *
48. /                    → /
49. %                    → %

---

## Logical Operators
50. AND                  → &&
51. OR                   → ||
52. NOT                  → !
53. XOR                  → ^

---

## Math Functions
54. POWO                 → pow()
55. RADIX                → sqrt()
56. FLOORUS              → floor()
57. CEILUS               → ceil()
58. ABSOLUTUS            → abs()
59. LOGUS                → log()
60. SINUS                → sin()
61. COSINUS              → cos()
62. TANUS                → tan()

---

## Tokens
63. IDENTIFIER           → [a-zA-Z_][a-zA-Z0-9_]*
64. NUMBER               → [0-9]+