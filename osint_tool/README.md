# All-in-One OSINT Tool

This is a powerful, stylish Python-based All-in-One OSINT (Open-Source Intelligence) Tool designed for educational and testing purposes only. It provides a range of features to gather and analyze publicly available information.

## Features

*   **Username Scanner:** Check the presence of a username across various social media platforms and websites.
*   **IP Tracker:** Geolocation information for a given IP address, including city, ISP, and reverse DNS.
*   **Email Breach Checker:** Check if an email address has been compromised in any known data breaches.
*   **Phone Number Lookup:** Extract carrier, country, and location information for a phone number.
*   **Metadata Extractor:** Upload image or PDF files to extract hidden metadata.
*   **Domain Scanner:** Check domain reputation, DNS information, subdomains, and WHOIS data.
*   **GUI Dashboard:** A stylish and colorful GUI with tabs for each feature and a log window to display results.

## Installation

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-username/osint-tool.git
    cd osint-tool
    ```

2.  **Install the dependencies:**
    ```bash
    pip install -r requirements.txt
    ```

## Usage

1.  **Run the application:**
    ```bash
    python main.py
    ```

2.  **API Keys:**
    *   **Email Breach Checker:** You need an API key from [Have I Been Pwned](https://haveibeenpwned.com/API/Key).
    *   **Phone Number Lookup:** You need an API key from [Numverify](https://numverify.com/).
    *   **IP Tracker:** An optional API key from [IPinfo](https://ipinfo.io/signup) can be used for more detailed IP tracking.

3.  **Navigate through the tabs** to access different features. Enter the required information and click the respective buttons to start the scans. The results will be displayed in the log window.

## Disclaimer

This tool is intended for educational and testing purposes only. The user is responsible for any misuse of this tool. The developers assume no liability and are not responsible for any misuse or damage caused by this tool.
