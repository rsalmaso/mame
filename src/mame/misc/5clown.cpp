// license:BSD-3-Clause
// copyright-holders:Roberto Fresca, Grull Osgo
/******************************************************************************


  FIVE CLOWN HARDWARE
  IGS - International Games System.

  Driver by Roberto Fresca & Grull Osgo.


  Games running on this hardware:

  * Five Clown (English, set 1), 1993, IGS.
  * Five Clown (English, set 2), 1993, IGS.
  * Five Clown (Spanish, hack),  1993, IGS.


  This hardware seems to be based on Bonanza's Golden Poker, but on steroids...
  Double CPU, FM sounds, samples system, different encryption schemes and other
  complex things mark the difference.

  Special Thanks to Iris Falbala that kindly offered herself as beta tester. :)


*******************************************************************************


  *** Hardware notes ***


  - CPU:  2x Rockwell R6502P.   (u16, u42)
  - CRTC: 1x GoldStar GM68B45S. (u10)
  - I/O:  2x ST EF6821P PIA's.  (u9, u15)

  - RAM:  1x GM76C28A-10 CMOS SRAM (2048 x 8, DIP). (u45)
          2x Hitachi 6116. (u1, u36)

  - CLK:  1x 10MHz. crystal.

  - ROMs: 1x M27C512.    (u50)
          1x M27256.     (u34)
          1x M5L2764.    (u35)
          1x DB (5,6,7). (u2)

  - PLDs: 1x PEEL 18CV8P. (u33)
          1x PALCE22V10H. (u43)
          2x PALCE16V6Q.  (u13, u14)

  - PROM: 1x Fairchild Semiconductor DM74S287AN. (u32)

  - SOUND: 1x Winbond WF19054 (AY8910 compatible). (u59)
           1x (unknown) A66 (OKI 6295 compatible). (u51)

           1x NEC Electronics UPC1242H (7W AF Power Amplifier). (u54)


  Other components:

  3x 8 DIP Switches Banks. (SW1, SW2, SW3)
  1x 4 DIP Switches Bank.  (SW4)
  1x Reset Switch.         (SW5)

  1x 3.6 Battery.
  1x (2x) 22-pins Edge Connector.
  1x (2x) 10-pins Edge Connector.

  1x 19 Diodes Matrix (near edge connector)



  PCB Layout:
   ____________________________________________________________________________________________
  |      ________    ________    ________    ________   ____       ___________   ___________   |
  |     |DS2003CN|  |12345678|  |12345678|  |12345678| |1234|     |           | |  27C512   |  |
  |     |________|  |________|  |________|  |________| |____|     |  HM-6116  | | ...or DB  |  |
  |      U30         SW1         SW2         SW3        SW4       |___________| |___________|  |
  |___   ________   _________   _________   _________   _________  U1            U2            |
      | | 74LS02 | | 74LS245 | | 74LS245 | | 74LS245 | | 74LS138 |     _________   _________   |
      | |________| |_________| |_________| |_________| |_________|    | 74LS245 | | 74LS245 |  |
      |  U31        U3          U4          U5          U6            |_________| |_________|  |
   ___|                ____________________    ____________________    U7          U8          |
  |___  22x2          |                    |  |                    |   ________    _________   |
  |___  CONN          |      EF6821P       |  |     GM68B45S       |  |74LS193?|  | 74LS374 |  |
  |___    __          |____________________|  |____________________|  |________|  |_________|  |
  |___   |__| D        U9                      U10                     U11         U12         |
  |___   |__| I        ____________________    ____________________    _________   _________   |
  |___   |__| O       |                    |  |                    |  |PALCE16V8| |PALCE16V8|  |
  |___   |__| D       |      EF6821P       |  |      R6502P        |  |_________| |_________|  |
  |___   |__| E       |____________________|  |____________________|   U13         U14         |
  |___   |__|          U15                     U16                     _________   _________   |
  |___   |__| M                                                       | 74LS166 | | 74LS245 |  |
  |___   |__| A        _________                                      |_________| |_________|  |
  |___   |__| T       | HD7407P |  ________                            U17         U18         |
  |___   |__| R       |_________| | 74LS04 |   ________    ________    _________   _________   |
  |___   |__| I        U19        |________|  | 74LS86 |  | 74LS36?|  | 74LS273 | | 74LS166 |  |
  |___   |__| X        _________   U20        |________|  |________|  |_________| |_________|  |
  |___   |__|         | HD7407P |              U21         U22         U23         U24         |
  |___   |__|         |_________|   _______    ___________             _________   _________   |
  |___   |__|          U25         | XTAL  |  | 74LS244   |           | 74LS273 | | 74LS166 |  |
  |___   |__|          _________   | 10MHz |  |___________|           |_________| |_________|  |
  |___   |__|         | 74LS174 |  |_______|   U26                     U27         U28         |
  |___   |__|         |_________|   X1                                                         |
  |___                 U29                   _____________    _____________     ____________   |
      |          __________    __________   |             |  |             |   |            |  |
      |         |DM74S287AN|  |PEEL18CV8P|  | [7] M27256  |  | [9] M5L2764 |   |  HM-6116   |  |
      |         |__________|  |__________|  |_____________|  |_____________|   |____________|  |
      |          U32           U33           U34              U35               U36            |
      |          ___________    _________    __________    ___________________    _________    |
      |         |  74LS273  |  | 74LS157 |  | 74LS273  |  |                   |  | 74LS367 |   |
      |         |___________|  |_________|  |__________|  |      R6502P       |  |_________|   |
      |          U37            U38          U39          |___________________|   U40          |
      |                         _________    ___________   U42                    _________    |
      |                        | 74LS157 |  |PALCE22V10H|       ______           | 74LS245 |   |
      | _______ ____________   |_________!  |___________|      |      |          |_________|   |
   ___||       |            |   U41          U43               | A66  |           U44          |
  |___ | 3.6v  |  GoldStar  |   _________       _____________  |(6295)| _________   _________  |
  |___ |BATTERY|  GM76C28A  |  | 74LS157 |     |             | |______|| 74LS04  | | PAL16L8 | |
  |___ |       |____________|  |_________|     | [8] M27C512 |  U51    |_________| |_________| |
  |___ |_______| U45             U46           |_____________|          U47         U48        |
  |___           _________   _______  _______   U50                     __________             |
  |___          | 74LS161 | |(NO IC)||       |       ________          | 74LS245  |            |
  |___          |_________| |  UPC  ||  UPC  |      |        |         |__________|            |
  |___           U49        | 1241H || 1242H |      | GL7805 |          U52                    |
  |___  10x2     _________  |_______||_______|      |________|          _________   _________  |
  |___  CONN    | 74HC132 |  U53      U54            U55               | 74LS374 | | 74LS174 | |
      |         |_________|                                            |_________| |_________| |
      |  _|_|_|_ U56                                                    U57         U58        |
      | |       |                                                       _____________________  |
   ___| |  SW5  |                                                      |                     | |
  |     |_______|                                                      |  WF19054 (AY8910)   | |
  |       |_|                                                          |_____________________| |
  |                                                                     U59                    |
  |____________________________________________________________________________________________|


  U58 = 74LS174 (Quad Latch).....
  U57 = 74LS374 (Octal Latch)..... Near to AY8910! :)



  DIP Switches Layout:

   SW1:                    SW2:                    SW3:                    SW4:
  +-------------------+   +-------------------+   +-------------------+   +-----------+
  | ON                |   | ON                |   | ON                |   | ON        |
  | +---------------+ |   | +---------------+ |   | +---------------+ |   | +-------+ |
  | |_|_|_|_|_|o|o|o| |   | |_|o|o|o|o|_|_|_| |   | |_|o|_|_|o|_|_|_| |   | |_|_|_|_| |
  | |o|o|o|o|o| | | | |   | |o| | | | |o|o|o| |   | |o| |o|o| |o|o|o| |   | |o|o|o|o| |
  | +---------------+ |   | +---------------+ |   | +---------------+ |   | +-------+ |
  |  1 2 3 4 5 6 7 8  |   |  1 2 3 4 5 6 7 8  |   |  1 2 3 4 5 6 7 8  |   |  1 2 3 4  |
  +-------------------+   +-------------------+   +-------------------+   +-----------+


*******************************************************************************


  MEMORY MAP
  ----------

  Main CPU:

  0000 - 07FF  NVRAM.

  0800 - 0800  MC6845 address.
  0801 - 0801  MC6845 register.

  0844 - 0847  PIA 0.
  0848 - 084B  PIA 1.

  1000 - 13FF  Video RAM (Init'ed at $2042).
  1800 - 1BFF  Color RAM (Init'ed at $2054).

  2000 - 7FFF  ROM space.

  C048 - C048  Unknown sound related writes.
  D800 - D800  Main/Sound latch.

  C400 - C400  DIP Switches bank: SW1.
  CC00 - CC00  DIP Switches bank: SW2.
  D400 - D400  DIP Switches bank: SW3.

  E000 - FFFF  ROM space.


*******************************************************************************


    Game Notes
    ----------

  - Checksums.

  The checksum routine starts at $FE1D
  Once checksumed the whole range, compares the result with a harcoded value.
  If match, jumps to a subroutine that prints "ROM x ERR".
  If doesn't match, prints "ROM x OK"

  So... It's inverted. Seems that someone replaced the BEQ's instructions with
  BNE's, to pass the checksums.

  $FE1D ---> checks ROM 5 (4000-5FFF)
  $FE44 ---> checks ROM 6 (2000-3FFF)
  $FE6B ---> checks ROM 7 (E000-FDFF)


*******************************************************************************


  Game Settings
  -------------

  From a datasheet. Seems to be unaccurate...


  DIP Switches Settings:

  DIP SW.1  Win-Up "7" Rate     1       2
            Lowest              on      on
            Low                 off     on
            Medium              on      off
            High                off     off

            Two Pair            3
            High                on
            Low                 off

            One Joker Rate      5       6
            Lowest              on      on
            Low                 on      off
            Medium              off     on
            High                off     off

            Three of a kind     7
            High                on
            Low                 off

  DIP SW.2  Key In              1       2
            10                  on      on
            20                  on      off
            50                  off     on
            100                 off     off

  DIP SW.3  2 Jokers Rate       1       8
            Lowest              on      on
            Low                 on      off
            Medium              off     on
            High                off     off

            3,4,5 Jokers Rate   2       3
            Lowest              on      on
            Low                 off     on
            Medium              on      off
            High                off     off

            Max Bet             4       5
            80                  on      on
            40                  on      off
            50                  off     on
            10                  off     off

  DIP SW.4  "J" Pair            1
            Win                 on
            Lose                off

            Pay Mode            2
            Key out             on
            Coin out            off

            Coin                3
            1 Coin 1 Credit     on
            1 Coin 10 Credit    off



  CHANCE SETTINGS
  ---------------

  Settings Table:

  -----+-----------------+-----------+-------------------------------------
  Loc. |    Function     | Val.Range |      Comment
  -----+-----------------+-----------+-------------------------------------
   A   | Five Jokers     |    0-8    | If 0 never comes out.
   B   | Royal Flush     |    0-8    | If 0 never comes out.
   C   | Five of a Kind  |    0-8    | If 0 never comes out.
   D   | Straight Flush  |    0-8    | If 0 never comes out.
   E   | Four of a Kind  |    0-8    | If 0 never comes out.
   F   | Full House      |    0-8    | 0 Lowest -  8 Highest.
   G   | Flush           |    0-8    | 0 Lowest -  8 Highest.
   H   | Straight        |    0-8    | 0 Lowest -  8 Highest.
   I   | W-Up            |    0-10   | 0 Lowest - 10 Highest.
   J   | Bet Limit       |    0-1    | 0 Over bet setting never comes out.
       |                 |           | 1 Below bet setting never comes out.


  Chance Settings Screen:

              A   B   C   D   E   F   G   H   I   J
  Value       0   0   4   3   5   6   6   7   10  0
  Bet Limit   0   0   50  50  50  -   -   -   -   -

  Bet Range: 1 to 80 Credits.



  Chance Settings Key Function:

  BIG:     Move left
  SMALL:   Move right
  START:   Increase value.
  COLLECT: Decrease value.
  HOLD 5:  Exit.



  To access Chance Settings, follow the following directions precisely:

  From the game screen, start with 0 credits remaining.
  Press SETTING to get a black screen. (HOLD 5 exits back to the game from here.)
  From this black screen, press BET to get a crosshatch pattern.
  Press PAYOUT once. (The screen will not change.)
  Then press RECORD 5 times, then HOLD 5 7 times, then SETTING 2 times.
  If this sequence was successfully entered, the screen will now go black.
  Wait a few seconds for the settings table to appear.
  (If you get stuck at the crosshatch screen, HOLD 1 will return to the game.)


  Settings Suggestions
  --------------------

  Suggestion One:

    Dip.Sw. Setting
    Dip.Sw. 2   3=On    4=On    Others=Off
    Dip.Sw. 3   2=On    4=On    Others=Off

    Chance Setting
    A   B   C   D   E   F   G   H   I   J
    1   2   2   4   6   7   8   8   8   0
    5   10  20  40  40  -   -   -   -   -


  Suggestion Two:

    Dip.Sw. Setting
    Dip.Sw. 2   4=On    7=On    Others=Off
    Dip.Sw. 3   2=On    4=On    Others=Off

    Chance Setting
    A   B   C   D   E   F   G   H   I   J
    1   2   2   3   5   6   6   7   8   0
    5   10  20  40  40  -   -   -   -   -


*******************************************************************************

    Keyboard Matrix
    ---------------

    Com. A      Bet         Record      W-Up        Start       N/U
    Com. B      Payout      Keyout      Small       Big         Collect
    Com. C      Hold 1      Hold 2      Hold 3      Hold 4      Hold 5
    Com. D      Key In      Coin        Setting     N/U         N/U


*******************************************************************************

  Findings about the samples encryption
  -------------------------------------

  For every byte, if bit 1 is active is automatically XOR'ed.
  Otherwise bit 1 & bit 5 are XOR'ed.


  XOR'ing bit 1, I got the following vector table:

  01) 0400-1E67
  02) 1E78-06D7 <--- No sense!
  03) 06C8-2067
  04) 2078-3737
  05) 3728-3FEF
  06) 3000-382F
  07) 3840-49B0
  08) 49B1-7671
  09) 7662-84D2
  10) 84D3-A893
  11) A884-C8CC
  12) C8CD-DEEC
  13) DEED-D8DC <--- No sense!
  14) D8DD-DD4C

  But... If we involve bit 5 in the changes, vectors are perfect!.

  01) 0400-1E67  --->  0400-0E77 (ok)
  02) 1E78-06D7  --->  0E78-16C7 (ok)
  03) 06C8-2067  --->  16C8-2077 (ok)
  04) 2078-3737  --->  2078-2727 (ok)
  05) 3728-3FEF  --->  2728-2FFF (ok)
  06) 3000-382F  --->  3000-383F (ok)
  07) 3840-49B0  --->  3840-49B0 (ok)
  08) 49B1-7671  --->  49B1-6671 (ok)
  09) 7662-84D2  --->  6672-84C2 (ok)
  10) 84D3-A893  --->  84C3-A883 (ok)
  11) A884-C8CC  --->  A884-C8CC (ok)
  12) C8CD-DEEC  --->  C8CD-CEEC (ok)
  13) DEED-D8DC  --->  CEED-D8DC (ok)
  14) D8DD-DD4C  --->  D8DD-DD4C (ok)


  Happy End! :)


*******************************************************************************


  Driver updates:


  [2010-03-08]

  - Initial release.
  - Hooked both 6502 CPU's.
  - Hooked the M6845 CRT.
  - Decrypted main program and bulk GFX.
  - Decoded GFX and color PROM.
  - Hooked both PIA's 6821.
  - Worked inputs from the scratch.
  - Hooked DIP Switches banks 2 & 4.
  - Hooked proper interrupts for audio CPU.
  - Hooked the AY8910 latches...
  - Hooked the OKI6295.
  - Hooked DIP switches ports $C400 & $CC00.
  - Found the sound samples decryption algorithm.
  - Added NVRAM support.
  - Added a complete PCB layout.
  - Added game notes.
  - Added technical notes.


  TODO:

  - Proper clock frequencies for CPU, AY8910 & OKI6295.
  - Verify OKI6295 pin 7 (high/low).
  - Try to implement SW5 (Hard Reset + NVRAM clear).
  - Main CPU:  0xC048 - Unknown sound stuff writes (related to ay8910).
  - Audio CPU: 0x2004 - Unknown writes.
  - Audio CPU: 0x2006 - Unknown reads.
  - PIA0: Unknown reads to port B.
  - PIA1: Massive unknown reads to port B.
  - Verify the background color.


*******************************************************************************/

