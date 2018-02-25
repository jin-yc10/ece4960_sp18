//
// Created by yj374 on 2/25/2018.
//

#ifndef ECE4960_SP18_MEMORY_H
#define ECE4960_SP18_MEMORY_H

// www.helleboreconsulting.com

///
/// \namespace npas4
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
/// Special thanks to the band "3" for providing the development soundtrack.
///
/// Reference:
/// http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
///

#define NPAS4_STATIC

#ifdef NPAS4_STATIC
  #define NPAS4_EXPORT
#else
  #ifdef WIN32
    #if defined NPAS4_EXPORTS
      #define NPAS4_EXPORT __declspec(dllexport)
    #else
      #define NPAS4_EXPORT __declspec(dllimport)
    #endif
  #else
    #define NPAS4_EXPORT
  #endif
#endif

#include <cstdint>

namespace npas4
{
  ///
  /// Swap file + Physical RAM
  ///
  NPAS4_EXPORT double GetTotalSystemRAM();

  ///
  /// Swap file + Physical RAM
  ///
  NPAS4_EXPORT double GetTotalSystemRAMAvailable();

  ///
  /// Swap file + Physical RAM
  ///
  NPAS4_EXPORT double GetTotalSystemRAMUsed();

  NPAS4_EXPORT double GetTotalSystemRAMUsedByCurrentProcess();

  NPAS4_EXPORT double GetTotalPhysicalRAM();

  NPAS4_EXPORT double GetTotalPhysicalRAMAvailable();

  NPAS4_EXPORT double GetTotalPhysicalRAMUsed();

  NPAS4_EXPORT double GetTotalPhysicalRAMUsedByCurrentProcess();
}

void report_mem_usage();

#endif //ECE4960_SP18_MEMORY_H
