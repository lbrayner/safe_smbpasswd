Run */usr/bin/smbpasswd* and */usr/bin/net* in a safe way.

---
A wrapper for */usr/bin/smbpasswd* that: sanitizes arguments; and forks.

The only allowed usage is:

```
smbpasswd (-s -a?|-x) USERNAME
```

*USERNAME* has to match the regex `"^[a-zA-Z][0-9a-zA-Z._\\-]+$"`.
*USERNAME* *root* is not allowed.

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
