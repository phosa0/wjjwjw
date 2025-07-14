import argparse
from . import port_scanner
from . import password_cracker
from . import vulnerability_scanner

def main():
    parser = argparse.ArgumentParser(description="All In One Ethical Hacking Toolkit")
    subparsers = parser.add_subparsers(dest="tool")

    port_scanner_parser = subparsers.add_parser("port-scanner", help="Scan for open ports")
    port_scanner_parser.add_argument("host", help="Host to scan")
    port_scanner_parser.add_argument("--ports", nargs="+", type=int, help="Ports to scan")

    password_cracker_parser = subparsers.add_parser("password-cracker", help="Crack a password hash")
    password_cracker_parser.add_argument("hash", help="Password hash to crack")
    password_cracker_parser.add_argument("wordlist", help="Path to wordlist file")

    vulnerability_scanner_parser = subparsers.add_parser("vulnerability-scanner", help="Scan for web vulnerabilities")
    vulnerability_scanner_parser.add_argument("url", help="URL to scan")

    args = parser.parse_args()

    if args.tool == "port-scanner":
        open_ports = port_scanner.scan_ports(args.host, args.ports)
        print(f"Open ports on {args.host}: {open_ports}")
    elif args.tool == "password-cracker":
        with open(args.wordlist) as f:
            wordlist = [line.strip() for line in f]
        password = password_cracker.crack_password(args.hash, wordlist)
        if password:
            print(f"Password found: {password}")
        else:
            print("Password not found")
    elif args.tool == "vulnerability-scanner":
        vulnerability_scanner.scan_vulnerabilities(args.url)

if __name__ == "__main__":
    main()
