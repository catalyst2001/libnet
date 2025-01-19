#include <stdio.h>
#include <stdint.h>

/* utils */
template<class T>
consteval T pow_ct(T base, T exp) {
  return (exp == 0) ? 1 : base * pow_ct(base, exp - 1);
}

int main()
{


  return 0;
}