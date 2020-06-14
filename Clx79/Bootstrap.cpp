/**
MIT License

Copyright (c) 2020 Davide Inglima <limacat@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include "Bootstrap.h"
#include <stdint.h>
#include <iostream>
#include <fstream>

#include <boost/bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>

namespace memory_loader {

	void make_new_patch(const unsigned int destination, std::vector<Patch>& p) {
#ifdef DEBUG_PATCHING_MESSAGES
		std::cout << "make_new_patch: at " << destination << " vector_size: " << p.size() << std::endl;
#endif
		Patch new_patch;
		new_patch.destination = destination;
		p.push_back(new_patch);
	}

	Patch* get_current_patch(std::vector<Patch>& p) {
		if (p.size() == 0) {
#ifdef DEBUG_PATCHING_MESSAGES
			std::cout << "created a new patch" << std::endl;
#endif
			Patch new_patch;
			new_patch.destination = 0x0000;
			p.push_back(new_patch);
		}
		return &p[p.size() - 1];
	}

	void push_byte(const unsigned int value, std::vector<Patch>& p) {
#ifdef DEBUG_PATCHING_MESSAGES
		std::cout << "push_byte: value" << value << " vector_size: " << p.size() << std::endl;
#endif
		Patch* target = get_current_patch(p);
		target->content.push_back(value);
#ifdef DEBUG_PATCHING_MESSAGES
		std::cout << "push_byte: content_size now: " << target->content.size() << std::endl;
#endif
	}

	namespace spirit = boost::spirit;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;
	namespace repository = boost::spirit::repository;
	using boost::placeholders::_1;

	template <typename Iterator>
	bool parse_bytes(Iterator first, Iterator last, std::vector<unsigned int>& v, std::vector<Patch>& p)
	{

		qi::uint_parser<unsigned, 16, 4, 4> word_;

		// You can't use auto with Spirit v2, you need to wrap parsers in a qi::copy
		auto org_parser = qi::copy(qi::lit(".org") >> (
			qi::lexeme[qi::lit("$") >> word_[boost::bind(&make_new_patch, _1, boost::ref(p))]]
		));

		qi::uint_parser<unsigned, 16, 2, 2> byte_;
		qi::uint_parser<unsigned, 2, 8, 8> bitmask_;
		auto byte_parser = qi::copy(qi::lit(".byte") >> (
			qi::lexeme[qi::lit("#%") >> bitmask_[boost::bind(&push_byte, _1, boost::ref(p))]] |
			qi::lexeme[qi::lit("$") >> byte_[boost::bind(&push_byte, _1, boost::ref(p))]]
		) % qi::lit(','));

		auto asm_comment = qi::copy(repository::qi::confix(";", (spirit::eol | spirit::eoi))[*(qi::char_)]);

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

std::vector<Patch> manual_loader_test() {
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
	return p;
}


std::vector<Patch> file_loader_test(std::string filename) {
	std::vector<Patch> p;
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::vector<unsigned int> v;
			if (memory_loader::parse_bytes(line.begin(), line.end(), v, p))
			{
#ifdef DEBUG_PATCHING_MESSAGES
				std::cout << "-------------------------\n";
				std::cout << line << " Parses OK: " << std::endl;
				std::cout << "\n-------------------------\n";
#else
				;
#endif
			}
			else
			{
#ifdef DEBUG_PATCHING_MESSAGES
				std::cout << "-------------------------\n";
				std::cout << line << " Parsing failed\n";
				std::cout << "-------------------------\n";
#else
				;
#endif
			}
		}
		file.close();
	}
	else {
		std::cout << "File not found! :(" << std::endl;
	}
	return p;
}

