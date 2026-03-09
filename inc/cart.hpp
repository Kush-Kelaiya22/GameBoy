#include <inc/inc.hpp>
#include <string>

#ifndef __CART__
#define __CART__

namespace gameboy
{
	class MBC_base
	{
	protected:
		Byte* ROM;
		Byte* RAM;
	public:
		MBC_base(Byte* rom, Byte* ram = nullptr);
		virtual Byte read(Word Addr) = 0;
		virtual void write(Word Addr, Byte data) = 0;
	};
	class MBC0 : public MBC_base
	{
	public:
		MBC0(Byte* rom, Byte* ram);
		Byte read(Word Addr) override;
		void write(Word Addr, Byte data) override;
	};
	class MBC1 : public MBC_base
	{
	private:
	public:
	};
	class MBC2 : public MBC_base
	{
	private:
		struct
		{
			Byte low : 4;
			Byte high : 4;
		} sram[256];
		Byte* rom_bank_ptr;
		Byte bank_number : 4;
		Byte ram_enable : 1;
	public:
		MBC2(Byte* rom, Byte* ram);
		Byte read(Word Addr) override;
		void write(Word Addr, Byte data) override;
	};
	class MBC3 : public MBC_base
	{
	};
	class MBC5 : public MBC_base
	{
	};
	class MBC6 : public MBC_base
	{
	};
	class MBC7 : public MBC_base
	{
	};
	class CART
	{
	private:
		static constexpr Byte ram_lut[] = {
			[0x00] = 0,
			[0x01] = 0,
			[0x02] = 8,
			[0x03] = 32,
			[0x04] = 128,
			[0x05] = 64
		};
		static constexpr Byte rom_lut[] = {
			[0x00] = 0,
			[0x01] = 1,
			[0x02] = 2,
			[0x03] = 3,
			[0x04] = 4,
			[0x05] = 5,
			[0x06] = 6,
			[0x07] = 7,
			[0x08] = 8
		};
		// static constexpr Byte ram_enable_lut[] = {
		// 	[0x00] = 0,
		// 	[0x01] = 0,
		// 	[0x02] = 1,
		// 	[0x03] = 1,
		// 	[0x05] = 0,
		// 	[0x06] = 0,
		// 	[0x08] = 1,
		// 	[0x09] = 1,
		// 	[0x0B] = 0,
		// 	[0x0C] = 1,
		// 	[0x0D] = 1,
		// 	[0x0F] = 0,
		// 	[0x10] = 1,
		// 	[0x11] = 0,
		// 	[0x12] = 1,
		// 	[0x13] = 1,
		// 	[0x19] = 0,
		// 	[0x1A] = 1,
		// 	[0x1B] = 1,
		// 	[0x1C] = 0,
		// 	[0x1D] = 1,
		// 	[0x1E] = 1,
		// 	[0x20] = 0,
		// 	[0x22] = 1,
		// 	[0xFE] = 0,
		// 	[0xFF] = 1
		// };
		std::string ROM_NAME;
		Byte* ROM;
		Byte* RAM;
		MBC_base* mbc;
	public:
		CART(std::string filename);
		~CART();
		Byte read(Word Addr);
		void write(Word Addr, Byte data);
	};
}

#endif // __CART__