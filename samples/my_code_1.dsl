/* This program is just a copy of sample_1.dsl. You should write your FCAL program to multiply two
2-dimensional matrices and save it
in a file with this name. */

main () {
  matrix ma = matrix_read ("./samples/my_code_1a.data") ;
  matrix mb = matrix_read ("./samples/my_code_1b.data") ;
  matrix mr = ma * mb;
  print ( mr );
}
