#include "../../../Marlin/src/core/fp_type.h"

DeclareTest("FP1.15.16", []() -> bool {
  typedef fixp<true, 16, 16> FP1616;
  // Construction
  FP1616 one(1);
  if ((double)one != 1.0) return False;
  FP1616 two(2.0);
  if ((double)two != 2.0) return False;
  FP1616 three(3.0f);
  if ((int)three != 3)    return False;
  FP1616 half(0.5);
  if ((float)half != 0.5f) return False;
  FP1616 mfour(-4);
  if ((float)mfour != -4.0f) return False;

  // Arithmetic operators
  FP1616 tmp = one * two;
  if ((int)tmp != 2) return False;
  tmp = half + half;
  if ((int)tmp != 1) return False;
  tmp = one - half;
  if ((float)tmp != 0.5f) return False;
  tmp = two / half;
  if ((int)tmp != 4) return False;
  tmp = one / two;
  if ((float)tmp != 0.5f) return False;
  tmp *= two;
  if ((int)tmp != 1) return False;
  tmp = three % two;
  if ((int)tmp != 1) return False;

  // Bitwise operators
  tmp = one | two;
  if (tmp != three) return False;
  tmp = three & one;
  if ((int)tmp != 1) return False;
  tmp = one ^ one;
  if ((float)tmp != 0) return False;

  return true;
});

DeclareTest("FP0.15.16", []() -> bool {
  typedef fixp<false, 16, 16> FP1616;
  // Construction
  FP1616 one(1);
  if ((double)one != 1.0) return False;
  FP1616 two(2.0);
  if ((double)two != 2.0) return False;
  FP1616 three(3.0f);
  if ((int)three != 3)    return False;
  FP1616 half(0.5);
  if ((float)half != 0.5f) return False;

  // Arithmetic operators
  FP1616 tmp = one * two;
  if ((int)tmp != 2) return False;
  tmp = half + half;
  if ((int)tmp != 1) return False;
  tmp = one - half;
  if ((float)tmp != 0.5f) return False;
  tmp = two / half;
  if ((int)tmp != 4) return False;
  tmp = one / two;
  if ((float)tmp != 0.5f) return False;
  tmp *= two;
  if ((int)tmp != 1) return False;
  tmp = three % two;
  if ((int)tmp != 1) return False;

  // Bitwise operators
  tmp = one | two;
  if (tmp != three) return False;
  tmp = three & one;
  if ((int)tmp != 1) return False;
  tmp = one ^ one;
  if ((float)tmp != 0) return False;

  return true;
});

DeclareTest("FP1.31.32", []() -> bool {
  typedef fixp<true, 32, 32> FP3232;
  // Construction
  FP3232 one(1);
  if ((double)one != 1.0) return False;
  FP3232 two(2.0);
  if ((double)two != 2.0) return False;
  FP3232 three(3.0f);
  if ((int)three != 3)    return False;
  FP3232 half(0.5);
  if ((float)half != 0.5f) return False;
  FP3232 mfour(-4);
  if ((float)mfour != -4.0f) return False;

  // Arithmetic operators
  FP3232 tmp = one * two;
  if ((int)tmp != 2) return False;
  tmp = half + half;
  if (tmp != one) return False;
  tmp = one - half;
  if ((float)tmp != 0.5f) return False;
  tmp = two / half;
  if ((int)tmp != 4) return False;
  tmp = one / two;
  if ((float)tmp != 0.5f) return False;
  tmp *= two;
  if ((int)tmp != 1) return False;
  tmp = three % two;
  if ((int)tmp != 1) return False;

  // Bitwise operators
  tmp = one | two;
  if (tmp != three) return False;
  tmp = three & one;
  if ((int)tmp != 1) return False;
  tmp = one ^ one;
  if ((float)tmp != 0) return False;

  return true;
});

DeclareTest("FP0.31.32", []() -> bool {
  typedef fixp<false, 32, 32> FP3232;
  // Construction
  FP3232 one(1);
  if ((double)one != 1.0) return False;
  FP3232 two(2.0);
  if ((double)two != 2.0) return False;
  FP3232 three(3.0f);
  if ((int)three != 3)    return False;
  FP3232 half(0.5);
  if ((float)half != 0.5f) return False;

  // Arithmetic operators
  FP3232 tmp = one * two;
  if ((int)tmp != 2) return False;
  tmp = half + half;
  if ((int)tmp != 1) return False;
  tmp = one - half;
  if ((float)tmp != 0.5f) return False;
  tmp = two / half;
  if ((int)tmp != 4) return False;
  tmp = one / two;
  if ((float)tmp != 0.5f) return False;
  tmp *= two;
  if ((int)tmp != 1) return False;
  tmp = three % two;
  if ((int)tmp != 1) return False;

  // Bitwise operators
  tmp = one | two;
  if (tmp != three) return False;
  tmp = three & one;
  if ((int)tmp != 1) return False;
  tmp = one ^ one;
  if ((float)tmp != 0) return False;

  return true;
});

