#include <bits/stdc++.h>
using namespace std;

struct ins
{
	uint8_t opcode;
	uint8_t opreand_count : 2;
	uint8_t operand_1_type : 2;
	uint8_t bytes : 2;
	uint8_t prefixed : 1;
};

map<string, uint8_t> r8 = {
	{"b", 0},
	{"c", 1},
	{"d", 2},
	{"e", 3},
	{"h", 4},
	{"l", 5},
	{"[hl]", 6},
	{"a", 7},
};

map<string, uint8_t> r16 = {
	{"bc", 0},
	{"de", 1},
	{"hl", 2},
	{"sp", 3}
};

map<string, uint8_t> r16stk = {
	{"bc", 0},
	{"de", 1},
	{"hl", 2},
	{"af", 3}
};

map<string, uint8_t> r16mem = {
	{"bc", 0},
	{"de", 1},
	{"[hl+]", 2},
	{"[hl-]", 3},
	{"[hli]", 2},
	{"[hld]", 3}
};

map<int, map<string, uint8_t>> op_type = {
	{0, r8},
	{1, r16},
	{2, r16stk},
	{3, r16mem}
};

map<int, int> rst_tgt = {
	{0x00, 0},
	{0x08, 1},
	{0x10, 2},
	{0x18, 3},
	{0x20, 4},
	{0x28, 5},
	{0x30, 6},
	{0x38, 7}
};

map<string, ins> opcodes = {
	{"nop", {0x00, 0, 0, 1, false}},		// completed [1]
	{"inc", {0x00, 1, 0, 1, false}},		// completed [12]
	{"dec", {0x00, 1, 0, 1, false}},		// completed [12]
	{"rlca", {0x07, 0, 0, 1, false}},		// completed [1]
	{"rrca", {0x0f, 0, 0, 1, false}},		// completed [1]
	{"rla", {0x17, 0, 0, 1, false}},		// completed [1]
	{"rra", {0x1f, 0, 0, 1, false}},		// completed [1]
	{"daa", {0x27, 0, 0, 1, false}},		// completed [1]
	{"cpl", {0x2f, 0, 0, 1, false}},		// completed [1]
	{"scf", {0x37, 0, 0, 1, false}},		// completed [1]
	{"ccf", {0x3f, 0, 0, 1, false}},		// completed [1]
	{"jr", {0x00, 2, 0, 2, false}},			//
	{"stop", {0x10, 0, 0, 2, false}},		// completed [1]
	{"ld", {0x00, 2, 0, 0, false}},			//
	{"halt", {0x76, 0, 0, 1, false}},		// completed [1]
	{"hlt", {0x76, 0, 0, 1, false}},		// completed [1]
	{"add", {0x80, 2, 0, 0, false}},		// completed [9]
	{"adc", {0x88, 2, 0, 0, false}},		// completed [9]
	{"sub", {0x90, 2, 0, 0, false}},		// completed [9]
	{"sbc", {0x98, 2, 0, 0, false}},		// completed [9]
	{"and", {0xA0, 2, 0, 0, false}},		// completed [9]
	{"xor", {0xA8, 2, 0, 0, false}},		// completed [9]
	{"or", {0xB0, 2, 0, 0, false}},			// completed [9]
	{"cp", {0xB8, 2, 0, 0, false}},			// completed [9]
	{"ret", {0xC0, 2, 0, 1, false}},		//
	{"reti", {0xD9, 0, 0, 1, false}},		// completed [1]
	{"jp", {0xC0, 2, 0, 3, false}},			//
	{"call", {0xC4, 2, 0, 3, false}},		//
	{"rst", {0xC7, 1, 0, 1, false}},		// completed [8]
	{"pop", {0xC1, 1, 0, 1, false}},		// completed [4]
	{"push", {0xC5, 1, 0, 1, false}},		// completed [4]
	{"ldh", {0xE0, 2, 0, 1, false}},		//
	{"di", {0xf3, 0, 0, 1, false}},			// completed [1]
	{"ei", {0xfb, 0, 0, 1, false}},			// completed [1]
	{"rlc", {0x00, 1, 0, 1, true}},			// completed [8]
	{"rrc", {0x08, 1, 0, 1, true}},			// completed [8]
	{"rl", {0x10, 1, 0, 1, true}},			// completed [8]
	{"rr", {0x18, 1, 0, 1, true}},			// completed [8]
	{"sla", {0x20, 1, 0, 1, true}},			// completed [8]
	{"sra", {0x28, 1, 0, 1, true}},			// completed [8]
	{"swap", {0x30, 1, 0, 1, true}},		// completed [8]
	{"srl", {0x38, 1, 0, 1, true}},			// completed [8]
	{"bit", {0x40, 2, 0, 1, true}},			// completed [64]
	{"res", {0x80, 2, 0, 1, true}},			// completed [64]
	{"set", {0xC0, 2, 0, 1, true}}			// completed [64]
};

