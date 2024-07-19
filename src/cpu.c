#include <inc/cpu.h>
#include <inc/bus.h>
#include <inc/ins.h>

#define b gameboyCPU.reg[0]				 // can be used anywhere
#define c gameboyCPU.reg[1]				 // can be used anywhere
#define d gameboyCPU.reg[2]				 // can be used anywhere
#define e gameboyCPU.reg[3]				 // can be used anywhere
#define h gameboyCPU.reg[4]				 // can be used anywhere
#define l gameboyCPU.reg[5]				 // can be used anywhere
#define hl_ptr gameboyCPU.reg[6]		 // can be used anywhere
#define a gameboyCPU.reg[7]				 // can be used anywhere
#define hl ((h << 8) || l)				 // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define bc ((b << 8) || c)				 // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define de ((d << 8) || e)				 // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define af ((a << 8) || gameboyCPU.flag) // can be only used to refer data from memory, cannot be used to store data into the specified register by the macro
#define BC(X)                      \
	{                              \
		b = (((X) & 0xFF00) >> 8); \
		c = ((X) & 0x00FF);        \
	}
#define DE(X)                      \
	{                              \
		d = (((X) & 0xFF00) >> 8); \
		e = ((X) & 0x00FF);        \
	}
#define HL(X)                      \
	{                              \
		h = (((X) & 0xFF00) >> 8); \
		l = ((X) & 0x00FF);        \
	}
#define AF(X)                             \
	{                                     \
		a = (((X) & 0xFF00) >> 8);        \
		gameboyCPU.flag = ((X) & 0x00F0); \
	}

gbCpu gameboyCPU;
gbBus gameboyBUS;

void init(void)
{
	a = b = c = d = e = h = l = hl_ptr = 0x00;
	gameboyCPU.SP = 0x0000;
	gameboyCPU.PC = 0x0150;
}

Byte FetchByte()
{
	Byte Data = gameboyBUS.read(gameboyCPU.PC);
	gameboyCPU.PC++;
	return Data;
}

Word FetchWord()
{
	Word Data = FetchByte();
	Data |= (FetchByte() << 8);
	return Data;
}

Byte ReadByte(Word Addr)
{
	Byte Data = gameboyBUS.read(Addr);
	return Data;
}

Word ReadWord(Word Addr)
{
	Word Data = ReadByte(Addr);
	Data |= (ReadByte(Addr + 1) << 8);
}

void WriteByte(Word Addr, Byte Val)
{
	gameboyBUS.write(Addr, Val);
}

void WriteWord(Word Addr, Word Val)
{
	WriteByte(Addr, Val & 0x00FF);
	WriteByte(Addr + 1, (Val >> 8) & 0x00FF);
}

Byte set_reset(Byte reg, Byte bit, bool SR)
{
	switch (SR)
	{
	case 1:
	{
		reg |= (0b1 << bit);
	}
	break;
	case 0:
	{
		reg &= (~(0b1 << bit));
	}
	break;
	}
	return reg;
}

void PREFIX(void)
{
	switch (gameboyCPU.ins & 0b11'000'000)
	{
	case 0b00'000'000:
	{
		if ((gameboyCPU.ins & 0b00000111) == 0b00000110)
		{
			hl_ptr = ReadByte(hl);
		}
		switch (gameboyCPU.ins & 0b00'111'000)
		{
		case 0b00'110'000:
		{
			Byte buffer = gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)] >> 4;
			gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)] <<= 4;
			gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)] |= buffer;
			gameboyCPU.flag &= 0b10000000;
			gameboyCPU.flag &= (gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)] == 0) << 7;
		}
		break;
		case 0b00'111'000:
		{
			gameboyCPU.flag = 0;
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] & 0b01) << 4);
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] >>= 1;
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] == 0) << 7);
		}
		break;
		case 0b00'100'000:
		{
			gameboyCPU.flag = 0;
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] & 0b1000'0000) << 4);
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] <<= 1;
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] == 0) << 7);
		}
		break;
		case 0b00'101'000:
		{
			Byte bit7 = gameboyCPU.reg[gameboyCPU.ins] & 0b1000'0000;
			gameboyCPU.flag = 0;
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] & 0b01) << 4);
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] >>= 1;
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] |= bit7;
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] == 0) << 7);
		}
		break;
		case 0b00'010'000:
		{
			Byte bit7 = gameboyCPU.reg[gameboyCPU.ins & 0b00000111] & 0b1000'0000;
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] <<= 1;
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] |= ((gameboyCPU.flag & 0b0001'000) >> 4);
			gameboyCPU.flag = 0;
			gameboyCPU.flag |= (bit7 >> 3);
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] == 0) << 7);
		}
		break;
		case 0b00'011'000:
		{
			Byte bit0 = gameboyCPU.reg[gameboyCPU.ins & 0b00000111] & 0b0000'0001;
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] >>= 1;
			gameboyCPU.reg[gameboyCPU.ins & 0b00000111] |= ((gameboyCPU.flag & 0b0001'000) << 3);
			gameboyCPU.flag = 0;
			gameboyCPU.flag |= (bit0 << 4);
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] == 0) << 7);
		}
		break;
		case 0b00'000'000:
		{
			Byte bit7 = gameboyCPU.reg[gameboyCPU.ins & 0b00'000'111] & 0b1000'0000;
			gameboyCPU.reg[gameboyCPU.ins & 0b00'000'111] <<= 1;
			gameboyCPU.reg[gameboyCPU.ins & 0b00'000'111] |= (bit7 >> 7);
			gameboyCPU.flag = 0;
			gameboyCPU.flag |= (bit7 >> 3);
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] == 0) << 7);
		}
		break;
		case 0b00'001'000:
		{
			Byte bit0 = gameboyCPU.reg[gameboyCPU.ins & 0b00'000'111] & 0b0000'0001;
			gameboyCPU.reg[gameboyCPU.ins & 0b00'000'111] >>= 1;
			gameboyCPU.reg[gameboyCPU.ins & 0b00'000'111] |= (bit0 << 7);
			gameboyCPU.flag = 0;
			gameboyCPU.flag |= (bit0 << 4);
			gameboyCPU.flag |= ((gameboyCPU.reg[gameboyCPU.ins & 0b00000111] == 0) << 7);
		}
		break;
		}
		if ((gameboyCPU.ins & 0b00000111) == 0b00000110)
		{
			WriteByte(hl, hl_ptr);
		}
	}
	break;
	case 0b01'000'000:
	{
		if ((gameboyCPU.ins & 0b00000111) == 0b00000110)
		{
			hl_ptr = ReadByte(hl);
		}
		gameboyCPU.flag &= (0b0011'0000);
		gameboyCPU.flag |= (0b0010'0000);
		gameboyCPU.flag |= (gameboyCPU.reg[gameboyCPU.ins & 0b00000111] & (0b1 << ((gameboyCPU.ins & 0b00111000) >> 3))) << 7;
	}
	break;
	case 0b10'000'000:
	case 0b11'000'000:
	{
		if ((gameboyCPU.ins & 0b00000111) == 0b00000110)
		{
			hl_ptr = ReadByte(hl);
		}
		gameboyCPU.reg[gameboyCPU.ins & 0b00000111] = set_reset(gameboyCPU.reg[gameboyCPU.ins & 0b00000111], (gameboyCPU.ins & 0b00111000) >> 3, (gameboyCPU.ins & 0b01000000) >> 6);
		if ((gameboyCPU.ins & 0b00000111) == 0b00000110)
		{
			WriteByte(hl, hl_ptr);
		}
	}
	break;
	}
}

