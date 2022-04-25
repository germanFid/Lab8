struct __attribute__((__packed__)) BYTE {
    unsigned short int bit1: 1;
    unsigned short int bit2: 1;
    unsigned short int bit3: 1;
    unsigned short int bit4: 1;
    unsigned short int bit5: 1;
    unsigned short int bit6: 1;
    unsigned short int bit7: 1;
    unsigned short int bit8: 1;
};

union byte
{
    unsigned char _byte;
    struct BYTE _BYTE;
};

void bInit(union byte* b, unsigned char value)
{
    b->_byte = value;
}

void bApplyMask(unsigned char mask, union byte* b)
{
    b->_byte = b->_byte & mask;
}

unsigned char bGetBit(int n, union byte* b)
{
    switch (n)
    {
    case 1:
        return b->_BYTE.bit1;
        break;
    
    case 2:
        return b->_BYTE.bit2;
        break;
        
    case 3:
        return b->_BYTE.bit3;
        break;

    case 4:
        return b->_BYTE.bit4;
        break;

    case 5:
        return b->_BYTE.bit5;
        break;

    case 6:
        return b->_BYTE.bit6;
        break;

    case 7:
        return b->_BYTE.bit7;
        break;

    case 8:
        return b->_BYTE.bit8;
        break;

    default:
        return 2;
        break;
    }
}

unsigned char bGetBits(int begin, int end, union byte* b)
{
    unsigned char result = 0;

    if (begin <= 8 && begin >= 1 && end <= 8 && end >= 1)
    {
        for (int i = begin; i < end; i++)
        {
            result = result << 1;
            result += bGetBit(i, b);
        }
    }
    
    return result;
}