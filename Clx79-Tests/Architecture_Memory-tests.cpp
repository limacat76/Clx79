#define BOOST_TEST_MODULE Bootstrap
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <string>
#include "../Clx79/Host/Patcher.h"
#include "../Clx79/Architecture/Memory.h"

BOOST_AUTO_TEST_CASE(Canary)
{
	BOOST_TEST(1 == 1);
	BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(Host_Patcher_decode_patches)
{
	std::string program =
		"; hello I am a comment \n" 
		".org $002A \n" //
		".byte $43, $49, $53, $48 \n" //
		";.org $012A \n" //
		";.byte $43, $49, $53, $48 \n" //
		".byte $99, $AA, ; $BB, $CC ";

	std::vector<Patch> p;
	std::istringstream from(program);

	Host::Patcher::decode_patches(from, p);
	BOOST_CHECK_EQUAL(p.size(), 1);
	BOOST_CHECK_EQUAL(p[0].destination, 0x002A);
	BOOST_CHECK_EQUAL(p[0].content.size(), 6);
}

BOOST_AUTO_TEST_CASE(Memory_patch)
{
	std::string program =
		"; hello I am a comment \n"
		".org $002A \n" //
		".byte $43, $49, $53, $48 \n" //
		";.org $012A \n" //
		";.byte $43, $49, $53, $48 \n" //
		".byte $99, $AA, ; $BB, $CC ";

	std::vector<Patch> p;
	std::istringstream from(program);
	Host::Patcher::decode_patches(from, p);

	byte* ram = new byte[1024];
	clear(ram, 0x00, 1024);
	patch(ram, p[0]);

	BOOST_CHECK_EQUAL(ram[0x2A], 0x43);
	BOOST_CHECK_EQUAL(ram[0x2B], 0x49);
	BOOST_CHECK_EQUAL(ram[0x2C], 0x53);
	BOOST_CHECK_EQUAL(ram[0x2D], 0x48);

	for (unsigned i = 0x012A; i <= 0x012C; i++) {
		BOOST_CHECK_EQUAL(ram[i], 0x00);
	}

	BOOST_CHECK_EQUAL(ram[0x2E], 0x99);
	BOOST_CHECK_EQUAL(ram[0x2F], 0xAA);
	BOOST_CHECK_EQUAL(ram[0x30], 0x00);
	BOOST_CHECK_EQUAL(ram[0x31], 0x00);

}

