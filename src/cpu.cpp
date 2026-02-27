#include <inc/gameboy.hpp>

namespace gameboy
{
	CPU::CPU()
	{
		for (Byte& r : this->reg) r = 0x00;
		this->PC = 0x0150;
		this->SP = 0x0000;
		this->flag = 0x00;
	}

	void CPU::set_bus(BUS* b)
	{
		this->bus = b;
	}

	Word CPU::HL(void)
	{
		return (this->h << 8) | this->l;
	}

	Word CPU::DE(void)
	{
		return (this->d << 8) | this->e;
	}

	Word CPU::BC(void)
	{
		return (this->b << 8) | this->c;
	}

	Word CPU::AF(void)
	{
		return (this->a << 8) | this->flag;
	}

	void CPU::HL(Word data)
	{
		this->l = data & 0x00FF;
		this->h = (data >> 8);
	}

	void CPU::AF(Word data)
	{
		this->flag = data & 0x00FF;
		this->a = (data >> 8);
	}

	void CPU::DE(Word data)
	{
		this->e = data & 0x00FF;
		this->d = (data >> 8);
	}

	void CPU::BC(Word data)
	{
		this->c = data & 0x00FF;
		this->b = (data >> 8);
	}

	Byte CPU::FetchByte()
	{
		return bus->read(this->PC++);
	}

	Word CPU::FetchWord()
	{
		Word w = bus->read(this->PC++);
		w |= bus->read(this->PC++) << 8;
		return w;
	}

	Byte CPU::ReadByte(Word addr)
	{
		return bus->read(addr);
	}

	Word CPU::ReadWord(Word addr)
	{
		Word w = bus->read(addr);
		w |= bus->read(addr + 1) << 8;
		return w;
	}

	void CPU::WriteByte(Word addr, Byte data)
	{
		bus->write(addr, data);
	}

	void CPU::WriteWord(Word addr, Word data)
	{
		bus->write(addr, data & 0x00FF);
		bus->write(addr + 1, data >> 8);
	}

	inline void CPU::set_reset(Byte& r, Byte bit, bool SR)
	{
		(SR == 1) ? (r |= (0b1 << bit)) : (r &= (~(0b1 << bit)));
	}

