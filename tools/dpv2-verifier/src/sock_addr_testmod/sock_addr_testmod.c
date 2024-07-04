// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2024 Google LLC. */
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/nsproxy.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/debugfs.h>
#include <net/sock.h>

#define CONNECT_TIMEOUT_SEC 1

static struct debugfs_blob_wrapper addr_blob;
static struct dentry *debugfs_dentry;
static struct sockaddr_storage addr;

static struct socket *sock;

static int kernel_connect_testmod_init(void)
{
	int err;

	debugfs_dentry = debugfs_create_dir("sock_addr_testmod", NULL);

	addr_blob.data = &addr;
	addr_blob.size = sizeof(addr);

	debugfs_create_blob("addr", 0444, debugfs_dentry, &addr_blob);

	err = inet_pton_with_scope(&init_net, AF_INET, "192.168.0.2", "80", &addr);
	if (err) {
		pr_err("inet_pton_with_scope() returned %d\n", err);
		goto out;
	}

	err = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	if (err) {
		pr_err("sock_create_kern() returned %d\n", err);
		goto out;
	}

	/* Set timeout for call to kernel_connect() to prevent it from hanging,
	 * and consider the connection attempt failed if it returns
	 * -EINPROGRESS.
	 */
	sock->sk->sk_sndtimeo = CONNECT_TIMEOUT_SEC * HZ;

	err = kernel_connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_storage), 0);
	if (err)
		pr_err("kernel_connect() returned %d\n", err);
out:
	return 0;
}

static void kernel_connect_testmod_exit(void)
{
	if (sock)
		sock_release(sock);

	debugfs_remove_recursive(debugfs_dentry);
}

module_init(kernel_connect_testmod_init);
module_exit(kernel_connect_testmod_exit);

MODULE_AUTHOR("Jordan Rife");
MODULE_DESCRIPTION("BPF kernel_connect test module");
MODULE_LICENSE("Dual BSD/GPL");
