#include <inc/inc.hpp>

#ifndef __CPU__
#define __CPU__

namespace gameboy
{
	class BUS;
	class CPU
	{
	private:
		ui32 ticks;
		Word SP;
		Word PC;
		Byte reg[8];
		Byte flag;
		Byte& b = reg[0];
		Byte& c = reg[1];
		Byte& d = reg[2];
		Byte& e = reg[3];
		Byte& h = reg[4];
		Byte& l = reg[5];
		Byte& hl_ptr = reg[6];
		Byte& a = reg[7];
		Byte IE;
		BUS* bus;
		Byte ins;
		bool IME;
		Word HL(void);
		Word BC(void);
		Word DE(void);
		Word AF(void);
		void AF(Word data);
		void HL(Word data);
		void BC(Word data);
		void DE(Word data);
		Byte FetchByte();
		Word FetchWord();
		Byte ReadByte(Word addr);
		Word ReadWord(Word addr);
		void WriteByte(Word addr, Byte data);
		void WriteWord(Word addr, Word data);
		void prefix_execute();
		void set_reset(Byte& reg, Byte bit, bool set);
	public:
		void set_bus(BUS* b);
		CPU();
		void execute();
	};
}

#endif