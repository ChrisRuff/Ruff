#include <ruff/security/rsa.hpp>
#include <iostream>

using namespace ruff::security;
int main(int argc, char* argv[])
{
    // ignore compiler errors of unused vars
    (void)argc;
    (void)argv;

	std::string p_in = "19211916981990472618936322908621863986876987146317321175477459636156953561475008733870517275438245830106443145241548501528064000686696553079813968930084003413592173929258239545538559059522893001415540383237712787805857248668921475503029012210091798624401493551321836739170290569343885146402734119714622761918874473987849224658821203492683692059569546468953937059529709368583742816455260753650612502430591087268113652659115398868234585603351162620007030560547611";
	std::string q_in = "49400957163547757452528775346560420645353827504469813702447095057241998403355821905395551250978714023163401985077729384422721713135644084394023796644398582673187943364713315617271802772949577464712104737208148338528834981720321532125957782517699692081175107563795482281654333294693930543491780359799856300841301804870312412567636723373557700882499622073341225199446003974972311496703259471182056856143760293363135470539860065760306974196552067736902898897585691";
	RSA rsa(p_in, q_in);

    auto [e, n] = rsa.GetPublicKey();
    auto [d, p, q] = rsa.GetPrivateKey();

    std::cout << "The first prime is p=" << p << std::endl;
    std::cout << "The second prime is q=" << q << std::endl;
    std::cout << "The composite modulus n=" << n << std::endl;
    std::cout << "The encryption exponent is e=" << e << std::endl;
    std::cout << "The decryption exponent is d=" << d << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Please enter an option:" << std::endl;
    std::cout << "1 to Encrypt" << std::endl;
    std::cout << "2 to Decrypt" << std::endl;
    std::cout << "Your option:";
    int option;
    std::cin >> option;
    std::cout << "-------------------------------" << std::endl;

    if(option == 1)
    {
        std::string msg_start, msg;
        std::cout << "Encryption:" << std::endl;
        std::cout << "Plaintext (randomly generate) to be encrypted is m=";
        std::cin >> msg_start;
        std::getline(std::cin, msg);
        std::cout << "Ciphertext is c=" << rsa.Encrypt(msg_start + msg);
    }
    else
    {
        std::string ciphertext;
        std::cout << "Decryption:" << std::endl;
        std::cout << "Ciphertext to be decrypted is c=";
        std::cin >> ciphertext;
        std::cout << "Plaintext is m=" << rsa.Decrypt(ciphertext);
    }
}
