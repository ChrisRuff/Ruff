#include <ruff/security/rsa.hpp>
#include <ruff/security/symmetric.hpp>
#include <iostream>

using namespace ruff::security;
int main()
{
    // Alice generates RSA keys
    std::string p_in = "19211916981990472618936322908621863986876987146317321175477459636156953561475008733870517275438245830106443145241548501528064000686696553079813968930084003413592173929258239545538559059522893001415540383237712787805857248668921475503029012210091798624401493551321836739170290569343885146402734119714622761918874473987849224658821203492683692059569546468953937059529709368583742816455260753650612502430591087268113652659115398868234585603351162620007030560547611";
    std::string q_in = "49400957163547757452528775346560420645353827504469813702447095057241998403355821905395551250978714023163401985077729384422721713135644084394023796644398582673187943364713315617271802772949577464712104737208148338528834981720321532125957782517699692081175107563795482281654333294693930543491780359799856300841301804870312412567636723373557700882499622073341225199446003974972311496703259471182056856143760293363135470539860065760306974196552067736902898897585691";
    RSA rsa(p_in, q_in);
    auto [e, n] = rsa.GetPublicKey();


    // Bob generates the 1MB message
    std::string msg;
    for(int i = 0; i < 1048576; ++i)
    {
        msg.push_back('A' + (rand() % 26)); // generate random char
    }
    // Bob generates aes key
    AES bob_aes;
    const auto bob_aes_key = bob_aes.GetKey();
    const auto bob_aes_iv = bob_aes.GetIV();

    // Bob encrypts key using rsa and Alices public key [e,n]
    std::string bob_aes_k = ""; // convert key to string
    for(uint8_t c : bob_aes_key) { bob_aes_k.push_back(c); }
    std::string encrypted_key = RSA::Encrypt(bob_aes_k, e, n);

    // Bob encrypts his message
    std::string ciphertext = bob_aes.Encrypt(msg);

    // Send ciphertext and encrypted key to alice
    // ------------------------------------------
    // ------------------------------------------
    auto key = rsa.Decrypt(encrypted_key); // Alice uses her private key to decrypt the message
    std::array<uint8_t, 32> key_array;
    for(size_t i = 0; i < key.size(); ++i) key_array[i] = key[i];
    AES alice_aes(key_array, bob_aes_iv); // Use the public initial vector from bob, and decrypted key
    std::string recovered_msg = alice_aes.Decrypt(ciphertext);

    printf("Original Key %s\n", bob_aes_k.c_str());
    printf("Recovered Key %s\n", key.c_str());
    printf("Was the decryption successful: %s\n", msg == recovered_msg ? "Success" : "Failed");


}