map<string, uint64_t> label;
set<string> dot_directives = {
	".org",
	".byte",
	".word",
	".metadata:",
	".metadata"
};

map<char, int> hex_to_dec = {
	{'0', 0},
	{'1', 1},
	{'2', 2},
	{'3', 3},
	{'4', 4},
	{'5', 5},
	{'6', 6},
	{'7', 7},
	{'8', 8},
	{'9', 9},
	{'a', 10},
	{'b', 11},
	{'c', 12},
	{'d', 13},
	{'e', 14},
	{'f', 15}
};

uint8_t toByte(string s)
{
	uint8_t Byte = 0;
	switch (s.at(0))
	{
	case '$':
		for (int i = 1; i < s.length(); i++)
		{
			Byte <<= 4;
			if ((s[i] >= '0' and s[i] <= '9') or (s[i] >= 'a' and s[i] <= 'f'))Byte += hex_to_dec[s[i]];
			else throw s[i];
		} break;
	case '%':
		for (int i = 1; i < s.length(); i++)
		{
			Byte <<= 1;
			if (s[i] == '1' or s[i] == '0')Byte += s[i] - '0';
			else throw s[i];
		} break;
	default:
		for (int i = 0; i < s.length(); i++)
		{
			Byte *= 10;
			if (s[i] >= '0' and s[i] <= '9')Byte += s[i] - '0';
			else throw s[i];
		} break;
	}
	return Byte;
}

uint16_t toWord(string s)
{
	uint16_t Byte = 0;
	switch (s.at(0))
	{
	case '$':
		for (int i = 1; i < s.length(); i++)
		{
			Byte <<= 4;
			if ((s[i] >= '0' and s[i] <= '9') or (s[i] >= 'a' and s[i] <= 'f'))Byte += hex_to_dec[s[i]];
			else throw s[i];
		} break;
	case '%':
		for (int i = 1; i < s.length(); i++)
		{
			Byte <<= 1;
			if (s[i] == '1' or s[i] == '0')Byte += s[i] - '0';
			else throw s[i];
		} break;
	default:
		for (int i = 0; i < s.length(); i++)
		{
			Byte *= 10;
			if (s[i] >= '0' and s[i] <= '9')Byte += s[i] - '0';
			else throw "Unexpected character " + s[i];
		} break;
	}
	return Byte;
}

void toLower(string& s)
{
	for (auto& c : s)
		if (c >= 'A' and c <= 'Z') c += 32;
}

void removeGrammar(string& s) {
	string cleaned;
	bool insideBrackets = false;
	for (char c : s) {
		if (c == '[') insideBrackets = true;
		if (c == ']') insideBrackets = false;

		if (c == ',') {
			cleaned += ' ';
		}
		else if (isspace(c) && insideBrackets) {
			continue;
		}
		else {
			cleaned += c;
		}
	}
	s = cleaned;
}

void removeComments(string& s)
{
	s = s.substr(0, s.find_first_of(';'));
}

vector<string>* line_vec(string& s)
{
	removeComments(s);
	toLower(s);
	removeGrammar(s);
	stringstream ss(s);
	auto line = new vector<string>();
	string word;
	while (ss >> word) line->push_back(word);
	return line;
}

