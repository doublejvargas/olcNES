#pragma once

#include <memory>
#include <cstdint>
#include "Cartridge.h"

#include "olcPixelGameEngine.h"		// for visual debugging purposes

class olc2C02
{
public:
	olc2C02();
	~olc2C02();

private:
	uint8_t		tblName[2][1024];			// VRAM in ppu bus used to hold name table information. split into two 1Kb chunks, 1kb = 1 table, NES has capacity for 2 tables
	uint8_t		tblPalette[32];				// VRAM connected to ppu bus for palette info
	uint8_t		tblPattern[4][4096];		//Javid particular

public:
	// Communication with Main Bus
	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void cpuWrite(uint16_t addr, uint8_t data);

	// Communication with PPU Bus
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void ppuWrite(uint16_t addr, uint8_t data);


private:
	// The Cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;

public:
	// Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();

private:
	// For visual debugging purposes
	olc::Pixel  palScreen[0x40];
	olc::Sprite sprScreen          =   olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2]    = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };

public:
	// More Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i);
	bool frame_complete = false;

private:
	int16_t scanline = 0;			// Represents which row on the screen
	int16_t cycle = 0;				// Represents which column on the screen
};

