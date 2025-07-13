import requests

def check_email_breach(email, app, api_key):
    app.log(f"[*] Checking for breaches for email: {email}")
    url = f"https://haveibeenpwned.com/api/v3/breachedaccount/{email}"
    headers = {
        "hibp-api-key": api_key,
        "user-agent": "OSINT-Tool"
    }

    try:
        response = requests.get(url, headers=headers, timeout=10)
        if response.status_code == 200:
            breaches = response.json()
            if breaches:
                app.log("[+] Email found in the following breaches:")
                for breach in breaches:
                    app.log(f"  - {breach['Name']}")
            else:
                app.log("[-] No breaches found for this email.")
        elif response.status_code == 404:
            app.log("[-] No breaches found for this email.")
        else:
            app.log(f"[!] Error: {response.status_code} - {response.text}")

    except requests.RequestException as e:
        app.log(f"[!] An error occurred: {e}")
