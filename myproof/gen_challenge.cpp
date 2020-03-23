//  generate challenge to be sent to the storage provider
//  chal = (c,k1,k2,gs)
//  c => no of blocks
//  s => random no
//  k1 => random no.
//  k2 => random no.
//  gs => g^s
//  g => generator , take g and n from keygen
//  k => security parameter

#include <iostream>
#include <gmp.h>
#include <time.h>
#include <fstream>
// #include "all_functions.h"
using namespace std;
// #include "../random_num.cpp"
mpz_t n_key;
void read_n_key();
int gen_random_no_ch(int out,int bit_cnt,mpz_t max_no,mpz_t rand_no,gmp_randstate_t r_state1);

void genChallenge()
{
    mpz_t c, k1, k2, s, gs, g, r_max, k;
    mpz_inits(c, k1, k2, s, g, gs, n_key, r_max, k, NULL);
    read_n_key();

    unsigned long int seed;
    gmp_randstate_t r_state;
    seed = time(NULL);
    gmp_randinit_default(r_state);
    gmp_randseed_ui(r_state, seed);

    mpz_set_ui(r_max, 1000);                // 0 <= c < f(total no of file blocks)
    gen_random_no_ch(2, 0, r_max, c, r_state); // generate c

    mpz_set_ui(r_max, 0);
    gen_random_no_ch(1, 16, r_max, k1, r_state); //  generate k1
    gen_random_no_ch(1, 20, r_max, k2, r_state); // generate k2

    gen_random_no_ch(1, 16, r_max, s, r_state);
    gen_random_no_ch(1, 20, r_max, g, r_state);
    // gen_random_no_ch(1,20,r_max,n);
    mpz_powm(gs, g, s, n_key); // gs = g^s mod n

    gmp_printf("k1 => %Zd\nk2 => %Zd\n s => %Zd\n c => %Zd\n g => %Zd\n gs => %Zd\n", k1, k2, s, c, g, gs);
    ofstream out("gen_chal_out.txt");
    char *tmp_k1 = mpz_get_str(NULL, 10, k1);
    char *tmp_k2 = mpz_get_str(NULL, 10, k2);
    char *tmp_s = mpz_get_str(NULL, 10, s);
    char *tmp_c = mpz_get_str(NULL, 10, c);
    char *tmp_g = mpz_get_str(NULL, 10, g);
    char *tmp_gs = mpz_get_str(NULL, 10, gs);
    out << "k1 => " << tmp_k1 << endl;
    out << "k2 => " << tmp_k2 << endl;
    out << "s => " << tmp_s << endl;
    out << "c => " << tmp_c << endl;
    out << "g => " << tmp_g << endl;
    out << "gs => " << tmp_gs << endl;
    out.close();
}

void read_n_key()
{

    fstream key_file;
    string word, filename, n_keygen;
    // read values from keygen and file_blocks
    filename = "./key_gen_out.txt";
    key_file.open(filename.c_str());
    int i = 1;
    while (key_file >> word)
    {
        if (i % 3 == 0 && i % 6 != 0 && i % 9 != 0 && i % 12 != 0 && i % 15 != 0)
            n_keygen = word;
        i++;
    }
    mpz_set_str(n_key, n_keygen.c_str(), 10);
}

// to do ---
/*
  change bitcnt for all random nos.
  g and n are taken from keygen algorithms
  for c to generate , r_max should be f
  remove n,g from initializations
*/
int gen_random_no_ch(int out,int bit_cnt,mpz_t max_no,mpz_t rand_no,gmp_randstate_t r_state1)
{

  mpz_inits(rand_no,NULL);

  // unsigned long int seed;
  // gmp_randstate_t r_state;
  // seed = time(NULL);
  // gmp_randinit_default (r_state);
  // gmp_randseed_ui(r_state, seed);

  if(out==1)
    mpz_urandomb (rand_no,r_state1,bit_cnt);
  // 1. void mpz_urandomb (mpz t rop , gmp randstate t state , mp bitcnt t n )
  // Generate a uniformly distributed random integer in the range 0 to 2 n − 1, inclusive.

  if(out==2)
    mpz_urandomm (rand_no,r_state1,max_no);
  // 2. void mpz_urandomm (mpz t rop , gmp randstate t state , const mpz t n )
  // Generate a uniform random integer in the range 0 to n − 1, inclusive.

  if(out==3)
    mpz_rrandomb (rand_no ,r_state1,bit_cnt);
  // 3. void mpz_rrandomb (mpz t rop , gmp randstate t state , mp bitcnt t n )
  // Generate a random integer with long strings of zeros and ones in the binary representation.
  // Useful for testing functions and algorithms, since this kind of random numbers have proven
  // to be more likely to trigger corner-case bugs. The random number will be in the range 0 to
  // 2 n − 1, inclusive.

  return 0;
}