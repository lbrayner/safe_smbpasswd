Run `/usr/bin/smbpasswd` and `/usr/bin/net` in a safe way.

---

Meant for **apache** or **nginx**. You can safely grant *sudo* privileges to
e.g. the **apache** user *www-data*.

Copy the generated binary:

```
$ git clone 'https://github.com/lbrayner/safe_smbpasswd'
$ cd safe_smbpasswd
safe_smbpasswd$ make
safe_smbpasswd$ cp -f smbpasswd/safe_smbpasswd /usr/local/bin/smbpasswd
```

Add this *sudo* rule (don't forget to use *visudo*):

```
www-data ALL = (root) NOPASSWD: /usr/local/bin/smbpasswd
```

Now *www-data* can safely run *smbpasswd* via *safe_smbpasswd*.
