#include <bits/stdc++.h>
#include <gmp.h>
#include <openssl/aes.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>

using namespace std;

#include "../crypto_funcs/hmac.cpp"
#include "../crypto_funcs/sha1.cpp"
#include "../crypto_funcs/simple_hash.cpp"
#include "../random_num.cpp"
void read_all();
mpz_t c, k1, k2, s, T, rho, n, g, e, v, gs, iv_temp;

int main() {
    mpz_t f, hashed, hashed_val, tempT;
    mpz_inits(c, k1, k2, s, T, rho, n, g, gs, e, v, iv_temp, f, hashed,
              hashed_val, tempT, NULL);
    int j, temp, in, te, success = 0;
    char *V, *I, *w;
    read_all();
    // gmp_printf("c-%Zd\nk1-%Zd\nk2-%Zd\ns-%Zd\nT-%Zd\nrho-%Zd\nn-%Zd\ng-%Zd\ngs-%Zd\ne-%Zd\nv-%Zd\n",c,k1,k2,s,T,rho,n,g,gs,e,v);

    in = mpz_get_ui(c) + 1;
    mpz_t W[in], i[in], a[in];
    mpz_set_ui(f, 25);
    mpz_set_ui(tempT, 1);
    // mpz_powm(T,T,e,n);
    gmp_printf("T -> %Zd\n", T);

    fstream file;
    string word, tempi, filename = "../outputs/PRP_out.txt";
    file.open(filename.c_str());
    temp = 1;
    j = 1;
    while (file >> word) {
        if (temp == 6) {
            mpz_inits(i[j], a[j], NULL);
            mpz_set_str(i[j], word.c_str(), 10);
            temp = 1;
            // PRF(a[j],k2,j);
            // mpz_mod(a[j],a[j],f);

            V = mpz_get_str(NULL, 10, v);  // compute W[] for each block
            I = mpz_get_str(NULL, 10, i[j]);
            w = (char *)malloc(1 + strlen(V) + strlen(I));
            strcpy(w, V);
            strcat(w, I);  // concatenate
            // gmp_printf("j - %d\t i - %Zd\t a - %Zd\n",j,i[j],a[j]);

            te = mpz_get_ui(i[j]);
            mpz_init(W[te]);
            // gmp_printf("i - %d\t%Zd\n",te,W[te]);
            mpz_set_str(W[te], w, 10);
            // gmp_printf("i - %d\t%Zd\n",te,W[te]);
            hash_func(W[te], hashed);
            mpz_mul(tempT, tempT, hashed);
            mpz_mod(tempT, tempT, n);
            // gmp_printf("tempT -> %Zd\n", tempT);
            // mpz_powm(hashed,hashed,a[j],n);

            // mpz_invert(hashed,hashed,n);
            // mpz_mul(T,T,hashed);
            // mpz_mod(T,T,n);
            // mpz_cdiv_q(T, T, hashed);
            // mpz_mod(T,T,n);
            // gmp_printf("T: %Zd\n", T);

            j++;
        } else
            temp++;
    }
    gmp_printf("tempT : %Zd\n", tempT);
    mpz_cdiv_q(T, T, tempT);
    // mpz_powm(T, T, s, n);
    HASH(T, hashed_val);
    gmp_printf("hashed val - %Zd\nrho - %Zd\n", hashed_val, rho);
    if (mpz_cmp(hashed_val, rho) == 0) {
        success = 1;
        cout<<"success - "<<success<<endl;
    }
    

    return 0;
}

void read_all() {
    fstream read_file;
    string filename, word, c_te, k1_te, k2_te, s_te, T_te, rho_te, n_te, g_te,
        e_te, v_te, gs_te, iv_te;
    // read keys
    filename = "../outputs/key_gen_out.txt";
    read_file.open(filename.c_str());
    int i;
    i = 1;
    while (read_file >> word) {
        if (i == 3)
            n_te = word;
        else if (i == 6)
            e_te = word;
        else if (i == 12)
            v_te = word;
        else if (i == 15)
            iv_te = word;
        i++;
    }
    read_file.close();
    // read challenge
    filename = "../outputs/gen_chal_out.txt";
    read_file.open(filename.c_str());
    i = 1;
    while (read_file >> word) {
        if (i == 3)
            k1_te = word;
        else if (i == 6)
            k2_te = word;
        else if (i == 9)
            s_te = word;
        else if (i == 12)
            c_te = word;
        else if (i == 15)
            g_te = word;
        else if (i == 18)
            gs_te = word;
        i++;
    }
    read_file.close();
    // read proof
    filename = "../outputs/gen_proof_out.txt";
    read_file.open(filename.c_str());
    i = 1;
    while (read_file >> word) {
        if (i == 3)
            T_te = word;
        else if (i == 6)
            rho_te = word;
        i++;
    }
    read_file.close();

    T_te =
        "8934155058287707256361196536613787976255467636989419148696453383282490"
        "5221348601835949360385054533283296056228038226345388732258435110512430"
        "5878295844741015696927306064482280569112920141335937627321946727978109"
        "7943547232625027211201769963153321304022638698826710972989089672929195"
        "8471552765693477616561531187";
    T_te =
        "1799191442700329434486684315503550239646271765172864033922563088127218"
        "6473385195892233963004069796791707674144296380628306915963267647373184"
        "9143922787679539649998676704226934823167669067846510261274795655862539"
        "2720403150441665639736374639368799782580170291907148962395768799497177"
        "4029679139211494408785377998772253916481565014435147057516984131503270"
        "4688620768934475239195146531585518666177390690418491225987910677991134"
        "2067695584949126694173790219467796803630404361697273699603532648524917"
        "5256588670100423088856001769431654661221202532161087818099514307512164"
        "74968623980048852004942435238950837904517642989464045036";
    rho_te = "12902254831351111884612012315227189631001002061446679";
    mpz_set_str(s, s_te.c_str(), 10);
    mpz_set_str(c, c_te.c_str(), 10);
    mpz_set_str(k1, k1_te.c_str(), 10);
    mpz_set_str(k2, k2_te.c_str(), 10);
    mpz_set_str(gs, gs_te.c_str(), 10);
    mpz_set_str(g, g_te.c_str(), 10);
    mpz_set_str(v, v_te.c_str(), 10);
    mpz_set_str(iv_temp, iv_te.c_str(), 10);
    mpz_set_str(e, e_te.c_str(), 10);
    mpz_set_str(n, n_te.c_str(), 10);
    mpz_set_str(T, T_te.c_str(), 10);
    mpz_set_str(rho, rho_te.c_str(), 10);
    // gmp_printf("T - %Zd\nrho - %Zd\n",T,rho);
}
