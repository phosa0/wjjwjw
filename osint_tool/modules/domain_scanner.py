import whois
import dns.resolver
import requests
from bs4 import BeautifulSoup

def get_whois_info(domain, app):
    try:
        w = whois.whois(domain)
        app.log("[+] WHOIS Information:")
        for key, value in w.items():
            app.log(f"  - {key}: {value}")
    except Exception as e:
        app.log(f"[!] Error fetching WHOIS info: {e}")

def get_dns_info(domain, app):
    app.log("\n[+] DNS Information:")
    try:
        for record_type in ['A', 'AAAA', 'MX', 'NS', 'TXT', 'SOA']:
            try:
                answers = dns.resolver.resolve(domain, record_type)
                app.log(f"  - {record_type} Records:")
                for rdata in answers:
                    app.log(f"    - {rdata.to_text()}")
            except dns.resolver.NoAnswer:
                app.log(f"  - No {record_type} records found.")
            except dns.resolver.NXDOMAIN:
                app.log(f"  - Domain not found.")
                break
    except Exception as e:
        app.log(f"[!] Error fetching DNS info: {e}")

def get_subdomains(domain, app):
    app.log("\n[+] Subdomain Scan:")
    try:
        url = f"https://crt.sh/?q=%25.{domain}&output=json"
        response = requests.get(url, timeout=20)
        if response.status_code == 200:
            subdomains = sorted(list(set(entry['name_value'] for entry in response.json())))
            app.log(f"  - Found {len(subdomains)} subdomains:")
            for sub in subdomains:
                app.log(f"    - {sub}")
        else:
            app.log(f"  - Error fetching subdomains from crt.sh: {response.status_code}")
    except requests.RequestException as e:
        app.log(f"[!] Error during subdomain scan: {e}")

def scan_domain(domain, app):
    app.log(f"[*] Scanning domain: {domain}")
    get_whois_info(domain, app)
    get_dns_info(domain, app)
    get_subdomains(domain, app)