#include "emu.h"
#include "cpu/m6502/m6502.h"
#include "machine/6821pia.h"
#include "machine/nvram.h"
#include "sound/ay8910.h"
#include "sound/okim6295.h"
#include "video/mc6845.h"
#include "emupal.h"
#include "screen.h"
#include "speaker.h"


namespace {

#define MASTER_CLOCK    XTAL(10'000'000)


class _5clown_state : public driver_device
{
public:
	_5clown_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_ay8910(*this, "ay8910"),
		m_palette(*this, "palette"),
		m_gfxbanks(*this, "gfxbanks"),
		m_videoram(*this, "videoram"),
		m_colorram(*this, "colorram")
	{
	}

	void fclown(machine_config &config);

	void init_fclown();

protected:
	virtual void machine_start() override ATTR_COLD;

private:
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	required_device<ay8910_device> m_ay8910;
	required_device<palette_device> m_palette;

	required_region_ptr<uint8_t> m_gfxbanks;
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_colorram;

	uint8_t m_main_latch_d800 = 0;
	uint8_t m_snd_latch_0800 = 0;
	uint8_t m_snd_latch_0a02 = 0;
	uint8_t m_ay8910_addr = 0;
	int m_mux_data = 0;

	void cpu_c048_w(uint8_t data);
	void cpu_d800_w(uint8_t data);
	uint8_t snd_e06_r();
	void snd_800_w(uint8_t data);
	void snd_a02_w(uint8_t data);
	uint8_t mux_port_r();
	void mux_w(uint8_t data);
	void counters_w(uint8_t data);
	void trigsnd_w(uint8_t data);
	uint8_t pia0_b_r();
	uint8_t pia1_b_r();
	void fclown_ay8910_w(offs_t offset, u8 data);
	MC6845_UPDATE_ROW(update_row);
	void _5clown_palette(palette_device &palette) const;

