#include <cryptopp/base64.h>

#include <string>

#include "../../inc/crypt++.h"
using namespace CryptoPP;

std::string b64_encode_string(std::string in) {
    SecByteBlock out(reinterpret_cast<const byte*>(&in[0]), in.size());
    return b64_encode(out);
}

std::string b64_encode(SecByteBlock ibytes) {
    std::string encoded;
    StringSource ss(ibytes, ibytes.size(), true,
        new Base64Encoder(
            new StringSink(encoded)
        )
    );
    return encoded;
}

std::string b64_decode(std::string encoded) {
    std::string decoded;
   
    StringSource ss(encoded, true,
        new Base64Decoder(
            new StringSink(decoded)
        )
    );
    return decoded;
}