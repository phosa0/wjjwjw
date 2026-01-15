import argparse
import asyncio
import aiohttp
import time

async def fetch(session, url):
    """
    Asynchronously fetches a single URL, returning its status.
    """
    start_time = time.time()
    try:
        async with session.get(url, timeout=30) as response:
            # We must read the response to free up the connection
            await response.read()
            end_time = time.time()
            duration = end_time - start_time
            print(f"Status: {response.status} | Time: {duration:.2f}s | URL: {url}")
            return response.status
    except Exception as e:
        print(f"Request Failed for {url}: {e}")
        return None

async def generate_traffic(url, num_requests):
    """
    Generates a high volume of concurrent GET requests to a URL using asyncio and aiohttp.

    This function is intended for load testing your own web applications.
    Misuse of this script on websites you do not own is unethical and
    may be illegal.
    """
    print(f"Starting to send {num_requests} requests to {url} concurrently...")
    start_total_time = time.time()

    # Create a single session to be reused for all requests for efficiency
    async with aiohttp.ClientSession() as session:
        # Create a list of tasks to be run concurrently
        tasks = [fetch(session, url) for _ in range(num_requests)]
        # Wait for all tasks to complete
        results = await asyncio.gather(*tasks, return_exceptions=True)

    end_total_time = time.time()
    total_duration = end_total_time - start_total_time

    success_count = sum(1 for r in results if r is not None and isinstance(r, int) and 200 <= r < 300)
    failure_count = len(results) - success_count

    print("\n--- Traffic Generation Summary ---")
    print(f"Total requests attempted: {num_requests}")
    print(f"Successful requests (2xx): {success_count}")
    print(f"Failed requests: {failure_count}")
    print(f"Total time taken: {total_duration:.2f} seconds")
    if total_duration > 0:
        print(f"Requests Per Second (RPS): {num_requests / total_duration:.2f}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="A high-speed tool for generating web traffic for load testing using asyncio.",
        epilog="""\
IMPORTANT: This tool is for educational and ethical use only.
Only use it for load testing on web applications that you own or have
explicit permission to test. Unauthorized use against other websites is
strictly prohibited.
"""
    )
    parser.add_argument("url", help="The URL of the website to test.")
    parser.add_argument("num_requests", type=int, help="The number of concurrent requests to send.")

    args = parser.parse_args()

    if not args.url.startswith(('http://', 'https://')):
        args.url = 'http://' + args.url

    # Run the main asynchronous function
    asyncio.run(generate_traffic(args.url, args.num_requests))