	void fcaudio_map(address_map &map) ATTR_COLD;
	void fclown_map(address_map &map) ATTR_COLD;
};

void _5clown_state::machine_start()
{
	// assumes it can make an address mask with m_videoram.length() - 1
	assert(!(m_videoram.length() & (m_videoram.length() - 1)));

	m_main_latch_d800 = m_snd_latch_0800 = m_snd_latch_0a02 = m_ay8910_addr = m_mux_data = 0;

	save_item(NAME(m_main_latch_d800));
	save_item(NAME(m_snd_latch_0800));
	save_item(NAME(m_snd_latch_0a02));
	save_item(NAME(m_ay8910_addr));
	save_item(NAME(m_mux_data));
}

/*************************
*     Video Hardware     *
*************************/



MC6845_UPDATE_ROW(_5clown_state::update_row)
{
/*  - bits -
    7654 3210
    ---- ---x   Tiles extended address (MSB).
    ---- --x-   Tiles bank.
    --xx xx--   Tiles color.
    -x-- ----   Chars extended address.
    x--- ----   Extra color for 7's.
*/

	uint32_t *pix = &bitmap.pix(y);
	ra &= 0x07;

	for (int x = 0; x < x_count; x++)
	{
		int tile_index = (x + ma) & (m_videoram.length() - 1);
		int attr = m_colorram[tile_index];
		int code = ((attr & 0x01) << 8) | ((attr & 0x40) << 2) | m_videoram[tile_index];    /* bit 8 for extended char set */
		int bank = (attr & 0x02) >> 1;                                                  /* bit 1 switch the gfx banks */
		int color = (attr & 0x3c) >> 2 | ((attr & 0x80) >> 3);                          /* bits 2-3-4-5-7 for color */

		const uint8_t plane2 = bank ? m_gfxbanks[code << 3 | ra | 0x6000] : 0;
		const uint8_t plane1 = bank ? m_gfxbanks[code << 3 | ra | 0x5000] : 0;
		const uint8_t plane0 = m_gfxbanks[code << 3 | ra | (bank ? 0x4000 : 0x7000)];
		const pen_t *pens = &m_palette->pen(color << 3);
		for (int n = 7; n >= 0; n--)
			*pix++ = pens[BIT(plane2, n) << 2 | BIT(plane1, n) << 1 | BIT(plane0, n)];
	}
}


void _5clown_state::_5clown_palette(palette_device &palette) const
{
	/*
	    7654 3210
	    ---- ---x   RED component.
	    ---- --x-   GREEN component.
	    ---- -x--   BLUE component.
	    ---- x---   background killer.
	    xxxx ----   unused.
	*/

	/* 0000KBGR */

	uint8_t const *const color_prom = memregion("proms")->base();
	if (!color_prom)
		return;

	for (int i = 0; i < m_palette->entries(); i++)
	{
		// background killer
		int const bk = BIT(color_prom[i], 3);

		// red component
		int const r = BIT(color_prom[i], 0) * 0xff;

		// green component
		int const g = BIT(color_prom[i], 1) * 0xff;

		// blue component
		int const b = bk * BIT(color_prom[i], 2) * 0xff;

		m_palette->set_pen_color(i, rgb_t(r, g, b));
	}
}


/******************************
*         R/W Handlers        *
******************************/


/* Inputs (buttons) are multiplexed.
   There are 4 sets of 5 bits each and are connected to PIA0, portA.
   The selector bits are located in PIA1, portB (bits 4-7).
*/
uint8_t _5clown_state::mux_port_r()
{
	switch( m_mux_data & 0xf0 )     /* bits 4-7 */
	{
		case 0x10: return ioport("IN0-0")->read();
		case 0x20: return ioport("IN0-1")->read();
		case 0x40: return ioport("IN0-2")->read();
		case 0x80: return ioport("IN0-3")->read();
	}

	return 0xff;
}


void _5clown_state::mux_w(uint8_t data)
{
	m_mux_data = data ^ 0xff;   /* Inverted */
}


void _5clown_state::counters_w(uint8_t data)
{
/*  Counters:

    7654 3210
    ---- xxxx   Unused.
    ---x ----   Key In / Note counter.
    --x- ----   Payout counter.
    -x-- ----   Unknown (increments at start).
    x--- ----   Unknown (increments at start).
*/
	machine().bookkeeping().coin_counter_w(0, data & 0x10);  /* Key In */
	machine().bookkeeping().coin_counter_w(1, data & 0x20);  /* Payout */
	machine().bookkeeping().coin_counter_w(2, data & 0x40);  /* unknown */
	machine().bookkeeping().coin_counter_w(3, data & 0x80);  /* unknown */

}


void _5clown_state::trigsnd_w(uint8_t data)
{
	/************ Interrupts trigger **************

	Writes 0x07 & 0x0F each time a sound is triggered through $D800 */

	if ( (data & 0x0f) == 0x07 )
	{
		m_audiocpu->set_input_line(INPUT_LINE_NMI, ASSERT_LINE );
	}

	else
	{
		m_audiocpu->set_input_line(INPUT_LINE_NMI, CLEAR_LINE );
	}

}

uint8_t _5clown_state::pia0_b_r()
{
	/* often read the port */
	return 0x00;
}

uint8_t _5clown_state::pia1_b_r()
{
	/* constantly read the port */
	return 0x00;    /* bit 2 shouldn't be active to allow work the key out system */
}


/**********************************/


void _5clown_state::cpu_c048_w(uint8_t data)
{
	logerror("Main: Write to $C048: %02X\n", data);
}

void _5clown_state::cpu_d800_w(uint8_t data)
{
	logerror("Main: Write to $D800: %02x\n", data);
	m_main_latch_d800 = data;
}


/********************************
*  AY3-8910 R/W Handlers        *
********************************/

void _5clown_state::fclown_ay8910_w(offs_t offset, u8 data)
{
	m_ay8910->address_w(offset);
	m_ay8910->data_w(data);
}


/******************************
*  SOUND  R/W Handlers        *
******************************/

uint8_t _5clown_state::snd_e06_r()
{
	logerror("Sound: Read from $0E06 \n");
	return m_main_latch_d800;
}

void _5clown_state::snd_800_w(uint8_t data)
{
	m_snd_latch_0800 = data;

	if (m_snd_latch_0a02 == 0xc0)
	{
		m_ay8910_addr = m_snd_latch_0800;
	}

	if (m_snd_latch_0a02 == 0x00)
	{
		fclown_ay8910_w(m_ay8910_addr, m_snd_latch_0800);
	}
}

void _5clown_state::snd_a02_w(uint8_t data)
{
	m_snd_latch_0a02 = data & 0xff;
	logerror("Sound: Write to $0A02: %02x\n", m_snd_latch_0a02);
}


/*************************
* Memory map information *
*************************/

void _5clown_state::fclown_map(address_map &map)
{
	map(0x0000, 0x07ff).ram().share("nvram");
	map(0x0800, 0x0800).w("crtc", FUNC(mc6845_device::address_w));
	map(0x0801, 0x0801).rw("crtc", FUNC(mc6845_device::register_r), FUNC(mc6845_device::register_w));
	map(0x0844, 0x0847).rw("pia0", FUNC(pia6821_device::read), FUNC(pia6821_device::write));
	map(0x0848, 0x084b).rw("pia1", FUNC(pia6821_device::read), FUNC(pia6821_device::write));
	map(0x1000, 0x13ff).ram().share(m_videoram);   // Init'ed at $2042
	map(0x1800, 0x1bff).ram().share(m_colorram);   // Init'ed at $2054
	map(0x2000, 0x7fff).rom();                     // ROM space

	map(0xc048, 0xc048).w(FUNC(_5clown_state::cpu_c048_w));
	map(0xd800, 0xd800).w(FUNC(_5clown_state::cpu_d800_w));

	map(0xc400, 0xc400).portr("SW1");              // DIP Switches bank
	map(0xcc00, 0xcc00).portr("SW2");              // DIP Switches bank
	map(0xd400, 0xd400).portr("SW3");              // Second DIP Switches bank

	map(0xe000, 0xffff).rom();                     // ROM space
}

/*

  $C048 & $D800
  --------------

  $C048: SND Writes.

   VAL  MEANS
  ----+-------
   0F | Mute or Ack Command?
   12 | Hold
   31 | D-UP
   A6 | D-UP 7's


  $D800: SND Writes.
  Seems that enumerate the different sounds (from table?)
  After each $D800 write, 0x07 & 0x0F are written to PIA1, port A,
  triggering the needed interrupt for audio CPU.

   VAL  MEANS
  ----+-------
   01 | Deal 1 (Coin In)
   02 | Deal 2 (Key In)
   03 | Deal 3 (bet, Coin Out)
   04 | Deal 4
   05 | Deal 5
   06 | Hold
   07 | Un-Hold
   0C | D-UP
   34 | D-UP
   13 | Big
   15 | D-UP 7's


  WIN seq --> D800: 1B 1B 18 1B 10 1B 1B 1B 19 1D 18 1A 19 1B 10 1C 10 1A 1A 1A 10 1A
                    1D 18 1B 18 1D 18 10 18 19 10 10 10 1B 1B 10 19 1B 1C 3F () 16 ()

  WIN seq --> C048: 33 () 35 37 35 33 35 37 35 33 35 30 (0F)

  Empty parenthesis means switch to the other offset.
  Last value (0x0F) seems to be a kind of mute command.

  Example:

  Main: Write to $D800: 02      : Sound 2.
  PIA 1: Write to port A: 07    : Trigger audio CPU interrupt.
  PIA 1: Write to port A: 0F    : Clear audio CPU interrupt.
  Main: Write to $C048: XX      : Writes (to other snd device?) if apply.
  Main: Write to $C048: XX      : Writes (to other snd device?) if apply.
  Main: Write to $C048: XX      : Writes (to other snd device?) if apply.
  Main: Write to $C048: 0F      : Kind of mute or ack command.

*/

void _5clown_state::fcaudio_map(address_map &map)
{
	map(0x0000, 0x07ff).ram();
	map(0x0800, 0x0800).w(FUNC(_5clown_state::snd_800_w));
	map(0x0a02, 0x0a02).w(FUNC(_5clown_state::snd_a02_w));
	map(0x0c04, 0x0c04).w("oki6295", FUNC(okim6295_device::write));
	map(0x0c06, 0x0c06).r("oki6295", FUNC(okim6295_device::read));
	map(0x0e06, 0x0e06).r(FUNC(_5clown_state::snd_e06_r));
	map(0xe000, 0xffff).rom();                 /* ROM space */
}


/*************************
*      Input ports       *
*************************/

static INPUT_PORTS_START( fclown )
	/* Multiplexed - 4x5bits */
	PORT_START("IN0-0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_BET )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_GAMBLE_BOOK )    PORT_NAME("Record")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_GAMBLE_D_UP )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_GAMBLE_DEAL )    PORT_NAME("Start")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )        /* cancel */
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("IN0-1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_GAMBLE_KEYOUT )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_GAMBLE_PAYOUT )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_GAMBLE_TAKE )    PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_GAMBLE_HIGH )    PORT_NAME("Big")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_GAMBLE_LOW )     PORT_NAME("Small")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("IN0-2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_POKER_HOLD1 )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_POKER_HOLD2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_POKER_HOLD3 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_POKER_HOLD4 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_POKER_HOLD5 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("IN0-3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_SERVICE )    PORT_NAME("Setting")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_COIN1 )      PORT_IMPULSE(3) PORT_NAME("Coin In")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_GAMBLE_KEYIN)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("SW1")
	PORT_DIPNAME( 0x01, 0x01, "SW1-01 (C400)" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, "SW1-02 (C400)" )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, "SW1-04 (C400)" )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "SW1-08 (C400)" )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, "SW1-10 (C400)" )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "SW1-20 (C400)" )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, "SW1-40 (C400)" )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "SW1-80 (C400)" )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

