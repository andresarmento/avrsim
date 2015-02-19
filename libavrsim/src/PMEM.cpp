#include "PMEM.h"

PMEM::PMEM(BUS* bus) {
    this->bus = bus;
}

PMEM::~PMEM() {

}

void PMEM::setData(uint16_t addr, uint16_t val) {
    data[addr] = (val & 0xFF) << 8 | (val & 0xFF00) >> 8;
}

uint16_t PMEM::getData(uint16_t addr) {
    return (data[addr] & 0xFF) << 8 | (data[addr] & 0xFF00) >> 8;
}

uint16_t PMEM::getMinAddr() {
    return minAddr;
}

//Paul Stoffregen
int PMEM::parseHexLine(char *theline, int bytes[], uint16_t* addr, unsigned int* num, int*code) {
	int sum, cksum;
	unsigned int len;
	char *ptr;

	*num = 0;
	if (theline[0] == '#') return 0;
	if (theline[0] != ':') return -1;
	if (strlen(theline) < 11) return -1;
	ptr = theline+1;
	if (!sscanf(ptr, "%02x", &len)) return -1;
	ptr += 2;
	if ( strlen(theline) < (11 + (len * 2)) ) return -1;
	if (!sscanf(ptr, "%04x", (uint32_t *)addr)) return -1;
	ptr += 4;
	  /* printf("Line: length=%d Addr=%d\n", len, *addr); */
	if (!sscanf(ptr, "%02x", code)) return -1;
	ptr += 2;
	sum = (len & 255) + ((*addr >> 8) & 255) + (*addr & 255) + (*code & 255);
	while(*num != len) {
		if (!sscanf(ptr, "%02x", &bytes[*num])) return -1;
		ptr += 2;
		sum += bytes[*num] & 255;
		(*num)++;
		if (*num >= 256) return -1;
	}
	if (!sscanf(ptr, "%02x", &cksum)) return -1;
	if ( ((sum & 255) + (cksum & 255)) & 255 ) return -2; /* checksum error */
	return 1;
}

int PMEM::readHex(char* filename) {
	char line[1000];
	FILE *fin;
	int status, bytes[256];
	uint16_t addr;
	uint16_t addr2;
	unsigned int i, n;
	int total=0, lineno=1;
	minAddr=65535;
	maxAddr=0;
	int parseResp;

	if (strlen(filename) == 0) {
		//printf("ERROR: File name not specified.");
		return 0;
	}
	fin = fopen(filename, "r");
	if (fin == NULL) {
		//printf("ERROR: File '%s' cant be open for reading.\n", filename);
		return 0;
	}
	while (!feof(fin) && !ferror(fin)) {
		line[0] = '\0';
		fgets(line, 1000, fin);
		if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';
		if (line[strlen(line)-1] == '\r') line[strlen(line)-1] = '\0';
		parseResp = parseHexLine(line, bytes, &addr, &n, &status);
		addr2 = addr >> 1;
		if (parseResp > 0) {
			if (status == 0) {  /* data */
				for(i=0; i<=(n-1); i+=2) {
					data[addr2] = (bytes[i] & 255)*256 + (bytes[i+1] & 255);
					if (addr2 < minAddr) minAddr = addr2;
					if (addr2 > maxAddr) maxAddr = addr2;
					total+=2;
					addr2++;
				}
			}
			if (status == 1) {  /* end of file */
				fclose(fin);
				printf("Loaded %d bytes.\n", total);
				printf("Flash address: %04X to %04X\n", minAddr, maxAddr);
				return 1;
			}
			if (status == 2) ;  /* begin of file */
		} else {
            if (parseResp < 0)	printf("ERROR: '%s', line: %d\n", filename, lineno);
            //else skip line...
		}
		lineno++;
	}

	return 0;
}

void PMEM::erase() {
    for (int addr=0; addr < ATMEGA328P_PROGMEMSIZE; addr++) {
        data[addr] = 0x0000;
    }
}

/*
void PMEM::readHex(string filename) {
    ifstream hexfile(filename.c_str());
    string line;

    if (hexfile.is_open()) {
        while (getline (hexfile,line)) {
            cout << line << '\n';
        }
        hexfile.close();
    }

  else cout << "Unable to open file";
}
*/
