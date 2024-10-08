// license:BSD-3-Clause
// copyright-holders:R. Belmont
/***************************************************************************

    Peripheral Technology PT68K2/PT68K4 family

    2011-01-03 Skeleton driver.
    2013-09-30 Connected to a terminal
    2014-01-03 Connect real DUARTs, FDC, and TimeKeeper.  Settings now save properly, floppies can be read.
    2014-01-19 ISA bus and compatible cards, PC keyboard support, speaker support
    2014-09-20 Add PT68K2, add save states, we have a working SK*DOS disk!

This has the appearance of a PC, including pc power supply, slots, etc
on a conventional pc-like motherboard and case.

Some pics: http://www.wormfood.net/old_computers/

Source code and manuals for the HUMBUG BIOS and SK*DOS are at:
http://www.users.cloud9.net/~stark/sources.html

Usage:
    Start up and press Enter as prompted.  Type he to see a command list, or fd to boot from the
    first floppy drive.

    The stock NVRAM configures PT68k2 for 2 DSDD 5.25" drives, and PT68k4 for 2 DSHD 5.25" drives.

Chips:
    68230 Parallel Interface/Timer @ FE0081
    68681 DUART/Timer (x2) @ FE0001 and FE0041
    WD37C65 FDC (PC FDC compatible, even mapped as an ISA device)
    MK48T02 TimeKeeper @ odd bytes from FF0001 to FF0FFF.  even bytes in that range are a standard SRAM chip which is not backed up.
    Keyboard at FE01C1 (status/IRQ clear)/FE01C3 (AT scan codes)
    WD1002 HDD controller @ FE0141-FE014F.  "Monk" BIOS also supports an 8-bit ISA IDE card.

Video: ISA MDA or CGA/EGA/VGA-style boards
    ISA memory is C00001-DFFFFF odd bytes only.  So the MDA B0000 framebuffer becames (B0000*2) + C00001 = D60001.
    ISA I/O is at FA0001-FBFFFF odd bytes only, and the mapping is similar.

    HUMBUG BIOS tests MDA and CGA VRAM to determine existence, falls back to serial console if neither exists.  If both exist, MDA is used.
    VRAM is every other byte for ISA cards.  (Only 8 bit cards are supported).

    OP3 on DUART1 drives a speaker.
    IP2 on DUART1 signals if a new keyboard scan code is available.

IRQs:
    2: 68230 PIT
    4: DUART2
    5: DUART1
    6: PC FDC IRQ

TODO: 68230 device
      This system and SK*DOS don't like our ISA WDXT-GEN emulation so HDD installs are not currently possible.

****************************************************************************/

#include "emu.h"

#include "cpu/m68000/m68000.h"
#include "machine/mc68681.h"
#include "machine/timekpr.h"
#include "machine/wd_fdc.h"
#include "sound/spkrdev.h"

#include "bus/isa/cga.h"
#include "bus/isa/ega.h"
#include "bus/isa/fdc.h"
#include "bus/isa/isa.h"
#include "bus/isa/isa_cards.h"
#include "bus/isa/lpt.h"
#include "bus/isa/mda.h"
#include "bus/isa/vga.h"
#include "bus/isa/wdxt_gen.h"
#include "bus/isa/xtide.h"
#include "bus/pc_kbd/keyboards.h"
#include "bus/pc_kbd/pc_kbdc.h"

#include "softlist.h"
#include "speaker.h"


namespace {

#define M68K_TAG "maincpu"
#define DUART1_TAG  "duart1"
#define DUART2_TAG  "duart2"
#define TIMEKEEPER_TAG  "timekpr"
#define ISABUS_TAG "isa"
#define SPEAKER_TAG "speaker"
#define WDFDC_TAG   "wdfdc"

class pt68k4_state : public driver_device
{
public:
	pt68k4_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_p_base(*this, "rambase")
		, m_maincpu(*this, M68K_TAG)
		, m_duart1(*this, DUART1_TAG)
		, m_duart2(*this, DUART2_TAG)
		, m_isa(*this, ISABUS_TAG)
		, m_speaker(*this, SPEAKER_TAG)
		, m_wdfdc(*this, WDFDC_TAG)
		, m_floppy_connector(*this, WDFDC_TAG":%u", 0U)
	{ }

