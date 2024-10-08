// license: BSD-3-Clause
// copyright-holders: Dirk Best
/***************************************************************************

    Visual X Display Station Keyboard (HLE)

***************************************************************************/

#ifndef MAME_VISUAL_XDS_KBD_H
#define MAME_VISUAL_XDS_KBD_H

#pragma once

#include "machine/keyboard.h"
#include "diserial.h"


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

class xds_kbd_hle_device :
	public device_t,
	public device_buffered_serial_interface<16>,
	protected device_matrix_keyboard_interface<8>
{
public:
	// construction/destruction
	xds_kbd_hle_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

	// callbacks
	auto tx_handler() { return m_tx_handler.bind(); }
	auto cts_handler() { return m_cts_handler.bind(); }

	// from host
	void rx_w(int state);

protected:
	// device_t overrides
	virtual ioport_constructor device_input_ports() const override ATTR_COLD;
	virtual void device_start() override ATTR_COLD;
	virtual void device_reset() override ATTR_COLD;

	// device_buffered_serial_interface overrides
	virtual void tra_callback() override;
	virtual void received_byte(uint8_t byte) override;

	// device_matrix_keyboard_interface overrides
	virtual void key_make(uint8_t row, uint8_t column) override;
	virtual void key_break(uint8_t row, uint8_t column) override;
	virtual void key_repeat(uint8_t row, uint8_t column) override;

private:
	devcb_write_line m_tx_handler;
	devcb_write_line m_cts_handler;
};

// device type definition
DECLARE_DEVICE_TYPE(XDS_KBD_HLE, xds_kbd_hle_device)

#endif // MAME_VISUAL_XDS_KBD_H
