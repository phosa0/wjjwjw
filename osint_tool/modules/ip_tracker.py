import ipinfo

def track_ip(ip_address, app, api_key=None):
    app.log(f"[*] Tracking IP address: {ip_address}")
    try:
        handler = ipinfo.getHandler(api_key)
        details = handler.getDetails(ip_address)

        app.log("[+] IP Information:")
        for key, value in details.all.items():
            app.log(f"  {key}: {value}")

    except Exception as e:
        app.log(f"[!] Error tracking IP: {e}")
