#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static int read_result(const char *path, void *val, size_t len)
{
	FILE *f;
	int err;

	f = fopen(path, "r");
	if (!f)
		goto err;

	err = fread(val, 1, len, f);
	if (err != len)
		goto err;

	err = 0;
	goto out;

err:
	err = -1;
out:
	if (f)
		fclose(f);

	return err;
}

int main()
{
	struct sockaddr_storage addr;
	char str[256];

	if (read_result("/sys/kernel/debug/sock_addr_testmod/addr",
			&addr, sizeof(addr)))
		return 1;

	inet_ntop(AF_INET, &(((struct sockaddr_in *)&addr)->sin_addr),
                    str, sizeof(str)-1);

	printf("%s:%d", str, ntohs(((struct sockaddr_in *)&addr)->sin_port));

	return 0;
}