void process_dot(vector<string>& line, char* rom, int& address) {
	if (line.empty()) return;

	string directive = line[0];

	if (directive == ".org")
	{
		if (line.size() < 2) { throw "Missing address for .org"; }
		string val = line[1];
		address = toWord(val);
		cout << "Address label:" << address << '\n';
	}
	else if (directive == ".byte" || directive == ".db")
	{
		for (size_t i = 1; i < line.size(); ++i)
		{
			rom[address++] = toByte(line[i]);
		}
	}
	else if (directive == ".word" || directive == ".dw")
	{
		for (size_t i = 1; i < line.size(); ++i)
		{
			uint16_t value = toWord(line[i]);
			rom[address++] = value & 0xFF;
			rom[address++] = (value >> 8) & 0xFF;
		}
	}
	else if (directive.at(0) == '.' and directive.back() == ':')
	{
		label[directive] = address;
	}
	else throw "Given directive is not valid";
}

void parse_line(vector<string>* line, char* rom)
{
	if (line->empty()) return;
	static int line_index = 0;
	static int address = 0;
	line_index++;
	if (line->at(0)[0] == '.') process_dot((*line), rom, address);
	else
	{
		string mnemonics = (*line)[0];
		if (opcodes.contains(mnemonics))
		{
			ins ins_data = opcodes.at(mnemonics);
			if (ins_data.prefixed)
			{
				rom[address] = 0xCB;
				address++;
				uint8_t opdata = ins_data.opcode;
				switch (ins_data.opreand_count)
				{
				case 1:
					opdata |= r8.at((*line)[1]);
					break;
				case 2:
					opdata |= (toByte((*line)[1]) & 0x07) << 3;
					opdata |= r8.at((*line)[2]);
					break;
				}
				rom[address] = opdata;
				address++;
			}
			else
			{
				if (ins_data.opreand_count == 0)
				{
					rom[address] = ins_data.opcode;
					address++;
				}
				else if ((ins_data.opcode & 0xCF) == 0x80)
				{
					if (line->at(1) == "hl" and mnemonics == "add")
					{
						uint8_t opdata = 0x09;
						if (r16.contains(line->at(2))) opdata |= (r16.at(line->at(2)) << 4);
						else throw line->at(2) + ": invalid operand";
						return;
					}
					else if (line->at(2) == "sp" and mnemonics == "add")
					{
					}
					uint8_t opdata = ins_data.opcode;
					if (r8.at((*line)[1]) != 7)
					{
						throw "First operand " + mnemonics + " must be register A.";
					}
					if (r8.contains((*line)[2]))
					{
						opdata |= r8.at((*line)[2]);
						rom[address] = opdata;
						address++;
					}
					else
					{
						opdata |= 0x46;
						rom[address] = opdata;
						address++;
						rom[address] = toByte((*line).at(2));
						address++;
					}
				}
				else if (ins_data.opreand_count == 1)
				{
					uint8_t opdata = ins_data.opcode;
					switch (opdata)
					{
					case 0x00:
						if (r8.contains(line->at(1))) opdata = 0x04 | (r8.at(line->at(1)) << 3) | (mnemonics == "dec");
						else if (r16.contains(line->at(1))) opdata = (0x03) | (r16.at(line->at(1)) << 4) + ((mnemonics == "dec") ? 8 : 0);
						else throw "Invalid register representation";
						break;
					case 0xC1:
					case 0xC5:
						opdata |= (r16stk.at(line->at(1)) << 4);
						break;
					case 0xC7:
						opdata |= rst_tgt.at(toByte(line->at(1))) << 3;
						break;
					default:
						throw "Invalid instructions.";
						break;
					}
					rom[address] = opdata;
					address++;
				}
				else if (ins_data.opreand_count == 2)
				{
					if (mnemonics == "jr")
					{
					}
					else if (mnemonics == "jp")
					{
					}
					else if (mnemonics == "ld")
					{
					}
					else if (mnemonics == "call")
					{
					}
					else if (mnemonics == "ret")
					{
					}
					else if (mnemonics == "ldh")
					{
					}
				}
			}
		}
		else
		{
			throw "Invalid opcode";
		}
	}
}