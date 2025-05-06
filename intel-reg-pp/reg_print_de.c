#include "reg_print_de.h"
#include <stdio.h>

// ARMELLIN
#define IsBitSet(val, bit) ((val) >> (bit)) & 0x1
#define GetVal(val, bit, len) (((val) >> (bit)) & (((unsigned long long)(0x1) << (len)) - 1)) 

// https://software.intel.com/sites/default/files/managed/39/c5/325462-sdm-vol-1-2abcd-3abcd.pdf

//#############################################################################
//#
//#############################################################################
static void REG_PRINT_DE__makeBinaryString(unsigned long long ullVal, char* pcBuff, unsigned int uiBuffSize)
{
    unsigned int uiIndex;
    unsigned int uiMaxRegBits = sizeof(unsigned long long) * 8;
    char* pcAddr = pcBuff;

    if (0 != pcBuff)
    {
        if (uiBuffSize >= 67)
        {
            *pcAddr++ = '0';
            *pcAddr++ = 'b';

            for (uiIndex = 0; uiIndex <= uiMaxRegBits; uiIndex++)
            {
                if (ullVal & ((unsigned long long)1 << (uiMaxRegBits - uiIndex)))
                {
                    *pcAddr++ = '1';
                }
                else
                {
                    *pcAddr++ = '0';
                }
            }
        }
    }
}

//#############################################################################
//# IA32_PERF_CTL
//# https://www.intel.com/content/www/us/en/embedded/testing-and-validation/cpu-monitoring-dts-peci-paper.html 
//# 
//#############################################################################
void REG_PRINT_DE__printIA32PerfCtl(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };
    unsigned int uiTargPerfStateVal = 0;

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    uiTargPerfStateVal = (unsigned int)((unsigned long long)0x7FFF & ullVal);

    printf("[199H] IA32_PERF_CTL\n");
    printf("  Command to read: sudo rdmsr 0x199 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └───────────────┬─────────────┘│└──────┬───────┘└──────┬───────┘\n");
    printf("              Reserved            │       │               │        \n");
    printf("IDA Engage (R/W) 1:disengage IDA ─┘       │               │  %llu\n", IsBitSet(ullVal, 32));
    printf("Reserved ─────────────────────────────────┘               │        \n");
    printf("Target Performance State Value ───────────────────────────┘  0x%x -> %d  \n", uiTargPerfStateVal, uiTargPerfStateVal);
}

//#############################################################################
//# IA32_CLOCK_MODULATION
//# http://kib.kiev.ua/x86docs/SDMs/335592-063.pdf
//# 
//#############################################################################
void REG_PRINT_DE__print_IA32_CLOCK_MODULATION(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[19AH] IA32_CLOCK_MODULATION\n");
    printf("  Command to read: sudo rdmsr 0x19A -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └─────────────────────────────┬───────────────────────────┘│└┬┘│\n");
    printf("                             Reserved                         │ │ │\n");
    printf("On-Demand Clock Modulation Enable ────────────────────────────┘ │ │ %llu\n", IsBitSet(ullVal,  4));
    printf("On-Demand Clock Modulation Duty Cycle ──────────────────────────┘ │ 0x%llx -> %llu  \n", GetVal(ullVal,  1, 3), GetVal(ullVal,  1, 3));
    printf("Extended On-Demand Clock Modulation Duty Cycle ───────────────────┘ %llu\n", IsBitSet(ullVal,  0));
}

