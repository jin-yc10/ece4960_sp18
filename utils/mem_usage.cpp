//
// Created by yj374 on 2/25/2018.
//

///
/// \author	John Farrier
///
/// \copyright Copyright 2014 John Farrier
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
/// http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///

#include "./mem_usage.h"
#include "stdio.h"

#ifdef WIN32
#include <Windows.h>
#include <Psapi.h>
#else
#include <sys/types.h>
	#include <sys/sysinfo.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
#endif

const unsigned int Kilobytes2Bytes = 1024;

void report_mem_usage() {
  double system_mem_used = npas4::GetTotalSystemRAMUsed();
  double system_mem_used_by_process = npas4::GetTotalSystemRAMUsedByCurrentProcess();
  double total_phy_mem = npas4::GetTotalPhysicalRAM();
  double total_phy_mem_available = npas4::GetTotalPhysicalRAMAvailable();
  double phy_mem_used = npas4::GetTotalPhysicalRAMUsed();
  double phy_mem_used_by_process = npas4::GetTotalPhysicalRAMUsedByCurrentProcess();

  printf("%.3fMB\n", system_mem_used_by_process/1048576.0);
}

double npas4::GetTotalSystemRAM()
{
#ifdef WIN32
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);
  return static_cast<double>(memInfo.ullTotalPageFile);
#else
  struct sysinfo memInfo;
		sysinfo (&memInfo);
		auto totalVirtualMem = memInfo.totalram;

		// Add other values in next statement to avoid int overflow on right hand side...
		totalVirtualMem += memInfo.totalswap;
		totalVirtualMem *= memInfo.mem_unit;
		return static_cast<double>(totalVirtualMem);
#endif
}

double npas4::GetTotalSystemRAMAvailable()
{
#ifdef WIN32
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);
  return static_cast<double>(memInfo.ullAvailPageFile);
#else
  return npas4::GetTotalSystemRAM() - npas4::GetTotalSystemRAMUsed();
#endif
}

double npas4::GetTotalSystemRAMUsed()
{
#ifdef WIN32
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);
  return static_cast<double>(memInfo.ullTotalPageFile - memInfo.ullAvailPageFile);
#else
  struct sysinfo memInfo;
		sysinfo(&memInfo);
		auto virtualMemUsed = memInfo.totalram - memInfo.freeram;

		//Add other values in next statement to avoid int overflow on right hand side...
		virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
		virtualMemUsed *= memInfo.mem_unit;

		return static_cast<double>(virtualMemUsed);
#endif
}

double npas4::GetTotalSystemRAMUsedByCurrentProcess()
{
#ifdef WIN32
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
  return static_cast<double>(pmc.PrivateUsage);
#else
  auto parseLine = [](char* line)->int
			{
				auto i = strlen(line);

				while(*line < '0' || *line > '9')
				{
					line++;
				}

				line[i-3] = '\0';
				i = atoi(line);
				return i;
			}

		auto file = fopen("/proc/self/status", "r");
		auto result = -1;
		char line[128];

		while(fgets(line, 128, file) != nullptr)
		{
			if(strncmp(line, "VmSize:", 7) == 0)
			{
				result = parseLine(line);
				break;
			}
		}

		fclose(file);
		return static_cast<double>(result) * Kilobytes2Bytes;
	}

#endif
}

double npas4::GetTotalPhysicalRAM()
{
#ifdef WIN32
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);
  return static_cast<double>(memInfo.ullTotalPhys);
#else
  struct sysinfo memInfo;
		sysinfo(&memInfo);

		auto totalPhysMem = memInfo.totalram;

		//Multiply in next statement to avoid int overflow on right hand side...
		totalPhysMem *= memInfo.mem_unit;
		return static_cast<double>(totalPhysMem);
#endif
}

double npas4::GetTotalPhysicalRAMAvailable()
{
#ifdef WIN32
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);
  return static_cast<double>(memInfo.ullAvailPhys);
#else
  return npas4::GetTotalPhysicalRAM() - npas4::GetTotalPhysicalRAMUsed();
#endif
}

double npas4::GetTotalPhysicalRAMUsed()
{
#ifdef WIN32
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);
  return static_cast<double>(memInfo.ullTotalPhys - memInfo.ullAvailPhys);
#else
  struct sysinfo memInfo;
		sysinfo(&memInfo);

		physMemUsed = memInfo.totalram - memInfo.freeram;

		// Multiply in next statement to avoid int overflow on right hand side...
		physMemUsed *= memInfo.mem_unit;
		return static_cast<double>(physMemUsed);
#endif
}

double npas4::GetTotalPhysicalRAMUsedByCurrentProcess()
{
#ifdef WIN32
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
  return static_cast<double>(pmc.WorkingSetSize);
#else
  auto parseLine = [](char* line)->int
			{
				auto i = strlen(line);

				while(*line < '0' || *line > '9')
				{
					line++;
				}

				line[i-3] = '\0';
				i = atoi(line);
				return i;
			}

		auto file = fopen("/proc/self/status", "r");
		auto result = -1;
		char line[128];

		while(fgets(line, 128, file) != nullptr)
		{
			if(strncmp(line, "VmRSS:", 6) == 0)
			{
				result = parseLine(line);
				break;
			}
		}

		fclose(file);
		return static_cast<double>(result) * Kilobytes2Bytes;

#endif
}