	void pt68k2(machine_config &config);
	void pt68k4(machine_config &config);

protected:
	virtual void machine_start() override ATTR_COLD;
	virtual void machine_reset() override ATTR_COLD;

private:
	uint8_t hiram_r(offs_t offset);
	void hiram_w(offs_t offset, uint8_t data);
	uint8_t keyboard_r(offs_t offset);
	void keyboard_w(uint8_t data);

	uint8_t pia_stub_r();
	void duart1_out(uint8_t data);

	void fdc_select_w(uint8_t data);

	void duart1_irq(int state);
	[[maybe_unused]] void duart2_irq(int state);

	void irq5_w(int state);

	void keyboard_clock_w(int state);
	void keyboard_data_w(int state);

	void pt68k2_mem(address_map &map) ATTR_COLD;
	void pt68k4_mem(address_map &map) ATTR_COLD;

	required_shared_ptr<uint16_t> m_p_base;
	required_device<cpu_device> m_maincpu;
	required_device<mc68681_device> m_duart1;
	required_device<mc68681_device> m_duart2;
	required_device<isa8_device> m_isa;
	required_device<speaker_sound_device> m_speaker;
	optional_device<wd1772_device> m_wdfdc;
	optional_device_array<floppy_connector, 2> m_floppy_connector;

	void irq5_update();

	uint8_t m_hiram[0x800]{};