/*  SW2 is ok... The remaining bits are checked in unused routines,
    or just NOP'ed...
*/
	PORT_START("SW2")
	PORT_DIPNAME( 0x01, 0x01, "SW2-01 (CC00)" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, "SW2-02 (CC00)" )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, "SW2-04 (CC00)" )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "SW2-08 (CC00)" )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x00, "SW2-10 (CC00) System Boot" ) /* Should be turned ON to boot */
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x00, "Key In (value)" )
	PORT_DIPSETTING(    0x20, "1 Pulse / 20 Credits" )
	PORT_DIPSETTING(    0x00, "1 Pulse / 100 Credits" )
	PORT_DIPNAME( 0x40, 0x40, "SW2-40 (CC00)" )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "SW2-80 (CC00)" )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("SW3")
	PORT_DIPNAME( 0x01, 0x01, DEF_STR( Unknown ) )  PORT_DIPLOCATION("SW3:1")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )  PORT_DIPLOCATION("SW3:2")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )  PORT_DIPLOCATION("SW3:3")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x18, 0x10, "Min/Max Bet" )       PORT_DIPLOCATION("SW3:4,5") /* Always 1-40 on screen */
	PORT_DIPSETTING(    0x00, "Min:10; Max:80" )
	PORT_DIPSETTING(    0x08, "Min:10; Max:50" )
	PORT_DIPSETTING(    0x10, "Min:05; Max:40" )
	PORT_DIPSETTING(    0x18, "Min:05; Max:10" )
	PORT_DIPNAME( 0x60, 0x40, "Key In (screen)" )   PORT_DIPLOCATION("SW3:6,7") /* Only on screen */
	PORT_DIPSETTING(    0x60, "20" )
	PORT_DIPSETTING(    0x40, "100" )
	PORT_DIPSETTING(    0x20, "120" )
	PORT_DIPSETTING(    0x00, "130" )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )  PORT_DIPLOCATION("SW3:8")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("SW4")
	/* only bits 4-6-7 are connected here and were routed to SW4 1-4 */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_DIPNAME( 0x10, 0x00, "Jacks or Better" )   PORT_DIPLOCATION("SW4:1")
	PORT_DIPSETTING(    0x10, DEF_STR( No ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Yes ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unused ) )   PORT_DIPLOCATION("SW4:2")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x00, "Payout Mode" )       PORT_DIPLOCATION("SW4:3")
	PORT_DIPSETTING(    0x40, "Manual" )
	PORT_DIPSETTING(    0x00, "Auto" )
	PORT_DIPNAME( 0x80, 0x80, "Coin In" )           PORT_DIPLOCATION("SW4:4")
	PORT_DIPSETTING(    0x80, "x10" )
	PORT_DIPSETTING(    0x00, "x5" )
