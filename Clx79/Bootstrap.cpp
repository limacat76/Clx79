#include "Bootstrap.h"
#include <string>

Patch hello_world() {
	Patch result;
	// byte* x =  new byte[14]{ 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x00 };
	byte* x = new byte[13]{ 0x00, 0x01, 0x02, 0x02, 0x03, 0x04, 0x05, 0x06, 0x03, 0x07, 0x02, 0x08, 0x09 };
	// H  e      l      l   o      ,            W    o     r     l    d      !
	result.content = x;
	result.size = 13;
	result.destination = 0x0000;
	return result;
}

Patch characters() {
	Patch result;
	byte* x = new byte[80]{

		// H 0
		0b00000000,
		0b01000010,
		0b01000010,
		0b01111110,
		0b01000010,
		0b01000010,
		0b01000010,
		0b00000000,

		// e 1
		0b00000000,
		0b00000000,
		0b00111100,
		0b01000010,
		0b01111110,
		0b01000000,
		0b00111110,
		0b00000000,

		// l 2
		0b00000000,
		0b01100000,
		0b00100000,
		0b00100000,
		0b00100000,
		0b00100000,
		0b01110000,
		0b00000000,

		// o 3
		0b00000000,
		0b00000000,
		0b00111100,
		0b01000010,
		0b01000010,
		0b01000010,
		0b00111100,
		0b00000000,

		// , 4
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00001100,
		0b00001100,
		0b00000100,
		0b00001000,

		// :space: 5
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,

		// w
		0b00000000,
		0b01000010,
		0b01000010,
		0b01000010,
		0b01011010,
		0b01011010,
		0b00100100,
		0b00000000,

		// r
		0b00000000,
		0b00000000,
		0b00111100,
		0b01000010,
		0b01000000,
		0b01000000,
		0b01000000,
		0b00000000,

		// d
		0b00000000,
		0b00000010,
		0b00000010,
		0b00111010,
		0b01000110,
		0b01000010,
		0b00111100,
		0b00000000,

		// !
		0b00000000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00000000,
		0b00010000,
		0b00000000,
	};
	result.content = x;
	result.size = 80;
	result.destination = 0x1000;
	return result;

}

std::vector<Patch> read_patches() {
	std::vector<Patch> result;
	result.push_back(hello_world());
	result.push_back(characters());
	return result;
}

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>

#include <iostream>
#include <vector>

namespace memory_loader {

	// TODO D.I
	void make_new_patch() {
		;
	}

	// TODO D.I
	Patch get_current_patch() {
		return hello_world();
	}

	// TODO D.I
	void push_byte() {
		;
	}


	namespace spirit = boost::spirit;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;
	namespace repository = boost::spirit::repository;


	// TODO D.I note the vector that was passed by reference
	template <typename Iterator>
	bool parse_bytes(Iterator first, Iterator last, std::vector<unsigned int>& v, std::vector<Patch>& p)
	{
		qi::uint_parser<unsigned, 16, 2, 2> byte_;
		qi::uint_parser<unsigned, 2, 8, 8> bitmask_;
		qi::uint_parser<unsigned, 16, 4, 4> word_;

		auto byte_parser = qi::lit(".byte") >> (
			qi::lexeme[qi::lit("#%") >> bitmask_[phoenix::push_back(phoenix::ref(v), qi::_1)]] |
			qi::lexeme[qi::lit("$") >> byte_[phoenix::push_back(phoenix::ref(v), qi::_1)]] 
		) % qi::lit(',');

		auto asm_comment = repository::qi::confix(";", (spirit::eol | spirit::eoi))[*(qi::char_)];

		auto org_parser = qi::lit(".org") >> (
			qi::lexeme[qi::lit("$") >> word_[phoenix::push_back(phoenix::ref(v), qi::_1)]]
		);

		bool r = qi::phrase_parse(first, last,

			(
				//  Begin grammar
				-( org_parser | byte_parser) >> -asm_comment
				//  End grammar
				),

			ascii::space
		);

		if (first != last)
			return false;

		return r;
	}

}

void manual_loader_test() {
	std::string str;
	std::vector<Patch> p;
	while (getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q') {
			std::cout << "bye!" << std::endl;
			break;
		}

		std::vector<unsigned int> v;
		if (memory_loader::parse_bytes(str.begin(), str.end(), v, p))
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded\n";
			std::cout << str << " Parses OK: " << std::endl;

			double toSum = 0.0;
			for (std::vector<unsigned int>::size_type i = 0; i < v.size(); ++i) {
				std::cout << i << ": " << v[i] << std::endl;
				toSum += v[i];
			}
			std::cout << "total: " << toSum;

			std::cout << "\n-------------------------\n";
		}
		else
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "-------------------------\n";
		}
	}
}