//#############################################################################
//# IA32_THERM_INTERRUPT
//# https://www.intel.com/content/www/us/en/embedded/testing-and-validation/cpu-monitoring-dts-peci-paper.html 
//# 
//#############################################################################
void REG_PRINT_DE__printIA32ThermInterrupt(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[19BH] IA32_THERM_INTERRUPT\n");
    printf("  Command to read: sudo rdmsr 0x19b -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └───────────────┬──────────────────────┘│└──┬──┘│└──┬──┘└┬┘│││││\n");
    printf("              Reserved                     │   │   │   │    │ │││││\n");
    printf("Threshold #2 INT Enable ───────────────────┘   │   │   │    │ │││││ %llu\n", IsBitSet(ullVal, 23));
    printf("Threshold #2 Value ────────────────────────────┘   │   │    │ │││││ 0x%llx -> %llu  \n", GetVal(ullVal, 16, 7), GetVal(ullVal, 16, 7));
    printf("Threshold #1 INT Enable ───────────────────────────┘   │    │ │││││ %llu\n", IsBitSet(ullVal, 15));
    printf("Threshold #1 Value ────────────────────────────────────┘    │ │││││ 0x%llx -> %llu  \n", GetVal(ullVal,  8, 7), GetVal(ullVal,  8, 7));
    printf("Reserved ───────────────────────────────────────────────────┘ │││││\n");
    printf("Critical Temperature Enable ──────────────────────────────────┘││││ %llu\n", IsBitSet(ullVal,  4));
    printf("FORCEPR# INT Enable ───────────────────────────────────────────┘│││ %llu\n", IsBitSet(ullVal,  3));
    printf("PROCHOT# INT enable ────────────────────────────────────────────┘││ %llu\n", IsBitSet(ullVal,  2));
    printf("Low-Temperature INT enable ──────────────────────────────────────┘│ %llu\n", IsBitSet(ullVal,  1));
    printf("High-Temperature INT Enable ──────────────────────────────────────┘ %llu\n", IsBitSet(ullVal,  0));
}

//#############################################################################
//#
//#############################################################################
void REG_PRINT_DE__printThemStatus(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };
    //unsigned long long ullTempMask = 0x0078000000;
    unsigned long long ullTempMask = 0x07F0000;
    unsigned long long ullTempValue;
    unsigned int uiTempValueInDegC;
    unsigned int uiTempValueInDegF;

    // Extract the temperature
    ullTempValue = (ullVal & ullTempMask);
    uiTempValueInDegC = (unsigned int)(ullTempValue >> 16);
    uiTempValueInDegF = ((uiTempValueInDegC * 9) / 5) + 32;

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[19CH] IA32_HWP_THERM_STATUS Register With HWP Feedback\n");
    printf("  Command to read: sudo rdmsr 0x19c -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("  └───────────────┬───────────────┘│└─┬┘└─┬┘└──┬──┘││││││││││││││││\n");
    printf("              Reserved             │  │   │    │   ││││││││││││││││\n");
    printf("Reading Valid ─────────────────────┘  │   │    │   ││││││││││││││││\n");
    printf("Reading in Deg. Celcius ──────────────┘   │    │   ││││││││││││││││\n");
    printf("Reserved ─────────────────────────────────┘    │   ││││││││││││││││\n");
    printf("Digital Readout ───────────────────────────────┘   ││││││││││││││││ %d C -> %d F\n", uiTempValueInDegC, uiTempValueInDegF);
    printf("Cross-domain Limit Log ────────────────────────────┘│││││││││││││││ %llu\n", IsBitSet(ullVal, 15));
    printf("Cross-domain Limit Status ──────────────────────────┘││││││││││││││ %llu\n", IsBitSet(ullVal, 14));
    printf("Current Limit Log ───────────────────────────────────┘│││││││││││││ %llu\n", IsBitSet(ullVal, 13));
    printf("Current Limit Status ─────────────────────────────────┘││││││││││││ %llu\n", IsBitSet(ullVal, 12));
    printf("Power Limit Notification Log ──────────────────────────┘│││││││││││ %llu\n", IsBitSet(ullVal, 11));
    printf("Power Limit Notification Status ────────────────────────┘││││││││││ %llu\n", IsBitSet(ullVal, 10));
    printf("Thermal Threshold #2 Log ────────────────────────────────┘│││││││││ %llu\n", IsBitSet(ullVal,  9));
    printf("Thermal Threshold #2 Status ──────────────────────────────┘││││││││ %llu\n", IsBitSet(ullVal,  8));
    printf("Thermal Threshold #1 Log ──────────────────────────────────┘│││││││ %llu\n", IsBitSet(ullVal,  7));
    printf("Thermal Threshold #1 Status ────────────────────────────────┘││││││ %llu\n", IsBitSet(ullVal,  6));
    printf("Critical Temperature Log ────────────────────────────────────┘│││││ %llu\n", IsBitSet(ullVal,  5));
    printf("Critical Temperature Status ──────────────────────────────────┘││││ %llu\n", IsBitSet(ullVal,  4));
    printf("PROCHOT# or FORCEPR# Log ──────────────────────────────────────┘│││ %llu\n", IsBitSet(ullVal,  3));
    printf("PROCHOT# or FORCEPR# Event ─────────────────────────────────────┘││ %llu\n", IsBitSet(ullVal,  2));
    printf("Thermal Status Log ──────────────────────────────────────────────┘│ %llu\n", IsBitSet(ullVal,  1));
    printf("Thermal Status ───────────────────────────────────────────────────┘ %llu\n", IsBitSet(ullVal,  0));
}