INPUT_PORTS_END


/*************************
*    Graphics Layouts    *
*************************/

static const gfx_layout tilelayout =
{
	8, 8,
	RGN_FRAC(1,3),
	3,
	{ 0, RGN_FRAC(1,3), RGN_FRAC(2,3) },    /* bitplanes are separated */
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8
};


/******************************
* Graphics Decode Information *
******************************/

static GFXDECODE_START( gfx_fclown )
	GFXDECODE_ENTRY( "gfx1", 0, tilelayout, 0, 16 )
	GFXDECODE_ENTRY( "gfx2", 0, tilelayout, 0, 16 )
GFXDECODE_END

/*************************
*    Machine Drivers     *
*************************/

void _5clown_state::fclown(machine_config &config)
{
	/* basic machine hardware */
	M6502(config, m_maincpu, MASTER_CLOCK/8);  /* guess, seems ok */
	m_maincpu->set_addrmap(AS_PROGRAM, &_5clown_state::fclown_map);

	M6502(config, m_audiocpu, MASTER_CLOCK/8); /* guess, seems ok */
	m_audiocpu->set_addrmap(AS_PROGRAM, &_5clown_state::fcaudio_map);

	NVRAM(config, "nvram", nvram_device::DEFAULT_ALL_0);

	pia6821_device &pia0(PIA6821(config, "pia0"));
	pia0.readpa_handler().set(FUNC(_5clown_state::mux_port_r));
	pia0.readpb_handler().set(FUNC(_5clown_state::pia0_b_r));
	pia0.writepb_handler().set(FUNC(_5clown_state::counters_w));

	pia6821_device &pia1(PIA6821(config, "pia1"));
	pia1.readpa_handler().set_ioport("SW4");
	pia1.readpb_handler().set(FUNC(_5clown_state::pia1_b_r));
	pia1.writepa_handler().set(FUNC(_5clown_state::trigsnd_w));
	pia1.writepb_handler().set(FUNC(_5clown_state::mux_w));

	/* video hardware */
	screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_RASTER));
	screen.set_raw(MASTER_CLOCK/2, 320, 0, 256, 312, 0, 256);
	screen.set_screen_update("crtc", FUNC(mc6845_device::screen_update));

	GFXDECODE(config, "gfxdecode", m_palette, gfx_fclown);
	PALETTE(config, m_palette, FUNC(_5clown_state::_5clown_palette), 256);

	mc6845_device &crtc(MC6845(config, "crtc", MASTER_CLOCK/16)); /* guess */
	crtc.set_screen("screen");
	crtc.set_show_border_area(false);
	crtc.set_char_width(8);
	crtc.out_vsync_callback().set_inputline(m_maincpu, INPUT_LINE_NMI);
	crtc.set_update_row_callback(FUNC(_5clown_state::update_row));

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	AY8910(config, m_ay8910, MASTER_CLOCK/8).add_route(ALL_OUTPUTS, "mono", 1.00);        /* guess, seems ok */

	OKIM6295(config, "oki6295", MASTER_CLOCK/12, okim6295_device::PIN7_LOW).add_route(ALL_OUTPUTS, "mono", 1.20);    /* guess, seems ok; pin7 guessed, seems ok */
}