DeclareTest("Celsius_t", []() -> bool {
  typedef fixp<true, 10, 6> C_t;
  // Construction
  C_t one(1);
  if ((double)one != 1.0) return False;
  C_t two(2.0);
  if ((double)two != 2.0) return False;
  C_t three(3.0f);
  if ((int)three != 3)    return False;
  C_t half(0.5);
  if ((float)half != 0.5f) return False;

  // Arithmetic operators
  C_t tmp = one * two;
  if ((int)tmp != 2) return False;
  tmp = half + half;
  if ((int)tmp != 1) return False;
  tmp = one - half;
  if ((float)tmp != 0.5f) return False;
  tmp = two / half;
  if ((int)tmp != 4) return False;
  tmp = one / two;
  if ((float)tmp != 0.5f) return False;
  tmp *= two;
  if ((int)tmp != 1) return False;
  tmp = three % two;
  if ((int)tmp != 1) return False;

  // Bitwise operators
  tmp = one | two;
  if (tmp != three) return False;
  tmp = three & one;
  if ((int)tmp != 1) return False;
  tmp = one ^ one;
  if ((float)tmp != 0) return False;

  return true;
});

DeclareTest("Celsius_t fixp benchmark", []() -> bool {
  typedef fixp<true, 10, 6> C_t;
  // Multiplication
  {
    Test::ScopeProfile profile("Multiply");
    C_t final = C_t(0);
    for (int j = 0; j < 200; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 512; i++) {
        C_t r(Test::randomBetween(-1.0, 1.0)), g(Test::randomBetween(0, 1.0));
        result += r * g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  // Addition
  {
    Test::ScopeProfile profile("Add");
    C_t final = C_t(0);
    for (int j = 0; j < 400; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 256; i++) {
        C_t r(Test::randomBetween(-1.0, 1.0)), g(Test::randomBetween(0, 1.0));
        result += r + g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  // Subtraction
  {
    Test::ScopeProfile profile("Subtract");
    C_t final = C_t(0);
    for (int j = 0; j < 400; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 256; i++) {
        C_t r(Test::randomBetween(-1.0, 1.0)), g(Test::randomBetween(0, 1.0));
        result += r - g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  // Division
  {
    Test::ScopeProfile profile("Divide");
    C_t final = C_t(0);
    for (int j = 0; j < 200; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 512; i++) {
        C_t r(Test::randomBetween(0, 1.0)), g(Test::randomBetween(1.0, 32.0));
        result += r / g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  return true;
});

DeclareTest("Floating point benchmark", []() -> bool {
  typedef float C_t;
  // Multiplication
  {
    Test::ScopeProfile profile("Multiply");
    C_t final = C_t(0);
    for (int j = 0; j < 200; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 512; i++) {
        C_t r(Test::randomBetween(-1.0, 1.0)), g(Test::randomBetween(0, 1.0));
        result += r * g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  // Addition
  {
    Test::ScopeProfile profile("Add");
    C_t final = C_t(0);
    for (int j = 0; j < 400; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 256; i++) {
        C_t r(Test::randomBetween(-1.0, 1.0)), g(Test::randomBetween(0, 1.0));
        result += r + g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  // Subtraction
  {
    Test::ScopeProfile profile("Subtract");
    C_t final = C_t(0);
    for (int j = 0; j < 400; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 256; i++) {
        C_t r(Test::randomBetween(-1.0, 1.0)), g(Test::randomBetween(0, 1.0));
        result += r - g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  // Division
  {
    Test::ScopeProfile profile("Divide");
    C_t final = C_t(0);
    for (int j = 0; j < 200; j++) {
      C_t result = C_t(1);
      for (int i = 0; i < 512; i++) {
        C_t r(Test::randomBetween(0, 1.0)), g(Test::randomBetween(1.0, 32.0));
        result += r / g;
      }

      if (result > 128) final++;
      else if (result < -128) final--;
    }

    fprintf(stdout, "  Final result: %g\n", (float)final);
  }

  return true;
});
