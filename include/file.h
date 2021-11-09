#pragma once

#include <sys/types.h>

off_t	file_map(int fd, void**addr);
int		file_unmap(void **addr, off_t size);