/*************************
*        Rom Load        *
*************************/

ROM_START( 5clown )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "4.u2",       0x2000, 0x8000, CRC(96e3e8ab) SHA1(fec20b9a8bde5306162f8288cdc9580f445cadf5) )
	ROM_COPY( "maincpu",    0x8000, 0xe000, 0x2000 )


	ROM_REGION( 0x8000,  "gfxbanks", 0 )
	ROM_LOAD( "7.u34",  0x0000, 0x8000, CRC(64c9f4ee) SHA1(6e695feee826e319f84d91f6bbf7cfacd443fc8f) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "9.u35",  0xe000, 0x2000, CRC(7e3c3af5) SHA1(ebc09da981dbdf4eac90bcf982e5bc8ede47e81a) )

	ROM_REGION( 0x40000, "oki6295", 0 )
	ROM_LOAD( "8.u50",  0x0000, 0x10000, CRC(e1e37180) SHA1(e162abcb01952e26deee74ece5719239961e1b69) )


	ROM_REGION( 0x3000, "gfx1", 0 )
	ROM_FILL(               0x0000, 0x2000, 0x00 ) /* filling the remaining bitplanes */
	ROM_COPY( "gfxbanks",   0x7000, 0x2000, 0x1000 )

	ROM_REGION( 0x3000, "gfx2", 0 )
	ROM_COPY( "gfxbanks",   0x6000, 0x0000, 0x1000 )
	ROM_COPY( "gfxbanks",   0x5000, 0x1000, 0x1000 )
	ROM_COPY( "gfxbanks",   0x4000, 0x2000, 0x1000 )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "dm74s287an.u32", 0x0000, 0x0100, CRC(2d207266) SHA1(374b4830a0a8ed3001cf0df16daa8dffee503cbe) )

