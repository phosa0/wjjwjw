import requests

def lookup_phone_number(phone_number, app, api_key):
    app.log(f"[*] Looking up phone number: {phone_number}")
    url = f"http://apilayer.net/api/validate?access_key={api_key}&number={phone_number}"

    try:
        response = requests.get(url, timeout=10)
        if response.status_code == 200:
            data = response.json()
            if data.get("valid"):
                app.log("[+] Phone number information:")
                app.log(f"  - Country: {data.get('country_name')}")
                app.log(f"  - Location: {data.get('location')}")
                app.log(f"  - Carrier: {data.get('carrier')}")
                app.log(f"  - Line Type: {data.get('line_type')}")
            else:
                app.log("[-] Invalid phone number or no information found.")
        else:
            app.log(f"[!] Error: {response.status_code} - {response.text}")

    except requests.RequestException as e:
        app.log(f"[!] An error occurred: {e}")
