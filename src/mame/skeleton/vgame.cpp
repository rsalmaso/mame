// license:BSD-3-Clause
// copyright-holders:

/*
Video slots / mahjongs by VGame.

The main components are:
rectangular 128-pin chip, marked VGAME-007 (probably CPU)
square 208-pin chip, marked VGAME-008 (probably video chip)
44 MHz XTAL
LY62256SL SRAM (near VGAME-007)
2x LY61L256JL SRAM (near VGAME-008)
U6295 sound chip
2 banks of 8 DIP switches

TODO: everything. CPU core isn't identified and code is encrypted. Possibly internal ROM?
*/


#include "emu.h"

#include "cpu/m68000/m68000.h"
#include "sound/okim6295.h"

#include "emupal.h"
#include "screen.h"
#include "speaker.h"
#include "tilemap.h"


namespace {

class vgame_state : public driver_device
{
public:
	vgame_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu")
	{ }

	void vgame(machine_config &config) ATTR_COLD;

	void init_hilice() ATTR_COLD;

protected:
	virtual void video_start() override ATTR_COLD;

private:
	required_device<cpu_device> m_maincpu;

	uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);

	void program_map(address_map &map) ATTR_COLD;
};


uint32_t vgame_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{

	return 0;
}

void vgame_state::video_start()
{
}


void vgame_state::program_map(address_map &map)
{
	map(0x000000, 0x1fffff).rom();
}


static INPUT_PORTS_START( hilice )
	PORT_START("IN0")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("IN1")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("IN2")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("DSW1")
	PORT_DIPUNKNOWN_DIPLOC(0x0001, 0x0001, "SW1:1")
	PORT_DIPUNKNOWN_DIPLOC(0x0002, 0x0002, "SW1:2")
	PORT_DIPUNKNOWN_DIPLOC(0x0004, 0x0004, "SW1:3")
	PORT_DIPUNKNOWN_DIPLOC(0x0008, 0x0008, "SW1:4")
	PORT_DIPUNKNOWN_DIPLOC(0x0010, 0x0010, "SW1:5")
	PORT_DIPUNKNOWN_DIPLOC(0x0020, 0x0020, "SW1:6")
	PORT_DIPUNKNOWN_DIPLOC(0x0040, 0x0040, "SW1:7")
	PORT_DIPUNKNOWN_DIPLOC(0x0080, 0x0080, "SW1:8")
	PORT_DIPUNKNOWN_DIPLOC(0x0100, 0x0100, "SW2:1")
	PORT_DIPUNKNOWN_DIPLOC(0x0200, 0x0200, "SW2:2")
	PORT_DIPUNKNOWN_DIPLOC(0x0400, 0x0400, "SW2:3")
	PORT_DIPUNKNOWN_DIPLOC(0x0800, 0x0800, "SW2:4")
	PORT_DIPUNKNOWN_DIPLOC(0x1000, 0x1000, "SW2:5")
	PORT_DIPUNKNOWN_DIPLOC(0x2000, 0x2000, "SW2:6")
	PORT_DIPUNKNOWN_DIPLOC(0x4000, 0x4000, "SW2:7")
	PORT_DIPUNKNOWN_DIPLOC(0x8000, 0x8000, "SW2:8")
INPUT_PORTS_END


// TODO
static GFXDECODE_START( gfx_vgame )
GFXDECODE_END


void vgame_state::vgame(machine_config &config)
{
	M68000(config, m_maincpu, 44_MHz_XTAL); // CPU core and divider unknown
	m_maincpu->set_addrmap(AS_PROGRAM, &vgame_state::program_map);
	// m_maincpu->set_vblank_int("screen", FUNC(vgame_state::irq0_line_hold));

	screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_RASTER)); // TODO
	screen.set_refresh_hz(60);
	screen.set_vblank_time(ATTOSECONDS_IN_USEC(0));
	screen.set_size(64*8, 64*8);
	screen.set_visarea(0, 64*8-1, 0, 32*8-1);
	screen.set_screen_update(FUNC(vgame_state::screen_update));

	GFXDECODE(config, "gfxdecode", "palette", gfx_vgame);

	PALETTE(config, "palette").set_entries(0x100); // TODO

	SPEAKER(config, "mono").front_center();

	OKIM6295(config, "oki", 44_MHz_XTAL / 44, okim6295_device::PIN7_HIGH).add_route(ALL_OUTPUTS, "mono", 1.0); // pin 7 and clock not verified
}

// VGAME-0030-02-AI PCB
ROM_START( hilice )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD( "hi_lice_v102fa.u13", 0x000000, 0x200000, CRC(4da87481) SHA1(5a20b254cfe8a2f087faa0dd17f682218a2ca1b2) )

	ROM_REGION( 0x400000, "gfx", 0 )
	ROM_LOAD( "hi_lice_cg_01fu3.u3", 0x000000, 0x200000, CRC(8ad6b233) SHA1(deaffd391265c885afb2f171089c1b33429470f1) )
	ROM_LOAD( "hi_lice_cg_01fu8.u8", 0x200000, 0x200000, BAD_DUMP CRC(b1070209) SHA1(4568977fca2ff96b756a9600ad9a4730a6f8749a) ) // didn't give consistent reads

	ROM_REGION( 0x200000, "oki", 0 )
	ROM_LOAD( "hi_lice_sp_100f.u45", 0x000000, 0x200000, CRC(b2588f54) SHA1(0d046e56596611954a9d2a9a30746d8aa370431b) ) // 1xxxxxxxxxxxxxxxxxxxx = 0x00
ROM_END

// VGAME-0030-02-AG PCB, almost identical to the hilice one.
// all labels have 麻將學園 (Mahjong Academy) prepended to what's below.
ROM_START( mjxy )
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "u12_v108tw.u12", 0x000000, 0x400000, BAD_DUMP CRC(c9989af0) SHA1(0edd65518688602810549955e79ad3f0edc0b1a5) ) // xxxxxxxxxxxxxxxxxxxx1x = 0xFF

	ROM_REGION( 0x800000, "gfx", 0 )
	ROM_LOAD( "u3_cg_v105.u3", 0x000000, 0x400000, BAD_DUMP CRC(7ed9736c) SHA1(69a3ad01a0582de8b1f206921544063e6eb3b4b0) ) // xxxxxxxxxxxxxxxxxxxx1x = 0xFF
	ROM_LOAD( "u7_cg_v105.u7", 0x400000, 0x400000, BAD_DUMP CRC(177111ce) SHA1(3834e3613f59b938c69d5a8679ffc3ea6baf999a) ) // xxxxxxxxxxxxxxxxxxxx1x = 0xFF

	ROM_REGION( 0x200000, "oki", 0 )
	ROM_LOAD( "u43_sp_v105.u43", 0x000000, 0x200000, CRC(5d1ab8f1) SHA1(56473b632dfdb210208ce3b35cb6861f07861cd7) )
ROM_END


void vgame_state::init_hilice()
{
	// TODO: decryption
}

} // anonymous namespace


GAME( 200?, hilice,  0, vgame, hilice, vgame_state, init_hilice, ROT0, "VGame", "Hi Lice (V102FA)",         MACHINE_NO_SOUND | MACHINE_NOT_WORKING )
GAME( 200?, mjxy,    0, vgame, hilice, vgame_state, init_hilice, ROT0, "VGame", "Majiang Xueyuan (V108TW)", MACHINE_NO_SOUND | MACHINE_NOT_WORKING )
