#ifndef LZSS_H
#define LZSS_H

class LZSS
{
public:
    static void decompress(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputSize);
};

#endif // LZSS_H
