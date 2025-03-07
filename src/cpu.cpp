#include <inc/gameboy.hpp>

namespace gameboy
{
	CPU::CPU()
	{
		for(Byte& r : reg) r = 0x00;
		PC = 0x0150;
		SP = 0x0000;
		flag = 0x00;
	}

	Byte CPU::FetchByte()
	{
		return bus[PC++];
	}

	Word CPU::HL(void)
	{
		return (h << 8) | l;
	}

	Word CPU::DE(void)
	{
		return (d << 8) | e;
	}

	Word CPU::BC(void)
	{
		return (b << 8) | c;
	}

	Word CPU::AF(void)
	{
		return (a << 8) | flag;
	}

	void CPU::HL(Word data)
	{
		h = data & 0x00FF;
		l = (data >> 8);
	}

	void CPU::DE(Word data)
	{
		d = data & 0x00FF;
		e = (data >> 8);
	}

	void CPU::BC(Word data)
	{
		b = data & 0x00FF;
		c = (data >> 8);
	}

	Word CPU::FetchWord()
	{
		Word w = bus[PC++];
		w |= bus[PC++] << 8;
		return w;
	}

	Byte CPU::ReadByte(Word addr)
	{
		return bus[addr];
	}

	Word CPU::ReadWord(Word addr)
	{
		Word w = bus[addr];
		w |= bus[addr + 1] << 8;
		return w;
	}

	void CPU::WriteByte(Word addr, Byte data)
	{
		bus[addr] = data;
	}

	void CPU::WriteWord(Word addr, Word data)
	{
		bus[addr] = data & 0x00FF;
		bus[addr + 1] = data >> 8;
	}

	inline void CPU::set_reset(Byte& reg, Byte bit, bool SR)
	{
		(SR == 1) ? (reg |= (0b1 << bit)) : (reg &= (~(0b1 << bit)));
	}