ROM_END


ROM_START( 5clowna )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "6.u2",   0x2000, 0x2000, BAD_DUMP CRC(15ecca37) SHA1(d8d0a43f559168cccc3a46418557ccfc30c8108e) )
	ROM_LOAD( "5.u2",   0x4000, 0x2000, BAD_DUMP CRC(2e962007) SHA1(86599ba5030068c08ff5a19ada7250c410b2ba35) )
	ROM_LOAD( "7.u2",   0xe000, 0x2000, BAD_DUMP CRC(308c4771) SHA1(f6ee402c120ff16601347cc994a45a7092511050) )


	ROM_REGION( 0x8000,  "gfxbanks", 0 )
	ROM_LOAD( "7.u34",  0x0000, 0x8000, CRC(64c9f4ee) SHA1(6e695feee826e319f84d91f6bbf7cfacd443fc8f) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "9.u35",  0xe000, 0x2000, CRC(7e3c3af5) SHA1(ebc09da981dbdf4eac90bcf982e5bc8ede47e81a) )

	ROM_REGION( 0x40000, "oki6295", 0 )
	ROM_LOAD( "8.u50",  0x0000, 0x10000, CRC(e1e37180) SHA1(e162abcb01952e26deee74ece5719239961e1b69) )


	ROM_REGION( 0x3000, "gfx1", 0 )
	ROM_FILL(               0x0000, 0x2000, 0x00 ) /* filling the remaining bitplanes */
	ROM_COPY( "gfxbanks",   0x7000, 0x2000, 0x1000 )

	ROM_REGION( 0x3000, "gfx2", 0 )
	ROM_COPY( "gfxbanks",   0x6000, 0x0000, 0x1000 )
	ROM_COPY( "gfxbanks",   0x5000, 0x1000, 0x1000 )
	ROM_COPY( "gfxbanks",   0x4000, 0x2000, 0x1000 )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "dm74s287an.u32", 0x0000, 0x0100, CRC(2d207266) SHA1(374b4830a0a8ed3001cf0df16daa8dffee503cbe) )

