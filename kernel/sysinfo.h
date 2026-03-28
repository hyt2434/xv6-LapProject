#ifndef _SYSINFO_H_
#define _SYSINFO_H_

struct sysinfo {
  uint64 freemem;   // Lưu lượng bộ nhớ trống (bytes)
  uint64 nproc;     // Lưu số lượng tiến trình đang chạy
  uint64 nopenfiles; // Lưu số lượng file đang mở
};

#endif