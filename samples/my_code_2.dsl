/* Program that computes a number raised to specified power */

main () { 
  float base;
  base = 5;
  int exponent;
  exponent = 3;
  float result;
  result = 1;

  while(exponent != 0)
  {
  	result = result*base;
  	exponent = exponent - 1;
  }

  print("The answer is: ");
  print(result);
}