	bool m_kclk = false;
	uint8_t m_kdata = 0;
	uint8_t m_scancode = 0;
	uint8_t m_kbdflag = 0;
	int m_kbit = 0;
	int m_lastdrive = 0;
	bool m_irq5_duart1 = false, m_irq5_isa = false;
};

static void pt68k_floppies(device_slot_interface &device)
{
	device.option_add("525dd", FLOPPY_525_DD);
}

// XT keyboard interface - done in TTL instead of an 804x
void pt68k4_state::keyboard_clock_w(int state)
{
//  printf("KCLK: %d\n", state ? 1 : 0);

	// rising edge?
	if ((state == ASSERT_LINE) && (!m_kclk))
	{
		if (m_kbit >= 1 && m_kbit <= 8)
		{
			m_scancode >>= 1;
			m_scancode |= m_kdata;
		}

		// stop bit?
		if (m_kbit == 9)
		{
//          printf("scancode %02x\n", m_scancode);
			m_kbit = 0;
			m_kbdflag = 0x80;
			m_duart1->ip2_w(CLEAR_LINE);
		}
		else
		{
			m_kbit++;
		}
	}

	m_kclk = (state == ASSERT_LINE) ? true : false;
}

void pt68k4_state::keyboard_data_w(int state)
{
//  printf("KDATA: %d\n", state ? 1 : 0);
	m_kdata = (state == ASSERT_LINE) ? 0x80 : 0x00;
}

void pt68k4_state::duart1_out(uint8_t data)
{
	m_speaker->level_w((data >> 3) & 1);
}

uint8_t pt68k4_state::pia_stub_r()
{
	return 0;
}

void pt68k4_state::fdc_select_w(uint8_t data)
{
	floppy_image_device *floppy = m_floppy_connector[0] ? m_floppy_connector[0]->get_device() : nullptr;
	floppy_image_device *floppy2 = m_floppy_connector[1] ? m_floppy_connector[1]->get_device() : nullptr;
	int drive = data & 3;

	if (drive != m_lastdrive)
	{
		switch (drive)
		{
			case 0:
				m_wdfdc->set_floppy(floppy);
				break;

			case 1:
				m_wdfdc->set_floppy(floppy2);
				break;

			default:
				m_wdfdc->set_floppy(nullptr);
				break;
		}

		m_lastdrive = drive;
	}

	switch (drive)
	{
		case 0:
			if (floppy)
				floppy->ss_w((data & 0x40) ? 1 : 0);
			break;

		case 1:
			if (floppy2)
				floppy2->ss_w((data & 0x40) ? 1 : 0);
			break;

		default:
			break;
	}
}

void pt68k4_state::pt68k2_mem(address_map &map)
{
	map.unmap_value_high();
	map(0x000000, 0x0fffff).ram().share("rambase"); // 1MB RAM
	map(0xf80000, 0xf8ffff).rom().region("roms", 0);
	map(0xc00000, 0xdfffff).rw(m_isa, FUNC(isa8_device::mem_r), FUNC(isa8_device::mem_w)).umask16(0x00ff);
	map(0xfa0000, 0xfbffff).rw(m_isa, FUNC(isa8_device::io_r), FUNC(isa8_device::io_w)).umask16(0x00ff);
	map(0xfe0000, 0xfe001f).rw(m_duart1, FUNC(mc68681_device::read), FUNC(mc68681_device::write)).umask16(0x00ff);
	map(0xfe0040, 0xfe005f).rw(m_duart2, FUNC(mc68681_device::read), FUNC(mc68681_device::write)).umask16(0x00ff);
	map(0xfe0080, 0xfe00bf).r(FUNC(pt68k4_state::pia_stub_r)).umask16(0x00ff);
	map(0xfe00c0, 0xfe00ff).w(FUNC(pt68k4_state::fdc_select_w)).umask16(0x00ff);
	map(0xfe0100, 0xfe013f).rw(m_wdfdc, FUNC(wd1772_device::read), FUNC(wd1772_device::write)).umask16(0x00ff);
	map(0xfe01c0, 0xfe01c3).rw(FUNC(pt68k4_state::keyboard_r), FUNC(pt68k4_state::keyboard_w)).umask16(0x00ff);
	map(0xff0000, 0xff0fff).rw(FUNC(pt68k4_state::hiram_r), FUNC(pt68k4_state::hiram_w)).umask16(0xff00);
	map(0xff0000, 0xff0fff).rw(TIMEKEEPER_TAG, FUNC(timekeeper_device::read), FUNC(timekeeper_device::write)).umask16(0x00ff);
}

void pt68k4_state::pt68k4_mem(address_map &map)
{
	map.unmap_value_high();
	map(0x000000, 0x0fffff).ram().share("rambase"); // 1MB RAM (OS9 needs more)
	map(0xf80000, 0xf8ffff).rom().region("roms", 0);
	map(0xc00000, 0xdfffff).rw(m_isa, FUNC(isa8_device::mem_r), FUNC(isa8_device::mem_w)).umask16(0x00ff);
	map(0xfa0000, 0xfbffff).rw(m_isa, FUNC(isa8_device::io_r), FUNC(isa8_device::io_w)).umask16(0x00ff);
	map(0xfe0000, 0xfe001f).rw(m_duart1, FUNC(mc68681_device::read), FUNC(mc68681_device::write)).umask16(0x00ff);
	map(0xfe0040, 0xfe005f).rw(m_duart2, FUNC(mc68681_device::read), FUNC(mc68681_device::write)).umask16(0x00ff);
	map(0xfe01c0, 0xfe01c3).rw(FUNC(pt68k4_state::keyboard_r), FUNC(pt68k4_state::keyboard_w)).umask16(0x00ff);
	map(0xff0000, 0xff0fff).rw(FUNC(pt68k4_state::hiram_r), FUNC(pt68k4_state::hiram_w)).umask16(0xff00);
	map(0xff0000, 0xff0fff).rw(TIMEKEEPER_TAG, FUNC(timekeeper_device::read), FUNC(timekeeper_device::write)).umask16(0x00ff);
}

/* Input ports */
static INPUT_PORTS_START( pt68k4 )
INPUT_PORTS_END

/* built in keyboard: offset 0 reads 0x80 if key ready, 0 if not.  If key ready, offset 1 reads scancode.  Read or write to offs 0 clears key ready */
uint8_t pt68k4_state::keyboard_r(offs_t offset)
{
	if (offset == 0)
	{
		uint8_t rv = m_kbdflag;

		m_kbdflag = 0;
		m_duart1->ip2_w(ASSERT_LINE);

		return rv;
	}

	return m_scancode;
}

void pt68k4_state::keyboard_w(uint8_t data)
{
	m_kbdflag = 0;
	m_duart1->ip2_w(ASSERT_LINE);
}

uint8_t pt68k4_state::hiram_r(offs_t offset)
{
	return m_hiram[offset];
}

void pt68k4_state::hiram_w(offs_t offset, uint8_t data)
{
	m_hiram[offset] = data;
}

void pt68k4_state::machine_start()
{
	save_item(NAME(m_hiram));
	save_item(NAME(m_kclk));
	save_item(NAME(m_kdata));
	save_item(NAME(m_scancode));
	save_item(NAME(m_kbdflag));
	save_item(NAME(m_kbit));
	save_item(NAME(m_lastdrive));
	save_item(NAME(m_irq5_duart1));
	save_item(NAME(m_irq5_isa));

	m_irq5_isa = false;
}

void pt68k4_state::machine_reset()
{
	uint8_t* user1 = memregion("roms")->base();
	memcpy((uint8_t*)m_p_base.target(), user1, 8);

	m_kclk = true;
	m_kbit = 0;
	m_scancode = 0;
	m_kbdflag = 0;
	m_irq5_duart1 = CLEAR_LINE;
	m_irq5_isa = CLEAR_LINE;

	// set line to asserted (no key code ready)
	m_duart1->ip2_w(ASSERT_LINE);

	if (m_wdfdc)
	{
		floppy_image_device *floppy = m_floppy_connector[0] ? m_floppy_connector[0]->get_device() : nullptr;

		m_wdfdc->set_floppy(floppy);

		if (floppy)
			floppy->ss_w(0);

		m_lastdrive = 0;
	}
}

void pt68k4_state::irq5_update()
{
	if ((m_irq5_duart1) || (m_irq5_isa))
	{
		m_maincpu->set_input_line(M68K_IRQ_5, ASSERT_LINE);
	}
	else
	{
		m_maincpu->set_input_line(M68K_IRQ_5, CLEAR_LINE);
	}
}

void pt68k4_state::duart1_irq(int state)
{
	m_irq5_duart1 = state;
	irq5_update();
}

void pt68k4_state::irq5_w(int state)
{
	m_irq5_isa = state;
	irq5_update();
}

void pt68k4_state::duart2_irq(int state)
{
	m_maincpu->set_input_line(M68K_IRQ_4, state);
}

// these are cards supported by the HUMBUG and Monk BIOSes
void pt68k4_isa8_cards(device_slot_interface &device)
{
	device.option_add("mda", ISA8_MDA);
	device.option_add("cga", ISA8_CGA);
	device.option_add("ega", ISA8_EGA); // Monk only
	device.option_add("vga", ISA8_VGA); // Monk only
	device.option_add("fdc_at", ISA8_FDC_AT);
	device.option_add("wdxt_gen", ISA8_WDXT_GEN);
	device.option_add("lpt", ISA8_LPT);
	device.option_add("xtide", ISA8_XTIDE); // Monk only
}

void pt68k4_state::pt68k2(machine_config &config)
{
	/* basic machine hardware */
	M68000(config, m_maincpu, 16_MHz_XTAL / 2);    // 68k2 came in 8, 10, and 12 MHz versions
	m_maincpu->set_addrmap(AS_PROGRAM, &pt68k4_state::pt68k2_mem);

	MC68681(config, m_duart1, 3.6864_MHz_XTAL);
	m_duart1->irq_cb().set(FUNC(pt68k4_state::duart1_irq));
	m_duart1->outport_cb().set(FUNC(pt68k4_state::duart1_out));

	MC68681(config, m_duart2, 3.6864_MHz_XTAL);

	pc_kbdc_device &pc_kbdc(PC_KBDC(config, "kbd", pc_xt_keyboards, STR_KBD_IBM_PC_XT_83));
	pc_kbdc.out_clock_cb().set(FUNC(pt68k4_state::keyboard_clock_w));
	pc_kbdc.out_data_cb().set(FUNC(pt68k4_state::keyboard_data_w));

	M48T02(config, TIMEKEEPER_TAG, 0);

	WD1772(config, m_wdfdc, 16_MHz_XTAL / 2);
	FLOPPY_CONNECTOR(config, m_floppy_connector[0], pt68k_floppies, "525dd", floppy_image_device::default_mfm_floppy_formats);
	FLOPPY_CONNECTOR(config, m_floppy_connector[1], pt68k_floppies, "525dd", floppy_image_device::default_mfm_floppy_formats);

	ISA8(config, m_isa, 0);
	m_isa->set_custom_spaces();
	m_isa->irq5_callback().set(FUNC(pt68k4_state::irq5_w));

	ISA8_SLOT(config, "isa1", 0, m_isa, pt68k4_isa8_cards, "cga", false); // FIXME: determine ISA bus clock
	ISA8_SLOT(config, "isa2", 0, m_isa, pt68k4_isa8_cards, nullptr, false);
	ISA8_SLOT(config, "isa3", 0, m_isa, pt68k4_isa8_cards, nullptr, false);
	ISA8_SLOT(config, "isa4", 0, m_isa, pt68k4_isa8_cards, nullptr, false);
	ISA8_SLOT(config, "isa5", 0, m_isa, pt68k4_isa8_cards, nullptr, false);
	ISA8_SLOT(config, "isa6", 0, m_isa, pt68k4_isa8_cards, nullptr, false),

	SPEAKER(config, "mono").front_center();
	SPEAKER_SOUND(config, m_speaker).add_route(ALL_OUTPUTS, "mono", 1.00);

	SOFTWARE_LIST(config, "flop525_list").set_original("pt68k2");
}

void pt68k4_state::pt68k4(machine_config &config)
{
	/* basic machine hardware */
	M68000(config, m_maincpu, XTAL(16'000'000));
	m_maincpu->set_addrmap(AS_PROGRAM, &pt68k4_state::pt68k4_mem);

	// add the DUARTS.  first one has the console on channel A at 19200.
	MC68681(config, m_duart1, XTAL(16'000'000) / 4);
	m_duart1->irq_cb().set(FUNC(pt68k4_state::duart1_irq));
	m_duart1->outport_cb().set(FUNC(pt68k4_state::duart1_out));

	MC68681(config, m_duart2, XTAL(16'000'000) / 4);

	pc_kbdc_device &pc_kbdc(PC_KBDC(config, "kbd", pc_xt_keyboards, STR_KBD_IBM_PC_XT_83));
	pc_kbdc.out_clock_cb().set(FUNC(pt68k4_state::keyboard_clock_w));
	pc_kbdc.out_data_cb().set(FUNC(pt68k4_state::keyboard_data_w));

	M48T02(config, TIMEKEEPER_TAG, 0);

	ISA8(config, m_isa, 0);
	m_isa->set_custom_spaces();

	ISA8_SLOT(config, "isa1", 0, m_isa, pt68k4_isa8_cards, "fdc_at", false); // FIXME: determine ISA bus clock
	ISA8_SLOT(config, "isa2", 0, m_isa, pt68k4_isa8_cards, "cga", false);
	ISA8_SLOT(config, "isa3", 0, m_isa, pt68k4_isa8_cards, nullptr, false);
	ISA8_SLOT(config, "isa4", 0, m_isa, pt68k4_isa8_cards, nullptr, false);
	ISA8_SLOT(config, "isa5", 0, m_isa, pt68k4_isa8_cards, nullptr, false);
	ISA8_SLOT(config, "isa6", 0, m_isa, pt68k4_isa8_cards, nullptr, false),
	ISA8_SLOT(config, "isa7", 0, m_isa, pt68k4_isa8_cards, nullptr, false),

	SPEAKER(config, "mono").front_center();
	SPEAKER_SOUND(config, m_speaker).add_route(ALL_OUTPUTS, "mono", 1.00);

	SOFTWARE_LIST(config, "flop525_list").set_original("pt68k2");
}

/* ROM definition */
ROM_START( pt68k2 )
	ROM_REGION16_BE( 0x10000, "roms", 0 )
	ROM_LOAD16_BYTE( "hum_u20.bin",  0x000000, 0x008000, CRC(69db483a) SHA1(9dfea73e4d7deef7c66a27cca92eb7c9ff767215) )
	ROM_LOAD16_BYTE( "hum_u27.bin",  0x000001, 0x008000, CRC(54441b06) SHA1(0e2d63b1cd01f88f37fc4859c11c252c4fea220b) )

