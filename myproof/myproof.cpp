#include "./ch_proof.cpp"
#include "./file_blocks.cpp"
#include "./gen_challenge.cpp"
#include "./gen_proof.cpp"
#include "./keygen.cpp"
#include "./tag_block.cpp"

int main() {
    // cout << "keygen:" << endl;
    // keygen();
    // cout << "\n" << endl;
    // cout << "file_blocks:"<< endl;
    // file_blocks();
    // cout << "\n" << endl;
    // cout << "tag_block:"<< endl;
    // tag_block();
    // cout << "\n" << endl;
    // cout << "genChallenge:"<< endl;
    // genChallenge();
    // cout << "\n" << endl;
    cout << "gen_proof:" << endl;
    gen_proof();
    cout << "\n" << endl;
    cout << "ch_proof:" << endl;
    ch_proof();
    cout << "\n" << endl;
}
