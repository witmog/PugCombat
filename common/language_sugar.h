#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define global   static
#define internal static
#define persist  static

#define SIGNUM(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
#define MIN(a, b) ((a > b) ? b : a)
#define MAX(a, b) ((a > b) ? a : b)
#define CLAMP(n, min, max) ((val > max) ? max : ((val < min) ? min : val))
