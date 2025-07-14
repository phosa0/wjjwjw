# All In One Ethical Hacking Toolkit

This is a collection of ethical hacking tools for penetration testing and security research.

## Tools

* **Port Scanner:** Scan for open ports on a target host.
* **Password Cracker:** Crack password hashes using a wordlist.
* **Vulnerability Scanner:** Scan for common web vulnerabilities.

## Usage

```
python -m toolkit.main <tool> [options]
```

### Port Scanner

```
python -m toolkit.main port-scanner <host> --ports <port1> <port2> ...
```

### Password Cracker

```
python -m toolkit.main password-cracker <hash> <wordlist>
```

### Vulnerability Scanner

```
python -m toolkit.main vulnerability-scanner <url>
```