	void CPU::prefix_execute()
	{
		this->ins = FetchByte();
		switch (this->ins & 0b11'000'000)
		{
		case 0b00'000'000:
		{
			if (this->ins & 0b00'000'111 == 0b00'000'110) this->hl_ptr = ReadByte(HL());
			switch (this->ins & 0b00'111'000)
			{
			case 0b00'110'000:
			{
				Byte buffer = this->reg[(this->ins & 0b00000111)] >> 4;
				this->reg[(this->ins & 0b00000111)] <<= 4;
				this->reg[(this->ins & 0b00000111)] |= buffer;
				this->flag &= 0b10000000;
				this->flag &= (this->reg[(this->ins & 0b00000111)] == 0) << 7;
			}
			break;
			case 0b00'111'000:
			{
				this->flag = 0;
				this->flag |= ((this->reg[this->ins & 0b00000111] & 0b01) << 4);
				this->reg[this->ins & 0b00000111] >>= 1;
				this->flag |= ((this->reg[this->ins & 0b00000111] == 0) << 7);
			}
			break;
			case 0b00'100'000:
			{
				this->flag = 0;
				this->flag |= ((this->reg[this->ins & 0b00000111] & 0b1000'0000) << 4);
				this->reg[this->ins & 0b00000111] <<= 1;
				this->flag |= ((this->reg[this->ins & 0b00000111] == 0) << 7);
			}
			break;
			case 0b00'101'000:
			{
				Byte bit7 = this->reg[this->ins & 0b00000111] & 0b1000'0000;
				this->flag = 0;
				this->flag |= ((this->reg[this->ins & 0b00000111] & 0b01) << 4);
				this->reg[this->ins & 0b00000111] >>= 1;
				this->reg[this->ins & 0b00000111] |= bit7;
				this->flag |= ((this->reg[this->ins & 0b00000111] == 0) << 7);
			}
			break;
			case 0b00'010'000:
			{
				Byte bit7 = this->reg[this->ins & 0b00000111] & 0b1000'0000;
				this->reg[this->ins & 0b00000111] <<= 1;
				this->reg[this->ins & 0b00000111] |= ((this->flag & 0b0001'000) >> 4);
				this->flag = 0;
				this->flag |= (bit7 >> 3);
				this->flag |= ((this->reg[this->ins & 0b00000111] == 0) << 7);
			}
			break;
			case 0b00'011'000:
			{
				Byte bit0 = this->reg[this->ins & 0b00000111] & 0b0000'0001;
				this->reg[this->ins & 0b00000111] >>= 1;
				this->reg[this->ins & 0b00000111] |= ((this->flag & 0b0001'000) << 3);
				this->flag = 0;
				this->flag |= (bit0 << 4);
				this->flag |= ((this->reg[this->ins & 0b00000111] == 0) << 7);
			}
			break;
			case 0b00'000'000:
			{
				Byte bit7 = this->reg[this->ins & 0b00'000'111] & 0b1000'0000;
				this->reg[this->ins & 0b00'000'111] <<= 1;
				this->reg[this->ins & 0b00'000'111] |= (bit7 >> 7);
				this->flag = 0;
				this->flag |= (bit7 >> 3);
				this->flag |= ((this->reg[this->ins & 0b00000111] == 0) << 7);
			}
			break;
			case 0b00'001'000:
			{
				Byte bit0 = this->reg[this->ins & 0b00'000'111] & 0b0000'0001;
				this->reg[this->ins & 0b00'000'111] >>= 1;
				this->reg[this->ins & 0b00'000'111] |= (bit0 << 7);
				this->flag = 0;
				this->flag |= (bit0 << 4);
				this->flag |= ((this->reg[this->ins & 0b00000111] == 0) << 7);
			}
			break;
			}
			if (this->ins & 0b00'000'111 == 0b00'000'110) WriteByte(HL(), this->hl_ptr);
		} break;
		case 0b01'000'000:
		{
			if (this->ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
			this->flag &= 0b0011'0000;
			this->flag |= 0b0010'0000;
			this->flag |= (this->reg[this->ins & 0b00'000'111] & (0b1 << ((this->ins & 0b00'111'000) >> 3))) << 7;
		} break;
		case 0b10'000'000:
		case 0b11'000'000:
		{
			if (this->ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
			set_reset(this->reg[this->ins & 0b00'000'111], (this->ins & 0b00'111'000) >> 3, (this->ins & 0b010'000'000) >> 6);
			if (this->ins & 0b00'000'111 == 0b00'000'110) WriteByte(HL(), hl_ptr);
		} break;
		}
	}

	void CPU::execute()
	{
#ifndef __DEBUG__
		while (1)
		{
#endif
			this->ins = FetchByte();
			switch (this->ins & 0b11'000'000)
			{
			case 0b00'000'000:
			{
				switch (this->ins)
				{
				case rlca:
				{
					Byte bit7 = this->a & 0b1000'0000;
					this->a <<= 1;
					this->a |= (bit7 >> 7);
					this->flag = 0;
					this->flag |= (bit7 >> 3);
				} break;
				case rrca:
				{
					Byte bit0 = this->a & 0b0000'0001;
					this->a >>= 1;
					this->a |= (bit0 << 7);
					this->flag = 0;
					this->flag |= (bit0 << 4);
				} break;
				case rla:
				{
					Byte bit7 = this->a & 0b1000'0000;
					this->a <<= 1;
					this->a |= ((this->flag & 0b0001'000) >> 4);
					this->flag = 0;
					this->flag |= (bit7 >> 3);
				} break;
				case rra:
				{
					Byte bit0 = this->a & 0b0000'0001;
					this->a >>= 1;
					this->a |= ((this->flag & 0b0001'000) << 3);
					this->flag = 0;
					this->flag |= (bit0 << 4);
				} break;
				case add_hl_bc:
				{
					ui32 res = HL() + BC();
					this->flag &= 0b0001'0000;
					this->flag |= ((res & 0x10000) >> 16) << 4;
					this->flag |= ((((HL() ^ BC() ^ res) >> 11) & 0b0011'0000) == 0b0010'0000) << 5;
					HL(res);
				} break;
				case add_hl_de:
				{
					ui32 res = HL() + DE();
					this->flag &= 0b0001'0000;
					this->flag |= ((res & 0x10000) >> 16) << 4;
					this->flag |= ((((HL() ^ DE() ^ res) >> 11) & 0b0011'0000) == 0b0010'0000) << 5;
					HL(res);
				} break;
				case add_hl_hl:
				{
					ui32 res = HL() + HL();
					this->flag &= 0b0001'0000;
					this->flag |= ((res & 0x10000) >> 16) << 4;
					this->flag |= ((((HL() ^ HL() ^ res) >> 11) & 0b0011'0000) == 0b0010'0000) << 5;
					HL(res);
				} break;
				case add_hl_sp:
				{
					ui32 res = HL() + SP;
					this->flag &= 0b0001'0000;
					this->flag |= ((res & 0x10000) >> 16) << 4;
					this->flag |= ((((HL() ^ SP ^ res) >> 11) & 0b0011'0000) == 0b0010'0000) << 5;
					HL(res);
				} break;
				case inc_b:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->b + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->b++;
					this->flag |= ((this->b == 0) << 7);
				} break;
				case inc_c:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->c + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->c++;
					this->flag |= ((this->c == 0) << 7);
				} break;
				case inc_d:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->d + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->d++;
					this->flag |= ((this->d == 0) << 7);
				} break;
				case inc_e:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->e + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->e++;
					this->flag |= ((this->e == 0) << 7);
				} break;
				case inc_h:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->h + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->h++;
					this->flag |= ((this->h == 0) << 7);
				} break;
				case inc_l:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->l + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->l++;
					this->flag |= ((this->l == 0) << 7);
				} break;
				case inc_hl_r8:
				{
					Byte hl_data = ReadByte(HL());
					this->flag &= 0b0001'0000;
					this->flag |= ((((hl_data + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					hl_data++;
					WriteByte(HL(), hl_data);
					this->flag |= ((hl_data == 0) << 7);
				} break;
				case inc_a:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->a + 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->a++;
					this->flag |= ((this->a == 0) << 7);
				} break;
				case dec_b:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->b - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->b--;
					this->flag |= ((this->b == 0) << 7);
				} break;
				case dec_c:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->c - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->c--;
					this->flag |= ((this->c == 0) << 7);
				} break;
				case dec_d:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->d - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->d--;
					this->flag |= ((this->d == 0) << 7);
				} break;
				case dec_e:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->e - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->e--;
					this->flag |= ((this->e == 0) << 7);
				} break;
				case dec_h:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->h - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->h--;
					this->flag |= ((this->h == 0) << 7);
				} break;
				case dec_l:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->l - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->l--;
					this->flag |= ((this->l == 0) << 7);
				} break;
				case dec_hl_r8:
				{
					Byte hl_data = ReadByte(HL());
					this->flag &= 0b0001'0000;
					this->flag |= ((((hl_data - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					hl_data--;
					WriteByte(HL(), hl_data);
					this->flag |= ((hl_data == 0) << 7);
				} break;
				case dec_a:
				{
					this->flag &= 0b0001'0000;
					this->flag |= ((((this->a - 1) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->a--;
					this->flag |= ((this->a == 0) << 7);
				} break;
				case ccf:
				{
					this->flag ^= 0b0001'0000;
				}
				break;
				case scf:
				{
					this->flag |= 0b0001'0000;
				}
				break;
				case cpl:
				{
					this->flag |= 0b0110'0000;
					this->a = ~this->a;
				}
				break;
				case daa:
				{
					Word res = 0x00;
					Byte adj = 0x00;
					if (this->flag & 0b0100'0000)
					{
						if (this->flag & 0b0010'0000) adj += 0x06;
						if (this->flag & 0b0001'0000) adj += 0x60;
						res = this->a + (~adj) + 1;
					}
					else
					{
						if ((this->flag & 0b0010'0000) or ((this->a & 0x0f) > 0x09)) adj += 0x06;
						if ((this->flag & 0b0001'0000) or (this->a > 0x99)) adj += 0x60;
						res = this->a + adj;
					}
					this->flag |= ((a == 0) ? 0b1000'0000 : 0x00);
					this->flag |= ((res & 0x0100) ? 0b0001'0000 : 0x00);
					this->flag &= 0b0100'0000;
				}
				break;
				case inc_bc:
				{
					BC(BC() + 1);
				}
				break;
				case inc_de:
				{
					DE(DE() + 1);
				}
				break;
				case inc_hl:
				{
					HL(HL() + 1);
				}
				break;
				case inc_sp:
				{
					this->SP++;
				}
				break;
				case dec_bc:
				{
					BC(BC() - 1);
				}
				break;
				case dec_de:
				{
					DE(DE() - 1);
				}
				break;
				case dec_hl:
				{
					HL(HL() - 1);
				}
				break;
				case dec_sp:
				{
					this->SP--;
				}
				break;
				case ld_b_imm:
				{
					this->b = FetchByte();
				}
				break;
				case ld_c_imm:
				{
					this->c = FetchByte();
				}
				break;
				case ld_d_imm:
				{
					this->d = FetchByte();
				}
				break;
				case ld_e_imm:
				{
					this->e = FetchByte();
				}
				break;
				case ld_h_imm:
				{
					this->h = FetchByte();
				}
				break;
				case ld_l_imm:
				{
					this->l = FetchByte();
				}
				break;
				case ld_hl_r8_imm:
				{
					this->hl_ptr = FetchByte();
					WriteByte(HL(), this->hl_ptr);
				}
				break;
				case ld_a_imm:
				{
					this->a = FetchByte();
				}
				break;
				case ld_bc_imm:
				{
					BC(FetchWord());
				}
				break;
				case ld_de_imm:
				{
					DE(FetchWord());
				}
				break;
				case ld_hl_imm:
				{
					HL(FetchWord());
				}
				break;
				case ld_sp_imm:
				{
					this->SP = FetchWord();
				}
				break;
				case ld_imm_sp:
				{
					Word addr = FetchWord();
					WriteWord(addr, this->SP);
				}
				break;
				case ld_a_to_loc_bc:
				{
					WriteByte(BC(), this->a);
				}
				break;
				case ld_a_from_loc_bc:
				{
					a = ReadByte(BC());
				}
				break;
				case ld_a_from_loc_de:
				{
					a = ReadByte(DE());
				}
				break;
				case ld_a_from_loc_hld:
				{
					a = ReadByte(HL());
					HL(HL() - 1);
				}
				break;
				case ld_a_from_loc_hli:
				{
					a = ReadByte(HL());
					HL(HL() + 1);
				}
				break;
				case ld_a_to_loc_de:
				{
					WriteByte(DE(), a);
				}
				break;
				case ld_a_to_loc_hld:
				{
					WriteByte(HL(), a);
					HL(HL() - 1);
				}
				break;
				case ld_a_to_loc_hli:
				{
					WriteByte(HL(), a);
					HL(HL() + 1);
				}
				break;
				case jr:
				{
					SByte addr = (SByte)FetchByte();
					PC += addr;
				}
				break;
				case jrc_c:
				{
					SByte addr = (SByte)FetchByte();
					if (this->flag & 0b0001'0000) PC += addr;
				}
				break;
				case jrc_nc:
				{
					SByte addr = (SByte)FetchByte();
					if (!(this->flag & 0b0001'0000)) PC += addr;
				}
				break;
				case jrc_z:
				{
					SByte addr = (SByte)FetchByte();
					if (this->flag & 0b1000'0000) PC += addr;
				}
				break;
				case jrc_nz:
				{
					SByte addr = (SByte)FetchByte();
					if (!(this->flag & 0b1000'0000)) PC += addr;
				}
				break;
				case stop:
				{}
				break;
				case nop:
				{}
				break;
				}
			}
			break;
			case 0b01'000'000:
			{
				if (this->ins == hlt)
				{
				}
				else
				{
					if (this->ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
					this->reg[(this->ins & 0b00'111'000) >> 3] = this->reg[this->ins & 0b00'000'111];
					if (this->ins & 0b00'111'000 == 0b00'110'000) WriteByte(HL(), hl_ptr);
				}
			}
			break;
			case 0b10'000'000:
			{
				if (this->ins & 0b00'000'111 == 0b00'000'110) hl_ptr = ReadByte(HL());
				this->flag &= 0b0001'0000;
				switch ((this->ins & 0b00'111'000) >> 3)
				{
				case 0:
				{
					Word Data = a + this->reg[this->ins & 0b00'000'111];
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
				}
				break;
				case 1:
				{
					Word Data = a + (this->reg[this->ins & 0b00'000'111] + ((this->flag >> 4) & 0b0001));
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
				}
				break;
				case 2:
				{
					Word Data = a + (~this->reg[this->ins & 0b00'000'111] + 1);
					this->flag = 0b0100'0000;
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
				}
				break;
				case 3:
				{
					Word Data = a + (~this->reg[this->ins & 0b00'000'111] + 1 + ((this->flag >> 4) & 0b0001));
					this->flag = 0b0100'0000;
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
				}
				break;
				case 4:
				{
					a &= this->reg[this->ins & 0b00'000'111];
					this->flag = 0b0010'0000;
				}
				break;
				case 5:
				{
					a ^= this->reg[this->ins & 0b00'000'111];
					this->flag = 0;
				}
				break;
				case 6:
				{
					a |= this->reg[this->ins & 0b00'000'111];
					this->flag = 0;
				}
				break;
				case 7:
				{
					Word Data = a - this->reg[this->ins & 0b00'000'111];
					this->flag = 0b0100'0000;
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
				}
				break;
				}
				this->flag &= 0b0111'0000;
				this->flag |= ((a == 0) << 7);
			}
			break;
			case 0b11'000'000:
			{
				switch (this->ins)
				{
				case prefix:
				{
					prefix_execute();
				} break;
				case psh_bc:
				{
					WriteWord(--SP, BC());
					SP--;
				} break;
				case psh_de:
				{
					WriteWord(--SP, DE());
					SP--;
				} break;
				case psh_hl:
				{
					WriteWord(--SP, HL());
					SP--;
				} break;
				case psh_af:
				{
					WriteWord(--SP, AF());
					SP--;
				} break;
				case pop_af:
				{
					SP++;
					AF(ReadWord(SP));
					SP++;
				} break;
				case pop_bc:
				{
					SP++;
					BC(ReadWord(SP));
					SP++;
				} break;
				case pop_de:
				{
					SP++;
					DE(ReadWord(SP));
					SP++;
				} break;
				case pop_hl:
				{
					SP++;
					HL(ReadWord(SP));
					SP++;
				} break;
				case ldh_a_c:
				{
					a = ReadByte(0xFF00 + c);
				} break;
				case ldh_c_a:
				{
					WriteByte(0xFF00 + c, a);
				} break;
				case ldh_a_imm_loc:
				{
					a = ReadByte(0xFF00 + FetchByte());
				} break;
				case ldh_imm_loc_a:
				{
					WriteByte(0xFF00 + FetchByte(), a);
				} break;
				case ld_imm_loc_a:
				{
					Word addr = FetchWord();
					a = ReadByte(addr);
				} break;
				case ld_a_imm_loc:
				{
					Word addr = FetchWord();
					WriteByte(addr, a);
				} break;
				case di:
				{
					IME = false;
				} break;
				case ei:
				{
					IME = true;
				} break;
				case add_a_imm:
				{
					this->flag &= 0b0001'0000;
					Word Data = a + FetchByte();
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case adc_a_imm:
				{
					this->flag &= 0b0001'0000;
					Word Data = a + FetchByte() + ((this->flag & 0x10) >> 4);
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case sub_a_imm:
				{
					this->flag &= 0b0001'0000;
					Word Data = a + (~FetchByte() + 1);
					this->flag = 0b0100'0000;
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case sbc_a_imm:
				{
					this->flag &= 0b0001'0000;
					Word Data = a + (~FetchByte() + 1 + ((this->flag & 0x10) >> 4));
					this->flag = 0b0100'0000;
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					a = Data;
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case and_a_imm:
				{
					this->flag &= 0b0001'0000;
					a &= FetchByte();
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case xor_a_imm:
				{
					this->flag &= 0b0001'0000;
					a ^= FetchByte();
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case or_a_imm:
				{
					this->flag &= 0b0001'0000;
					a |= FetchByte();
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case cp_a_imm:
				{
					this->flag &= 0b0001'0000;
					Word Data = a + (~FetchByte() + 1);
					this->flag = 0b0100'0000;
					this->flag |= ((Data & 0x100) >> 8) << 4;
					this->flag |= ((((Data ^ a) >> 3) & 0b0011'0000) == 0b0010'0000) << 5;
					this->flag &= 0b0111'0000;
					this->flag |= ((a == 0) << 7);
				} break;
				case rst_tgt3_0:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0000;
				} break;
				case rst_tgt3_1:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0008;
				} break;
				case rst_tgt3_2:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0010;
				} break;
				case rst_tgt3_3:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0018;
				} break;
				case rst_tgt3_4:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0020;
				} break;
				case rst_tgt3_5:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0028;
				} break;
				case rst_tgt3_6:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0030;
				} break;
				case rst_tgt3_7:
				{
					WriteWord(--SP, PC);
					SP--;
					PC = 0x0038;
				} break;
				case call_imm:
				{
					Word addr = FetchWord();
					WriteWord(--SP, PC);
					SP--;
					PC = addr;
				} break;
				case call_z_imm:
				{
					Word addr = FetchWord();
					if (this->flag & 0b1000'0000)
					{
						WriteWord(--SP, PC);
						SP--;
						PC = addr;
					}
				} break;
				case call_nc_imm:
				{
					Word addr = FetchWord();
					if (!(this->flag & 0b0001'0000))
					{
						WriteWord(--SP, PC);
						SP--;
						PC = addr;
					}
				} break;
				case call_c_imm:
				{
					Word addr = FetchWord();
					if (this->flag & 0b0001'0000)
					{
						WriteWord(--SP, PC);
						SP--;
						PC = addr;
					}
				} break;
				case call_nz_imm:
				{
					Word addr = FetchWord();
					if (!(this->flag & 0b1000'0000))
					{
						WriteWord(--SP, PC);
						SP--;
						PC = addr;
					}
				} break;
				case return_from_jumps:
				{
					SP++;
					PC = ReadWord(SP);
					SP++;
				} break;
				case retc_z:
				{
					if (this->flag & 0b1000'0000)
					{
						SP++;
						PC = ReadWord(SP);
						SP++;
					}
				} break;
				case retc_nz:
				{
					if (!(this->flag & 0b1000'0000))
					{
						SP++;
						PC = ReadWord(SP);
						SP++;
					}
				} break;
				case retc_c:
				{
					if (this->flag & 0b0001'0000)
					{
						SP++;
						PC = ReadWord(SP);
						SP++;
					}
				} break;
				case retc_nc:
				{
					if (!(this->flag & 0b0001'0000))
					{
						SP++;
						PC = ReadWord(SP);
						SP++;
					}
				} break;
				case reti:
				{
					SP++;
					PC = ReadWord(SP);
					SP++;
					IME = true;
				} break;
				case jp:
				{
					PC = FetchWord();
				} break;
				case jpc_z:
				{
					Word addr = FetchWord();
					if (this->flag & 0b1000'0000) PC = addr;
				} break;
				case jpc_nz:
				{
					Word addr = FetchWord();
					if (!(this->flag & 0b1000'0000)) PC = addr;
				} break;
				case jpc_c:
				{
					Word addr = FetchWord();
					if (this->flag & 0b0001'0000) PC = addr;
				} break;
				case jpc_nc:
				{
					Word addr = FetchWord();
					if (!(this->flag & 0b0001'0000)) PC = addr;
				} break;
				case ld_sp_hl:
				{
					this->SP = HL();
				} break;
				case jp_hl:
				{
					PC = HL();
				} break;
				case ld_hl_sp_with_imm8:
				{
					SByte addr = (SByte)FetchByte();
					this->flag &= 0b0001'0000;
					this->flag |= ((SP + addr) > 0xFFFF) << 4;
					this->flag |= ((((SP ^ addr ^ (SP + addr)) >> 3) & 0b0001'0000) == 0b0001'0000) << 5;
					HL(SP + addr);
				} break;
				case add_sp_imm8:
				{
					SByte addr = (SByte)FetchByte();
					this->flag &= 0b0001'0000;
					this->flag |= ((SP + addr) > 0xFFFF) << 4;
					this->flag |= ((((SP ^ addr ^ (SP + addr)) >> 3) & 0b0001'0000) == 0b0001'0000) << 5;
					SP += addr;
				} break;
				default:
					break;
				}
			} break;
			}
#ifndef __DEBUG__
		}
#endif
	}
}