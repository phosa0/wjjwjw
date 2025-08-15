import argparse
import requests
import time

def generate_traffic(url, num_requests):
    """
    Sends a specified number of GET requests to a URL.

    This function is intended for load testing your own web applications.
    Misuse of this script on websites you do not own is unethical and
    may be illegal.
    """
    print(f"Starting to send {num_requests} requests to {url}...")
    for i in range(num_requests):
        try:
            start_time = time.time()
            response = requests.get(url, timeout=10)
            end_time = time.time()
            duration = end_time - start_time
            print(f"Request {i+1}/{num_requests}: Status Code: {response.status_code} | Response Time: {duration:.2f}s")
        except requests.exceptions.RequestException as e:
            print(f"Request {i+1}/{num_requests}: Failed - {e}")
        time.sleep(0.1)  # Small delay between requests
    print("Traffic generation complete.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="A simple tool for generating web traffic for load testing.",
        epilog="""\
IMPORTANT: This tool is for educational and ethical use only.
Only use it for load testing on web applications that you own or have
explicit permission to test. Unauthorized use against other websites is
strictly prohibited.
"""
    )
    parser.add_argument("url", help="The URL of the website to test.")
    parser.add_argument("num_requests", type=int, help="The number of requests to send.")

    args = parser.parse_args()

    # Add http:// if no scheme is present
    if not args.url.startswith(('http://', 'https://')):
        args.url = 'http://' + args.url

    generate_traffic(args.url, args.num_requests)
