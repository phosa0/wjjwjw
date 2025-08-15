# Website Traffic Generator for Load Testing

This is a simple tool to generate a specified number of GET requests to a website. It is intended to be used for **load testing your own web applications** to identify performance bottlenecks.

## ⚠️ Important: Ethical Use Only

- **DO NOT** use this tool on any website or service that you do not own or have explicit, written permission to load test.
- Unauthorized traffic generation can be considered a **denial-of-service (DoS) attack** and may be illegal.
- The author of this script is not responsible for any misuse. Use this tool responsibly and ethically.

## Setup

1.  **Install dependencies:**
    Before running the script, you need to install the required Python library. Open your terminal and run:
    ```bash
    pip install -r requirements.txt
    ```

## How to Run

Use the following command in your terminal to run the script:

```bash
python traffic_generator.py <URL> <NUMBER_OF_REQUESTS>
```

### Examples

- To send 100 requests to `http://example.com`:
  ```bash
  python traffic_generator.py http://example.com 100
  ```

- If you omit the scheme, `http://` will be added automatically:
  ```bash
  python traffic_generator.py example.com 50
  ```

The script will print the status code and response time for each request.