void *cpu(void *)
{
	while (1)
	{
		gameboyCPU.ins = FetchByte();
		switch (gameboyCPU.ins & 0b11000000)
		{
		case 0b00000000:
		{
		}
		break;
		case 0b01000000:
		{
			if (gameboyCPU.ins == hlt)
			{
			}
			else
			{
				if ((gameboyCPU.ins & 0b00000111) == 0b00000110)
				{
					hl_ptr = ReadByte(hl);
				}
				gameboyCPU.reg[((gameboyCPU.ins & 0b00111000) >> 3)] = gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)];
				if ((gameboyCPU.ins & 0b00111000) == 0b00110000)
				{
					WriteByte(hl, hl_ptr);
				}
			}
		}
		break;
		case 0b10000000:
		{
			if ((gameboyCPU.ins & 0b00000111) == 0b00000110)
			{
				hl_ptr = ReadByte(hl);
			}
			gameboyCPU.flag = 0;
			switch ((gameboyCPU.ins & 0b00111000) >> 3)
			{
			case 0:
			{
				Word Data = a + gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)];
				gameboyCPU.flag |= ((Data & 0x100) >> 8) << 4;
				gameboyCPU.flag |= (((Data ^ a) >> 3) == 0) << 5;
				a = Data;
			}
			break;
			case 1:
			{
				Word Data = a + (gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)] + ((gameboyCPU.flag >> 4) & 0b0001));
				gameboyCPU.flag |= ((Data & 0x100) >> 8) << 4;
				gameboyCPU.flag |= (((Data ^ a) >> 3) == 0) << 5;
				a = Data;
			}
			break;
			case 2:
			{
				Word Data = a - gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)];
				gameboyCPU.flag = 0b0100'0000;
				gameboyCPU.flag |= ((Data & 0x100) >> 8) << 4;
				gameboyCPU.flag |= (((Data ^ a) >> 3) == 0) << 5;
				a = Data;
			}
			break;
			case 3:
			{
				Word Data = a - (gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)] + ((gameboyCPU.flag >> 4) & 0b0001));
				gameboyCPU.flag = 0b0100'0000;
				gameboyCPU.flag |= ((Data & 0x100) >> 8) << 4;
				gameboyCPU.flag |= (((Data ^ a) >> 3) == 0) << 5;
				a = Data;
			}
			break;
			case 4:
			{
				a &= gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)];
				gameboyCPU.flag = 0b0010'0000;
			}
			break;
			case 5:
			{
				a ^= gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)];
			}
			break;
			case 6:
			{
				a |= gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)];
			}
			break;
			case 7:
			{
				Word Data = a - gameboyCPU.reg[(gameboyCPU.ins & 0b00000111)];
				gameboyCPU.flag |= 0b0100'0000;
				gameboyCPU.flag |= ((Data & 0x100) >> 8) << 4;
				gameboyCPU.flag |= (((Data ^ a) >> 3) == 0) << 5;
			}
			break;
			}
			gameboyCPU.flag &= 0b0111'0000;
			gameboyCPU.flag |= ((a == 0) << 7);
		}
		break;
		case 0b11000000:
		{
		}
		break;
		}
	}
}