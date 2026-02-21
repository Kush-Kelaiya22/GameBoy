#include "ins.hpp"

int main(int argc, char** argv)
{
	string inFile = "main.s";
	string outFile = "out.bin";
	if (argc > 1) inFile = argv[1];
	if (argc > 2) outFile = argv[2];
	ifstream file(inFile);
	ofstream output(outFile, ios_base::binary);

	try
	{
		if (!file.is_open() or !output.is_open()) throw  "Unable to open " + ((file.is_open()) ? outFile : inFile);
		string instream;
		char* rom = new char[0x10000] {};
		while (getline(file, instream))
		{
			cout << instream << '\n';
			auto line = line_vec(instream);
			parse_line(line, rom);
			line->clear();
		}
		output.write(rom, 0x10000);
	}
	catch (string s)
	{
		cerr << s << '\n';
	}
	catch (const char* err)
	{
		cerr << err << '\n';
	}
	catch (...)
	{
		cerr << "Encountered unexpected error" << '\n';
	}

	output.close();
	file.close();

	return 0;
}