//#############################################################################
//# IA32_MISC_ENABLE
//# https://www.intel.com/content/dam/www/public/us/en/documents/datasheets/atom-processor-s1200-datasheet-vol-2.pdf
//# 
//#############################################################################
void REG_PRINT_DE__printIA32MiscEnable(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[1A0H] IA32_MISC_ENABLE\n");
    printf("  Command to read: sudo rdmsr 0x1a0 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("   43210987654321098765432109876543210987654321098765432109876543210\n");
    printf(" %s\n", cBuffValAsBin);
    printf("   └────────────────┬───────────┘│││└┬─┘│││└┤│││││││└┤│└┤│└┤│└┬┘│└┤│\n");
    printf("               Reserved          │││ │  │││ ││││││││ ││ ││ ││ │ │ ││\n");
    printf("Execute-Disable Bit Disable(R/W) ┘││ │  │││ ││││││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 34));
    printf("Hard C4E Enable (R/W)  ───────────┘│ │  │││ ││││││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 33));
    printf("C4E Enable (R/W)  ─────────────────┘ │  │││ ││││││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 32));
    printf("Reserved ────────────────────────────┘  │││ ││││││││ ││ ││ ││ │ │ ││\n");
    printf("ACNT2 Enable (R/W) ─────────────────────┘││ ││││││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 27));
    printf("C2E Enable (R/W) ────────────────────────┘│ ││││││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 26));
    printf("C1E Enable (R/W) ─────────────────────────┘ ││││││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 25));
    printf("Reserved ───────────────────────────────────┘│││││││ ││ ││ ││ │ │ ││\n");
    printf("Limit CPUID Maxval (R/W) ────────────────────┘││││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 22));
    printf("RORCEPR Input Enable (R/W) ───────────────────┘│││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 21));
    printf("SpeedStep Tech. Select Lock (R/W) ─────────────┘││││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 20));
    printf("Reserved ───────────────────────────────────────┘│││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 19));
    printf("Enable Monitor FSM (R/W) ────────────────────────┘││ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 18));
    printf("Bi-Dir PROCHOT# Enable (R0) ──────────────────────┘│ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 17));
    printf("SpeedStep Tech. Enable (R/W) ──────────────────────┘ ││ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 16));
    printf("Reserved ────────────────────────────────────────────┘│ ││ ││ │ │ ││\n");
    printf("TM2 Enable (R/W) ─────────────────────────────────────┘ ││ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 13));
    printf("Reserved ───────────────────────────────────────────────┘│ ││ │ │ ││\n");
    printf("FERR# Multiplexing Enable (R/W) ─────────────────────────┘ ││ │ │ ││ %llu\n", IsBitSet(ullVal, 10));
    printf("Reserved ──────────────────────────────────────────────────┘│ │ │ ││\n");
    printf("Performance Monitoring Available (R0) ──────────────────────┘ │ │ ││ %llu\n", IsBitSet(ullVal,  7));
    printf("Reserved ─────────────────────────────────────────────────────┘ │ ││\n");
    printf("TM1 Enable (R/W) ───────────────────────────────────────────────┘ ││ %llu\n", IsBitSet(ullVal,  3));
    printf("Reserved ─────────────────────────────────────────────────────────┘│\n");
    printf("Fast String Enable (R/W) ──────────────────────────────────────────┘ %llu\n", IsBitSet(ullVal,  0));
}

//#############################################################################
//# IA32_PACKAGE_THERM_MARGIN
//# https://www.intel.com/content/dam/www/public/us/en/documents/design-guides/4th-gen-core-family-desktop-tmsdg.pdf
//# 
//#############################################################################
void REG_PRINT_DE__printIA32PackageThermMargin(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };
    unsigned int uiPkgThermMargin = 0;

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    uiPkgThermMargin = (unsigned int)((unsigned long long)0xFFFF & ullVal);

    printf("[1A1H] IA32_PACKAGE_THERM_MARGIN\n");
    printf("  Command to read: sudo rdmsr 0x1a1 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └────────────────────────┬─────────────────────┘└──────┬───────┘\n");
    printf("                        Reserved                          │        \n");
    printf("THERM Marging  ───────────────────────────────────────────┘  0x%x -> %d\n", uiPkgThermMargin, uiPkgThermMargin);
}

//#############################################################################
//# IA32_TEMPERATURE_TARGET
//# cpu-monitoring-dts-peci-paper.pdf
//# 
//#############################################################################
void REG_PRINT_DE__printIA32TermperatureTarget(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };
    unsigned int uiTargPerfStateVal = 0;

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    uiTargPerfStateVal = (unsigned int)((unsigned long long)0x0FF0000 & ullVal);
    uiTargPerfStateVal = (uiTargPerfStateVal >> 16);

    printf("[1A2H] IA32_TEMPERATURE_TARGET\n");
    printf("  Command to read: sudo rdmsr 0x1a2 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └────────────────────┬─────────────────┘└───┬──┘└───────┬──────┘\n");
    printf("Reserved ───────────────┘                      │           │       \n");
    printf("Temperature Target, min (deg C) ───────────────┘           │ 0x%x -> %d C\n", uiTargPerfStateVal, uiTargPerfStateVal);
    printf("Not defined ───────────────────────────────────────────────┘       \n");
    printf("0x%llx -> %llu  \n", GetVal(ullVal, 16, 8), GetVal(ullVal, 16, 8));
}

//#############################################################################
//# IA32_PKG_THERM_STATUS/IA32_PACKAGE_THERM_STATUS
//# https://tc.gtisc.gatech.edu/cs3210/2016/fall/r/ia32/IA32-3B.pdf
//# 
//#############################################################################
void REG_PRINT_DE__print_IA32_PKG_THERM_STATUS(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[1B1H] IA32_PKG_THERM_STATUS/IA32_PACKAGE_THERM_STATUS\n");
    printf("  Command to read: sudo rdmsr 0x1B1 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └──────────────────┬────────────────────┘└──┬──┘└─┬┘││││││││││││\n");
    printf("                  Reserved                     │     │ ││││││││││││\n");
    printf("Pkg Digital Readout (RO) ──────────────────────┘     │ ││││││││││││ 0x%llx -> %llu  \n", GetVal(ullVal, 16, 7), GetVal(ullVal, 16, 7));
    printf("Reserved ────────────────────────────────────────────┘ ││││││││││││\n");
    printf("Pkg Power Limitation log (R/WC0) ──────────────────────┘│││││││││││ %llu\n", IsBitSet(ullVal, 11));
    printf("Pkg Power Limitation Status (RO) ───────────────────────┘││││││││││ %llu\n", IsBitSet(ullVal, 10));
    printf("Pkg Thermal Threshold #1 log (R/WC0) ────────────────────┘│││││││││ %llu\n", IsBitSet(ullVal,  9));
    printf("Pkg Thermal Threshold #2 Status (RO) ─────────────────────┘││││││││ %llu\n", IsBitSet(ullVal,  8));
    printf("Pkg Thermal Threshold #1 log (R/WC0) ──────────────────────┘│││││││ %llu\n", IsBitSet(ullVal,  7));
    printf("Pkg Thermal Threshold #1 Status (RO) ───────────────────────┘││││││ %llu\n", IsBitSet(ullVal,  6));
    printf("Pkg Critical Temperature Status log (R/WC0) ─────────────────┘│││││ %llu\n", IsBitSet(ullVal,  5));
    printf("Pkg Critical Temperature Status (RO) ─────────────────────────┘││││ %llu\n", IsBitSet(ullVal,  4));
    printf("Pkg PROCHOT # log (R/WC0) ─────────────────────────────────────┘│││ %llu\n", IsBitSet(ullVal,  3));
    printf("Pkg PROCHOT # event (RO) ───────────────────────────────────────┘││ %llu\n", IsBitSet(ullVal,  2));
    printf("Pkg Thermal Status Log (R/W): ───────────────────────────────────┘│ %llu\n", IsBitSet(ullVal,  1));
    printf("Pkg Thermal Status (RO): ─────────────────────────────────────────┘ %llu\n", IsBitSet(ullVal,  0));
}
//
//#############################################################################
//# MSR_PKG_POWER_LIMIT (RW)
//# https://gist.github.com/Mnkai/5a8edd34bd949199224b33bd90b8c3d4
//# 
//#############################################################################
void REG_PRINT_DE__print_MSR_PKG_POWER_LIMIT(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[610H] MSR_PKG_POWER_LIMIT\n");
    printf("  Command to read: sudo rdmsr 0x610 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   │       └┤└─┬─┘││└───────┬─────┘        └┤└─┬─┘││└───────┬─────┘\n");
    printf("MSR lock    │  │  ││        │               │  │  ││        │  %llu\n", IsBitSet(ullVal, 63));
    printf("P2 lmt time─┘  │  ││        │               │  │  ││        │  0x%llx -> %llu  \n", GetVal(ullVal, 54,  2), GetVal(ullVal, 54,  2));
    printf("P2 lmt time (e)┘  ││        │               │  │  ││        │  0x%llx -> %llu  \n", GetVal(ullVal, 49,  5), GetVal(ullVal, 49,  5));
    printf("P2 clamping limit─┘│        │               │  │  ││        │  %llu\n", IsBitSet(ullVal, 48));
    printf("P2 power enabled───┘        │               │  │  ││        │  %llu\n", IsBitSet(ullVal, 47));
    printf("P2 power limit ─────────────┘               │  │  ││        │  0x%llx -> %llu  \n", GetVal(ullVal, 32, 14), GetVal(ullVal, 32, 14));
    printf("Pkg power 1 limit time window (kof) ────────┘  │  ││        │  0x%llx -> %llu  \n", GetVal(ullVal, 22,  2), GetVal(ullVal, 22,  2));
    printf("Pkg power 1 limit time window (exp) ───────────┘  ││        │  0x%llx -> %llu  \n", GetVal(ullVal, 17,  5), GetVal(ullVal, 17,  5));
    printf("Pkg power 1 clamping limit ───────────────────────┘│        │  %llu\n", IsBitSet(ullVal, 16));
    printf("Pkg power enabled ─────────────────────────────────┘        │  %llu\n", IsBitSet(ullVal, 15));
    printf("Pkg power limit = Powerunit * decimal ──────────────────────┘  0x%llx -> %llu  \n", GetVal(ullVal,  0, 14), GetVal(ullVal,  0, 14));
}


//#############################################################################
//# MSR_PKG_ENERGY_STATUS/MSR_RAPL_POWER_UNIT
//# http://kib.kiev.ua/x86docs/SDMs/335592-063.pdf
//# https://tc.gtisc.gatech.edu/cs3210/2016/fall/r/ia32/IA32-3B.pdf
//# 
//#############################################################################
void REG_PRINT_DE__print_MSR_PKG_ENERGY_STATUS(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[611H] MSR_PKG_ENERGY_STATUS/MSR_RAPL_POWER_UNIT\n");
    printf("  Command to read: sudo rdmsr 0x611 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └─────────────────────┬────────────────────┘└─┬┘└┬┘└─┬─┘└─┬┘└─┬┘\n");
    printf("                     Reserved                    │  │   │    │   │\n");
    printf("Time Units ──────────────────────────────────────┘  │   │    │   │ 0x%llx -> %llu  \n", GetVal(ullVal, 16, 4), GetVal(ullVal, 16, 4));
    printf("Reserved ───────────────────────────────────────────┘   │    │   │\n");
    printf("Energy Status Units ────────────────────────────────────┘    │   │ 0x%llx -> %llu  \n", GetVal(ullVal, 8, 5), GetVal(ullVal, 8, 5));
    printf("Reserved ────────────────────────────────────────────────────┘   │\n");
    printf("Power Units ─────────────────────────────────────────────────────┘ 0x%llx -> %llu  \n", GetVal(ullVal, 0, 4), GetVal(ullVal, 0, 4));
}

//#############################################################################
//# MSR_PKG_STATUS/MSR_PKG_PERF_STATUS
//# http://kib.kiev.ua/x86docs/SDMs/335592-063.pdf
//# https://tc.gtisc.gatech.edu/cs3210/2016/fall/r/ia32/IA32-3B.pdf
//# 
//#############################################################################
void REG_PRINT_DE__print_MSR_PKG_STATUS(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[613H] MSR_PKG_STATUS/MSR_PKG_PERF_STATUS\n");
    printf("  Command to read: sudo rdmsr 0x613 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └───────────────┬──────────────┘└────────────────┬─────────────┘\n");
    printf("                Reserved                            │              \n");
    printf("Accumulated Package Throttle Time  ─────────────────┘  0x%llx -> %llu  \n", GetVal(ullVal, 0, 32), GetVal(ullVal, 0, 32));
}

//#############################################################################
//# MSR_CORE_PERF_LIMIT_REASONS
//# http://kib.kiev.ua/x86docs/SDMs/335592-066.pdf
//# https://stackoverflow.com/questions/45472147/lost-cycles-on-intel-an-inconsistency-between-rdtsc-and-cpu-clk-unhalted-ref-ts
//# 
//#############################################################################
void REG_PRINT_DE__printMsrCorePerfLimitReasons(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[690H] MSR_CORE_PERF_LIMIT_REASONS\n");
    printf("  Command to read: sudo rdmsr 0x690 -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └───────────────┬───────────────┘││││││└─┬─┘│││││││││││└─┬─┘││││\n");
    printf("              Reserved              ││││││  │  │││││││││││  │  ││││\n");
    printf("Maximum Efficiency Frequency Log ───┘│││││  │  │││││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 30));
    printf("Turbo Transistion Attenuation Log ───┘││││  │  │││││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 29));
    printf("Electical Design Point Log ───────────┘│││  │  │││││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 28));
    printf("Max Turbo Limit Log ───────────────────┘││  │  │││││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 27));
    printf("VR Them Alert Log ──────────────────────┘│  │  │││││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 26));
    printf("Core Power Limiting Log ─────────────────┘  │  │││││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 25));
    printf("Reserved ───────────────────────────────────┘  │││││││││││  │  ││││\n");
    printf("Package-Level PL2 Power Limiting Log ──────────┘││││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 19));
    printf("Package-Level PL1 Power Limiting Log ───────────┘│││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 18));
    printf("Thermal Log ─────────────────────────────────────┘││││││││  │  ││││ %llu\n", IsBitSet(ullVal, 17));
    printf("PROCHOT Log ──────────────────────────────────────┘│││││││  │  ││││ %llu\n", IsBitSet(ullVal, 16));
    printf("Reserved ──────────────────────────────────────────┘││││││  │  ││││\n");
    printf("Maximum Efficiency Frequency Status (R0)────────────┘│││││  │  ││││ %llu\n", IsBitSet(ullVal, 14));
    printf("Turbo Transition Attenuation Status (R0)─────────────┘││││  │  ││││ %llu\n", IsBitSet(ullVal, 13));
    printf("Electrical Design Point Status (R0)───────────────────┘│││  │  ││││ %llu\n", IsBitSet(ullVal, 12));
    printf("Max Turbo Limit Status (R0) ───────────────────────────┘││  │  ││││ %llu\n", IsBitSet(ullVal, 11));
    printf("VR Therm Alert Status (R0)──────────────────────────────┘│  │  ││││ %llu\n", IsBitSet(ullVal, 10));
    printf("Core Power Limiting Status (R0)──────────────────────────┘  │  ││││ %llu\n", IsBitSet(ullVal,  9));
    printf("Reserved ───────────────────────────────────────────────────┘  ││││\n");
    printf("Package-Level PL2 Power Limiting Status (R0) ──────────────────┘│││ %llu\n", IsBitSet(ullVal,  3));
    printf("Package-Level Power Limiting PL1 Status (R0)────────────────────┘││ %llu\n", IsBitSet(ullVal,  2));
    printf("Thermal Status (R0) ─────────────────────────────────────────────┘│ %llu\n", IsBitSet(ullVal,  1));
    printf("PROCHOT Status (R0) ──────────────────────────────────────────────┘ %llu\n", IsBitSet(ullVal,  0));
}

//#############################################################################
//# MSR_CORE_PERF_LIMIT_REASONS
//# http://kib.kiev.ua/x86docs/SDMs/335592-066.pdf
//# https://stackoverflow.com/questions/45472147/lost-cycles-on-intel-an-inconsistency-between-rdtsc-and-cpu-clk-unhalted-ref-ts
//# 
//#############################################################################
void REG_PRINT_DE__printMsrCorePerfLimitReasons64f(unsigned long long ullVal)
{
    char cBuffValAsBin[100] = { 0 };

    // Create a binary string from value
    REG_PRINT_DE__makeBinaryString(ullVal, cBuffValAsBin, sizeof(cBuffValAsBin));

    printf("[64FH] MSR_CORE_PERF_LIMIT_REASONS (gen 6+)\n");
    printf("  Command to read: sudo rdmsr 0x64f -f 63:0\n");
    printf("Value of register is: %08llx\n", ullVal);
    printf("  64  60        50        40        30        20        10\n");
    printf("  43210987654321098765432109876543210987654321098765432109876543210\n");
    printf("%s\n", cBuffValAsBin);
    printf("   └───────────────┬────────────────┘││││││││││└┤││└┤││││││││││└┤││\n");
    printf("              Reserved               ││││││││││ │││ │││││││││││ │││\n");
    printf("Turbo Transition Attenuation Log ────┘│││││││││ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 29));
    printf("Max Turbo Limit Log ──────────────────┘││││││││ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 28));
    printf("Pkg/Platf-Lvl PL2 Pwr Limiting Log ────┘│││││││ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 27));
    printf("Pkg/Platf-Lvl PL1 Pwr Limiting Log ─────┘││││││ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 26));
    printf("Reserved ────────────────────────────────┘│││││ │││ │││││││││││ │││\n");
    printf("Other Log ────────────────────────────────┘││││ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 24));
    printf("VR Thermal Design Current Log ─────────────┘│││ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 23));
    printf("VR Therm Alert Log ─────────────────────────┘││ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 22));
    printf("Running Average Thermal Limit Log ───────────┘│ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 21));
    printf("Residency State Regulation Log ───────────────┘ │││ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 20));
    printf("Reserved ───────────────────────────────────────┘││ │││││││││││ │││\n");
    printf("Thermal Log ─────────────────────────────────────┘│ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 17));
    printf("PROCHOT Log ──────────────────────────────────────┘ │││││││││││ │││ %llu\n", IsBitSet(ullVal, 16));
    printf("Reserved ───────────────────────────────────────────┘││││││││││ │││\n");
    printf("Turbo Transition Attenuation Status (R0) ────────────┘│││││││││ │││ %llu\n", IsBitSet(ullVal, 13));
    printf("Max Turbo Limit Status (RO) ──────────────────────────┘││││││││ │││ %llu\n", IsBitSet(ullVal, 12));
    printf("Package/Platform-Level PL2 Power Limiting Status (RO)──┘│││││││ │││ %llu\n", IsBitSet(ullVal, 11));
    printf("Package/Platform-Level Power Limiting PL1 Status (RO)───┘││││││ │││ %llu\n", IsBitSet(ullVal, 10));
    printf("Reserved ────────────────────────────────────────────────┘│││││ │││\n");
    printf("Other (RO) ───────────────────────────────────────────────┘││││ │││ %llu\n", IsBitSet(ullVal,  8));
    printf("VR Therm Design Current Status (R0)────────────────────────┘│││ │││ %llu\n", IsBitSet(ullVal,  7));
    printf("VR Therm Alert Status (R0)──────────────────────────────────┘││ │││ %llu\n", IsBitSet(ullVal,  6));
    printf("Running Average Thermal Limit Status (RO)────────────────────┘│ │││ %llu\n", IsBitSet(ullVal,  5));
    printf("Residency State Regulator Status (RO)─────────────────────────┘ │││ %llu\n", IsBitSet(ullVal,  4));
    printf("Reserved ───────────────────────────────────────────────────────┘││\n");
    printf("Thermal Status (R0) ─────────────────────────────────────────────┘│ %llu\n", IsBitSet(ullVal,  1));
    printf("PROCHOT Status (R0) ──────────────────────────────────────────────┘ %llu\n", IsBitSet(ullVal,  0));
}
