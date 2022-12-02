#include <ruff/core/random.hpp>
#include <ruff/security/sha256.hpp>
#include <ruff/security/diffie_hellman.hpp>
#include <ruff/security/dsa.hpp>
#include <iostream>

int main()
{
    using namespace ruff::security;
    using namespace ruff::core;

    size_t session_id = Rand(0, 1000000);

    std::string p = "50702342087986984684596540672785294493370824085308498450535565701730450879745310594069460940052367603038103747343106687981163754506284021184158903198888031001641800021787453760919626851704381009545624331468658731255109995186698602388616345118779571212089090418972317301933821327897539692633740906524461904910061687459642285855052275274576089050579224477511686171168825003847462222895619169935317974865296291598100558751976216418469984937110507061979400971905781410388336458908816885758419125375047408388601985300884500733923194700051030733653434466714943605845143519933901592158295809020513235827728686129856549511535000228593790299010401739984240789015389649972633253273119008010971111107028536093543116304613269438082468960788836139999390141570158208410234733780007345264440946888072018632119778442194822690635460883177965078378404035306423001560546174260935441728479454887884057082481520089810271912227350884752023760663";
    std::string q = "63762351364972653564641699529205510489263266834182771617563631363277932854227";

    // Large primes wont load in properly
    p = "127";
    q = "9";
    std::string g = "2";

    DiffieHellman alice_dh(p, g);
    DSA alice_dsa(p, q);
    std::string id_a = "A";

    DiffieHellman bob_dh(p, g);
    DSA bob_dsa(p, q);
    std::string id_b = "B";

    // ------------ Step 1 ------------
    // Alice sends g^x mod p to bob
    auto alice_gx = alice_dh.Send();

    // ------------ Step 2 ------------
    // Bob recieves g^x mod p
    auto bob_gy = bob_dh.Send(); // Compute g^y mod p
    bob_dh.Recieve(alice_gx); // Bob uses X to compute X^y mod p to determine the key


    std::string key_b = SHA256(bob_dh.Key().get_str());
    std::string k1_b = key_b.substr(0, 128);
    std::string k2_b = key_b.substr(128, 256);
    auto tag_b = SHA256(k1_b + std::to_string(session_id) + id_b); // Bob creates tag k_1||T||ID_b

    std::string sig_msg_b = std::to_string(session_id) + alice_gx.get_str() + bob_gy.get_str();
    auto sig_b = bob_dsa.Sign(sig_msg_b); // Bob signs the message of T||g^x||g^y

    // send over T, bob_gy, ID_b, tag_b, and sig_b to alice
    // ------------ Step 3 ------------
    alice_dh.Recieve(bob_gy); // Alice uses Y to compute Y^x mod p to determine key
    std::string key_a = SHA256(alice_dh.Key().get_str());
    std::string k1_a = key_a.substr(0, 128);
    std::string k2_a = key_a.substr(128, 256);

    auto tag_test_a = SHA256(k1_a + std::to_string(session_id) + id_b);
    if(tag_test_a != tag_b) throw "Failed verification of tags - Alice failed to confirm Bob";

    std::string sig_msg_a = std::to_string(session_id) + alice_gx.get_str() + bob_gy.get_str();
    auto bob_dsa_keys = bob_dsa.GetVerificationKey();
    bool valid_a = alice_dsa.Verify(sig_msg_a, sig_b, bob_dsa_keys); // Bob's verification key is public
    if(!valid_a) throw "Failed validation of signature - Alice failed to confirm Bob";

    auto sig_a = alice_dsa.Sign(sig_msg_a);
    auto tag_a = SHA256(k1_a + std::to_string(session_id) + id_a);

    // Send over T, ID_a, tag_a, and sig_a to Bob
    // ------------ Step 4 ------------
    auto tag_test_b = SHA256(k1_b + std::to_string(session_id) + id_a);
    if(tag_test_b != tag_a) throw "Failed verification of tags - Bob failed to confirm Alice";

    auto alice_dsa_keys = alice_dsa.GetVerificationKey();
    bool valid_b = bob_dsa.Verify(sig_msg_b, sig_a, alice_dsa_keys); // Alice's verification key is public
    if(!valid_b) throw "Failed validation of signature - Bob failed to confirm Alice";
    std::cout << "Succesfully transmitted and confirmed signatures and tags" << std::endl;
}