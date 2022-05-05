#include "Bus.h"

Bus::Bus()
{
	// Clear RAM contents, just in case
	for (auto& i : cpuRam) i = 0x00;

	// Connect CPU to communication bus
	cpu.ConnectBus(this);
}

Bus::~Bus()
{

}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
	if (cart->cpuWrite(addr, data))
	{

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)		// 8kb addressable range for RAM
	{
		cpuRam[addr & 0x07FF] = data;				// 2kb range due to mirroring of ram. bitwise & 0x07FF is equivalent to mod operation
	}
	else if (addr >= 2000 && addr <= 0x3FFF)		// Write targeting ppu addresable range.
	{
		ppu.cpuWrite(addr & 0x0007, data);
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	uint8_t data = 0x00;

	if (cart->cpuRead(addr, data))
	{

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)		// 8kb addressable range for RAM
	{
		data = cpuRam[addr & 0x07FF];				// 2kb range due to mirroring of ram.
	}
	else if (addr >= 2000 && addr <= 0x3FFF)		// Read targeting ppu addresable range.
	{
		data = ppu.cpuRead(addr & 0x0007, bReadOnly);
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

// The BUS clock is the SYSTEM (console as a whole) clock
void Bus::clock()
{
	ppu.clock();			// every time there is a system tick, the ppu needs to do something, hence the call here.
	if (nSystemClockCounter % 3 == 0)		// the cpu clock runs 3 TIMES SLOWER than the ppu clock
	{
		cpu.clock();
	}
	nSystemClockCounter++;
}
