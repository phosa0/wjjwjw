import requests

def scan_username(username, app):
    app.log(f"[*] Scanning for username: {username}")
    sites = {
        "Instagram": f"https://www.instagram.com/{username}",
        "GitHub": f"https://github.com/{username}",
        "Reddit": f"https://www.reddit.com/user/{username}",
        "Twitter": f"https://twitter.com/{username}",
        "Facebook": f"https://www.facebook.com/{username}"
    }

    for site, url in sites.items():
        try:
            response = requests.get(url, timeout=5)
            if response.status_code == 200:
                app.log(f"[+] Found on {site}: {url}")
            else:
                app.log(f"[-] Not found on {site}")
        except requests.RequestException as e:
            app.log(f"[!] Error checking {site}: {e}")