ROM_END


ROM_START( 5clownsp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "five_clown_sp.u2",   0x2000, 0x8000, CRC(fa18090d) SHA1(47feb5dbc77ae8621fc35b707c24d64a95227a39) )
	ROM_COPY( "maincpu",    0x8000, 0xe000, 0x2000 )


	ROM_REGION( 0x8000,  "gfxbanks", 0 )
	ROM_LOAD( "7.u34",  0x0000, 0x8000, CRC(64c9f4ee) SHA1(6e695feee826e319f84d91f6bbf7cfacd443fc8f) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "9.u35",  0xe000, 0x2000, CRC(7e3c3af5) SHA1(ebc09da981dbdf4eac90bcf982e5bc8ede47e81a) )

	ROM_REGION( 0x40000, "oki6295", 0 )
	ROM_LOAD( "8.u50",  0x0000, 0x10000, CRC(e1e37180) SHA1(e162abcb01952e26deee74ece5719239961e1b69) )


	ROM_REGION( 0x3000, "gfx1", 0 )
	ROM_FILL(               0x0000, 0x2000, 0x00 ) /* filling the remaining bitplanes */
	ROM_COPY( "gfxbanks",   0x7000, 0x2000, 0x1000 )

	ROM_REGION( 0x3000, "gfx2", 0 )
	ROM_COPY( "gfxbanks",   0x6000, 0x0000, 0x1000 )
	ROM_COPY( "gfxbanks",   0x5000, 0x1000, 0x1000 )
	ROM_COPY( "gfxbanks",   0x4000, 0x2000, 0x1000 )

	ROM_REGION( 0x0100, "proms", 0 )
	ROM_LOAD( "dm74s287an.u32", 0x0000, 0x0100, CRC(2d207266) SHA1(374b4830a0a8ed3001cf0df16daa8dffee503cbe) )

ROM_END


/*************************
*      Driver Init       *
*************************/

void _5clown_state::init_fclown()
{
	/* Decrypting main program */
	uint8_t *src = memregion( "maincpu" )->base();

	for (int x = 0x0000; x < 0x10000; x++)
	{
		src[x] = src[x] ^ 0x20;     /* Decrypting byte */
	}


	/* Decrypting GFX by segments */
	uint8_t *gfx1_src = memregion( "gfx1" )->base();
	uint8_t *gfx2_src = memregion( "gfx2" )->base();

	for (int x = 0x2000; x < 0x3000; x++)
	{
		gfx1_src[x] = gfx1_src[x] ^ 0x22;   /* Decrypting bulk GFX segment 7000-7fff */
		m_gfxbanks[0x7000 - 0x2000 + x] ^= 0x22;
	}

	for (int x = 0x0000; x < 0x1000; x++)
	{
		gfx2_src[x] = gfx2_src[x] ^ 0x3f;   /* Decrypting bulk GFX segment 6000-6fff */
		m_gfxbanks[0x6000 - 0x0000 + x] ^= 0x3f;
	}

	for (int x = 0x2000; x < 0x3000; x++)
	{
		gfx2_src[x] = gfx2_src[x] ^ 0x22;   /* Decrypting bulk GFX segment 4000-4fff */
		m_gfxbanks[0x4000 - 0x2000 + x] ^= 0x22;
	}


	/* Decrypting sound samples */
	uint8_t *samples_src = memregion( "oki6295" )->base();
	for (int x = 0x0000; x < 0x10000; x++)
	{
		if (samples_src[x] & 0x02)                      /* If bit 1 is active... */
		{
			samples_src[x] = samples_src[x] ^ 0x02;     /* Then bit 1 XOR'ed */
		}

		else
		{
			samples_src[x] = samples_src[x] ^ 0x12;     /* Otherwise bit 1 & 5 XOR'ed */
		}
	}
}

} // anonymous namespace


/*************************
*      Game Drivers      *
*************************/

//    YEAR  NAME      PARENT  MACHINE INPUT   CLASS          INIT         ROT   COMPANY  FULLNAME                       FLAGS...
GAME( 1993, 5clown,   0,      fclown, fclown, _5clown_state, init_fclown, ROT0, "IGS",   "Five Clown (English, set 1)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 1993, 5clowna,  5clown, fclown, fclown, _5clown_state, init_fclown, ROT0, "IGS",   "Five Clown (English, set 2)", MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 1993, 5clownsp, 5clown, fclown, fclown, _5clown_state, init_fclown, ROT0, "IGS",   "Five Clown (Spanish hack)",   MACHINE_IMPERFECT_SOUND | MACHINE_SUPPORTS_SAVE )