	ROM_REGION(0x800, TIMEKEEPER_TAG, 0)
	ROM_LOAD( "u21_ds1220.bin", 0x000000, 0x000800, CRC(7a6b75ce) SHA1(07663860aa6cc21aed04a568ff9c05bc75d62e4f) )
ROM_END

ROM_START( pt68k4 )
	ROM_REGION16_BE( 0x10000, "roms", 0 )
	ROM_SYSTEM_BIOS( 0, "humbug", "Humbug" )
	ROMX_LOAD( "humpta40.bin", 0x0000, 0x8000, CRC(af67ff64) SHA1(da9fa31338c6847bb0e66118679b1ec01f6dc30b), ROM_SKIP(1) | ROM_BIOS(0) )
	ROMX_LOAD( "humpta41.bin", 0x0001, 0x8000, CRC(a8b16e27) SHA1(218802f6e20d14cff736bb7423f06ce2f66e074c), ROM_SKIP(1) | ROM_BIOS(0) )
	ROM_SYSTEM_BIOS( 1, "monk", "Monk" )
	ROMX_LOAD( "monk_0.bin", 0x0000, 0x8000, CRC(420d6a4b) SHA1(fca8c53c9c3c8ebd09370499cf34f4cc75ed9463), ROM_SKIP(1) | ROM_BIOS(1) )
	ROMX_LOAD( "monk_1.bin", 0x0001, 0x8000, CRC(fc495e82) SHA1(f7b720d87db4d72a23e6c42d2cdd03216db04b60), ROM_SKIP(1) | ROM_BIOS(1) )