	void CPU::prefix_execute()
	{
		ins = FetchByte();
		switch(ins & 0b11'000'000)
		{
			case 0b00'000'000:
			{
				if(ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
				switch(ins & 0b00'111'000)
				{
					case 0b00'110'000:
					{
						Byte buffer = reg[(ins & 0b00000111)] >> 4;
						reg[(ins & 0b00000111)] <<= 4;
						reg[(ins & 0b00000111)] |= buffer;
						flag &= 0b10000000;
						flag &= (reg[(ins & 0b00000111)] == 0) << 7;
					}
					break;
					case 0b00'111'000:
					{
						flag = 0;
						flag |= ((reg[ins & 0b00000111] & 0b01) << 4);
						reg[ins & 0b00000111] >>= 1;
						flag |= ((reg[ins & 0b00000111] == 0) << 7);
					}
					break;
					case 0b00'100'000:
					{
						flag = 0;
						flag |= ((reg[ins & 0b00000111] & 0b1000'0000) << 4);
						reg[ins & 0b00000111] <<= 1;
						flag |= ((reg[ins & 0b00000111] == 0) << 7);
					}
					break;
					case 0b00'101'000:
					{
						Byte bit7 = reg[ins & 0b00000111] & 0b1000'0000;
						flag = 0;
						flag |= ((reg[ins & 0b00000111] & 0b01) << 4);
						reg[ins & 0b00000111] >>= 1;
						reg[ins & 0b00000111] |= bit7;
						flag |= ((reg[ins & 0b00000111] == 0) << 7);
					}
					break;
					case 0b00'010'000:
					{
						Byte bit7 = reg[ins & 0b00000111] & 0b1000'0000;
						reg[ins & 0b00000111] <<= 1;
						reg[ins & 0b00000111] |= ((flag & 0b0001'000) >> 4);
						flag = 0;
						flag |= (bit7 >> 3);
						flag |= ((reg[ins & 0b00000111] == 0) << 7);
					}
					break;
					case 0b00'011'000:
					{
						Byte bit0 = reg[ins & 0b00000111] & 0b0000'0001;
						reg[ins & 0b00000111] >>= 1;
						reg[ins & 0b00000111] |= ((flag & 0b0001'000) << 3);
						flag = 0;
						flag |= (bit0 << 4);
						flag |= ((reg[ins & 0b00000111] == 0) << 7);
					}
					break;
					case 0b00'000'000:
					{
						Byte bit7 = reg[ins & 0b00'000'111] & 0b1000'0000;
						reg[ins & 0b00'000'111] <<= 1;
						reg[ins & 0b00'000'111] |= (bit7 >> 7);
						flag = 0;
						flag |= (bit7 >> 3);
						flag |= ((reg[ins & 0b00000111] == 0) << 7);
					}
					break;
					case 0b00'001'000:
					{
						Byte bit0 = reg[ins & 0b00'000'111] & 0b0000'0001;
						reg[ins & 0b00'000'111] >>= 1;
						reg[ins & 0b00'000'111] |= (bit0 << 7);
						flag = 0;
						flag |= (bit0 << 4);
						flag |= ((reg[ins & 0b00000111] == 0) << 7);
					}
					break;
				}
				if(ins & 0b00'000'111 == 0b00'000'110) WriteByte(HL(), hl_ptr);
			} break;
			case 0b01'000'000:
			{
				if(ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
				flag &= 0b0011'0000;
				flag |= 0b0010'0000;
				flag |= (reg[ins & 0b00'000'111] & (0b1 << ((ins & 0b00'111'000) >> 3))) << 7;
			} break;
			case 0b10'000'000:
			case 0b11'000'000:
			{
				if(ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
				set_reset(reg[ins & 0b00'000'111], (ins & 0b00'111'000) >> 3, (ins & 0b010'000'000) >> 6);
				if(ins & 0b00'000'111 == 0b00'000'110) WriteByte(HL(), hl_ptr);
			} break;
		}
	}

	void CPU::execute()
	{
		#ifndef __DEBUG__
		while(1)
		{
		#endif
			ins = FetchByte();
			switch(ins & 0b11'000'000)
			{
				case 0b00'000'000:
				{
					switch(ins)
					{
						case inc_bc:
						{
							BC(BC() + 1);
						} break;
						case inc_de:
						{
							DE(DE() + 1);
						} break;
						case inc_hl:
						{
							HL(HL() + 1);
						} break;
						case inc_sp:
						{
							SP++;
						} break;
						case dec_bc:
						{
							BC(BC() - 1);
						} break;
						case dec_de:
						{
							DE(DE() - 1);
						} break;
						case dec_hl:
						{
							HL(HL() - 1);
						} break;
						case dec_sp:
						{
							SP--;
						} break;
						case ld_b_imm:
						{
							b = FetchByte();
						}
						break;
						case ld_c_imm:
						{
							c = FetchByte();
						}
						break;
						case ld_d_imm:
						{
							d = FetchByte();
						}
						break;
						case ld_e_imm:
						{
							e = FetchByte();
						}
						break;
						case ld_h_imm:
						{
							h = FetchByte();
						}
						break;
						case ld_l_imm:
						{
							l = FetchByte();
						}
						break;
						case ld_hl_r8_imm:
						{
							hl_ptr = FetchByte();
						}
						break;
						case ld_a_imm:
						{
							a = FetchByte();
						}
						break;
						case nop:
						{} break;
					}
				} break;
				case 0b01'000'000:
				{
					if(ins == hlt)
					{}
					else
					{
						if(ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
						reg[(ins & 0b00'111'000) >> 3] = reg[ins & 0b00'000'111];
						if(ins & 0b00'111'000 == 0b00'110'000) WriteByte(HL(), hl_ptr);
					}
				} break;
				case 0b10'000'000:
				{
					if(ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
					flag = 0;
					switch((ins & 0b00'111'000) >> 3)
					{
						case 0:
						{
							Word Data = a + reg[ins & 0b00'000'111];
							flag |= ((Data & 0x100) >> 8) << 4;
							flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
							a = Data;
						}
						break;
						case 1:
						{
							Word Data = a + (reg[ins & 0b00'000'111] + ((flag >> 4) & 0b0001));
							flag |= ((Data & 0x100) >> 8) << 4;
							flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
							a = Data;
						}
						break;
						case 2:
						{
							Word Data = a - reg[ins & 0b00'000'111];
							flag = 0b0100'0000;
							flag |= ((Data & 0x100) >> 8) << 4;
							flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
							a = Data;
						}
						break;
						case 3:
						{
							Word Data = a - (reg[ins & 0b00'000'111] + ((flag >> 4) & 0b0001));
							flag = 0b0100'0000;
							flag |= ((Data & 0x100) >> 8) << 4;
							flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
							a = Data;
						}
						break;
						case 4:
						{
							a &= reg[ins & 0b00'000'111];
							flag = 0b0010'0000;
						}
						break;
						case 5:
						{
							a ^= reg[ins & 0b00'000'111];
							flag = 0;
						}
						break;
						case 6:
						{
							a |= reg[ins & 0b00'000'111];
							flag = 0;
						}
						break;
						case 7:
						{
							Word Data = a - reg[ins & 0b00'000'111];
							flag = 0b0100'0000;
							flag |= ((Data & 0x100) >> 8) << 4;
							flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
						}
					}
					flag &= 0b0111'0000;
					flag |= ((a == 0) << 7);
				} break;
				case 0b11'000'000:
				{
					switch(ins)
					{
						case prefix:
						{
							prefix_execute();
						} break;
					}
				} break;
			}
		#ifndef __DEBUG__
		}
		#endif
	}
}