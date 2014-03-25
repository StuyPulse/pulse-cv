#include "wire.h" 

void init_pin() {
	char buffer[16];
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd == -1) {
		printf("Failed to open IO pin\n");
		return;
	}

	sprintf(buffer, "%d", PIN);
	write(fd, buffer, sizeof(buffer));
	close(fd);

	char path[256];
	sprintf(path, "/sys/class/gpio/gpio%d/direction", PIN)
		fd= open(path, O_WRONLY);
	if (fd == -1) {
		printf("failed to open direction setter\n");
		return;
	}

	sprintf(buffer, "out");
	write(fd, buffer, sizeof(buffer));
	close(fd);
}

void set_pin(int val) {
	char path[256];
	char buffer[32];
	int fd;
	sprintf(path, "/sys/class/gpio/gpio%d/value", PIN);
	fd = open(path, O_WRONLY);
	if (fd == -1) {
		printf("Failed to open pin value setter\n");
		return;
	}
	sprintf(buffer, "%d", val);
	write(fd, buffer, sizeof(buffer));
	close(fd);
}