	ROM_REGION(0x800, TIMEKEEPER_TAG, 0)
	ROM_LOAD( "u21_ds1220_k4.bin", 0x000000, 0x000800, CRC(753472e6) SHA1(58dc8bcc86191e4a4429fe6a9b4fdd7788abb0cd) )

	ROM_REGION( 0x0900, "proms", 0 )
	ROM_LOAD_OPTIONAL( "20l8.u71",    0x0000, 0x000149, CRC(77365121) SHA1(5ecf490ead119966a5c097d90740acde60462ab0) )
	ROM_LOAD_OPTIONAL( "16l8.u53",    0x0200, 0x000109, CRC(cb6a9984) SHA1(45b9b14e7b45cda6f0edfcbb9895b6a14eacb852) )
	ROM_LOAD_OPTIONAL( "22v10.u40",   0x0400, 0x0002e1, CRC(24df92e4) SHA1(c183113956bb0db132b6f37b239ca0bb7fac2d82) )
	ROM_LOAD_OPTIONAL( "16l8.u11",    0x0700, 0x000109, CRC(397a1363) SHA1(aca2a02e1bf1f7cdb9b0ca24ebecb0b01ae472e8) )
ROM_END

} // Anonymous namespace


/* Driver */
//    YEAR  NAME    PARENT  COMPAT  MACHINE  INPUT   CLASS         INIT        COMPANY                  FULLNAME  FLAGS
COMP( 1988, pt68k2, 0,      0,      pt68k2,  pt68k4, pt68k4_state, empty_init, "Peripheral Technology", "PT68K2", MACHINE_SUPPORTS_SAVE )
COMP( 1990, pt68k4, 0,      0,      pt68k4,  pt68k4, pt68k4_state, empty_init, "Peripheral Technology", "PT68K4", MACHINE_SUPPORTS_SAVE )
