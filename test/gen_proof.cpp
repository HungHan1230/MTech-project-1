#include<iostream>
#include<gmp.h>
#include<time.h>
#include<openssl/evp.h>
#include<openssl/hmac.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;

#include "../random_num.cpp"
#include "../crypto_funcs/sha1.cpp"
#include "../crypto_funcs/hmac.cpp"
#include "../crypto_funcs/simple_hash.cpp"


mpz_t n,c,k1,k2,gs,b[25],tag[25],iv_temp;
void read_all();
void read_tag_block();

int main(){
  mpz_t T,ex,rho,hashed_val,f,tempz,v,hashed;
  mpz_inits(T,ex,rho,hashed_val,c,k1,k2,n,gs,f,iv_temp,tempz,v,hashed,NULL);
  char *V,*I,*w;
  read_tag_block();
  read_all();

  int j=0,jt,itr,te,in=mpz_get_ui(c)+1,temp, beforelong;
  mpz_t W[in],i[in],a[in];
  mpz_set_ui(f,25);
  mpz_set_ui(T,1);
  fstream file;
  string word,tempi,filename="../outputs/PRP_out.txt";
  file.open(filename.c_str());
  temp=1;
  j=1;
  while (file >> word) {

    if(temp==6){
      mpz_inits(i[j],a[j],NULL);
      mpz_set_str (i[j],word.c_str(),10);
      temp=1;
      PRF(a[j],k2,j);
      mpz_mod(a[j],a[j],f);      
      // beforelong = mpz_get_ui(a[j]);
      V=mpz_get_str (NULL,10,v);          // compute W[] for each block
      I=mpz_get_str (NULL,10,i[j]);
      w=(char *) malloc(1+strlen(V)+strlen(I));
      strcpy(w,V);
      strcat(w,I);      //concatenate
      
      te=mpz_get_ui(i[j]);
      mpz_init(W[te]);
      mpz_set_str(W[te],w,10);

      hash_func(W[te],hashed);
      mpz_powm(hashed,hashed,a[j],n);
      mpz_mul(T,T,hashed);
      mpz_mod(T,T,n);
      // gmp_printf("j - %d\t i - %Zd\t a - %Zd\n",j,i[j],a[j]);
      // gmp_printf("te - %d\ttag - %Zd\n",te,tag[te]);
      // mpz_pow_ui(tempz,tag[te],(unsigned long)beforelong);

      // mpz_powm(tempz,tag[te],a[j],n);
      // mpz_mul(T,T,tempz);
      // mpz_mod(T,T,n);

      mpz_mul(tempz,b[te],a[j]);
      mpz_add(ex,ex,tempz);
      mpz_mod(ex,ex,n);

      j++;
    }
    else
      temp++;
  }

  mpz_powm(tempz,gs,ex,n);
  HASH(tempz,hashed_val);
  mpz_set(rho,hashed_val);

   // (T,rho) is the proof generated
  gmp_printf("T - %Zd\nrho - %Zd\n",T,rho);

  return 0;
}


void read_all(){
  fstream key_file,key2_file;
  string word,filename,n_keygen,k1_gen,k2_gen,c_gen,gs_gen,iv_temp_gen;
  // read values from keygen and file_blocks
  filename = "../outputs/key_gen_out.txt";
  key_file.open(filename.c_str());
  int i=1;
  while (key_file >> word)
  {
    // cout<<word<<endl;
      if(i%15==0){
        iv_temp_gen=word;
      }
      else if(i%3==0 && i%6!=0 && i%9!=0 && i%12!=0 )
        n_keygen=word;
      i++;
  }
  filename = "../outputs/gen_chal_out.txt";
  key2_file.open(filename.c_str());
  i=1;
  while (key2_file >> word)
  {
      // cout<<word<<endl;
      if(i%18==0)
        gs_gen=word;
      else if(i%12==0)
        c_gen=word;
      else if(i%6==0)
        k2_gen=word;
      else if(i%3==0 && i%15!=0 && i%9!=0)
        k1_gen=word;
      i++;
  }
  mpz_set_str (iv_temp,iv_temp_gen.c_str(),10);
  mpz_set_str (n,n_keygen.c_str(),10);
  mpz_set_str (gs,gs_gen.c_str(),10);
  mpz_set_str (k1,k1_gen.c_str(),10);
  mpz_set_str (k2,k2_gen.c_str(),10);
  mpz_set_str (c,c_gen.c_str(),10);
  // gmp_printf("\nn - %Zd\nk1 - %Zd\nk2 - %Zd\nc - %Zd\ngs - %Zd\n",n,k1,k2,c,gs);
}

void read_tag_block(){
  fstream key_file,key2_file;
  string word,filename,tagtemp,blocktemp;
  // read values from keygen and file_blocks
  filename = "../outputs/tag_block_out.txt";
    // gmp_printf("%Zd\n",iv_temp);
  key_file.open(filename.c_str());
  int it=0;
  while (!key_file.eof())
  {
      mpz_inits(b[it],tag[it],NULL);
      key_file >> word;
      key_file >> blocktemp;
      mpz_set_str(b[it],blocktemp.c_str(),10);
      key_file >> tagtemp;
      mpz_set_str(tag[it],tagtemp.c_str(),10);
      it++;
  }

}
